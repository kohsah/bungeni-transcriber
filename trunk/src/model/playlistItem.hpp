/********************************************************************
 * playlistItem.hpp
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
#ifndef PLAYLISTITEM_HPP
#define PLAYLISTITEM_HPP
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QVariant>
#include <QModelIndex>
class PlaylistItem : public QObject
{
    Q_OBJECT
public:
    PlaylistItem(QString, QDateTime, QDateTime);
    ~PlaylistItem();

    virtual void appendChild(PlaylistItem *child);

    PlaylistItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PlaylistItem *parent();
    enum ItemType {
        RootItem, TypeSitting, TypeTake
    };
    virtual PlaylistItem::ItemType getType();
    QDateTime getStartDateTime();
    QDateTime getEndDateTime();
    QString getName();
    void setStartDateTime(QDateTime);
    void setEndDateTime(QDateTime);
    void setName(QString);
protected:
    QList<PlaylistItem*> childItems;
    QString name;
    QDateTime start;
    QDateTime end;
    PlaylistItem *parentItem;
};

#endif // PLAYLISTITEM_HPP
