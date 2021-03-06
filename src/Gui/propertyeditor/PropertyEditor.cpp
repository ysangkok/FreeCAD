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
# include <QApplication>
# include <QPainter>
#endif

//#include <Base/Console.h>
#include <App/Application.h>
#include <App/Document.h>
#include "PropertyEditor.h"
#include "PropertyItemDelegate.h"
#include "PropertyModel.h"

using namespace Gui::PropertyEditor;

PropertyEditor::PropertyEditor(QWidget *parent)
    : QTreeView(parent), autoupdate(false), committing(false), delaybuild(false)
{
    propertyModel = new PropertyModel(this);
    setModel(propertyModel);

    PropertyItemDelegate* delegate = new PropertyItemDelegate(this);
    delegate->setItemEditorFactory(new PropertyItemEditorFactory);
    setItemDelegate(delegate);

    setAlternatingRowColors(true);
    setRootIsDecorated(true);

    QStyleOptionViewItem opt = viewOptions();
    this->background = opt.palette.dark();
    this->groupColor = opt.palette.color(QPalette::BrightText);
}

PropertyEditor::~PropertyEditor()
{
}

void PropertyEditor::setAutomaticDocumentUpdate(bool v)
{
    autoupdate = v;
}

bool PropertyEditor::isAutomaticDocumentUpdate(bool) const
{
    return autoupdate;
}

QBrush PropertyEditor::groupBackground() const
{
    return this->background;
}

void PropertyEditor::setGroupBackground(const QBrush& c)
{
    this->background = c;
}

QColor PropertyEditor::groupTextColor() const
{
    return this->groupColor;
}

void PropertyEditor::setGroupTextColor(const QColor& c)
{
    this->groupColor = c;
}

QStyleOptionViewItem PropertyEditor::viewOptions() const
{
    QStyleOptionViewItem option = QTreeView::viewOptions();
    option.showDecorationSelected = true;
    return option;
}

void PropertyEditor::closeEditor (QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (autoupdate) {
        App::Document* doc = App::GetApplication().getActiveDocument();
        if (doc && doc->isTouched())
            doc->recompute();
    }
    QTreeView::closeEditor(editor, hint);
}

void PropertyEditor::commitData (QWidget * editor)
{
    committing = true;
    QTreeView::commitData(editor);
    committing = false;
    if (delaybuild) {
        delaybuild = false;
        propertyModel->buildUp(PropertyModel::PropertyList());
    }
}

void PropertyEditor::editorDestroyed (QObject * editor)
{
    QTreeView::editorDestroyed(editor);
}

void PropertyEditor::currentChanged ( const QModelIndex & current, const QModelIndex & previous )
{
    QTreeView::currentChanged(current, previous);
    if (previous.isValid())
        closePersistentEditor(model()->buddy(previous));
    if (current.isValid())
        openPersistentEditor(model()->buddy(current));
}

void PropertyEditor::reset()
{
    QTreeView::reset();

    QModelIndex index;
    int numRows = propertyModel->rowCount(index);
    if (numRows > 0)
        setEditorMode(index, 0, numRows-1);
}

void PropertyEditor::rowsInserted (const QModelIndex & parent, int start, int end)
{
    QTreeView::rowsInserted(parent, start, end);
    setEditorMode(parent, start, end);
}

void PropertyEditor::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    QTreeView::drawBranches(painter, rect, index);

    QStyleOptionViewItem opt = viewOptions();
    PropertyItem *property = static_cast<PropertyItem*>(index.internalPointer());
    if (property && property->isSeparator()) {
        painter->fillRect(rect, this->background);
    //} else if (selectionModel()->isSelected(index)) {
    //    painter->fillRect(rect, opt.palette.brush(QPalette::Highlight));
    }

    //QPen savedPen = painter->pen();
    //QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &opt));
    //painter->setPen(QPen(color));
    //painter->drawLine(rect.x(), rect.bottom(), rect.right(), rect.bottom());
    //painter->setPen(savedPen);
}

