/***************************************************************************
 *   Copyright (c) Jürgen Riegel          (juergen.riegel@web.de) 2002     *
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
#   include <assert.h>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......
//#include <CXX/Objects.hxx>
#include <Base/Exception.h>
#include <Base/Reader.h>
#include <Base/Writer.h>
//#include <Base/Console.h>

#include "DocumentObject.h"
//#include "DocumentObjectPy.h"
#include "Document.h"

#include "PropertyLinks.h"

using namespace App;
using namespace Base;
using namespace std;

//**************************************************************************
//**************************************************************************
// PropertyLink
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyLink , App::Property)

//**************************************************************************
// Construction/Destruction


PropertyLink::PropertyLink()
:_pcLink(0)
{

}


PropertyLink::~PropertyLink()
{

}

//**************************************************************************
// Base class implementer

void PropertyLink::setValue(App::DocumentObject * lValue)
{
    aboutToSetValue();
#ifndef USE_OLD_DAG
    // maintain the back link in the DocumentObject class
    if(_pcLink)
        _pcLink->_removeBackLink(static_cast<DocumentObject*>(getContainer()));
    if(lValue)
        lValue->_addBackLink(static_cast<DocumentObject*>(getContainer()));
#endif
    _pcLink=lValue;
    hasSetValue();
}

App::DocumentObject * PropertyLink::getValue(void) const
{
    return _pcLink;
}

App::DocumentObject * PropertyLink::getValue(Base::Type t) const
{
    return (_pcLink && _pcLink->getTypeId().isDerivedFrom(t)) ? _pcLink : 0;
}

void PropertyLink::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Link value=\"" <<  (_pcLink?_pcLink->getNameInDocument():"") <<"\"/>" << std::endl;
}

void PropertyLink::Restore(Base::XMLReader &reader)
{
    // read my element
    reader.readElement("Link");
    // get the value of my attribute
    std::string name = reader.getAttribute("value");

    // Property not in a DocumentObject!
    assert(getContainer()->getTypeId().isDerivedFrom(App::DocumentObject::getClassTypeId()) );

    if (name != "") {
        DocumentObject* parent = static_cast<DocumentObject*>(getContainer());

        App::Document* document = parent->getDocument();
        DocumentObject* object = document ? document->getObject(name.c_str()) : 0;
        if (!object) {
            if (reader.isVerbose()) {
                printf("Lost link to '%s' while loading, maybe "
                                        "an object was not loaded correctly\n",name.c_str());
            }
        }
        else if (parent == object) {
            if (reader.isVerbose()) {
                printf("Object '%s' links to itself, nullify it\n",name.c_str());
            }
            object = 0;
        }

        setValue(object);
    }
    else {
        setValue(0);
    }
}

Property *PropertyLink::Copy(void) const
{
    PropertyLink *p= new PropertyLink();
    p->_pcLink = _pcLink;
    return p;
}

void PropertyLink::Paste(const Property &from)
{
    if(!from.isDerivedFrom(PropertyLink::getClassTypeId()))
        throw Base::TypeError("Incompatible property to paste to");

    setValue(static_cast<const PropertyLink&>(from)._pcLink);
}

//**************************************************************************
// PropertyLinkList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyLinkList, App::PropertyLists)

//**************************************************************************
// Construction/Destruction


PropertyLinkList::PropertyLinkList()
{

}

PropertyLinkList::~PropertyLinkList()
{

}

void PropertyLinkList::setSize(int newSize)
{
    _lValueList.resize(newSize);
}

int PropertyLinkList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

void PropertyLinkList::setValue(DocumentObject* lValue)
{
#ifndef USE_OLD_DAG   
    //maintain the back link in the DocumentObject class
    for(auto *obj : _lValueList)
        obj->_removeBackLink(static_cast<DocumentObject*>(getContainer()));
    if(lValue)
        lValue->_addBackLink(static_cast<DocumentObject*>(getContainer()));
#endif
    
    if (lValue){
        aboutToSetValue();
        _lValueList.resize(1);
        _lValueList[0] = lValue;
        hasSetValue();
    }
    else {
        aboutToSetValue();
        _lValueList.clear();
        hasSetValue();
    }
}

void PropertyLinkList::setValues(const std::vector<DocumentObject*>& lValue)
{
    aboutToSetValue();
#ifndef USE_OLD_DAG
    //maintain the back link in the DocumentObject class
    for(auto *obj : _lValueList)
        obj->_removeBackLink(static_cast<DocumentObject*>(getContainer()));
    for(auto *obj : lValue)
        obj->_addBackLink(static_cast<DocumentObject*>(getContainer()));
#endif
    _lValueList = lValue;
    hasSetValue();
}

void PropertyLinkList::Save(Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<LinkList count=\"" << getSize() << "\">" << endl;
    writer.incInd();
    for (int i = 0; i<getSize(); i++) {
        DocumentObject* obj = _lValueList[i];
        if (obj)
            writer.Stream() << writer.ind() << "<Link value=\"" << obj->getNameInDocument() << "\"/>" << endl;
        else
            writer.Stream() << writer.ind() << "<Link value=\"\"/>" << endl;
    }

    writer.decInd();
    writer.Stream() << writer.ind() << "</LinkList>" << endl;
}

void PropertyLinkList::Restore(Base::XMLReader &reader)
{
    // read my element
    reader.readElement("LinkList");
    // get the value of my attribute
    int count = reader.getAttributeAsInteger("count");
    assert(getContainer()->getTypeId().isDerivedFrom(App::DocumentObject::getClassTypeId()));

    std::vector<DocumentObject*> values;
    values.reserve(count);
    for (int i = 0; i < count; i++) {
        reader.readElement("Link");
        std::string name = reader.getAttribute("value");
        // In order to do copy/paste it must be allowed to have defined some
        // referenced objects in XML which do not exist anymore in the new
        // document. Thus, we should silently ingore this.
        // Property not in an object!
        DocumentObject* father = static_cast<DocumentObject*>(getContainer());
        App::Document* document = father->getDocument();
        DocumentObject* child = document ? document->getObject(name.c_str()) : 0;
        if (child)
            values.push_back(child);
        else if (reader.isVerbose())
            printf("Lost link to '%s' while loading, maybe "
            "an object was not loaded correctly\n", name.c_str());
    }

    reader.readEndElement("LinkList");

    // assignment
    setValues(values);
}

Property *PropertyLinkList::Copy(void) const
{
    PropertyLinkList *p = new PropertyLinkList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyLinkList::Paste(const Property &from)
{
    setValues(dynamic_cast<const PropertyLinkList&>(from)._lValueList);
}

unsigned int PropertyLinkList::getMemSize(void) const
{
    return static_cast<unsigned int>(_lValueList.size() * sizeof(App::DocumentObject *));
}

//**************************************************************************
// PropertyLinkSub
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyLinkSub , App::Property)

//**************************************************************************
// Construction/Destruction


PropertyLinkSub::PropertyLinkSub()
:_pcLinkSub(0)
{

}


PropertyLinkSub::~PropertyLinkSub()
{

}

//**************************************************************************
// Base class implementer

void PropertyLinkSub::setValue(App::DocumentObject * lValue, const std::vector<std::string> &SubList)
{
    aboutToSetValue();
#ifndef USE_OLD_DAG
    if (_pcLinkSub)
        _pcLinkSub->_removeBackLink(static_cast<App::DocumentObject*>(getContainer()));
    if (lValue)
        lValue->_addBackLink(static_cast<App::DocumentObject*>(getContainer()));
#endif
    _pcLinkSub=lValue;
    _cSubList = SubList;
    hasSetValue();
}

App::DocumentObject * PropertyLinkSub::getValue(void) const
{
    return _pcLinkSub;
}

const std::vector<std::string>& PropertyLinkSub::getSubValues(void) const
{
    return _cSubList;
}

std::vector<std::string> PropertyLinkSub::getSubValuesStartsWith(const char* starter) const
{
    std::vector<std::string> temp;
    for(std::vector<std::string>::const_iterator it=_cSubList.begin();it!=_cSubList.end();++it)
        if(strncmp(starter,it->c_str(),strlen(starter))==0)
            temp.push_back(*it);
    return temp;
}

App::DocumentObject * PropertyLinkSub::getValue(Base::Type t) const
{
    return (_pcLinkSub && _pcLinkSub->getTypeId().isDerivedFrom(t)) ? _pcLinkSub : 0;
}

void PropertyLinkSub::Save (Base::Writer &writer) const
{
    const char* internal_name = "";
    // it can happen that the object is still alive but is not part of the document anymore and thus
    // returns 0
    if (_pcLinkSub && _pcLinkSub->getNameInDocument())
        internal_name = _pcLinkSub->getNameInDocument();
    writer.Stream() << writer.ind() << "<LinkSub value=\"" <<  internal_name <<"\" count=\"" <<  _cSubList.size() <<"\">" << std::endl;
    writer.incInd();
    for(unsigned int i = 0;i<_cSubList.size(); i++)
        writer.Stream() << writer.ind() << "<Sub value=\"" <<  _cSubList[i]<<"\"/>" << endl;
    writer.decInd();
    writer.Stream() << writer.ind() << "</LinkSub>" << endl ;
}

void PropertyLinkSub::Restore(Base::XMLReader &reader)
{
    // read my element
    reader.readElement("LinkSub");
    // get the values of my attributes
    std::string name = reader.getAttribute("value");
    int count = reader.getAttributeAsInteger("count");

    // Property not in a DocumentObject!
    assert(getContainer()->getTypeId().isDerivedFrom(App::DocumentObject::getClassTypeId()) );

    std::vector<std::string> values(count);
    for (int i = 0; i < count; i++) {
        reader.readElement("Sub");
        values[i] = reader.getAttribute("value");
    }

    reader.readEndElement("LinkSub");

    DocumentObject *pcObject;
    if (!name.empty()) {
        App::Document* document = static_cast<DocumentObject*>(getContainer())->getDocument();
        pcObject = document ? document->getObject(name.c_str()) : 0;
        if (!pcObject) {
            if (reader.isVerbose()) {
                printf("Lost link to '%s' while loading, maybe "
                                        "an object was not loaded correctly\n",name.c_str());
            }
        }
        setValue(pcObject,values);
    }
    else {
       setValue(0);
    }
}

Property *PropertyLinkSub::Copy(void) const
{
    PropertyLinkSub *p= new PropertyLinkSub();
    p->_pcLinkSub = _pcLinkSub;
    p->_cSubList = _cSubList;
    return p;
}

void PropertyLinkSub::Paste(const Property &from)
{
    setValue(dynamic_cast<const PropertyLinkSub&>(from)._pcLinkSub, dynamic_cast<const PropertyLinkSub&>(from)._cSubList);
}

//**************************************************************************
// PropertyLinkSubList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyLinkSubList , App::PropertyLists)

//**************************************************************************
// Construction/Destruction


PropertyLinkSubList::PropertyLinkSubList()
{

}

PropertyLinkSubList::~PropertyLinkSubList()
{

}

void PropertyLinkSubList::setSize(int newSize)
{
    _lValueList.resize(newSize);
    _lSubList  .resize(newSize);
}

int PropertyLinkSubList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

void PropertyLinkSubList::setValue(DocumentObject* lValue,const char* SubName)
{
#ifndef USE_OLD_DAG
    //maintain backlinks
    for(auto *obj : _lValueList)
        obj->_removeBackLink(static_cast<DocumentObject*>(getContainer()));
    if (lValue)
        lValue->_addBackLink(static_cast<DocumentObject*>(getContainer()));
#endif
    
    if (lValue) {
        aboutToSetValue();
        _lValueList.resize(1);
        _lValueList[0]=lValue;
        _lSubList.resize(1);
        _lSubList[0]=SubName;
        hasSetValue();
    }
    else {
        aboutToSetValue();
        _lValueList.clear();
        _lSubList.clear();
        hasSetValue();
    }
}

void PropertyLinkSubList::setValues(const std::vector<DocumentObject*>& lValue,const std::vector<const char*>& lSubNames)
{   
    if (lValue.size() != lSubNames.size())
        throw Base::ValueError("PropertyLinkSubList::setValues: size of subelements list != size of objects list");
    
#ifndef USE_OLD_DAG
    //maintain backlinks. _lValueList can contain items multiple times, but we trust the document 
    //object to ensure that this works
    for(auto *obj : _lValueList)
        obj->_removeBackLink(static_cast<DocumentObject*>(getContainer()));
    
    //maintain backlinks. lValue can contain items multiple times, but we trust the document 
    //object to ensure that the backlink is only added once
    for(auto *obj : lValue)
        obj->_addBackLink(static_cast<DocumentObject*>(getContainer()));
#endif
    
    aboutToSetValue();
    _lValueList = lValue;
    _lSubList.resize(lSubNames.size());
    int i = 0;
    for (std::vector<const char*>::const_iterator it = lSubNames.begin();it!=lSubNames.end();++it)
        _lSubList[i]  = *it;
    hasSetValue();
}

void PropertyLinkSubList::setValues(const std::vector<DocumentObject*>& lValue,const std::vector<std::string>& lSubNames)
{
    if (lValue.size() != lSubNames.size())
        throw Base::ValueError("PropertyLinkSubList::setValues: size of subelements list != size of objects list");
    
#ifndef USE_OLD_DAG
    //maintain backlinks. _lValueList can contain items multiple times, but we trust the document 
    //object to ensure that this works
    for(auto *obj : _lValueList)
        obj->_removeBackLink(static_cast<DocumentObject*>(getContainer()));
    
    //maintain backlinks. lValue can contain items multiple times, but we trust the document 
    //object to ensure that the backlink is only added once
    for(auto *obj : lValue)
        obj->_addBackLink(static_cast<DocumentObject*>(getContainer()));
#endif
    
    aboutToSetValue();
    _lValueList = lValue;
    _lSubList   = lSubNames;
    hasSetValue();
}

void PropertyLinkSubList::setValue(DocumentObject* lValue, const std::vector<string> &SubList)
{
#ifndef USE_OLD_DAG    
    //maintain backlinks. _lValueList can contain items multiple times, but we trust the document 
    //object to ensure that this works
    for(auto *obj : _lValueList)
        obj->_removeBackLink(static_cast<DocumentObject*>(getContainer()));
    
    //maintain backlinks. lValue can contain items multiple times, but we trust the document 
    //object to ensure that the backlink is only added once
    if(lValue)
        lValue->_addBackLink(static_cast<DocumentObject*>(getContainer()));
#endif
    
    aboutToSetValue();
    std::size_t size = SubList.size();
    this->_lValueList.clear();
    this->_lSubList.clear();
    if (size == 0) {
        if (lValue) {
            this->_lValueList.push_back(lValue);
            this->_lSubList.push_back(std::string());
        }
    }
    else {
        this->_lSubList = SubList;
        this->_lValueList.insert(this->_lValueList.begin(), size, lValue);
    }
    hasSetValue();
}

DocumentObject *PropertyLinkSubList::getValue() const
{
    App::DocumentObject* ret = 0;
    //FIXME: cache this to avoid iterating each time, to improve speed
    for (std::size_t i = 0; i < this->_lValueList.size(); i++) {
        if (ret == 0)
            ret = this->_lValueList[i];
        if (ret != this->_lValueList[i])
            return 0;
    }
    return ret;
}

void PropertyLinkSubList::setSubListValues(const std::vector<PropertyLinkSubList::SubSet>& values)
{
    std::vector<DocumentObject*> links;
    std::vector<std::string> subs;

    for (std::vector<PropertyLinkSubList::SubSet>::const_iterator it = values.begin(); it != values.end(); ++it) {
        for (std::vector<std::string>::const_iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
            links.push_back(it->first);
            subs.push_back(*jt);
        }
    }

    setValues(links, subs);
}

std::vector<PropertyLinkSubList::SubSet> PropertyLinkSubList::getSubListValues() const
{
    std::vector<PropertyLinkSubList::SubSet> values;
    if (_lValueList.size() != _lSubList.size())
        throw Base::ValueError("PropertyLinkSubList::getSubListValues: size of subelements list != size of objects list");

    std::map<App::DocumentObject*, std::vector<std::string> > tmp;
    for (std::size_t i = 0; i < _lValueList.size(); i++) {
        App::DocumentObject* link = _lValueList[i];
        std::string sub = _lSubList[i];
        if (tmp.find(link) == tmp.end()) {
            // make sure to keep the same order as in '_lValueList'
            PropertyLinkSubList::SubSet item;
            item.first = link;
            values.push_back(item);
        }

        tmp[link].push_back(sub);
    }

    for (std::vector<PropertyLinkSubList::SubSet>::iterator it = values.begin(); it != values.end(); ++it) {
        it->second = tmp[it->first];
    }

    return values;
}

void PropertyLinkSubList::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<LinkSubList count=\"" <<  getSize() <<"\">" << endl;
    writer.incInd();
    for (int i = 0; i < getSize(); i++) {
        writer.Stream() << writer.ind() <<
            "<Link " <<
            "obj=\"" << _lValueList[i]->getNameInDocument() << "\" " <<
            "sub=\"" << _lSubList[i] << "\"/>" << endl;
    }

    writer.decInd();
    writer.Stream() << writer.ind() << "</LinkSubList>" << endl ;
}

void PropertyLinkSubList::Restore(Base::XMLReader &reader)
{
    // read my element
    reader.readElement("LinkSubList");
    // get the value of my attribute
    int count = reader.getAttributeAsInteger("count");

    std::vector<DocumentObject*> values;
    values.reserve(count);
    std::vector<std::string> SubNames;
    SubNames.reserve(count);
    for (int i = 0; i < count; i++) {
        reader.readElement("Link");
        std::string name = reader.getAttribute("obj");
        // In order to do copy/paste it must be allowed to have defined some
        // referenced objects in XML which do not exist anymore in the new
        // document. Thus, we should silently ignore this.
        // Property not in an object!
        DocumentObject* father = static_cast<DocumentObject*>(getContainer());
        App::Document* document = father->getDocument();
        DocumentObject* child = document ? document->getObject(name.c_str()) : 0;
        if (child)
            values.push_back(child);
        else if (reader.isVerbose())
            printf("Lost link to '%s' while loading, maybe "
                                    "an object was not loaded correctly\n",name.c_str());
        std::string subName = reader.getAttribute("sub");
        SubNames.push_back(subName);
    }

    reader.readEndElement("LinkSubList");

    // assignment
    setValues(values,SubNames);
}

Property *PropertyLinkSubList::Copy(void) const
{
    PropertyLinkSubList *p = new PropertyLinkSubList();
    p->_lValueList = _lValueList;
    p->_lSubList   = _lSubList;
    return p;
}

void PropertyLinkSubList::Paste(const Property &from)
{
    setValues(dynamic_cast<const PropertyLinkSubList&>(from)._lValueList, dynamic_cast<const PropertyLinkSubList&>(from)._lSubList);
}

unsigned int PropertyLinkSubList::getMemSize (void) const
{
   unsigned int size = static_cast<unsigned int>(_lValueList.size() * sizeof(App::DocumentObject *));
   for(int i = 0;i<getSize(); i++)
       size += _lSubList[i].size();

   return size;
}
