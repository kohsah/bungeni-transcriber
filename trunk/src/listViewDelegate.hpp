/**********************************************************
 * ListViewDelegate.hpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2011 - UNDESA
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
 */
 
 


#ifndef _LIST_VIEW_DELEGATE_
#define _LIST_VIEW_DELEGATE_

#include <QAbstractItemDelegate>

class QAbstractItemModel;
class QObject;
class QPainter;
class QModelIndex;

class ListViewDelegate : public QAbstractItemDelegate {
	Q_OBJECT
private:
    QModelIndex current;
    bool editing;
    QStringList MPList;
    QStringList agendaList;
public:
	ListViewDelegate(QObject *parent);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;                 
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const; 
    void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void setMPList(QStringList);
    void setAgendaList(QStringList);
public slots:
    void currentEditing(QModelIndex index);
    void display(QModelIndex index);
    void editorClose(QWidget *editor);
signals:
    void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint = NoHint );   
};

#endif
