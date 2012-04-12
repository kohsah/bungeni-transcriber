/**********************************************************
 * takeEditorWidget.cpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - UNDESA
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
#include <QFileDialog>
#include "takeEditorWidget.hpp"

TakeEditorWidget::TakeEditorWidget() : QDialog(){
    ui.setupUi(this);
    this->setModal(true);
    ui.startDateTime->setDisplayFormat(QString("dd-MM-yyyy hh:mm:ss"));
    ui.endDateTime->setDisplayFormat(QString("dd-MM-yyyy hh:mm:ss"));
    QObject::connect(ui.startDateTime, SIGNAL(dateTimeChanged(QDateTime)), ui.endDateTime, SLOT(setDateTime(QDateTime)));
    QObject::connect(ui.startDateTime, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(startDateTimeChanged(QDateTime)));
    QObject::connect( ui.locateMediaLocation, SIGNAL(clicked()), this, SLOT(locateMediaLocation()));
}
QString TakeEditorWidget::getTakeName(){
    return ui.takeName->text();
}
QString TakeEditorWidget::getMediaLocation() {
    return ui.mediaLocation->text();
}

QDateTime TakeEditorWidget::getStartTime(){
    return ui.startDateTime->dateTime();
}

QDateTime TakeEditorWidget::getEndTime(){
    return ui.endDateTime->dateTime();
}

void TakeEditorWidget::locateMediaLocation()
{
    QString newFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Media Files (*.ogv *.ogg);;Any File (*.*)"));
    ui.mediaLocation->setText(newFilePath);
}

void TakeEditorWidget::setTakeName(QString takeName){
    ui.takeName->setText(takeName);
}

void TakeEditorWidget::setStartTime(QDateTime startTime){
    ui.startDateTime->setDateTime(startTime);
}

void TakeEditorWidget::setEndTime(QDateTime endTime){
    ui.endDateTime->setDateTime(endTime);
}

void TakeEditorWidget::setMediaLocation(QString mediaLocation){
    ui.mediaLocation->setText(mediaLocation);
}

void TakeEditorWidget::startDateTimeChanged(const QDateTime &dateTime){
    ui.endDateTime->setMinimumDateTime(dateTime);
}

void TakeEditorWidget::setParentTimes(const QDateTime & startDateTime, const QDateTime &endDateTime){
    ui.startDateTime->setMinimumDateTime(startDateTime);
    ui.startDateTime->setMaximumDateTime(endDateTime);
    ui.endDateTime->setMinimumDateTime(startDateTime);
    ui.endDateTime->setMaximumDateTime(endDateTime);
}
