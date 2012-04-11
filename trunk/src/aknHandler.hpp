/********************************************************************
 * aknHandler.hpp
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
#ifndef AKNPARSER_HPP
#define AKNPARSER_HPP
#include <QObject>
#include <QString>
#include <QXmlStreamWriter>
#include <QSet>
#include <QList>
#include "model/sitting.hpp"
#include "model/person.hpp"
#include "model/speech.hpp"

struct SpeechID {
    Speech *speech;
    QString id;
    Take *take;
};

class AknHandler : QObject{
    Q_OBJECT
public:
    AknHandler();
    bool saveSittingToFile(Sitting*, QString);
    Sitting* loadSittingFromFile(QString);
private:
    void writeSpeeches(QXmlStreamWriter*);
    void writeFRBRWork(QXmlStreamWriter*, Sitting*);
    void writeFRBRExpression(QXmlStreamWriter*, Sitting*);
    void writeFRBRManifestation(QXmlStreamWriter*, Sitting*);
    void writeReferences(QXmlStreamWriter*, Sitting*);
    QList<SpeechID*> speechIdList;
};

#endif // AKNPARSER_HPP
