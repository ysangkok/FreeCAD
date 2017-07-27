/***************************************************************************
 *   Copyright (c) 2004 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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
# include <algorithm>
# include <limits>
# include <QTextStream>
#endif
#if QT_VERSION >= 0x050200
# include <QMetaType>
#endif

// Uncomment this block to remove PySide support and switch back to PyQt
// #undef HAVE_SHIBOKEN
// #undef HAVE_PYSIDE

#ifdef FC_OS_WIN32
#undef max
#undef min
#ifdef _MSC_VER
#pragma warning( disable : 4099 )
#pragma warning( disable : 4522 )
#endif
#endif

// class and struct used for SbkObject
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wmismatched-tags"
# pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined (__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined (__GNUC__)
# pragma GCC diagnostic pop
#endif

//#include <CXX/Objects.hxx>
#include <App/Application.h>
//#include <Base/Console.h>
#include <Base/Exception.h>
//#include <Base/Interpreter.h>
#include <Base/Quantity.h>
//#include <Base/QuantityPy.h>


#include "WidgetFactory.h"
#include "PrefWidgets.h"
#include "PropertyPage.h"


using namespace Gui;


// --------------------------------------------------------


// ----------------------------------------------------


// ----------------------------------------------------


// ----------------------------------------------------

UiLoader::UiLoader(QObject* parent)
  : QUiLoader(parent)
{
    // do not use the plugins for additional widgets as we don't need them and
    // the application may crash under Linux (tested on Ubuntu 7.04 & 7.10).
    clearPluginPaths();
    this->cw = availableWidgets();
}

UiLoader::~UiLoader()
{
}

QWidget* UiLoader::createWidget(const QString & className, QWidget * parent,
                                const QString& name)
{
    if (this->cw.contains(className))
        return QUiLoader::createWidget(className, parent, name);
    QWidget* w = 0;
    abort();
    return w;
}

// ----------------------------------------------------


// ----------------------------------------------------

WidgetFactorySupplier* WidgetFactorySupplier::_pcSingleton = 0L;

WidgetFactorySupplier & WidgetFactorySupplier::instance()
{
    // not initialized?
    if (!_pcSingleton)
        _pcSingleton = new WidgetFactorySupplier;
    return *_pcSingleton;
}

void WidgetFactorySupplier::destruct()
{
   //abort();
}

// ----------------------------------------------------


// ----------------------------------------------------


// ----------------------------------------------------


// ----------------------------------------------------

/* TRANSLATOR Gui::ContainerDialog */

/**
 *  Constructs a ContainerDialog which embeds the child \a templChild.
 *  The dialog will be modal.
 */
ContainerDialog::ContainerDialog( QWidget* templChild )
  : QDialog( QApplication::activeWindow())
{
    setModal(true);
    setWindowTitle( templChild->objectName() );
    setObjectName( templChild->objectName() );

    setSizeGripEnabled( true );
    MyDialogLayout = new QGridLayout(this);

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName(QLatin1String("buttonOK"));
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );

    MyDialogLayout->addWidget( buttonOk, 1, 0 );
    QSpacerItem* spacer = new QSpacerItem( 210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    MyDialogLayout->addItem( spacer, 1, 1 );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName(QLatin1String("buttonCancel"));
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAutoDefault( true );

    MyDialogLayout->addWidget( buttonCancel, 1, 2 );

    templChild->setParent(this);

    MyDialogLayout->addWidget( templChild, 0, 0, 0, 2 );
    resize( QSize(307, 197).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/** Destroys the object and frees any allocated resources */
ContainerDialog::~ContainerDialog()
{
}

// ----------------------------------------------------

// ----------------------------------------------------


#include "moc_WidgetFactory.cpp"
