/***************************************************************************
 *   Copyright (c) 2009 Juergen Riegel (FreeCAD@juergen-riegel.net)        *
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
#ifdef __GNUC__
# include <unistd.h>
#endif

#include <sstream>

#include <App/Application.h>
#include <App/Document.h>
//#include <App/DocumentObjectPy.h>
#include <App/DocumentObject.h>
//#include <Base/Interpreter.h>
//#include <CXX/Objects.hxx>

#include "Selection.h"
#include "SelectionFilter.h"
//#include "SelectionFilterPy.h"
#include "Application.h"

using namespace Gui;

// suppress annoying warnings from generated source files
#ifdef _MSC_VER
# pragma warning(disable : 4003)
# pragma warning(disable : 4018)
# pragma warning(disable : 4065)
# pragma warning(disable : 4335) // disable MAC file format warning on VC
#endif



SelectionFilterGate::SelectionFilterGate(const char* filter)
{
    Filter = new SelectionFilter(filter);
}

SelectionFilterGate::SelectionFilterGate(SelectionFilter* filter)
{
    Filter = filter;
}

SelectionFilterGate::~SelectionFilterGate()
{
    delete Filter;
}

bool SelectionFilterGate::allow(App::Document* /*pDoc*/, App::DocumentObject*pObj, const char*sSubName)
{
    return Filter->test(pObj,sSubName);
}

// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------

SelectionFilter::SelectionFilter(const char* filter)
  : Ast(0)
{
    setFilter(filter);
}

SelectionFilter::SelectionFilter(const std::string& filter)
  : Ast(0)
{
    setFilter(filter.c_str());
}

void SelectionFilter::setFilter(const char* filter)
{
    if (!filter || filter[0] == 0) {
        delete Ast;
        Ast = 0;
        Filter.clear();
    }
    else {
        Filter = filter;
        if (!parse())
            throw Base::ParserError(Errors.c_str());
    }
}

SelectionFilter::~SelectionFilter()
{
}

bool SelectionFilter::match(void)
{
    if (!Ast)
        return false;
    Result.clear();

    for (std::vector< Node_Object *>::iterator it= Ast->Objects.begin();it!=Ast->Objects.end();++it) {
        int min;
        int max;

        if ((*it)->Slice) {
            min          = (*it)->Slice->Min;
            max          = (*it)->Slice->Max;
        }
        else {
            min          = 1;
            max          = 1;
        }

        std::vector<Gui::SelectionObject> temp = Gui::Selection().getSelectionEx(0,(*it)->ObjectType);

        // test if subnames present
        if ((*it)->SubName.empty()) {
            // if no subnames the count of the object get tested
            if ((int)temp.size()<min || (int)temp.size()>max)
                return false;
        }
        else {
            // if subnames present count all subs over the selected object of type
            int subCount=0;
            for (std::vector<Gui::SelectionObject>::const_iterator it2=temp.begin();it2!=temp.end();++it2) {
                const std::vector<std::string>& subNames = it2->getSubNames();
                if (subNames.empty())
                    return false;
                for (std::vector<std::string>::const_iterator it3=subNames.begin();it3!=subNames.end();++it3) {
                    if (it3->find((*it)->SubName) != 0)
                        return false;
                }
                subCount += subNames.size();
            }
            if (subCount<min || subCount>max)
                return false;
        }
        Result.push_back(temp);
    }
    return true;
}

bool SelectionFilter::test(App::DocumentObject*pObj, const char*sSubName)
{
    if (!Ast)
        return false;

    for (std::vector< Node_Object *>::iterator it= Ast->Objects.begin();it!=Ast->Objects.end();++it) {
        if (pObj->getTypeId().isDerivedFrom((*it)->ObjectType)) {
            if (!sSubName)
                return true;
            if ((*it)->SubName.empty())
                return true;
            if (std::string(sSubName).find((*it)->SubName) == 0)
                return true;
        }
    }
    return false;
}

void SelectionFilter::addError(const char* e)
{
    Errors+=e;
    Errors += '\n';
}

// ----------------------------------------------------------------------------


// === Parser & Scanner stuff ===============================================

// include the Scanner and the Parser for the filter language

SelectionFilter* ActFilter=0;
Node_Block *TopBlock=0;

// error func
void yyerror(char *errorinfo)
	{  ActFilter->addError(errorinfo);  }


// for VC9 (isatty and fileno not supported anymore)
#ifdef _MSC_VER
int isatty (int i) {return _isatty(i);}
int fileno(FILE *stream) {return _fileno(stream);}
#endif

namespace SelectionParser {

// show the parser the lexer method
#define yylex SelectionFilterlex
int SelectionFilterlex(void);

// Parser, defined in SelectionFilter.y
#include "SelectionFilter.tab.c"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Scanner, defined in SelectionFilter.l
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wsign-compare"
# pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#elif defined (__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#include "lex.SelectionFilter.c"
#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined (__GNUC__)
# pragma GCC diagnostic pop
#endif
#endif // DOXYGEN_SHOULD_SKIP_THIS
}

bool SelectionFilter::parse(void)
{
    Errors = "";
    SelectionParser::YY_BUFFER_STATE my_string_buffer = SelectionParser::SelectionFilter_scan_string (Filter.c_str());
    // be aware that this parser is not reentrant! Don't use with Threats!!!
    assert(!ActFilter);
    ActFilter = this;
    /*int my_parse_result =*/ SelectionParser::yyparse();
    ActFilter = 0;
    Ast = TopBlock;
    TopBlock = 0;
    SelectionParser::SelectionFilter_delete_buffer (my_string_buffer);

    if (Errors.empty()) {
        return true;
    }
    else {
        return false;
    }
}