void PropertyEditor::buildUp(const PropertyModel::PropertyList& props)
{
    if (committing) {
        printf("While committing the data to the property the selection has changed.\n");
        delaybuild = true;
        return;
    }

    QModelIndex index = this->currentIndex();
    QStringList propertyPath = propertyModel->propertyPathFromIndex(index);
    if (!propertyPath.isEmpty())
        this->selectedProperty = propertyPath;
    propertyModel->buildUp(props);
    if (!this->selectedProperty.isEmpty()) {
        QModelIndex index = propertyModel->propertyIndexFromPath(this->selectedProperty);
        this->setCurrentIndex(index);
    }

    propList = props;
}

void PropertyEditor::updateProperty(const App::Property& prop)
{
    // forward this to the model if the property is changed from outside
    if (!committing)
        propertyModel->updateProperty(prop);
}

void PropertyEditor::setEditorMode(const QModelIndex & parent, int start, int end)
{
    int column = 1;
    for (int i=start; i<=end; i++) {
        QModelIndex item = propertyModel->index(i, column, parent);
        PropertyItem* propItem = static_cast<PropertyItem*>(item.internalPointer());
        if (propItem && propItem->testStatus(App::Property::Hidden)) {
            setRowHidden (i, parent, true);
        }
    }
}

void PropertyEditor::updateEditorMode(const App::Property& prop)
{
    // check if the parent object is selected
    std::string editor = prop.getEditorName();
    if (editor.empty())
        return;

    bool hidden = prop.testStatus(App::Property::Hidden);
    bool readOnly = prop.testStatus(App::Property::ReadOnly);

    int column = 1;
    int numRows = propertyModel->rowCount();
    for (int i=0; i<numRows; i++) {
        QModelIndex item = propertyModel->index(i, column);
        PropertyItem* propItem = static_cast<PropertyItem*>(item.internalPointer());
        if (propItem && propItem->hasProperty(&prop)) {
            setRowHidden (i, QModelIndex(), hidden);

            propItem->updateData();
            if (item.isValid()) {
                updateItemEditor(!readOnly, column, item);
                dataChanged(item, item);
            }
            break;
        }
    }
}

void PropertyEditor::updateItemEditor(bool enable, int column, const QModelIndex& parent)
{
    QWidget* editor = indexWidget(parent);
    if (editor)
        editor->setEnabled(enable);

    int numRows = propertyModel->rowCount(parent);
    for (int i=0; i<numRows; i++) {
        QModelIndex item = propertyModel->index(i, column, parent);
        if (item.isValid()) {
            updateItemEditor(enable, column, item);
        }
    }
}

void PropertyEditor::appendProperty(const App::Property& prop)
{
    // check if the parent object is selected
    std::string editor = prop.getEditorName();
    if (editor.empty())
        return;
    App::PropertyContainer* parent = prop.getContainer();
    std::string context = prop.getName();

    bool canAddProperty = (!propList.empty());
    for (PropertyModel::PropertyList::iterator it = propList.begin(); it != propList.end(); ++it) {
        if (it->second.empty() || it->second.size() > 1) {
            canAddProperty = false;
            break;
        }
        else if (it->second.front()->getContainer() != parent) {
            canAddProperty = false;
            break;
        }
    }

    if (canAddProperty) {
        std::vector<App::Property*> list;
        list.push_back(const_cast<App::Property*>(&prop));
        std::pair< std::string, std::vector<App::Property*> > pair = std::make_pair(context, list);
        propList.push_back(pair);
        propertyModel->appendProperty(prop);
    }
}

void PropertyEditor::removeProperty(const App::Property& prop)
{
    for (PropertyModel::PropertyList::iterator it = propList.begin(); it != propList.end(); ++it) {
        // find the given property in the list and remove it if it's there
        std::vector<App::Property*>::iterator pos = std::find(it->second.begin(), it->second.end(), &prop);
        if (pos != it->second.end()) {
            it->second.erase(pos);
            // if the last property of this name is removed then also remove the whole group
            if (it->second.empty()) {
                propList.erase(it);
            }
            propertyModel->removeProperty(prop);
            break;
        }
    }
}

#include "moc_PropertyEditor.cpp"
