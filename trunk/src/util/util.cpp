/********************************************************************
 * util.cpp
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

#include "util.hpp"

#include <QKeyEvent>
#include <QWheelEvent>

#define HANDLE( qt, vk ) case Qt::qt : i_vlck |= vk; found = true;break

/***************************************************************************
 * Hotkeys converters
 ***************************************************************************/
int qtKeyModifiersToVLC( QInputEvent* e )
{
    int i_keyModifiers = 0;
    if( e->modifiers() & Qt::ShiftModifier ) i_keyModifiers |= KEY_MODIFIER_SHIFT;
    if( e->modifiers() & Qt::AltModifier ) i_keyModifiers |= KEY_MODIFIER_ALT;
    if( e->modifiers() & Qt::ControlModifier ) i_keyModifiers |= KEY_MODIFIER_CTRL;
    if( e->modifiers() & Qt::MetaModifier ) i_keyModifiers |= KEY_MODIFIER_META;
    return i_keyModifiers;
}

int qtEventToVLCKey( QKeyEvent *e )
{
    int i_vlck = 0;
    /* Handle modifiers */
    i_vlck |= qtKeyModifiersToVLC( e );

    bool found = false;
    /* Look for some special keys */
    switch( e->key() )
    {
        HANDLE( Key_Left, KEY_LEFT );
        HANDLE( Key_Right, KEY_RIGHT );
        HANDLE( Key_Up, KEY_UP );
        HANDLE( Key_Down, KEY_DOWN );
        HANDLE( Key_Space, KEY_SPACE );
        HANDLE( Key_Escape, KEY_ESC );
        HANDLE( Key_Return, KEY_ENTER );
        HANDLE( Key_Enter, KEY_ENTER );
        HANDLE( Key_F1, KEY_F1 );
        HANDLE( Key_F2, KEY_F2 );
        HANDLE( Key_F3, KEY_F3 );
        HANDLE( Key_F4, KEY_F4 );
        HANDLE( Key_F5, KEY_F5 );
        HANDLE( Key_F6, KEY_F6 );
        HANDLE( Key_F7, KEY_F7 );
        HANDLE( Key_F8, KEY_F8 );
        HANDLE( Key_F9, KEY_F9 );
        HANDLE( Key_F10, KEY_F10 );
        HANDLE( Key_F11, KEY_F11 );
        HANDLE( Key_F12, KEY_F12 );
        HANDLE( Key_PageUp, KEY_PAGEUP );
        HANDLE( Key_PageDown, KEY_PAGEDOWN );
        HANDLE( Key_Home, KEY_HOME );
        HANDLE( Key_End, KEY_END );
        HANDLE( Key_Insert, KEY_INSERT );
        HANDLE( Key_Delete, KEY_DELETE );
        HANDLE( Key_VolumeDown, KEY_VOLUME_DOWN);
        HANDLE( Key_VolumeUp, KEY_VOLUME_UP );
        HANDLE( Key_VolumeMute, KEY_VOLUME_MUTE );
        HANDLE( Key_MediaPlay, KEY_MEDIA_PLAY_PAUSE );
        HANDLE( Key_MediaStop, KEY_MEDIA_STOP );
        HANDLE( Key_MediaPrevious, KEY_MEDIA_PREV_TRACK );
        HANDLE( Key_MediaNext, KEY_MEDIA_NEXT_TRACK );

    }
    if( !found )
    {
        /* Force lowercase */
        if( e->key() >= Qt::Key_A && e->key() <= Qt::Key_Z )
            i_vlck += e->key() + 32;
        /* Rest of the ascii range */
        else if( e->key() >= Qt::Key_Space && e->key() <= Qt::Key_AsciiTilde )
            i_vlck += e->key();
    }
    return i_vlck;
}

int qtWheelEventToVLCKey( QWheelEvent *e )
{
    int i_vlck = 0;
    /* Handle modifiers */
    i_vlck |= qtKeyModifiersToVLC( e );
    if ( e->delta() > 0 )
        i_vlck |= KEY_MOUSEWHEELUP;
    else
        i_vlck |= KEY_MOUSEWHEELDOWN;
    return i_vlck;
}

QString VLCKeyToString( int val )
{
    const char *base = KeyToString (val & ~KEY_MODIFIER);

    QString r = "";
    if( val & KEY_MODIFIER_CTRL )
        r+= "Ctrl+";
    if( val & KEY_MODIFIER_ALT )
        r+= "Alt+";
    if( val & KEY_MODIFIER_SHIFT )
        r+= "Shift+";

    return r + (base ? base : "Unset");
}

void KeyShortcutEdit::mousePressEvent( QMouseEvent *)
{
    emit pressed();
}

QString timeSecondstoString(int time)
{

	int hours, minutes, seconds;
	QString temp;
	hours = time / 3600;
    minutes = (time % 3600) / 60;
    seconds = (time % 3600) % 60;
    QString timeText = "";

    if (hours < 10){
        timeText.append("0"); 
        temp.setNum(hours);
        timeText += temp;
    }
	else{
		temp.setNum(hours);
		timeText += temp;
	}
	if ( minutes < 10 ){
		timeText.append(":0"); 
		temp.setNum(minutes);
		timeText += temp;
	}
	else{
		temp.setNum(minutes);
		timeText += ":"+temp;
	}
	if ( seconds < 10 )
	{
		timeText.append(":0"); 
		temp.setNum(seconds);
		timeText += temp;
	}else
	{
		temp.setNum(seconds);
		timeText += ":" + temp;
	}
	return timeText;
}
