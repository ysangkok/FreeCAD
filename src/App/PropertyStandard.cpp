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
# include <sstream>
# include <boost/version.hpp>
# include <boost/filesystem/path.hpp>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......
#include <boost/math/special_functions/round.hpp>

#include <Base/Exception.h>
#include <Base/Reader.h>
#include <Base/Writer.h>
#include <Base/Stream.h>
#include <Base/Quantity.h>

#include "PropertyStandard.h"
//#include "MaterialPy.h"
#include "ObjectIdentifier.h"

using namespace App;
using namespace Base;
using namespace std;




//**************************************************************************
//**************************************************************************
// PropertyInteger
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyInteger , App::Property);

//**************************************************************************
// Construction/Destruction


PropertyInteger::PropertyInteger()
{
    _lValue = 0;
}


PropertyInteger::~PropertyInteger()
{

}

//**************************************************************************
// Base class implementer


void PropertyInteger::setValue(long lValue)
{
    aboutToSetValue();
    _lValue=lValue;
    hasSetValue();
}

long PropertyInteger::getValue(void) const
{
    return _lValue;
}

void PropertyInteger::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Integer value=\"" <<  _lValue <<"\"/>" << std::endl;
}

void PropertyInteger::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Integer");
    // get the value of my Attribute
    setValue(reader.getAttributeAsInteger("value"));
}

Property *PropertyInteger::Copy(void) const
{
    PropertyInteger *p= new PropertyInteger();
    p->_lValue = _lValue;
    return p;
}

void PropertyInteger::Paste(const Property &from)
{
    aboutToSetValue();
    _lValue = dynamic_cast<const PropertyInteger&>(from)._lValue;
    hasSetValue();
}

void PropertyInteger::setPathValue(const ObjectIdentifier &path, const boost::any &value)
{
    verifyPath(path);

    if (value.type() == typeid(long))
        setValue(boost::any_cast<long>(value));
    else if (value.type() == typeid(double))
        setValue(boost::math::round(boost::any_cast<double>(value)));
    else if (value.type() == typeid(Quantity) && boost::any_cast<Quantity>(value).getUnit().isEmpty())
        setValue(boost::math::round(boost::any_cast<Quantity>(value).getValue()));
    else if (value.type() == typeid(int))
        setValue(boost::any_cast<int>(value));
    else
        throw bad_cast();
}


//**************************************************************************
//**************************************************************************
// PropertyPath
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyPath , App::Property);

//**************************************************************************
// Construction/Destruction

PropertyPath::PropertyPath()
{

}

PropertyPath::~PropertyPath()
{

}


//**************************************************************************
// Base class implementer


//**************************************************************************
// Setter/getter for the property

void PropertyPath::setValue(const boost::filesystem::path &Path)
{
    aboutToSetValue();
    _cValue = Path;
    hasSetValue();
}

void PropertyPath::setValue(const char * Path)
{
    aboutToSetValue();
#if (BOOST_VERSION < 104600) || (BOOST_FILESYSTEM_VERSION == 2)
    _cValue = boost::filesystem::path(Path,boost::filesystem::no_check );
    //_cValue = boost::filesystem::path(Path,boost::filesystem::native );
    //_cValue = boost::filesystem::path(Path,boost::filesystem::windows_name );
#else
    _cValue = boost::filesystem::path(Path);
#endif
    hasSetValue();
}

boost::filesystem::path PropertyPath::getValue(void) const
{
    return _cValue;
}

void PropertyPath::Save (Base::Writer &writer) const
{
    std::string val = encodeAttribute(_cValue.string());
    writer.Stream() << writer.ind() << "<Path value=\"" <<  val <<"\"/>" << std::endl;
}

void PropertyPath::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Path");
    // get the value of my Attribute
    setValue(reader.getAttribute("value"));
}

Property *PropertyPath::Copy(void) const
{
    PropertyPath *p= new PropertyPath();
    p->_cValue = _cValue;
    return p;
}

void PropertyPath::Paste(const Property &from)
{
    aboutToSetValue();
    _cValue = dynamic_cast<const PropertyPath&>(from)._cValue;
    hasSetValue();
}

unsigned int PropertyPath::getMemSize (void) const
{
    return static_cast<unsigned int>(_cValue.string().size());
}

//**************************************************************************
//**************************************************************************
// PropertyEnumeration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyEnumeration, App::PropertyInteger);

//**************************************************************************
// Construction/Destruction


PropertyEnumeration::PropertyEnumeration()
{

}

PropertyEnumeration::PropertyEnumeration(const App::Enumeration &e)
{
    _enum = e;
}

PropertyEnumeration::~PropertyEnumeration()
{

}

void PropertyEnumeration::setEnums(const char **plEnums)
{
    // Setting the enum is done only once inside the constructor
    // but before the current index is already set. So, this needs
    // to be preserved.
    int index = _enum._index;
    _enum.setEnums(plEnums);
    _enum._index = index;
}

void PropertyEnumeration::setValue(const char *value)
{
    aboutToSetValue();
    _enum.setValue(value);
    hasSetValue();
}

void PropertyEnumeration::setValue(long value)
{
    aboutToSetValue();
    _enum.setValue(value);
    hasSetValue();
}

