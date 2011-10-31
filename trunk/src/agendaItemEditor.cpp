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
    //TranscribeWidget *transcribe = static_cast<TranscribeWidget*>(parent);
   // p_intf = transcribe->getp_intf();
    horizontalSlider = new QxtSpanSlider();
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    horizontalSlider->setOrientation(Qt::Horizontal);
    //horizontalSlider->setMaximum(10000);
    ui.gridLayout->addWidget(horizontalSlider, 0, 1, 1, 3);
    QObject::connect(ui.startTime, SIGNAL(timeChanged( const QTime & )), this, SLOT(updateStartTime( const QTime &)));
    QObject::connect(ui.endTime, SIGNAL(timeChanged( const QTime & )), this, SLOT(updateEndTime( const QTime &)));
    QObject::connect(horizontalSlider, SIGNAL( lowerValueChanged( int ) ), this, SLOT( updateStartTime( int ) ));
    QObject::connect(horizontalSlider, SIGNAL( upperValueChanged( int ) ), this, SLOT( updateEndTime( int ) ));  
    
    QObject::connect( horizontalSlider, SIGNAL(lowerValueChanged( int )), TranscribeWidget::getInstance(), SLOT(changePosition( int ) ));
    QObject::connect( horizontalSlider, SIGNAL(upperValueChanged( int )), TranscribeWidget::getInstance(), SLOT(changePosition( int ) ));
    
    
    QObject::connect( ui.save, SIGNAL(clicked( )), this, SLOT( save( ) ) );
    QObject::connect( ui.cancel, SIGNAL(clicked( )), this, SLOT( cancel( ) ) );
    this->setDuration(TranscribeWidget::getInstance()->getFileDuration());
}

AgendaItemEditor::~AgendaItemEditor()
{}
/*
void InplaceEditor::setValues(QString _name, QString _speech, int _startTime, int _endTime)
{
    ui.name->setText(_name);
    speechText->setText(_speech);
}
*/


void AgendaItemEditor::updateEndTime( const QTime & time )
{
	int temp;
	temp = time.hour() * 3600 + time.minute()*60 + time.second();
	horizontalSlider->setUpperValue(temp);
}

void AgendaItemEditor::updateStartTime( const QTime & time )
{
	int temp;
	temp = time.hour() * 3600 + time.minute()*60 + time.second();
	horizontalSlider->setLowerValue(temp);
}

void AgendaItemEditor::updateStartTime( int time )
{
	//int hours, minutes, seconds;
	QString temp;
	QTime start;
	QString timeText = "";
        timeText = timeSecondstoString(time);
	start = QTime::fromString(timeText);
	ui.startTime->setTime(start);
	
}

void AgendaItemEditor::updateEndTime( int time )
{
	//int hours, minutes, seconds;
	QString temp;
	QTime end;
	QString timeText = "";
        timeText = timeSecondstoString(time);
	end = QTime::fromString(timeText);
	ui.endTime->setTime(end);
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
   //return horizontalSlider->lowerValue();
    return QTime();
}
    
QTime AgendaItemEditor::getEndTime()
{
   //return horizontalSlider->upperValue();
    return QTime();
}

QString AgendaItemEditor::getAgendaItem()
{
    return ui.comboBox->currentText();
}

void AgendaItemEditor::setDuration(int sec)
{
    horizontalSlider->setMaximum(sec);
    int hour = sec / 3600;
    int min = (sec % 3600 ) / 60;
    int seconds = (sec % 3600) % 60;
    ui.startTime->setMaximumTime(QTime(hour, min, seconds));
    ui.endTime->setMaximumTime(QTime(hour, min, seconds));
}

void AgendaItemEditor::setStartTime(QTime start)
{
    //horizontalSlider->setLowerValue(start);
}
    
void AgendaItemEditor::setEndTime(QTime end)
{
    //horizontalSlider->setLowerValue(end);
}

void AgendaItemEditor::setAgendaList(QStringList list)
{
    agendaList = list;
}
