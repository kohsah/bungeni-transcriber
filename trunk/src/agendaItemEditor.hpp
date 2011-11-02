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


#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QTimeEdit>
#include <QModelIndex>
#include <QTime>
#include <QxtSpanSlider>
#include "util/util.hpp"
#include "transcriptionItemEditor.hpp"
#include "ui/ui_agendaItemEditor.h"
class AgendaItemEditor : public TranscriptionItemEditor
{
    Q_OBJECT
public:
    AgendaItemEditor(QWidget * parent = 0);
    virtual ~AgendaItemEditor();
    QTime getStartTime();
    QTime getEndTime();
    void setStartTime(QTime);
    void setEndTime(QTime);
    void setIndex(const QModelIndex& );
    void setDuration(int);
    void setAgendaList(const QStringList &);
    QString getAgendaItem();
    void setAgendaList(QStringList list);
private:
    Ui::agendaItemEditor ui;
    QxtSpanSlider *horizontalSlider; 
    QModelIndex index;
    QStringList agendaList;
public slots:
    void updateStartTime( const QTime & );
    void updateEndTime( const QTime & );
    void updateStartTime( int );
    void updateEndTime( int );
    void save();
    void cancel();
};



#endif
