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
AgendaItemEditor::AgendaItemEditor(QWidget * parent) : QWidget(parent)
{
    ui.setupUi(this);
    //TranscribeWidget *transcribe = static_cast<TranscribeWidget*>(parent);
   // p_intf = transcribe->getp_intf();
    horizontalSlider = new QxtSpanSlider();
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    horizontalSlider->setOrientation(Qt::Horizontal);
    //horizontalSlider->setMaximum(10000);
    ui.gridLayout->addWidget(horizontalSlider, 0, 1, 1, 3);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    
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
	timeText = AgendaItemEditor::timeSecondstoString(time);
	start = QTime::fromString(timeText);
	ui.startTime->setTime(start);
	
}

void AgendaItemEditor::updateEndTime( int time )
{
	//int hours, minutes, seconds;
	QString temp;
	QTime end;
	QString timeText = "";
	timeText = AgendaItemEditor::timeSecondstoString(time);
	end = QTime::fromString(timeText);
	ui.endTime->setTime(end);
}

//Converts number of seconds into time in this format hh:mm:ss
QString AgendaItemEditor::timeSecondstoString(int time)
{

	int hours, minutes, seconds;
	QString temp;
	hours = time / 3600;
	minutes = (time % 3600) / 60;
	seconds = (time % 3600) % 60;
	
QString timeText = "";

if (hours < 10)
	{
		timeText.append("0"); 
		temp.setNum(hours);
		timeText += temp;
	}
	else
	{
		temp.setNum(hours);
		timeText += temp;
	}
	if ( minutes < 10 )
	{
		timeText.append(":0"); 
		temp.setNum(minutes);
		timeText += temp;
	}
	else
	{
		temp.setNum(minutes);
		timeText += ":"+temp;
	}
	if ( seconds < 10 )
	{
		timeText.append(":0"); 
		temp.setNum(seconds);
		timeText += temp;
	}
	else
	{
		temp.setNum(seconds);
		timeText += ":" + temp;
	}
	return timeText;

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

int AgendaItemEditor::getStartTime()
{
  //  qDebug() << "inplace editor getStartTime " << ui.startTime->time();
   // return ui.startTime->time();
   qDebug() << "inplace editor getStartTime " << horizontalSlider->lowerValue();
   qDebug() << "inplace editor ui.starttime->time " << ui.startTime->time();;
   return horizontalSlider->lowerValue();
}
    
int AgendaItemEditor::getEndTime()
{
    //qDebug() << "inplace editor getendTime " << ui.endTime->time();
   // return ui.endTime->time();
   return horizontalSlider->upperValue();
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

void AgendaItemEditor::setStartTime(int start)
{
    /*
    int hour = start / 3600;
    int min = (start % 3600 ) / 60;
    int seconds = (start % 3600) % 60;
    ui.startTime->setTime(QTime(hour, min, seconds));
    */
    horizontalSlider->setLowerValue(start);
}
    
void AgendaItemEditor::setEndTime(int end)
{
    //ui.endTime->setTime(QTime::fromString(InplaceEditor::timeSecondstoString(end)));
    horizontalSlider->setLowerValue(end);
}
