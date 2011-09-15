/**********************************************************
 * util.hpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - UNDESA
 *
 * Author - Miano Njoka <miano@parliaments.info>
 *
 * Derived from  customwidgets.h: Custom widgets
 * Copyright (C) 2006 the VideoLAN team
 * Copyright (C) 2004 Daniel Molkentin <molkentin@kde.org>
 * $Id: cac087e8b295d5f5491e72ebe36432a7997b844e $
 *
 *
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
 
 
 
#ifndef _UTIL_H_
#define _UTIL_H_

#include "vlc_keys.h"
#include <QLineEdit>
#include  <QKeyEvent>
#include  <QWheelEvent>
#include  <QInputEvent>

class KeyShortcutEdit : public QLineEdit
{
    Q_OBJECT
public:
    void setValue( int _value ){ value = _value; }
    int getValue() const { return value; }
public slots:
    virtual void clear(void) { value = 0; QLineEdit::clear(); }
private:
    int value;
    virtual void mousePressEvent( QMouseEvent *event );
signals:
    void pressed();
};

int qtKeyModifiersToVLC( QInputEvent* e );
int qtEventToVLCKey( QKeyEvent *e );
int qtWheelEventToVLCKey( QWheelEvent *e );
QString VLCKeyToString( int val );
QString timeSecondstoString(int time);
#endif
