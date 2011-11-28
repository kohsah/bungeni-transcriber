/********************************************************************
 * transcriptionItem.hpp
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
#ifndef TRANSCRIPTIONITEM_HPP
#define TRANSCRIPTIONITEM_HPP

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QMetaType>
#include <QPainter>
#include <QSize>
#include <QList>
#include <QStyleOptionViewItem>
#include "../transcriptionItemEditor.hpp"
class TranscriptionItem
{
public:
    virtual void setStartTime(QTime) = 0;
    virtual void setEndTime(QTime) = 0;
    virtual QTime getStartTime() = 0;
    virtual QTime getEndTime() = 0;
    enum ItemType {
        TypeAgendaItem, TypeSpeech
    };
    virtual ItemType getType() = 0;
protected:
    QTime startTime;
    QTime endTime;
};

struct TranscriptionItemWrapper
{
    TranscriptionItem * ptr;
    TranscriptionItemWrapper()
    {
        ptr = NULL;
    }

    TranscriptionItemWrapper(TranscriptionItem* p)
    {
        ptr = p;
    }
};

Q_DECLARE_METATYPE(TranscriptionItemWrapper)
Q_DECLARE_METATYPE(TranscriptionItemWrapper*)
#endif // TRANSCRIPTIONITEM_HPP
