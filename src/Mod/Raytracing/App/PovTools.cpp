/***************************************************************************
 *   Copyright (c) Jürgen Riegel          (juergen.riegel@web.de) 2005     *
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

#include "PreCompiled.h"

#ifndef _PreComp_
# include <BRep_Tool.hxx>
# include <BRepMesh_IncrementalMesh.hxx>
# include <GeomAPI_ProjectPointOnSurf.hxx>
# include <GeomLProp_SLProps.hxx>
# include <Poly_Triangulation.hxx>
# include <TopExp_Explorer.hxx>
# include <TopoDS.hxx>
# include <TopoDS_Face.hxx>
# include <sstream>
#endif

#include <Base/Console.h>
#include <Base/Exception.h>
#include <Base/Sequencer.h>
#include <App/ComplexGeoData.h>


#include "PovTools.h"

using Base::Console;

using namespace Raytracing;
using namespace std;

#include "PovTools.h"

//#include "TempCamera.inc"
//camera {
//  location  CamPos
//  look_at   LookAt
//  sky       Up
//  angle     45
//}

std::string PovTools::getCamera(const CamDef& Cam, int width, int height)
{
    std::stringstream out;
    out << "// declares position and view direction\n" << endl
        << "// Generated by FreeCAD (http://www.freecadweb.org/)" << endl

        // writing Camera positions
        << "#declare cam_location =  <" << Cam.CamPos.X() <<"," << Cam.CamPos.Z() <<"," << Cam.CamPos.Y()<<">;" << endl

        // writing lookat
        << "#declare cam_look_at  = <" << Cam.LookAt.X()  <<"," << Cam.LookAt.Z()    <<"," << Cam.LookAt.Y()    <<">;"<< endl

        // writing the Up Vector
        << "#declare cam_sky      = <" << Cam.Up.X()  <<"," << Cam.Up.Z()    <<"," << Cam.Up.Y()    <<">;"<< endl

        // array of zoom factors
        << "#declare cam_angle    = 45; " << endl
        // instance of the camera
        << "camera {" << endl
        << "  location  cam_location" << endl
        << "  look_at   cam_look_at" << endl
        << "  sky       cam_sky" << endl
        << "  angle     cam_angle " << endl
        << "  right x*" << width << "/" << height << endl
        << "}"<< endl;
    return out.str();
}

void PovTools::writeCamera(const char* FileName, const CamDef& Cam)
{
    std::vector<CamDef> vCam;
    vCam.push_back(Cam);
    writeCameraVec(FileName,vCam);
}


void PovTools::writeCameraVec(const char* FileName, const std::vector<CamDef>& CamVec)
{
    std::stringstream out;
    std::vector<CamDef>::const_iterator It;
    out << "// declares position and view directions\n"
        << "// Generated by FreeCAD (http://www.freecadweb.org/)\n\n"
        << "// Total number of camera positions\n"
        << "#declare nCamPos = " << CamVec.size() << ";\n\n";

    // writing Camera positions
    out << "// Array of positions\n"
        << "#declare  CamPos = array[" << CamVec.size() << "] {\n";
    for (It = CamVec.begin(); It != CamVec.end(); ++It)
        out << "   <" << It->CamPos.X()  <<"," << It->CamPos.Z()    <<"," << It->CamPos.Y()    <<">,\n";
    out << "};\n"
        // writing Camera Direction vector
        << "// Array of Directions (only for special calculations)\n"
        << "#declare  CamDir = array[" << CamVec.size() << "] {\n";

    for (It = CamVec.begin(); It != CamVec.end(); ++It)
        out << "   <" << It->CamDir.X()  <<"," << It->CamDir.Z()    <<"," << It->CamDir.Y()    <<">,\n";
    out << "};\n"
        // writing lookat
        << "// Array of Look At positions\n"
        << "#declare  LookAt = array[" << CamVec.size() << "] {\n";

    for (It = CamVec.begin(); It != CamVec.end(); ++It)
        out << "   <" << It->LookAt.X()  <<"," << It->LookAt.Z()    <<"," << It->LookAt.Y()    <<">,\n";
    out << "};\n"
        // writing the Up Vector
        << "// // Array of up vectors\n"
        << "#declare  Up = array[" << CamVec.size() << "] {\n";

    for (It = CamVec.begin(); It != CamVec.end(); ++It)
        out << "   <" << It->Up.X()  <<"," << It->Up.Z()    <<"," << It->Up.Y()    <<">,\n";
    out << "};\n"
        // array of zoom factors
        << "// // Array of up vectors\n"
        << "#declare  CamZoom = array[" << CamVec.size() << "] {\n";

    for (It = CamVec.begin(); It != CamVec.end(); ++It)
        out << "   45,\n";
    out << "};\n";


    // open the file and write
    Base::ofstream fout(FileName);
    fout <<  out.str() << endl;
    fout.close();
}

void PovTools::writeData(const char *FileName, const char *PartName,
                         const Data::ComplexGeoData* data, float /*fMeshDeviation*/)
{
    // open the file and write
    Base::ofstream fout(FileName);
    // write the file
    fout <<  "// Written by FreeCAD http://www.freecadweb.org/" << endl;

    unsigned long count = data->countSubElements("Face");
    for (unsigned long i=0; i<count; i++) {
        std::vector<Base::Vector3d> points;
        std::vector<Base::Vector3d> normals;
        std::vector<Data::ComplexGeoData::Facet> facets;
        Data::Segment* segm = data->getSubElement("Face", i);
        data->getFacesFromSubelement(segm, points, normals, facets);
        delete segm;

        // writing per face header
        fout << "// element number" << i << " +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl
             << "#declare " << PartName << i << " = mesh2{" << endl
             << "  vertex_vectors {" << endl
             << "    " << points.size() << "," << endl;

        // writing vertices
        for (std::vector<Base::Vector3d>::iterator it = points.begin(); it != points.end(); ++it) {
            fout << "    <"
                 << it->x << ","
                 << it->y << ","
                 << it->z << ">,"
                 << endl;
        }

        // writing per vertex normals
        fout << "  }" << endl
             << "  normal_vectors {" << endl
             << "    " << normals.size() << "," << endl;

        for (std::vector<Base::Vector3d>::iterator it = normals.begin(); it != normals.end(); ++it) {
            fout << "    <" 
                 << it->x << ","
                 << it->y << ","
                 << it->z << ">,"
                 << endl;
        }

        // writing triangle indices
        fout << "  }" << endl
             << "  face_indices {" << endl
             << "    " << facets.size() << "," << endl;
        for (std::vector<Data::ComplexGeoData::Facet>::iterator it = facets.begin(); it != facets.end(); ++it) {
            fout << "    <" << it->I1 << ","<< it->I3 << ","<< it->I2 << ">," << endl;
        }

        // end of face
        fout << "  }" << endl
             << "} // end of element" << i << endl << endl;
    }

    fout << endl << endl << "// Declare all together +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl
         << "#declare " << PartName << " = union {" << endl;
    for (unsigned long i=1; i < count; i++) {
        fout << "mesh2{ " << PartName << i << "}" << endl;
    }
    fout << "}" << endl;
    fout.close();
}

