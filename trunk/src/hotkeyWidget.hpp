/**********************************************************
 * hotkeyWidget.hpp
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

#ifndef _HOTKEYWIDGET_H_
#define _HOTKEYWIDGET_H_

#include <QDialog>
#include "transcribeKeySelector.hpp"
class HotkeyWidget : public QDialog
{
    Q_OBJECT
    public:
        HotkeyWidget();
    private:
        transcribeKeySelectorControl * control;
    public slots:
        void apply();
};


#endif
