/********************************************************************
 * agendaItemEditor.cpp
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
 
 

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "agendaItemEditor.hpp"
#include "transcribeWidget.hpp"

#include <QHBoxLayout>
#include <QGroupBox>
AgendaItemEditor::AgendaItemEditor(QWidget * parent) : TranscriptionItemEditor(parent)
{
    ui.setupUi(this);
    horizontalSlider = new QxtSpanSlider();
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    horizontalSlider->setOrientation(Qt::Horizontal);
    ui.gridLayout->addWidget(horizontalSlider, 0, 1, 1, 3);
    this->setDuration(TranscribeWidget::getInstance()->getFileDuration());
    QObject::connect(ui.startTime, SIGNAL(timeChanged(const QTime &)), this, SLOT(updateStartTime(const QTime &)));
    QObject::connect(ui.endTime, SIGNAL(timeChanged(const QTime &)), this, SLOT(updateEndTime(const QTime &)));
    QObject::connect(horizontalSlider, SIGNAL(lowerValueChanged(int)), this, SLOT( updateStartTime(int )));
    QObject::connect(horizontalSlider, SIGNAL(upperValueChanged(int)), this, SLOT( updateEndTime(int)));
    QObject::connect( horizontalSlider, SIGNAL(lowerValueChanged(int)), TranscribeWidget::getInstance(), SLOT(changePosition( int ) ));
    QObject::connect( horizontalSlider, SIGNAL(upperValueChanged(int)), TranscribeWidget::getInstance(), SLOT(changePosition( int ) ));
    QObject::connect( ui.save, SIGNAL(clicked()), this, SLOT( save()));
    QObject::connect( ui.cancel, SIGNAL(clicked()), this, SLOT( cancel()));

}

AgendaItemEditor::~AgendaItemEditor()
{}

void AgendaItemEditor::updateStartTime( const QTime & time )
{
    int temp = time.hour() * 3600 + time.minute()*60 + time.second();
    horizontalSlider->setLowerPosition(temp);
}

void AgendaItemEditor::updateEndTime( const QTime & time )
{
    int temp = time.hour() * 3600 + time.minute()*60 + time.second();
    horizontalSlider->setUpperPosition(temp);
}

void AgendaItemEditor::updateStartTime( int time )
{
    ui.startTime->setTime(QTime::fromString(timeSecondstoString(time)));
}

void AgendaItemEditor::updateEndTime( int time )
{
    ui.endTime->setTime(QTime::fromString(timeSecondstoString(time)));
}

void AgendaItemEditor::save()
{
    emit close(this);
    emit closeIndex(index);
}

void AgendaItemEditor::cancel()
{
    emit cancelled(this);
    emit closeIndex(index);
}

void AgendaItemEditor::setIndex(const QModelIndex & index_)
{
    this->index = index_;
}


void AgendaItemEditor::setAgendaList(const QStringList & list)
{
    ui.comboBox->insertItems(0, list);
}

QTime AgendaItemEditor::getStartTime()
{
    return ui.startTime->time();
}
    
QTime AgendaItemEditor::getEndTime()
{
    return ui.endTime->time();
}

QString AgendaItemEditor::getAgendaItem()
{
    return ui.comboBox->currentText();
}

void AgendaItemEditor::setDuration(int sec)
{
    horizontalSlider->setMinimum(0);
    horizontalSlider->setMaximum(sec);
    int hour = sec / 3600;
    int min = (sec % 3600 ) / 60;
    int seconds = (sec % 3600) % 60;
    ui.startTime->setMaximumTime(QTime(hour, min, seconds));
    ui.endTime->setMaximumTime(QTime(hour, min, seconds));
}

void AgendaItemEditor::setStartTime(QTime start)
{
    ui.startTime->setTime(start);
}
    
void AgendaItemEditor::setEndTime(QTime end)
{
    ui.endTime->setTime(end);
}

void AgendaItemEditor::setAgendaList(QStringList list)
{
    agendaList = list;
}
