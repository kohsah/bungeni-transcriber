/********************************************************************
 * speechEditor.cpp
 ********************************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - UNDESA <www.parliaments.info>
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

#include <QHBoxLayout>
#include <QGroupBox>

#include "speechEditor.hpp"
#include "transcribeWidget.hpp"
#include "util/util.hpp"
SpeechEditor::SpeechEditor(QWidget * parent) : TranscriptionItemEditor(parent)
{
    ui.setupUi(this);
    horizontalSlider = new QxtSpanSlider();
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    horizontalSlider->setOrientation(Qt::Horizontal);
    ui.gridLayout->addWidget(horizontalSlider, 0, 3, 1, 3);
    ui.play->setIcon(QIcon(":/pixmaps/play.png"));
    speechText = new customTextEdit(parent);
    speechText->setObjectName(QString::fromUtf8("speech"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(3);
    sizePolicy.setHeightForWidth(speechText->sizePolicy().hasHeightForWidth());
    speechText->setSizePolicy(sizePolicy);
    ui.gridLayout->addWidget(speechText, 3, 0, 1, 7);
    
    QObject::connect(ui.startTime, SIGNAL(timeChanged( const QTime & )), this, SLOT(updateStartTime( const QTime &)));
    QObject::connect(ui.endTime, SIGNAL(timeChanged( const QTime & )), this, SLOT(updateEndTime( const QTime &)));
    QObject::connect(horizontalSlider, SIGNAL( lowerValueChanged( int ) ), this, SLOT( updateStartTime( int ) ));
    QObject::connect(horizontalSlider, SIGNAL( upperValueChanged( int ) ), this, SLOT( updateEndTime( int ) ));  
    
    QObject::connect( horizontalSlider, SIGNAL(lowerValueChanged( int )), TranscribeWidget::getInstance(), SLOT(changePosition( int ) ));
    QObject::connect( horizontalSlider, SIGNAL(upperValueChanged( int )), TranscribeWidget::getInstance(), SLOT(changePosition( int ) ));
    
    QObject::connect( ui.bold, SIGNAL(clicked( )), this, SLOT( bold( ) ) );
    QObject::connect( ui.italics, SIGNAL(clicked( )), this, SLOT( italics( ) ) );
    QObject::connect( ui.underline, SIGNAL(clicked( )), this, SLOT( underline( ) ) );
    QObject::connect( ui.save, SIGNAL(clicked( )), this, SLOT( save( ) ) );
    QObject::connect( ui.cancel, SIGNAL(clicked( )), this, SLOT( cancel( ) ) );
    QObject::connect( ui.play, SIGNAL(clicked( )), this, SLOT( play( ) ) );
    this->setDuration(TranscribeWidget::getInstance()->getFileDuration());
}

SpeechEditor::~SpeechEditor()
{}
/*
void SpeechEditor::setValues(QString _name, QString _speech, int _startTime, int _endTime)
{
    ui.name->setText(_name);
    speechText->setText(_speech);
}
*/

void SpeechEditor::setAutoCompleteWordlist(const QStringList & wordlist )
{
    QCompleter *completer = new QCompleter(wordlist);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui.name->setCompleter(completer);
}

void SpeechEditor::updateEndTime( const QTime & time )
{
	int temp;
	temp = time.hour() * 3600 + time.minute()*60 + time.second();
	horizontalSlider->setUpperValue(temp);
}

void SpeechEditor::updateStartTime( const QTime & time )
{
	int temp;
	temp = time.hour() * 3600 + time.minute()*60 + time.second();
	horizontalSlider->setLowerValue(temp);
}

void SpeechEditor::updateStartTime( int time )
{
	//int hours, minutes, seconds;
	QString temp;
	QTime start;
	QString timeText = "";
	timeText = timeSecondstoString(time);
	start = QTime::fromString(timeText);
	ui.startTime->setTime(start);
	
}

void SpeechEditor::updateEndTime( int time )
{
	//int hours, minutes, seconds;
	QString temp;
	QTime end;
	QString timeText = "";
	timeText = timeSecondstoString(time);
	end = QTime::fromString(timeText);
	ui.endTime->setTime(end);
}
void SpeechEditor::bold()
{
    QTextCursor cursor(speechText->textCursor());
    QTextCharFormat format;
    if (cursor.charFormat().fontItalic()==true)
    {    
        format.setFontItalic(true);
    }
    if (cursor.charFormat().fontUnderline()==true)
    {    
        format.setFontUnderline(true);
    }
    if (cursor.charFormat().fontWeight()==QFont::Bold)
    {    
        format.setFontWeight(QFont::Light);     
    }
    else
    {
        format.setFontWeight(QFont::Bold);
    }
    cursor.setCharFormat(format);
}

