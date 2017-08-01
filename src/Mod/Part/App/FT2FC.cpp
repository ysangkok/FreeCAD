/***************************************************************************
 *   Copyright (c) wandererfan       <wandererfan (at) gmail.com> 2013     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *  FreeType License (FTL) credit:                                         *
 *  Portions of this software are copyright (c) <1996-2011> The FreeType   *
 *  Project (www.freetype.org).  All rights reserved.                      *
 ***************************************************************************/


#ifdef FCUseFreeType

#include "PreCompiled.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio> 
#include <cstdlib> 
#include <stdexcept>
#include <vector>

#include <BRepLib.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom_Plane.hxx>
#include <Geom2d_BezierCurve.hxx>
#include <gp_Trsf.hxx>
#include <Precision.hxx>

#include "TopoShape.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#include FT_TYPES_H

#include "FT2FC.h"

using namespace Part;

typedef unsigned long UNICHAR;           // ul is FT2's codepoint type <=> Py_UNICODE2/4

// Private function prototypes
FT_Vector getKerning(FT_Face FTFont, UNICHAR lc, UNICHAR rc);
TopoDS_Wire edgesToWire(std::vector<TopoDS_Edge> Edges);


//********** FT Decompose callbacks and data defns
// FT Decomp Context for 1 char
struct FTDC_Ctx {               
  std::vector<TopoDS_Wire> Wires;
  std::vector<TopoDS_Edge> Edges;
  UNICHAR currchar;
  FT_Vector LastVert;
  Handle(Geom_Surface) surf;
};

// move_cb called for start of new contour. pt is xy of contour start.
// p points to the context where we remember what happened previously (last point, etc)
static int move_cb(const FT_Vector* pt, void* p) {
   FTDC_Ctx* dc = (FTDC_Ctx*) p;
   if (!dc->Edges.empty()){                    
       TopoDS_Wire newwire = edgesToWire(dc->Edges);
       dc->Wires.push_back(newwire);
       dc->Edges.clear();
   }
   dc->LastVert = *pt;
   return 0;
}

// line_cb called for line segment in the current contour: line(LastVert -- pt) 
static int line_cb(const FT_Vector* pt, void* p) {
   FTDC_Ctx* dc = (FTDC_Ctx*) p;
   gp_Pnt2d v1(dc->LastVert.x, dc->LastVert.y);
   gp_Pnt2d v2(pt->x, pt->y);
   if (!v1.IsEqual(v2, Precision::Confusion())) {
       Handle(Geom2d_TrimmedCurve) lseg = GCE2d_MakeSegment(v1,v2);
       TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(lseg , dc->surf);
       dc->Edges.push_back(edge);
       dc->LastVert = *pt;
   }
   return 0;
}
   
// quad_cb called for quadratic (conic) BCurve segment in the current contour 
// (ie V-C-V in TTF fonts). BCurve(LastVert -- pt0 -- pt1)
static int quad_cb(const FT_Vector* pt0, const FT_Vector* pt1, void* p) {
   FTDC_Ctx* dc = (FTDC_Ctx*) p;
   TColgp_Array1OfPnt2d Poles(1,3);
   gp_Pnt2d v1(dc->LastVert.x, dc->LastVert.y);
   gp_Pnt2d c1(pt0->x, pt0->y);
   gp_Pnt2d v2(pt1->x, pt1->y);
   Poles.SetValue(1, v1);
   Poles.SetValue(2, c1);
   Poles.SetValue(3, v2);
   Handle(Geom2d_BezierCurve) bcseg = new Geom2d_BezierCurve(Poles);
   TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(bcseg , dc->surf);
   dc->Edges.push_back(edge);
   dc->LastVert = *pt1;
   return 0;
}

// cubic_cb called for cubic BCurve segment in the current contour (ie V-C-C-V in
// Type 1 fonts). BCurve(LastVert -- pt0 -- pt1 -- pt2)
static int cubic_cb(const FT_Vector* pt0, const FT_Vector* pt1, const FT_Vector* pt2, void* p) {
   FTDC_Ctx* dc = (FTDC_Ctx*) p;
   TColgp_Array1OfPnt2d Poles(1,4);
   gp_Pnt2d v1(dc->LastVert.x, dc->LastVert.y);
   gp_Pnt2d c1(pt0->x, pt0->y);
   gp_Pnt2d c2(pt1->x, pt1->y);
   gp_Pnt2d v2(pt2->x, pt2->y);
   Poles.SetValue(1, v1);
   Poles.SetValue(2, c1);
   Poles.SetValue(3, c2);
   Poles.SetValue(4, v2);
   Handle(Geom2d_BezierCurve) bcseg = new Geom2d_BezierCurve(Poles);
   TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(bcseg , dc->surf);
   dc->Edges.push_back(edge);
   dc->LastVert = *pt2;
   return 0;
}

// FT Callbacks structure
static FT_Outline_Funcs FTcbFuncs = {
   (FT_Outline_MoveToFunc)move_cb,
   (FT_Outline_LineToFunc)line_cb,
   (FT_Outline_ConicToFunc)quad_cb,
   (FT_Outline_CubicToFunc)cubic_cb,
   0, 0 // not needed for FC
};


// get kerning values for this char pair
//TODO: should check FT_HASKERNING flag? returns (0,0) if no kerning?
FT_Vector getKerning(FT_Face FTFont, UNICHAR lc, UNICHAR rc) {
   FT_Vector retXY;
   FT_Error error;        
   std::stringstream ErrorMsg;  
   FT_Vector ftKern;
   FT_UInt lcx = FT_Get_Char_Index(FTFont, lc);    
   FT_UInt rcx = FT_Get_Char_Index(FTFont, rc);
   error = FT_Get_Kerning(FTFont,lcx,rcx,FT_KERNING_DEFAULT,&ftKern);
   if(error) {
      ErrorMsg << "FT_Get_Kerning failed: " << error;
      throw std::runtime_error(ErrorMsg.str());
   }
   retXY.x = ftKern.x;
   retXY.y = ftKern.y;
   return(retXY);
}

// Make a TopoDS_Wire from a list of TopoDS_Edges
TopoDS_Wire edgesToWire(std::vector<TopoDS_Edge> Edges) {
    TopoDS_Wire occwire;
    std::vector<TopoDS_Edge>::iterator iEdge;
    BRepBuilderAPI_MakeWire mkWire;
    for (iEdge = Edges.begin(); iEdge != Edges.end(); ++iEdge){
        mkWire.Add(*iEdge);
        if (!mkWire.IsDone()) {
            printf("FT2FC Trace edgesToWire failed to add wire\n");
        }
    }
    occwire = mkWire.Wire();
    BRepLib::BuildCurves3d(occwire);
    return(occwire);
}


#endif //#ifdef FCUseFreeType
