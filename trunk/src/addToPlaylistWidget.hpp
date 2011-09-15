/**********************************************************
 * addToPlaylistWidget.hpp
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
#ifndef _ADDTOPLAYLISTWIDGET_H_
#define _ADDTOPLAYLISTWIDGET_H_

#include <QString>
#include <QDateTime>
 
#include "ui/ui_addToPlaylist.h" 

class AddToPlaylistWidget : public QDialog
{
    Q_OBJECT
    public:
        AddToPlaylistWidget();
        ~AddToPlaylistWidget();
        QString getTrsFileLocation();
        QString getSittingName();
        //QDateTime getStartTime();
        //QDateTime getEndTime();
        QString getMediaFileLocation();
        bool addExisting();
    private:
        Ui::addToPlaylist ui;
    public slots:
        void addExistingGroupBoxProceessChecked(bool checked);
        void addNewGroupBoxProceessChecked(bool checked);
        void locateExistingPathSlot();
        void locateNewPathSlot();
};
#endif
