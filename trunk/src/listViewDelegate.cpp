/********************************************************************
 * ListViewDelegate.cpp
 ********************************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2011 - UNDESA <www.parliaments.info>
 *
 *
 * Author - Miano Njoka <miano@parliaments.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 ***********************************************************************/

#include "listViewDelegate.hpp"
#include "speechEditor.hpp"
#include "agendaItemEditor.hpp"
#include <QString>
#include <QTextDocument>
#include <QModelIndex>
#include <QPainter>
#include <QDebug>
#include <QCompleter>

#include "util/util.hpp"
#include "model/transcriptionItem.hpp"
#include "transcriptionItemEditor.hpp"
ListViewDelegate::ListViewDelegate(QObject *parent)
    		: QAbstractItemDelegate(parent)
	{
	   // current = NULL;
	}

void ListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(
                index.data());
    TranscriptionItem *item = wrapper->ptr;
    item->draw(painter, option);
}


QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(
                index.data());
    TranscriptionItem *item = wrapper->ptr;
    bool isCurrent;
    if (current == index) {
        isCurrent = true;
    }
    else{
        isCurrent = false;
    }
    return item->sizeHint(option, editing, isCurrent);
}
	
QWidget *ListViewDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem & option,
     const QModelIndex & index ) const {
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(index.data());
    TranscriptionItem *item = wrapper->ptr;
    TranscriptionItemEditor *editor = item->newEditor(parent);
    editor->setIndex(index);
    QObject::connect( editor, SIGNAL(close(QWidget *)), this, SLOT( editorClose(QWidget *) ) );
    QObject::connect( this, SIGNAL(commitData(QWidget *)), this, SIGNAL( closeEditor(QWidget *) ) );
    QObject::connect( editor, SIGNAL(closeIndex(QModelIndex)), this, SLOT( display(QModelIndex) ) );
    QObject::connect( editor, SIGNAL(cancelled(QWidget *)), this, SIGNAL( closeEditor(QWidget *) ) );
    editor->setGeometry(option.rect);
    return editor;
}

void ListViewDelegate::editorClose(QWidget *editor) {
    emit commitData(editor);
}

void ListViewDelegate::setEditorData(QWidget *editor_,
                                     const QModelIndex &index) const {
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(index.data());
    TranscriptionItem *item = wrapper->ptr;
    item->setEditorData(editor_);
}

void ListViewDelegate::setModelData(QWidget *editor_, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(index.data());
    TranscriptionItem *item = wrapper->ptr;
    item->setModelData(editor_);
 }
 
void ListViewDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }

void ListViewDelegate::currentEditing(QModelIndex index)
{
    current = index;
    editing = true;
    emit sizeHintChanged(index);
}

void ListViewDelegate::display(QModelIndex index)
{
    editing = false;
    current = index;
    emit sizeHintChanged(index);
}


