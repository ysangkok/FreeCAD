/***************************************************************************
 *   Copyright (c) Stefan Tröger          (stefantroeger@gmx.net) 2016     *
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
# include <cassert>
# include <algorithm>
#endif

#include "DocumentObjectExtension.h"
//#include "DocumentObjectExtensionPy.h"

using namespace App;

EXTENSION_PROPERTY_SOURCE(App::DocumentObjectExtension, App::Extension)

DocumentObjectExtension::DocumentObjectExtension() 
{
    initExtensionType(App::DocumentObjectExtension::getExtensionClassTypeId());
}

DocumentObjectExtension::~DocumentObjectExtension()
{

}

short int DocumentObjectExtension::extensionMustExecute(void) {
    
    return 0;
}

App::DocumentObjectExecReturn* DocumentObjectExtension::extensionExecute(void) {
    
    return App::DocumentObject::StdReturn;
}

void DocumentObjectExtension::onExtendedSettingDocument() {
    
}

void DocumentObjectExtension::onExtendedSetupObject() {
    
}

void DocumentObjectExtension::onExtendedUnsetupObject() {
    
}

const DocumentObject* DocumentObjectExtension::getExtendedObject() const {

    assert(getExtendedContainer()->isDerivedFrom(DocumentObject::getClassTypeId())); 
    return static_cast<const DocumentObject*>(getExtendedContainer());
}

DocumentObject* DocumentObjectExtension::getExtendedObject() {

    assert(getExtendedContainer()->isDerivedFrom(DocumentObject::getClassTypeId())); 
    return static_cast<DocumentObject*>(getExtendedContainer());
}