void PropertyEnumeration::setValue(const Enumeration &source)
{
    aboutToSetValue();
    _enum = source;
    hasSetValue();
}

long PropertyEnumeration::getValue(void) const
{
    return _enum.getInt();
}

bool PropertyEnumeration::isValue(const char *value) const
{
    return _enum.isValue(value);
}

bool PropertyEnumeration::isPartOf(const char *value) const
{
    return _enum.contains(value);
}

const char * PropertyEnumeration::getValueAsString(void) const
{
    return _enum.getCStr();
}

Enumeration PropertyEnumeration::getEnum(void) const
{
    return _enum;
}

std::vector<std::string> PropertyEnumeration::getEnumVector(void) const
{
    return _enum.getEnumVector();
}

const char ** PropertyEnumeration::getEnums(void) const
{
    return _enum.getEnums();
}

void PropertyEnumeration::Save(Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Integer value=\"" <<  _enum.getInt() <<"\"";
    if (_enum.isCustom())
        writer.Stream() << " CustomEnum=\"true\"";
    writer.Stream() << "/>" << std::endl;
    if (_enum.isCustom()) {
        std::vector<std::string> items = getEnumVector();
        writer.Stream() << writer.ind() << "<CustomEnumList count=\"" <<  items.size() <<"\">" << endl;
        writer.incInd();
        for(std::vector<std::string>::iterator it = items.begin(); it != items.end(); ++it) {
            std::string val = encodeAttribute(*it);
            writer.Stream() << writer.ind() << "<Enum value=\"" <<  val <<"\"/>" << endl;
        }
        writer.decInd();
        writer.Stream() << writer.ind() << "</CustomEnumList>" << endl;
    }
}

void PropertyEnumeration::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Integer");
    // get the value of my Attribute
    long val = reader.getAttributeAsInteger("value");

    if (reader.hasAttribute("CustomEnum")) {
        reader.readElement("CustomEnumList");
        int count = reader.getAttributeAsInteger("count");
        std::vector<std::string> values(count);

        for(int i = 0; i < count; i++) {
            reader.readElement("Enum");
            values[i] = reader.getAttribute("value");
        }

        reader.readEndElement("CustomEnumList");

        _enum.setEnums(values);
    }

    setValue(val);
}

Property * PropertyEnumeration::Copy(void) const
{
    return new PropertyEnumeration(_enum);
}

void PropertyEnumeration::Paste(const Property &from)
{
    aboutToSetValue();

    const PropertyEnumeration& prop = dynamic_cast<const PropertyEnumeration&>(from);
    _enum = prop._enum;

    hasSetValue();
}

void PropertyEnumeration::setPathValue(const ObjectIdentifier &path, const boost::any &value)
{
    verifyPath(path);

    if (value.type() == typeid(int))
        setValue(boost::any_cast<int>(value));
    else if (value.type() == typeid(double))
        setValue(boost::any_cast<double>(value));
    else if (value.type() == typeid(short))
        setValue(boost::any_cast<short>(value));
    else if (value.type() == typeid(std::string))
        setValue(boost::any_cast<std::string>(value).c_str());
    else if (value.type() == typeid(char*))
        setValue(boost::any_cast<char*>(value));
    else if (value.type() == typeid(const char*))
        setValue(boost::any_cast<const char*>(value));
    else
        throw bad_cast();
}

//**************************************************************************
//**************************************************************************
// PropertyIntegerConstraint
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyIntegerConstraint, App::PropertyInteger)

//**************************************************************************
// Construction/Destruction


PropertyIntegerConstraint::PropertyIntegerConstraint()
  : _ConstStruct(0)
{

}


PropertyIntegerConstraint::~PropertyIntegerConstraint()
{
    if (_ConstStruct && _ConstStruct->isDeletable())
        delete _ConstStruct;
}

void PropertyIntegerConstraint::setConstraints(const Constraints* sConstrain)
{
    if (_ConstStruct != sConstrain) {
        if (_ConstStruct && _ConstStruct->isDeletable())
            delete _ConstStruct;
    }

    _ConstStruct = sConstrain;
}

const PropertyIntegerConstraint::Constraints*  PropertyIntegerConstraint::getConstraints(void) const
{
    return _ConstStruct;
}

//**************************************************************************
//**************************************************************************
// PropertyPercent
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyPercent , App::PropertyIntegerConstraint);

const PropertyIntegerConstraint::Constraints percent = {0,100,1};

//**************************************************************************
// Construction/Destruction


PropertyPercent::PropertyPercent()
{
    _ConstStruct = &percent;
}

PropertyPercent::~PropertyPercent()
{
}

//**************************************************************************
//**************************************************************************
// PropertyIntegerList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyIntegerList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction


PropertyIntegerList::PropertyIntegerList()
{

}

PropertyIntegerList::~PropertyIntegerList()
{

}

void PropertyIntegerList::setSize(int newSize)
{
    _lValueList.resize(newSize);
}

int PropertyIntegerList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

//**************************************************************************
// Base class implementer