void PovTools::writeShape(const char *FileName, const char *PartName,
                          const TopoDS_Shape& Shape, float fMeshDeviation)
{
    // open the file and write
    Base::ofstream fout(FileName);
    writeShape(fout,PartName,Shape,fMeshDeviation);
    fout.close();
}

void PovTools::writeShape(std::ostream &out, const char *PartName,
                          const TopoDS_Shape& Shape, float fMeshDeviation)
{
    printf("Meshing with Deviation: %f\n",fMeshDeviation);

    TopExp_Explorer ex;
    BRepMesh_IncrementalMesh MESH(Shape,fMeshDeviation);


    // counting faces and start sequencer
    int l = 1;
    for (ex.Init(Shape, TopAbs_FACE); ex.More(); ex.Next(),l++) {}
    Base::SequencerLauncher seq("Writing file", l);

    // write the file
    out <<  "// Written by FreeCAD http://www.freecadweb.org/" << endl;
    l = 1;
    for (ex.Init(Shape, TopAbs_FACE); ex.More(); ex.Next(),l++) {

        // get the shape and mesh it
        const TopoDS_Face& aFace = TopoDS::Face(ex.Current());

        // this block mesh the face and transfers it in a C array of vertices and face indexes
        Standard_Integer nbNodesInFace,nbTriInFace;
        gp_Vec* vertices=0;
        gp_Vec* vertexnormals=0;
        long* cons=0;

        transferToArray(aFace,&vertices,&vertexnormals,&cons,nbNodesInFace,nbTriInFace);

        if (!vertices) break;
        // writing per face header
        out << "// face number" << l << " +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl
        << "#declare " << PartName << l << " = mesh2{" << endl
        << "  vertex_vectors {" << endl
        << "    " << nbNodesInFace << "," << endl;
        // writing vertices
        for (int i=0; i < nbNodesInFace; i++) {
            out << "    <" << vertices[i].X() << ","
            << vertices[i].Z() << ","
            << vertices[i].Y() << ">,"
            << endl;
        }
        out << "  }" << endl
        // writing per vertex normals
        << "  normal_vectors {" << endl
        << "    " << nbNodesInFace << "," << endl;
        for (int j=0; j < nbNodesInFace; j++) {
            out << "    <" << vertexnormals[j].X() << ","
            << vertexnormals[j].Z() << ","
            << vertexnormals[j].Y() << ">,"
            << endl;
        }

        out << "  }" << endl
        // writing triangle indices
        << "  face_indices {" << endl
        << "    " << nbTriInFace << "," << endl;
        for (int k=0; k < nbTriInFace; k++) {
            out << "    <" << cons[3*k] << ","<< cons[3*k+2] << ","<< cons[3*k+1] << ">," << endl;
        }
        // end of face
        out << "  }" << endl
        << "} // end of Face"<< l << endl << endl;

        delete [] vertexnormals;
        delete [] vertices;
        delete [] cons;

        seq.next();

    } // end of face loop


    out << endl << endl << "// Declare all together +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl
    << "#declare " << PartName << " = union {" << endl;
    for (int i=1; i < l; i++) {
        out << "mesh2{ " << PartName << i << "}" << endl;
    }
    out << "}" << endl;
}

