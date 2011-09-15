/**********************************************************
 * agendaItemEditor.hpp
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
 
 
 

#ifndef _AGENDAITEMEDITOR_H_
#define _AGENDAITEMEDITOR_H_

#include "qxtspanslider/qxtspanslider.h"
#include "ui/ui_agendaItemEditor.h"
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QTimeEdit>
#include <QModelIndex>
class AgendaItemEditor : public QWidget
{
    Q_OBJECT
public:
    AgendaItemEditor(QWidget * parent = 0);
    virtual ~AgendaItemEditor();
    int getStartTime();
    int getEndTime();
    void setStartTime(int);
    void setEndTime(int);
    void setIndex(const QModelIndex& );
    void setDuration(int);
    void setAgendaList(const QStringList &);
    QString getAgendaItem();
private:
    QString timeSecondstoString(int time);
    Ui::agendaItemEditor ui;
    QxtSpanSlider *horizontalSlider; 
    QModelIndex index;
public slots:
    void updateStartTime( const QTime & );
    void updateEndTime( const QTime & );
    void updateStartTime( int );
    void updateEndTime( int );
    void save();
    void cancel();
signals:
    void close(QWidget *);
    void cancelled(QWidget *);
    void closeIndex(QModelIndex); 
};



#endif
