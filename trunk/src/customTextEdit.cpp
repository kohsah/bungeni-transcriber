/********************************************************************
 * customTextEdit.cpp
 ********************************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - UNDESA <www.parliaments.info>
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
 
 

#include "customTextEdit.hpp"

#include <QDebug>
#include <QSettings>
#include <QKeyEvent>

customTextEdit::customTextEdit(QWidget *parent) : QTextEdit(parent)
{
}

void customTextEdit::keyPressEvent( QKeyEvent *keyEvent )
{
    if (keyEvent->modifiers() == Qt::NoModifier)
    {
        if( (keyEvent->key() >= Qt::Key_A) && (keyEvent->key() <= Qt::Key_Z) )
        {
            qDebug() << "accept";
    	    QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
        else if( (keyEvent->key() >= Qt::Key_Space) && (keyEvent->key() <= Qt::Key_AsciiTilde) )
        {
            qDebug() << "accept";
        	QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
        else if ( (keyEvent->key() == Qt::Key_Backspace) || (keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return) )
        {
            qDebug() << "accept";
        	QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
        else if ((keyEvent->key() >= Qt::Key_Clear) && (keyEvent->key() <= Qt::Key_Down))
        {
            qDebug() << "accept";
        	QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
        else
        {
            qDebug() << "ignore";
    	    keyEvent->ignore();
        }
    }
    else if (keyEvent->modifiers() == Qt::ControlModifier)
    {
        if( (keyEvent->key() == Qt::Key_C) || (keyEvent->key() <= Qt::Key_V) || (keyEvent->key() <= Qt::Key_Z) || (keyEvent->key() <= Qt::Key_X))
        {
            qDebug() << "accept";
        	QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
        else
        {
            qDebug() << "ignore";
    	    keyEvent->ignore();
        }
    }
    else if (keyEvent->modifiers() == Qt::ShiftModifier)
    {
        if( (keyEvent->key() >= Qt::Key_A) && (keyEvent->key() <= Qt::Key_Z) )
        {
            qDebug() << "accept";
    	    QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
        else if( (keyEvent->key() >= Qt::Key_Space) && (keyEvent->key() <= Qt::Key_AsciiTilde) )
        {
            qDebug() << "accept";
        	QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
        else if ( (keyEvent->key() >=  Qt::Key_Left ) && (keyEvent->key() <=  Qt::Key_Down ) )
        {
            qDebug() << "accept";
        	QTextEdit::keyPressEvent(keyEvent);
    	    keyEvent->accept();
        }
    }
    else
    {
        qDebug() << "ignore";
    	keyEvent->ignore();
    }
    
}
