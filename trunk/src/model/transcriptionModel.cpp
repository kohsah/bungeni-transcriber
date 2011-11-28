/********************************************************************
 * transcriptionModel.cpp
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
#include "transcriptionModel.hpp"
#include <QDebug>
TranscriptionModel::TranscriptionModel(QObject *parent) :
    QAbstractListModel(parent) {
    items = new QList<TranscriptionItem*>();
}

QVariant TranscriptionModel::data(const QModelIndex &index, int role ) const{
    if (role == Qt::UserRole) {
        TranscriptionItem *item = items->value(index.row());
        return item->getStartTime();
    }
    return QVariant::fromValue(new TranscriptionItemWrapper(items->value(index.row())));
}

Qt::ItemFlags TranscriptionModel::flags(const QModelIndex &index) const{
    if (index.isValid())
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
    return Qt::ItemIsEditable;
}

bool TranscriptionModel::removeRows(int row, int count, const QModelIndex &parent){
    if (row >= items->size() || row + count <= 0)
        return false;

    int beginRow = qMax(0, row);
    int endRow = qMin(row + count - 1, items->size() - 1);

    beginRemoveRows(parent, beginRow, endRow);

    while (beginRow <= endRow) {
        delete items->value(beginRow);
        items->removeAt(beginRow);
        ++beginRow;
    }

    endRemoveRows();
    return true;
}

bool TranscriptionModel::insertItem (int row, TranscriptionItem * item){
    int size = items->size();
    beginInsertRows(QModelIndex(), size, size);
    items->insert(size, item);
    endInsertRows();
    return true;
}

bool TranscriptionModel::setData(const QModelIndex & index,
                                 TranscriptionItem *item, int role = Qt::EditRole){
    items->replace(index.row(), item);
    emit dataChanged(index, index);
    return true;
}

int TranscriptionModel::rowCount(const QModelIndex &parent) const
{
    return items->size();
}

void TranscriptionModel::loadTranscriptionItems(QList<TranscriptionItem *>* items_){
    this->beginResetModel();
    items = items_;
    this->endResetModel();
}

void TranscriptionModel::transcriptionItemDataChanged(const QModelIndex &index){
    emit dataChanged(index, index);
}

TranscriptionSortModel::TranscriptionSortModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
}

bool TranscriptionSortModel::lessThan(const QModelIndex &left,
                                       const QModelIndex &right) const {
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(sourceModel()->data(left));
    TranscriptionItem *leftItem = wrapper->ptr;
    wrapper = qvariant_cast<TranscriptionItemWrapper*>(sourceModel()->data(right));
    TranscriptionItem *rightItem = wrapper->ptr;
    return leftItem->getStartTime() < rightItem->getStartTime();
}