void PovTools::writeShapeCSV(const char *FileName,
                             const TopoDS_Shape& Shape,
                             float fMeshDeviation,
                             float fLength)
{
    const char cSeperator = ',';

    printf("Meshing with Deviation: %f\n",fMeshDeviation);

    TopExp_Explorer ex;
    BRepMesh_IncrementalMesh MESH(Shape,fMeshDeviation);

    // open the file and write
    std::ofstream fout(FileName);

    // counting faces and start sequencer
    int l = 1;
    for (ex.Init(Shape, TopAbs_FACE); ex.More(); ex.Next(),l++) {}
    Base::SequencerLauncher seq("Writing file", l);

    // write the file
    l = 1;
    for (ex.Init(Shape, TopAbs_FACE); ex.More(); ex.Next(),l++) {

        // get the shape and mesh it
        const TopoDS_Face& aFace = TopoDS::Face(ex.Current());

        // this block mesh the face and transfers it in a C array of vertices and face indexes
        Standard_Integer nbNodesInFace,nbTriInFace;
        gp_Vec* vertices=0;
        gp_Vec* vertexnormals=0;
        long* cons=0;

        transferToArray(aFace,&vertices,&vertexnormals,&cons,nbNodesInFace,nbTriInFace);

        if (!vertices) break;
        // writing per face header
        // writing vertices
        for (int i=0; i < nbNodesInFace; i++) {
            fout << vertices[i].X() << cSeperator
            << vertices[i].Z() << cSeperator
            << vertices[i].Y() << cSeperator
            << vertexnormals[i].X() * fLength <<cSeperator
            << vertexnormals[i].Z() * fLength <<cSeperator
            << vertexnormals[i].Y() * fLength <<cSeperator
            << endl;
        }

        delete [] vertexnormals;
        delete [] vertices;
        delete [] cons;

        seq.next();

    } // end of face loop

    fout.close();
}

