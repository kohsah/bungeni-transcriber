/********************************************************************
 * addToPlaylistWidget.cpp
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
#include "addToPlaylistWidget.hpp"

AddToPlaylistWidget :: AddToPlaylistWidget() : QDialog()
{
    ui.setupUi(this);
    this->setModal(true);
    QObject::connect( ui.addNewGroupBox, SIGNAL(clicked(bool)), this, SLOT(addNewGroupBoxProceessChecked(bool)));
    QObject::connect( ui.addExistingGroupBox, SIGNAL(clicked(bool)), this, SLOT(addExistingGroupBoxProceessChecked(bool)));
    QObject::connect( ui.locateExistingButton, SIGNAL(clicked()), this, SLOT(locateExistingPathSlot()));
    QObject::connect( ui.locateNewButton, SIGNAL(clicked()), this, SLOT(locateNewPathSlot()));
}

AddToPlaylistWidget :: ~AddToPlaylistWidget()
{
}

QString AddToPlaylistWidget ::getTrsFileLocation()
{
    return ui.existingPathLineEdit->text();
}

QString AddToPlaylistWidget ::getSittingName()
{
    return ui.sittingNameLineEdit->text();
}
/*
QDateTime AddToPlaylistWidget ::getStartTime()
{
    return ui.startTime->dateTime();
}

QDateTime AddToPlaylistWidget ::getEndTime()
{
    return ui.endTime->dateTime();
}
*/
QString AddToPlaylistWidget ::getMediaFileLocation()
{
    return ui.newPathLineEdit->text();
}


void AddToPlaylistWidget :: locateExistingPathSlot()
{
    QString existingFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
    ui.existingPathLineEdit->setText(existingFilePath);
}

void AddToPlaylistWidget :: locateNewPathSlot()
{
    QString newFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Media Files (*.ogv *.ogg);;Any File (*.*)"));
    ui.newPathLineEdit->setText(newFilePath);
}

void AddToPlaylistWidget :: addExistingGroupBoxProceessChecked(bool checked)
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

bool AddToPlaylistWidget :: addExisting()
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

void AddToPlaylistWidget :: addNewGroupBoxProceessChecked(bool checked)
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
