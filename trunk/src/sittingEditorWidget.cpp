/********************************************************************
 * sittingEditorWidget.cpp
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
 
#include <QFileDialog>
#include "sittingEditorWidget.hpp"

SittingEditorWidget::SittingEditorWidget() : QDialog()
{
    ui.setupUi(this);
    this->setModal(true);
    ui.startDateTime->setDisplayFormat(QString("dd-MM-yyyy hh:mm:ss"));
    ui.endDateTime->setDisplayFormat(QString("dd-MM-yyyy hh:mm:ss"));
    QObject::connect(ui.startDateTime, SIGNAL(dateTimeChanged(QDateTime)), ui.endDateTime, SLOT(setDateTime(QDateTime)));
    QObject::connect(ui.startDateTime, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(startDateTimeChanged(QDateTime)));
    ui.startDateTime->setDateTime(QDateTime::currentDateTime());
}

SittingEditorWidget::~SittingEditorWidget()
{
}

QString SittingEditorWidget::getSittingName()
{
    return ui.sittingNameLineEdit->text();
}

QDateTime SittingEditorWidget::getStartDateTime()
{
    return ui.startDateTime->dateTime();
}

QDateTime SittingEditorWidget::getEndDateTime()
{
    return ui.endDateTime->dateTime();
}
void SittingEditorWidget::setStartDateTime(QDateTime startDateTime)
{
    ui.startDateTime->setDateTime(startDateTime);
}

void SittingEditorWidget::setEndDateTime(QDateTime endDateTime){
    ui.startDateTime->setDateTime(endDateTime);
}

void SittingEditorWidget::setSittingName(QString sittingName){
    ui.sittingNameLineEdit->setText(sittingName);
}

void SittingEditorWidget::startDateTimeChanged(const QDateTime &dateTime){
    ui.endDateTime->setMinimumDateTime(dateTime);
}