void PropertyIntegerList::setValue(long lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyIntegerList::setValues(const std::vector<long>& values)
{
    aboutToSetValue();
    _lValueList = values;
    hasSetValue();
}

void PropertyIntegerList::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<IntegerList count=\"" <<  getSize() <<"\">" << endl;
    writer.incInd();
    for(int i = 0;i<getSize(); i++)
        writer.Stream() << writer.ind() << "<I v=\"" <<  _lValueList[i] <<"\"/>" << endl; ;
    writer.decInd();
    writer.Stream() << writer.ind() << "</IntegerList>" << endl ;
}

void PropertyIntegerList::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("IntegerList");
    // get the value of my Attribute
    int count = reader.getAttributeAsInteger("count");
    
    std::vector<long> values(count);
    for(int i = 0; i < count; i++) {
        reader.readElement("I");
        values[i] = reader.getAttributeAsInteger("v");
    }
    
    reader.readEndElement("IntegerList");

    //assignment
    setValues(values);
}

Property *PropertyIntegerList::Copy(void) const
{
    PropertyIntegerList *p= new PropertyIntegerList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyIntegerList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyIntegerList&>(from)._lValueList;
    hasSetValue();
}

unsigned int PropertyIntegerList::getMemSize (void) const
{
    return static_cast<unsigned int>(_lValueList.size() * sizeof(long));
}




//**************************************************************************
//**************************************************************************
// PropertyIntegerSet
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyIntegerSet , App::Property);

//**************************************************************************
// Construction/Destruction


PropertyIntegerSet::PropertyIntegerSet()
{

}

PropertyIntegerSet::~PropertyIntegerSet()
{

}


//**************************************************************************
// Base class implementer

void PropertyIntegerSet::setValue(long lValue)
{
    aboutToSetValue();
    _lValueSet.clear();
    _lValueSet.insert(lValue);
    hasSetValue();
}

void PropertyIntegerSet::setValues(const std::set<long>& values)
{
    aboutToSetValue();
    _lValueSet = values;
    hasSetValue();
}

void PropertyIntegerSet::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<IntegerSet count=\"" <<  _lValueSet.size() <<"\">" << endl;
    writer.incInd();
    for(std::set<long>::const_iterator it=_lValueSet.begin();it!=_lValueSet.end();++it)
        writer.Stream() << writer.ind() << "<I v=\"" <<  *it <<"\"/>" << endl; ;
    writer.decInd();
    writer.Stream() << writer.ind() << "</IntegerSet>" << endl ;
}

void PropertyIntegerSet::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("IntegerSet");
    // get the value of my Attribute
    int count = reader.getAttributeAsInteger("count");
    
    std::set<long> values;
    for(int i = 0; i < count; i++) {
        reader.readElement("I");
        values.insert(reader.getAttributeAsInteger("v"));
    }
    
    reader.readEndElement("IntegerSet");

    //assignment
    setValues(values);
}

Property *PropertyIntegerSet::Copy(void) const
{
    PropertyIntegerSet *p= new PropertyIntegerSet();
    p->_lValueSet = _lValueSet;
    return p;
}

void PropertyIntegerSet::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueSet = dynamic_cast<const PropertyIntegerSet&>(from)._lValueSet;
    hasSetValue();
}

unsigned int PropertyIntegerSet::getMemSize (void) const
{
    return static_cast<unsigned int>(_lValueSet.size() * sizeof(long));
}



//**************************************************************************
//**************************************************************************
// PropertyFloat
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFloat , App::Property);

//**************************************************************************
// Construction/Destruction


PropertyFloat::PropertyFloat()
{
    _dValue = 0.0;
}

PropertyFloat::~PropertyFloat()
{

}

//**************************************************************************
// Base class implementer

void PropertyFloat::setValue(double lValue)
{
    aboutToSetValue();
    _dValue=lValue;
    hasSetValue();
}

double PropertyFloat::getValue(void) const
{
    return _dValue;
}

void PropertyFloat::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Float value=\"" <<  _dValue <<"\"/>" << std::endl;
}

void PropertyFloat::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Float");
    // get the value of my Attribute
    setValue(reader.getAttributeAsFloat("value"));
}

Property *PropertyFloat::Copy(void) const
{
    PropertyFloat *p= new PropertyFloat();
    p->_dValue = _dValue;
    return p;
}

void PropertyFloat::Paste(const Property &from)
{
    aboutToSetValue();
    _dValue = dynamic_cast<const PropertyFloat&>(from)._dValue;
    hasSetValue();
}

void PropertyFloat::setPathValue(const ObjectIdentifier &path, const boost::any &value)
{
    verifyPath(path);

    if (value.type() == typeid(double))
        setValue(boost::any_cast<double>(value));
    else if (value.type() == typeid(Quantity) && boost::any_cast<Quantity>(value).getUnit().isEmpty())
        setValue((boost::any_cast<Quantity>(value)).getValue());
    else
        throw bad_cast();
}

const boost::any PropertyFloat::getPathValue(const ObjectIdentifier &path) const
{
    verifyPath(path);
    return _dValue;
}

//**************************************************************************
//**************************************************************************
// PropertyFloatConstraint
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFloatConstraint, App::PropertyFloat)

//**************************************************************************
// Construction/Destruction


