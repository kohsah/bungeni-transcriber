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

CustomTextEdit::CustomTextEdit(QWidget *parent) : QTextEdit(parent)
{
}

void CustomTextEdit::keyPressEvent( QKeyEvent *keyEvent )
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


void CustomTextEdit::emitFragment(const QTextFragment &fragment){
    const QTextCharFormat format = fragment.charFormat();
    bool bold = false;
    bool italics = false;
    bool underline = false;
    if (format.hasProperty(QTextFormat::FontWeight)
        && format.fontWeight() != defaultCharFormat.fontWeight()) {
        html += QLatin1String("<b>");
        bold = true;
    }

    if (format.hasProperty(QTextFormat::FontItalic)
        && format.fontItalic() != defaultCharFormat.fontItalic()) {
        html += QLatin1String("<i>");
        italics = true;
    }

    if ((format.hasProperty(QTextFormat::FontUnderline) || format.hasProperty(QTextFormat::TextUnderlineStyle))
        && format.fontUnderline() != defaultCharFormat.fontUnderline()) {
        html += QLatin1String("<u>");
        underline = true;
    }

    QString txt = Qt::escape(fragment.text());

    // split for [\n{LineSeparator}]
    QString forcedLineBreakRegExp = QString::fromLatin1("[\\na]");
    forcedLineBreakRegExp[3] = QChar::LineSeparator;

    const QStringList lines = txt.split(QRegExp(forcedLineBreakRegExp));
    for (int i = 0; i < lines.count(); ++i) {
        if (i > 0)
            html += QLatin1String("<br />");
        html += lines.at(i);
    }
    if (bold)
        html += QLatin1String("</b>");
    if (italics)
        html += QLatin1String("</i>");
    if (underline)
        html += QLatin1String("</u>");
}

void CustomTextEdit::emitBlock(const QTextBlock &block){
    QTextBlock::Iterator it = block.begin();
    for (; !it.atEnd(); ++it)
        emitFragment(it.fragment());
}

void CustomTextEdit::emitFrame(QTextFrame::Iterator frameIt)
{
    if (!frameIt.atEnd()) {
        QTextFrame::Iterator next = frameIt;
        ++next;
        if (next.atEnd()
            && frameIt.currentFrame() == 0
            && frameIt.parentFrame() != doc->rootFrame()
            && frameIt.currentBlock().begin().atEnd())
            return;
    }

    for (QTextFrame::Iterator it = frameIt;
         !it.atEnd(); ++it) {
        if (it.currentBlock().isValid()) {
            emitBlock(it.currentBlock());
        }
    }
}

QString CustomTextEdit::toHtml(){
    doc = this->document();
    html = QLatin1String("");
    const QFont defaultFont = doc->defaultFont();
    defaultCharFormat.setFont(defaultFont);
    defaultCharFormat.clearProperty(QTextFormat::FontUnderline);
    defaultCharFormat.clearProperty(QTextFormat::FontOverline);
    defaultCharFormat.clearProperty(QTextFormat::FontStrikeOut);
    defaultCharFormat.clearProperty(QTextFormat::TextUnderlineStyle);
    emitFrame(doc->rootFrame()->begin());
    qDebug() << "HTML" << html;
    return html;
}
