/**********************************************************
 * inplaceEditor.hpp
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
 
 
 

#ifndef _INPLACEEDITOR_H_
#define _INPLACEEDITOR_H_

#include "qxtspanslider/qxtspanslider.h"
#include "ui/ui_inplace_editor.h"
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QTimeEdit>
#include <QModelIndex>
#include "customTextEdit.hpp"
#include <QCompleter>
class InplaceEditor : public QWidget
{
    Q_OBJECT
public:
    InplaceEditor(QWidget * parent = 0);
    virtual ~InplaceEditor();
   // void setValues(QString _name, QString _speech, int _startTime, int _endTime);
    QString getName();
    QString getSpeech();
    //QTime getStartTime();
   // QTime getEndTime();
    int getStartTime();
    int getEndTime();
    bool getComplete();
    void setComplete(bool incomplete);
    void setName(QString);
    void setSpeech(QString);
    void setStartTime(int);
    void setEndTime(int);
    void setIndex(const QModelIndex& );
    void setDuration(int);
    void setAutoCompleteWordlist(const QStringList &);
private:
    QString timeSecondstoString(int time);
    Ui::inPlaceEditor ui;
    QxtSpanSlider *horizontalSlider; 
    customTextEdit *speechText;
    QModelIndex index;
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
signals:
    void close(QWidget *);
    void cancelled(QWidget *);
    void closeIndex(QModelIndex); 
};



#endif
