/********************************************************************
 * takeEditorWidget.hpp
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
#ifndef TAKEEDITORWIDGET_HPP
#define TAKEEDITOREWIDGET_HPP

#include <QObject>
#include <QString>
#include <QDateTime>
#include "ui/ui_takeEditor.h"
#include "model/sitting.hpp"
class TakeEditorWidget : public QDialog
{
    Q_OBJECT
public:
    TakeEditorWidget();
    QString getTakeName();
    QString getMediaLocation();
    QDateTime getStartTime();
    QDateTime getEndTime();
    void setTakeName(QString);
    void setStartTime(QDateTime);
    void setEndTime(QDateTime);
    void setMediaLocation(QString);
    void setParentTimes(const QDateTime &, const QDateTime &);
private:
    Ui::addTakeDialog ui;
    Sitting *sitting;
public slots:
    void locateMediaLocation();
    void startDateTimeChanged(const QDateTime &);
};
#endif // TakeEditorWidget_HPP