void SpeechEditor::underline()
{
    QTextCursor cursor(speechText->textCursor());
    QTextCharFormat format;
    if (cursor.charFormat().fontItalic()==true)
    {    
        format.setFontItalic(true);
    }
    if (cursor.charFormat().fontWeight()==QFont::Bold)
    {    
        format.setFontWeight(QFont::Bold);     
    }
    if (cursor.charFormat().fontUnderline()==true)
    {    
        format.setFontUnderline(false);
    }
    else
    {
        format.setFontUnderline(true);
    }
    cursor.setCharFormat(format);
}

void SpeechEditor::italics()
{
    QTextCursor cursor(speechText->textCursor());
    QTextCharFormat format;
    if (cursor.charFormat().fontUnderline()==true)
    {    
        format.setFontUnderline(true);
    }
    if (cursor.charFormat().fontWeight()==QFont::Bold)
    {    
        format.setFontWeight(QFont::Bold);     
    }
    if (cursor.charFormat().fontItalic()==true)
    {    
        format.setFontItalic(false);
    }
    else
    {
        format.setFontItalic(true);
    }
    cursor.setCharFormat(format);
}

void SpeechEditor::save()
{
    emit close(this);
    emit closeIndex(index);
}

void SpeechEditor::play()
{
    TranscribeWidget *instance = TranscribeWidget::getInstance();
    instance->play();
    instance->changePosition(horizontalSlider->lowerValue());
}

void SpeechEditor::cancel()
{
    emit cancelled(this);
    emit closeIndex(index);
}

void SpeechEditor::setIndex(const QModelIndex & index_)
{
    this->index = index_;
}


QString SpeechEditor::getName()
{
    return ui.name->text();
}


QString SpeechEditor::getSpeech()
{
    return speechText->toPlainText();
    //return speechText->toHtml();
}


int SpeechEditor::getStartTime()
{
  //  qDebug() << "inplace editor getStartTime " << ui.startTime->time();
   // return ui.startTime->time();
   qDebug() << "inplace editor getStartTime " << horizontalSlider->lowerValue();
   qDebug() << "inplace editor ui.starttime->time " << ui.startTime->time();;
   return horizontalSlider->lowerValue();
}
    
int SpeechEditor::getEndTime()
{
    //qDebug() << "inplace editor getendTime " << ui.endTime->time();
   // return ui.endTime->time();
   return horizontalSlider->upperValue();
}

bool SpeechEditor::getComplete()
{
    if ( ui.incomplete->isChecked())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SpeechEditor::setName(QString name)
{
    ui.name->setText(name);
}

void SpeechEditor::setComplete(bool incomplete)
{
    if (incomplete)
    {
        ui.incomplete->setCheckState(Qt::Checked);
    }
    else
    {
        ui.incomplete->setCheckState(Qt::Unchecked);
    }
}

void SpeechEditor::setDuration(int sec)
{
    horizontalSlider->setMaximum(sec);
    int hour = sec / 3600;
    int min = (sec % 3600 ) / 60;
    int seconds = (sec % 3600) % 60;
    ui.startTime->setMaximumTime(QTime(hour, min, seconds));
    ui.endTime->setMaximumTime(QTime(hour, min, seconds));
}
 
void SpeechEditor::setSpeech(QString speech)
{
   // ui.speech->setText(speech);
   speechText->setHtml(speech);
}
    
void SpeechEditor::setStartTime(QTime start)
{
    /*
    int hour = start / 3600;
    int min = (start % 3600 ) / 60;
    int seconds = (start % 3600) % 60;
    ui.startTime->setTime(QTime(hour, min, seconds));
    */
    //horizontalSlider->setLowerValue(start);
}
    
void SpeechEditor::setEndTime(QTime end)
{
    //ui.endTime->setTime(QTime::fromString(SpeechEditor::timeSecondstoString(end)));
    //horizontalSlider->setLowerValue(end);
}

void SpeechEditor::setMPList(QStringList list)
{
    MPList = list;
}
