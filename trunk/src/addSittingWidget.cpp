/********************************************************************
 * addSittingWidget.cpp
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
#include "addSittingWidget.hpp"

AddSittingWidget :: AddSittingWidget() : QDialog()
{
    ui.setupUi(this);
    this->setModal(true);
    ui.startDateTime->setDisplayFormat(QString("dd-MM-yyyy hh:mm:ss"));
    ui.endDateTime->setDisplayFormat(QString("dd-MM-yyyy hh:mm:ss"));
    QObject::connect(ui.startDateTime, SIGNAL(dateTimeChanged(QDateTime)), ui.endDateTime, SLOT(setDateTime(QDateTime)));
    QObject::connect(ui.startDateTime, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(startDateTimeChanged(QDateTime)));
    ui.startDateTime->setDateTime(QDateTime::currentDateTime());
}

AddSittingWidget :: ~AddSittingWidget()
{
}

QString AddSittingWidget ::getTrsFileLocation()
{
    return ui.existingPathLineEdit->text();
}

QString AddSittingWidget ::getSittingName()
{
    return ui.sittingNameLineEdit->text();
}

QDateTime AddSittingWidget ::getStartDateTime()
{
    return ui.startDateTime->dateTime();
}

QDateTime AddSittingWidget ::getEndDateTime()
{
    return ui.endDateTime->dateTime();
}
void AddSittingWidget :: locateExistingPathSlot()
{
    QString existingFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
    ui.existingPathLineEdit->setText(existingFilePath);
}

void AddSittingWidget :: addExistingGroupBoxProceessChecked(bool checked)
{
    if (checked)
    {
        ui.addNewGroupBox->setChecked(false);
    }
    else
    {
        ui.addNewGroupBox->setChecked(true);
    }
}

bool AddSittingWidget :: addExisting()
{
    if (ui.addExistingGroupBox->isChecked())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void AddSittingWidget :: addNewGroupBoxProceessChecked(bool checked)
{
    if (checked)
    {
        ui.addExistingGroupBox->setChecked(false);
    }
    else
    {
        ui.addExistingGroupBox->setChecked(true);
    }
}

void AddSittingWidget::startDateTimeChanged(const QDateTime &dateTime){
    ui.endDateTime->setMinimumDateTime(dateTime);
}
