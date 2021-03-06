/**********************************************************
 * speechEditor.hpp
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
 
 
 

#ifndef _SPEECHEDITOR_H_
#define _SPEECHEDITOR_H_


#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QTimeEdit>
#include <QModelIndex>
#include "customTextEdit.hpp"
#include <QCompleter>
#include <QTime>
#include <QxtSpanSlider>
#include <QStandardItemModel>
#include "transcriptionItemEditor.hpp"
#include "ui/ui_speechEditor.h"
#include "model/personsModel.hpp"
#include "personLineEdit.hpp"
class SpeechEditor : public TranscriptionItemEditor
{
    Q_OBJECT
public:
    SpeechEditor(QWidget * parent = 0);
    virtual ~SpeechEditor();
   // void setValues(QString _name, QString _speech, int _startTime, int _endTime);
    Person* getPerson();
    QString getSpeech();
    QTime getStartTime();
    QTime getEndTime();
    bool getComplete();
    void setComplete(bool incomplete);
    void setPerson(Person*);
    void setSpeech(QString);
    void setStartTime(QTime);
    void setEndTime(QTime);
    void setIndex(const QModelIndex& );
    void setDuration(int);
    void setPersonsModel(PersonsModel*);

private:
    Ui::speechEditor ui;
    QxtSpanSlider *horizontalSlider; 
    CustomTextEdit *speechText;
    QModelIndex index;
    QStringList MPList;
    //void formatText(QTextCursor, QString);
    QString speech;
    PersonLineEdit *person;
public slots:
    void updateStartTime( const QTime & );
    void updateEndTime( const QTime & );
    void updateStartTime( int );
    void updateEndTime( int );
    void bold();
    void italics();
    void underline();
    void save();
    void cancel();
    void play();
};



#endif
