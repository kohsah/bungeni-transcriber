/********************************************************************
 * agendaItem.cpp
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
#include "agendaItem.hpp"
#include "../agendaItemEditor.hpp"
AgendaItem::AgendaItem(QTime startTime_, QTime endTime_,
                       QString title_, int docId_, int id_) : TranscriptionItem(){
    startTime = startTime_;
    endTime = endTime_;
    title = title_;
    docId = docId_;
    id = id_;
}

void AgendaItem::setStartTime(QTime startTime_){
    startTime = startTime_;
}

void AgendaItem::setEndTime(QTime endTime_){
    endTime = endTime_;
}

void AgendaItem::setTitle(QString title_){
    title = title_;
}

void AgendaItem::setDocId(int docId_){
    docId = docId_;
}

void AgendaItem::setId(int id_){
    id = id_;
}

QString AgendaItem::getTitle(){
    return title;
}

int AgendaItem::getId(){
    return id;
}

int AgendaItem::getDocId(){
    return docId;
}

QTime AgendaItem::getStartTime(){
    return startTime;
}

QTime AgendaItem::getEndTime(){
    return endTime;
}
TranscriptionItem::ItemType AgendaItem::getType(){
    return TranscriptionItem::TypeAgendaItem;
}