PropertyFloatConstraint::PropertyFloatConstraint()
  : _ConstStruct(0)
{

}

PropertyFloatConstraint::~PropertyFloatConstraint()
{
    if (_ConstStruct && _ConstStruct->isDeletable())
        delete _ConstStruct;
}

void PropertyFloatConstraint::setConstraints(const Constraints* sConstrain)
{
    if (_ConstStruct != sConstrain) {
        if (_ConstStruct && _ConstStruct->isDeletable())
            delete _ConstStruct;
    }
    _ConstStruct = sConstrain;
}

const PropertyFloatConstraint::Constraints*  PropertyFloatConstraint::getConstraints(void) const
{
    return _ConstStruct;
}


//**************************************************************************
// PropertyPrecision
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyPrecision, App::PropertyFloatConstraint);

//**************************************************************************
// Construction/Destruction
//
const PropertyFloatConstraint::Constraints PrecisionStandard = {0.0,DBL_MAX,0.001};

PropertyPrecision::PropertyPrecision()
{
    setConstraints(&PrecisionStandard);
}

PropertyPrecision::~PropertyPrecision()
{

}


//**************************************************************************
// PropertyFloatList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFloatList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction


PropertyFloatList::PropertyFloatList()
{

}

PropertyFloatList::~PropertyFloatList()
{

}

//**************************************************************************
// Base class implementer

void PropertyFloatList::setSize(int newSize)
{
    _lValueList.resize(newSize);
}

int PropertyFloatList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

void PropertyFloatList::setValue(double lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyFloatList::setValues(const std::vector<double>& values)
{
    aboutToSetValue();
    _lValueList = values;
    hasSetValue();
}

void PropertyFloatList::Save (Base::Writer &writer) const
{
    if (writer.isForceXML()) {
        writer.Stream() << writer.ind() << "<FloatList count=\"" <<  getSize() <<"\">" << endl;
        writer.incInd();
        for(int i = 0;i<getSize(); i++)
            writer.Stream() << writer.ind() << "<F v=\"" <<  _lValueList[i] <<"\"/>" << endl; ;
        writer.decInd();
        writer.Stream() << writer.ind() <<"</FloatList>" << endl ;
    }
    else {
        writer.Stream() << writer.ind() << "<FloatList file=\"" << 
        writer.addFile(getName(), this) << "\"/>" << std::endl;
    }
}

void PropertyFloatList::Restore(Base::XMLReader &reader)
{
    reader.readElement("FloatList");
    string file (reader.getAttribute("file") );

    if (!file.empty()) {
        // initate a file read
        reader.addFile(file.c_str(),this);
    }
}

void PropertyFloatList::SaveDocFile (Base::Writer &writer) const
{
    Base::OutputStream str(writer.Stream());
    uint32_t uCt = (uint32_t)getSize();
    str << uCt;
    if (writer.getFileVersion() > 0) {
        for (std::vector<double>::const_iterator it = _lValueList.begin(); it != _lValueList.end(); ++it) {
            str << *it;
        }
    }
    else {
        for (std::vector<double>::const_iterator it = _lValueList.begin(); it != _lValueList.end(); ++it) {
            float v = (float)*it;
            str << v;
        }
    }
}

void PropertyFloatList::RestoreDocFile(Base::Reader &reader)
{
    Base::InputStream str(reader);
    uint32_t uCt=0;
    str >> uCt;
    std::vector<double> values(uCt);
    if (reader.getFileVersion() > 0) {
        for (std::vector<double>::iterator it = values.begin(); it != values.end(); ++it) {
            str >> *it;
        }
    }
    else {
        for (std::vector<double>::iterator it = values.begin(); it != values.end(); ++it) {
            float val;
            str >> val;
            (*it) = val;
        }
    }
    setValues(values);
}

Property *PropertyFloatList::Copy(void) const
{
    PropertyFloatList *p= new PropertyFloatList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyFloatList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyFloatList&>(from)._lValueList;
    hasSetValue();
}

unsigned int PropertyFloatList::getMemSize (void) const
{
    return static_cast<unsigned int>(_lValueList.size() * sizeof(double));
}

//**************************************************************************
//**************************************************************************
// PropertyString
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyString , App::Property);

PropertyString::PropertyString()
{

}

PropertyString::~PropertyString()
{

}

void PropertyString::setValue(const char* sString)
{
    if (sString) {
        aboutToSetValue();
        _cValue = sString;
        hasSetValue();
    }
}

void PropertyString::setValue(const std::string &sString)
{
    aboutToSetValue();
    _cValue = sString;
    hasSetValue();
}

const char* PropertyString::getValue(void) const
{
    return _cValue.c_str();
}

void PropertyString::Save (Base::Writer &writer) const
{
    std::string val = encodeAttribute(_cValue);
    writer.Stream() << writer.ind() << "<String value=\"" <<  val <<"\"/>" << std::endl;
}

void PropertyString::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("String");
    // get the value of my Attribute
    setValue(reader.getAttribute("value"));
}

Property *PropertyString::Copy(void) const
{
    PropertyString *p= new PropertyString();
    p->_cValue = _cValue;
    return p;
}

