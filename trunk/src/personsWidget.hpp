/********************************************************************
 * persons.hpp
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

#ifndef PERSONS_HPP
#define PERSONS_HPP
#include <QStandardItemModel>
#include "ui/ui_personsWidget.h"

class PersonsWidget : public QDialog {
    Q_OBJECT
public:
    PersonsWidget(QWidget*);
    void setModel(QStandardItemModel *);
private:
    Ui::PersonsWidget ui;
};

#endif // PERSONS_HPP
