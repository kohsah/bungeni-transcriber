/********************************************************************
 * sitting.cpp
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
#include <QDebug>
#include "sitting.hpp"
#include "playlistModel.hpp"
#include "../util/util.hpp"

Sitting::Sitting(QString name_, QDateTime start_, QDateTime end_) :
    PlaylistItem(name_, start_, end_){
    aknFilePath = QString();
}

void Sitting::appendChild(Take *child){
    childItems.append(child);
}

PlaylistItem::ItemType Sitting::getType(){
    return PlaylistItem::TypeSitting;
}

QString Sitting::getAknFilePath(){
    return aknFilePath;
}

void Sitting::setAknFilePath(QString aknFilePath_){
    aknFilePath = aknFilePath_;
}

const QList<PlaylistItem*>* Sitting::getTakes() const{
    const QList<PlaylistItem*>* temp = &childItems;
    return temp;
}

void Sitting::addTake(QString name, QDateTime startTime, QDateTime endTime){
    Take* newTake = new Take(name, startTime, endTime, QString("test"));
    this->appendChild(newTake);
}

void Sitting::onTakesReadFinished(QNetworkReply *reply){
    QVariantMap result = parseReply(reply);
    if (!result.isEmpty()){
        QList<QVariant> nodes = result["nodes"].toList();
        for (int i = 0; i < nodes.size(); ++i) {
            QMap<QString, QVariant> node = nodes.at(i).toMap();
            this->addTake(node["debate_take_name"].toString(), QDateTime::fromString(node["start_date"].toString()), QDateTime::fromString(node["end_date"].toString()));
        }
    }
}