void PovTools::transferToArray(const TopoDS_Face& aFace,gp_Vec** vertices,gp_Vec** vertexnormals, long** cons,int &nbNodesInFace,int &nbTriInFace )
{
    TopLoc_Location aLoc;

    // doing the meshing and checking the result
    //BRepMesh_IncrementalMesh MESH(aFace,fDeflection);
    Handle(Poly_Triangulation) aPoly = BRep_Tool::Triangulation(aFace,aLoc);
    if (aPoly.IsNull()) {
        printf("Empty face trianglutaion\n");
        nbNodesInFace =0;
        nbTriInFace = 0;
        vertices = 0l;
        cons = 0l;
        return;
    }

    // geting the transformation of the shape/face
    gp_Trsf myTransf;
    Standard_Boolean identity = true;
    if (!aLoc.IsIdentity())  {
        identity = false;
        myTransf = aLoc.Transformation();
    }

    Standard_Integer i;
    // geting size and create the array
    nbNodesInFace = aPoly->NbNodes();
    nbTriInFace = aPoly->NbTriangles();
    *vertices = new gp_Vec[nbNodesInFace];
    *vertexnormals = new gp_Vec[nbNodesInFace];
    for (i=0; i < nbNodesInFace; i++) {
        (*vertexnormals)[i]= gp_Vec(0.0,0.0,0.0);
    }

    *cons = new long[3*(nbTriInFace)+1];

    // check orientation
    TopAbs_Orientation orient = aFace.Orientation();

    // cycling through the poly mesh
    const Poly_Array1OfTriangle& Triangles = aPoly->Triangles();
    const TColgp_Array1OfPnt& Nodes = aPoly->Nodes();
    for (i=1; i<=nbTriInFace; i++) {
        // Get the triangle
        Standard_Integer N1,N2,N3;
        Triangles(i).Get(N1,N2,N3);

        // change orientation of the triangles
        if ( orient != TopAbs_FORWARD )
        {
            Standard_Integer tmp = N1;
            N1 = N2;
            N2 = tmp;
        }

        gp_Pnt V1 = Nodes(N1);
        gp_Pnt V2 = Nodes(N2);
        gp_Pnt V3 = Nodes(N3);

        // transform the vertices to the place of the face
        if (!identity) {
            V1.Transform(myTransf);
            V2.Transform(myTransf);
            V3.Transform(myTransf);
        }

        // Calculate triangle normal
        gp_Vec v1(V1.X(),V1.Y(),V1.Z()),v2(V2.X(),V2.Y(),V2.Z()),v3(V3.X(),V3.Y(),V3.Z());
        gp_Vec Normal = (v2-v1)^(v3-v1);

        //Standard_Real Area = 0.5 * Normal.Magnitude();

        // add the triangle normal to the vertex normal for all points of this triangle
        (*vertexnormals)[N1-1] += gp_Vec(Normal.X(),Normal.Y(),Normal.Z());
        (*vertexnormals)[N2-1] += gp_Vec(Normal.X(),Normal.Y(),Normal.Z());
        (*vertexnormals)[N3-1] += gp_Vec(Normal.X(),Normal.Y(),Normal.Z());

        (*vertices)[N1-1].SetX((float)(V1.X()));
        (*vertices)[N1-1].SetY((float)(V1.Y()));
        (*vertices)[N1-1].SetZ((float)(V1.Z()));
        (*vertices)[N2-1].SetX((float)(V2.X()));
        (*vertices)[N2-1].SetY((float)(V2.Y()));
        (*vertices)[N2-1].SetZ((float)(V2.Z()));
        (*vertices)[N3-1].SetX((float)(V3.X()));
        (*vertices)[N3-1].SetY((float)(V3.Y()));
        (*vertices)[N3-1].SetZ((float)(V3.Z()));

        int j = i - 1;
        N1--;
        N2--;
        N3--;
        (*cons)[3*j] = N1;
        (*cons)[3*j+1] = N2;
        (*cons)[3*j+2] = N3;
    }

    // normalize all vertex normals
    for (i=0; i < nbNodesInFace; i++) {

        gp_Dir clNormal;

        try {
            Handle(Geom_Surface) Surface = BRep_Tool::Surface(aFace);

            gp_Pnt vertex((*vertices)[i].XYZ());
//     gp_Pnt vertex((*vertices)[i][0], (*vertices)[i][1], (*vertices)[i][2]);
            GeomAPI_ProjectPointOnSurf ProPntSrf(vertex, Surface);
            Standard_Real fU, fV;
            ProPntSrf.Parameters(1, fU, fV);

            GeomLProp_SLProps clPropOfFace(Surface, fU, fV, 2, gp::Resolution());

            clNormal = clPropOfFace.Normal();
            gp_Vec temp = clNormal;
            //printf("unterschied:%.2f",temp.dot((*vertexnormals)[i]));
            if ( temp * (*vertexnormals)[i] < 0 )
                temp = -temp;
            (*vertexnormals)[i] = temp;

        }
        catch (...) {
        }

        (*vertexnormals)[i].Normalize();
    }
}
