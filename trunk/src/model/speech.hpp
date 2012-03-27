/********************************************************************
 * speech.hpp
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
#ifndef SPEECH_HPP
#define SPEECH_HPP

#include <QObject>
#include <QTime>
#include "transcriptionItem.hpp"
#include "person.hpp"

class Speech : public TranscriptionItem{
public:
    explicit Speech(QTime, QTime, QString, Person*, int, bool);
    void setPerson(Person*);
    void setSpeech(QString);
    void setId(int id);
    void setStartTime(QTime);
    void setEndTime(QTime);
    Person* getPerson();
    QString getSpeech();
    int getId();
    QTime getStartTime();
    QTime getEndTime();
    void setComplete(bool);
    bool isComplete();
    ItemType getType();
protected:
    Person* person;
    QString speech;
    int id;
    bool complete;
};

#endif // SPEECH_HPP