void PropertyString::Paste(const Property &from)
{
    aboutToSetValue();
    _cValue = dynamic_cast<const PropertyString&>(from)._cValue;
    hasSetValue();
}

unsigned int PropertyString::getMemSize (void) const
{
    return static_cast<unsigned int>(_cValue.size());
}

void PropertyString::setPathValue(const ObjectIdentifier &path, const boost::any & /*value*/)
{
    verifyPath(path);
}

const boost::any PropertyString::getPathValue(const ObjectIdentifier &path) const
{
    verifyPath(path);
    return _cValue;
}

//**************************************************************************
//**************************************************************************
// PropertyUUID
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyUUID , App::Property);

PropertyUUID::PropertyUUID()
{

}

PropertyUUID::~PropertyUUID()
{

}

void PropertyUUID::setValue(const Base::Uuid &id)
{
    aboutToSetValue();
    _uuid = id;
    hasSetValue();
}

void PropertyUUID::setValue(const char* sString)
{
    if (sString) {
        aboutToSetValue();
        _uuid.setValue(sString);
        hasSetValue();
    }
}

void PropertyUUID::setValue(const std::string &sString)
{
    aboutToSetValue();
    _uuid.setValue(sString);
    hasSetValue();
}

const std::string& PropertyUUID::getValueStr(void) const
{
    return _uuid.getValue();
}

const Base::Uuid& PropertyUUID::getValue(void) const
{
    return _uuid;
}

void PropertyUUID::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Uuid value=\"" << _uuid.getValue() <<"\"/>" << std::endl;
}

void PropertyUUID::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Uuid");
    // get the value of my Attribute
    setValue(reader.getAttribute("value"));
}

Property *PropertyUUID::Copy(void) const
{
    PropertyUUID *p= new PropertyUUID();
    p->_uuid = _uuid;
    return p;
}

void PropertyUUID::Paste(const Property &from)
{
    aboutToSetValue();
    _uuid = dynamic_cast<const PropertyUUID&>(from)._uuid;
    hasSetValue();
}

unsigned int PropertyUUID::getMemSize (void) const
{
    return static_cast<unsigned int>(sizeof(_uuid));
}

//**************************************************************************
// PropertyFont
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyFont , App::PropertyString);

PropertyFont::PropertyFont()
{

}

PropertyFont::~PropertyFont()
{

}

//**************************************************************************
// PropertyStringList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyStringList , App::PropertyLists);

PropertyStringList::PropertyStringList()
{

}

PropertyStringList::~PropertyStringList()
{

}

//**************************************************************************
// Base class implementer

void PropertyStringList::setSize(int newSize)
{
    _lValueList.resize(newSize);
}

int PropertyStringList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

void PropertyStringList::setValue(const std::string& lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyStringList::setValues(const std::vector<std::string>& lValue)
{
    aboutToSetValue();
    _lValueList=lValue;
    hasSetValue();
}

void PropertyStringList::setValues(const std::list<std::string>& lValue)
{
    aboutToSetValue();
    _lValueList.resize(lValue.size());
    std::copy(lValue.begin(), lValue.end(), _lValueList.begin());
    hasSetValue();
}

unsigned int PropertyStringList::getMemSize (void) const
{
    size_t size=0;
    for(int i = 0;i<getSize(); i++) 
        size += _lValueList[i].size();
    return static_cast<unsigned int>(size);
}

void PropertyStringList::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<StringList count=\"" <<  getSize() <<"\">" << endl;
    writer.incInd();
    for(int i = 0;i<getSize(); i++) {
        std::string val = encodeAttribute(_lValueList[i]);
        writer.Stream() << writer.ind() << "<String value=\"" <<  val <<"\"/>" << endl;
    }
    writer.decInd();
    writer.Stream() << writer.ind() << "</StringList>" << endl ;
}

void PropertyStringList::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("StringList");
    // get the value of my Attribute
    int count = reader.getAttributeAsInteger("count");

    std::vector<std::string> values(count);
    for(int i = 0; i < count; i++) {
        reader.readElement("String");
        values[i] = reader.getAttribute("value");
    }
    
    reader.readEndElement("StringList");

    // assignment
    setValues(values);
}

Property *PropertyStringList::Copy(void) const
{
    PropertyStringList *p= new PropertyStringList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyStringList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyStringList&>(from)._lValueList;
    hasSetValue();
}


//**************************************************************************
// PropertyMap
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyMap , App::Property);

PropertyMap::PropertyMap()
{

}

PropertyMap::~PropertyMap()
{

}

//**************************************************************************
// Base class implementer


int PropertyMap::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

void PropertyMap::setValue(const std::string& key,const std::string& value)
{
    aboutToSetValue();
    _lValueList[key] = value;
    hasSetValue();
}

void PropertyMap::setValues(const std::map<std::string,std::string>& map)
{
    aboutToSetValue();
    _lValueList=map;
    hasSetValue();
}



const std::string& PropertyMap::operator[] (const std::string& key) const 
{
    static std::string empty;
    std::map<std::string,std::string>::const_iterator it = _lValueList.find(key);
    if(it!=_lValueList.end())
        return it->second;
    else
        return empty;
} 


