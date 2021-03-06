/********************************************************************
 * transcriptionModel.hpp
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
#ifndef TRANSCRIPIONMODEL_HPP
#define TRANSCRIPIONMODEL_HPP

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QList>

#include "transcriptionItem.hpp"

class TranscriptionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TranscriptionModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertItem (int row, TranscriptionItem *item);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    void transcriptionItemDataChanged(const QModelIndex &index);
public slots:
    void loadTranscriptionItems(QList<TranscriptionItem *>*);
signals:
    void dataChanged(const QModelIndex&, const QModelIndex&);
private:
    QList<TranscriptionItem *> *items;
};
#endif // TRANSCRIPIONMODEL_HPP