unsigned int PropertyMap::getMemSize (void) const
{
    size_t size=0;
    for (std::map<std::string,std::string>::const_iterator it = _lValueList.begin();it!= _lValueList.end(); ++it) {
        size += it->second.size();
        size += it->first.size();
    }
    return size;
}

void PropertyMap::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Map count=\"" <<  getSize() <<"\">" << endl;
    writer.incInd();
    for (std::map<std::string,std::string>::const_iterator it = _lValueList.begin();it!= _lValueList.end(); ++it) 
        writer.Stream() << writer.ind() << "<Item key=\"" <<  it->first <<"\" value=\"" <<  encodeAttribute(it->second) <<"\"/>" << endl;

    writer.decInd();
    writer.Stream() << writer.ind() << "</Map>" << endl ;
}

void PropertyMap::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Map");
    // get the value of my Attribute
    int count = reader.getAttributeAsInteger("count");

    std::map<std::string,std::string> values;
    for(int i = 0; i < count; i++) {
        reader.readElement("Item");
        values[reader.getAttribute("key")] = reader.getAttribute("value");
    }
    
    reader.readEndElement("Map");

    // assignment
    setValues(values);
}

Property *PropertyMap::Copy(void) const
{
    PropertyMap *p= new PropertyMap();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyMap::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyMap&>(from)._lValueList;
    hasSetValue();
}




//**************************************************************************
//**************************************************************************
// PropertyBool
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyBool , App::Property);

//**************************************************************************
// Construction/Destruction

PropertyBool::PropertyBool()
{
    _lValue = false;
}

PropertyBool::~PropertyBool()
{

}

//**************************************************************************
// Setter/getter for the property

void PropertyBool::setValue(bool lValue)
{
    aboutToSetValue();
    _lValue=lValue;
    hasSetValue();
}

bool PropertyBool::getValue(void) const
{
    return _lValue;
}

void PropertyBool::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<Bool value=\"" ;
    if (_lValue)
        writer.Stream() << "true" <<"\"/>" ;
    else
        writer.Stream() << "false" <<"\"/>" ;
    writer.Stream() << std::endl;
}

void PropertyBool::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("Bool");
    // get the value of my Attribute
    string b = reader.getAttribute("value");
    (b == "true") ? setValue(true) : setValue(false);
}


Property *PropertyBool::Copy(void) const
{
    PropertyBool *p= new PropertyBool();
    p->_lValue = _lValue;
    return p;
}

void PropertyBool::Paste(const Property &from)
{
    aboutToSetValue();
    _lValue = dynamic_cast<const PropertyBool&>(from)._lValue;
    hasSetValue();
}

void PropertyBool::setPathValue(const ObjectIdentifier &path, const boost::any &value)
{
    verifyPath(path);

    if (value.type() == typeid(bool))
        setValue(boost::any_cast<bool>(value));
    else if (value.type() == typeid(int))
        setValue(boost::any_cast<int>(value) != 0);
    else if (value.type() == typeid(double))
        setValue(boost::math::round(boost::any_cast<double>(value)));
    else if (value.type() == typeid(Quantity) && boost::any_cast<Quantity>(value).getUnit().isEmpty())
        setValue(boost::any_cast<Quantity>(value).getValue() != 0);
    else
        throw bad_cast();
}

const boost::any PropertyBool::getPathValue(const ObjectIdentifier &path) const
{
    verifyPath(path);

    return _lValue;
}

//**************************************************************************
//**************************************************************************
// PropertyBoolList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyBoolList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction


PropertyBoolList::PropertyBoolList()
{

}

PropertyBoolList::~PropertyBoolList()
{

}

void PropertyBoolList::setSize(int newSize)
{
    _lValueList.resize(newSize);
}

int PropertyBoolList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

//**************************************************************************
// Base class implementer

void PropertyBoolList::setValue(bool lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyBoolList::set1Value(const int idx, bool value)
{
    aboutToSetValue();
    _lValueList[idx]=value;
    hasSetValue();
}

void PropertyBoolList::setValues(const boost::dynamic_bitset<>& values)
{
    aboutToSetValue();
    _lValueList = values;
    hasSetValue();
}

void PropertyBoolList::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<BoolList value=\"" ;
    std::string bitset;
    boost::to_string(_lValueList, bitset);
    writer.Stream() << bitset <<"\"/>" ;
    writer.Stream() << std::endl;
}

void PropertyBoolList::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("BoolList");
    // get the value of my Attribute
    string str = reader.getAttribute("value");
    boost::dynamic_bitset<> bitset(str);
    setValues(bitset);
}

Property *PropertyBoolList::Copy(void) const
{
    PropertyBoolList *p= new PropertyBoolList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyBoolList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyBoolList&>(from)._lValueList;
    hasSetValue();
}

unsigned int PropertyBoolList::getMemSize (void) const
{
    return static_cast<unsigned int>(_lValueList.size());
}

//**************************************************************************
//**************************************************************************
// PropertyColor
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyColor , App::Property);

//**************************************************************************
// Construction/Destruction

PropertyColor::PropertyColor()
{

}

PropertyColor::~PropertyColor()
{

}

//**************************************************************************
// Base class implementer

void PropertyColor::setValue(const Color &col)
{
    aboutToSetValue();
    _cCol=col;
    hasSetValue();
}

void PropertyColor::setValue(uint32_t rgba)
{
    aboutToSetValue();
    _cCol.setPackedValue(rgba);
    hasSetValue();
}

void PropertyColor::setValue(float r, float g, float b, float a)
{
    aboutToSetValue();
    _cCol.set(r,g,b,a);
    hasSetValue();
}

const Color& PropertyColor::getValue(void) const 
{
    return _cCol;
}

void PropertyColor::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<PropertyColor value=\"" 
    <<  _cCol.getPackedValue() <<"\"/>" << endl;
}

void PropertyColor::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("PropertyColor");
    // get the value of my Attribute
    unsigned long rgba = reader.getAttributeAsUnsigned("value");
    setValue(rgba);
}

Property *PropertyColor::Copy(void) const
{
    PropertyColor *p= new PropertyColor();
    p->_cCol = _cCol;
    return p;
}

void PropertyColor::Paste(const Property &from)
{
    aboutToSetValue();
    _cCol = dynamic_cast<const PropertyColor&>(from)._cCol;
    hasSetValue();
}

//**************************************************************************
// PropertyColorList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyColorList , App::PropertyLists);

//**************************************************************************
// Construction/Destruction

PropertyColorList::PropertyColorList()
{

}

PropertyColorList::~PropertyColorList()
{

}

//**************************************************************************
// Base class implementer

void PropertyColorList::setSize(int newSize)
{
    _lValueList.resize(newSize);
}

int PropertyColorList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

void PropertyColorList::setValue(const Color& lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0]=lValue;
    hasSetValue();
}

void PropertyColorList::setValues (const std::vector<Color>& values)
{
    aboutToSetValue();
    _lValueList=values;
    hasSetValue();
}
void PropertyColorList::Save (Base::Writer &writer) const
{
    if (!writer.isForceXML()) {
        writer.Stream() << writer.ind() << "<ColorList file=\"" << writer.addFile(getName(), this) << "\"/>" << std::endl;
    }
}

void PropertyColorList::Restore(Base::XMLReader &reader)
{
    reader.readElement("ColorList");
    if (reader.hasAttribute("file")) {
        std::string file (reader.getAttribute("file"));

        if (!file.empty()) {
            // initate a file read
            reader.addFile(file.c_str(),this);
        }
    }
}

void PropertyColorList::SaveDocFile (Base::Writer &writer) const
{
    Base::OutputStream str(writer.Stream());
    uint32_t uCt = (uint32_t)getSize();
    str << uCt;
    for (std::vector<App::Color>::const_iterator it = _lValueList.begin(); it != _lValueList.end(); ++it) {
        str << it->getPackedValue();
    }
}

void PropertyColorList::RestoreDocFile(Base::Reader &reader)
{
    Base::InputStream str(reader);
    uint32_t uCt=0;
    str >> uCt;
    std::vector<Color> values(uCt);
    uint32_t value; // must be 32 bit long
    for (std::vector<App::Color>::iterator it = values.begin(); it != values.end(); ++it) {
        str >> value;
        it->setPackedValue(value);
    }
    setValues(values);
}

Property *PropertyColorList::Copy(void) const
{
    PropertyColorList *p= new PropertyColorList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyColorList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyColorList&>(from)._lValueList;
    hasSetValue();
}

unsigned int PropertyColorList::getMemSize (void) const
{
    return static_cast<unsigned int>(_lValueList.size() * sizeof(Color));
}

//**************************************************************************
//**************************************************************************
// PropertyMaterial
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyMaterial , App::Property);

PropertyMaterial::PropertyMaterial()
{

}

PropertyMaterial::~PropertyMaterial()
{

}

void PropertyMaterial::setValue(const Material &mat)
{
    aboutToSetValue();
    _cMat=mat;
    hasSetValue();
}

const Material& PropertyMaterial::getValue(void) const 
{
    return _cMat;
}

void PropertyMaterial::setAmbientColor(const Color& col)
{
    aboutToSetValue();
    _cMat.ambientColor = col;
    hasSetValue();
}

void PropertyMaterial::setDiffuseColor(const Color& col)
{
    aboutToSetValue();
    _cMat.diffuseColor = col;
    hasSetValue();
}

void PropertyMaterial::setSpecularColor(const Color& col)
{
    aboutToSetValue();
    _cMat.specularColor = col;
    hasSetValue();
}

void PropertyMaterial::setEmissiveColor(const Color& col)
{
    aboutToSetValue();
    _cMat.emissiveColor = col;
    hasSetValue();
}

void PropertyMaterial::setShininess(float val)
{
    aboutToSetValue();
    _cMat.shininess = val;
    hasSetValue();
}

void PropertyMaterial::setTransparency(float val)
{
    aboutToSetValue();
    _cMat.transparency = val;
    hasSetValue();
}
void PropertyMaterial::Save (Base::Writer &writer) const
{
    writer.Stream() << writer.ind() << "<PropertyMaterial ambientColor=\"" 
        <<  _cMat.ambientColor.getPackedValue() 
        << "\" diffuseColor=\"" <<  _cMat.diffuseColor.getPackedValue() 
        << "\" specularColor=\"" <<  _cMat.specularColor.getPackedValue()
        << "\" emissiveColor=\"" <<  _cMat.emissiveColor.getPackedValue()
        << "\" shininess=\"" <<  _cMat.shininess << "\" transparency=\"" 
        <<  _cMat.transparency << "\"/>" << endl;
}

void PropertyMaterial::Restore(Base::XMLReader &reader)
{
    // read my Element
    reader.readElement("PropertyMaterial");
    // get the value of my Attribute
    aboutToSetValue();
    _cMat.ambientColor.setPackedValue(reader.getAttributeAsUnsigned("ambientColor"));
    _cMat.diffuseColor.setPackedValue(reader.getAttributeAsUnsigned("diffuseColor"));
    _cMat.specularColor.setPackedValue(reader.getAttributeAsUnsigned("specularColor"));
    _cMat.emissiveColor.setPackedValue(reader.getAttributeAsUnsigned("emissiveColor"));
    _cMat.shininess = (float)reader.getAttributeAsFloat("shininess");
    _cMat.transparency = (float)reader.getAttributeAsFloat("transparency");
    hasSetValue();
}

const char* PropertyMaterial::getEditorName(void) const
{
    return "";// "Gui::PropertyEditor::PropertyMaterialItem";
}

Property *PropertyMaterial::Copy(void) const
{
    PropertyMaterial *p= new PropertyMaterial();
    p->_cMat = _cMat;
    return p;
}

void PropertyMaterial::Paste(const Property &from)
{
    aboutToSetValue();
    _cMat = dynamic_cast<const PropertyMaterial&>(from)._cMat;
    hasSetValue();
}

//**************************************************************************
// PropertyMaterialList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyMaterialList, App::PropertyLists);

//**************************************************************************
// Construction/Destruction

PropertyMaterialList::PropertyMaterialList()
{

}

PropertyMaterialList::~PropertyMaterialList()
{

}

//**************************************************************************
// Base class implementer

void PropertyMaterialList::setSize(int newSize)
{
    _lValueList.resize(newSize);
}

int PropertyMaterialList::getSize(void) const
{
    return static_cast<int>(_lValueList.size());
}

void PropertyMaterialList::setValue(const Material& lValue)
{
    aboutToSetValue();
    _lValueList.resize(1);
    _lValueList[0] = lValue;
    hasSetValue();
}

void PropertyMaterialList::setValues(const std::vector<Material>& values)
{
    aboutToSetValue();
    _lValueList = values;
    hasSetValue();
}

void PropertyMaterialList::Save(Base::Writer &writer) const
{
    if (!writer.isForceXML()) {
        writer.Stream() << writer.ind() << "<MaterialList file=\"" << writer.addFile(getName(), this) << "\"/>" << std::endl;
    }
}

void PropertyMaterialList::Restore(Base::XMLReader &reader)
{
    reader.readElement("MaterialList");
    if (reader.hasAttribute("file")) {
        std::string file(reader.getAttribute("file"));

        if (!file.empty()) {
            // initate a file read
            reader.addFile(file.c_str(), this);
        }
    }
}

void PropertyMaterialList::SaveDocFile(Base::Writer &writer) const
{
    Base::OutputStream str(writer.Stream());
    uint32_t uCt = (uint32_t)getSize();
    str << uCt;
    for (std::vector<App::Material>::const_iterator it = _lValueList.begin(); it != _lValueList.end(); ++it) {
        str << it->ambientColor.getPackedValue();
        str << it->diffuseColor.getPackedValue();
        str << it->specularColor.getPackedValue();
        str << it->emissiveColor.getPackedValue();
        str << it->shininess;
        str << it->transparency;
    }
}

void PropertyMaterialList::RestoreDocFile(Base::Reader &reader)
{
    Base::InputStream str(reader);
    uint32_t uCt = 0;
    str >> uCt;
    std::vector<Material> values(uCt);
    uint32_t value; // must be 32 bit long
    float valueF;
    for (std::vector<App::Material>::iterator it = values.begin(); it != values.end(); ++it) {
        str >> value;
        it->ambientColor.setPackedValue(value);
        str >> value;
        it->diffuseColor.setPackedValue(value);
        str >> value;
        it->specularColor.setPackedValue(value);
        str >> value;
        it->emissiveColor.setPackedValue(value);
        str >> valueF;
        it->shininess = valueF;
        str >> valueF;
        it->transparency = valueF;
    }
    setValues(values);
}

const char* PropertyMaterialList::getEditorName(void) const
{
    return "Gui::PropertyEditor::PropertyMaterialListItem";
}

Property *PropertyMaterialList::Copy(void) const
{
    PropertyMaterialList *p = new PropertyMaterialList();
    p->_lValueList = _lValueList;
    return p;
}

void PropertyMaterialList::Paste(const Property &from)
{
    aboutToSetValue();
    _lValueList = dynamic_cast<const PropertyMaterialList&>(from)._lValueList;
    hasSetValue();
}

unsigned int PropertyMaterialList::getMemSize(void) const
{
    return static_cast<unsigned int>(_lValueList.size() * sizeof(Material));
}
