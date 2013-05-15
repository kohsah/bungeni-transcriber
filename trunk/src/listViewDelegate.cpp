/********************************************************************
 * ListViewDelegate.cpp
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

#include "listViewDelegate.hpp"
#include "speechEditor.hpp"
#include "agendaItemEditor.hpp"
#include <QString>
#include <QTextDocument>
#include <QModelIndex>
#include <QPainter>
#include <QDebug>
#include <QCompleter>

#include "util/util.hpp"
#include "model/transcriptionItem.hpp"
#include "transcriptionItemEditor.hpp"
#include "model/transcriptionModel.hpp"

ListViewDelegate::ListViewDelegate(QObject *parent)
    		: QAbstractItemDelegate(parent)
	{
	   // current = NULL;
	}

void ListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(
                index.data());
    TranscriptionItem *item = wrapper->ptr;
    if (item->getType() == TranscriptionItem::TypeSpeech){
        Speech *speech = static_cast<Speech*>(item);
        this->speechDraw(painter, option, speech);
    }
    else if (item->getType() == TranscriptionItem::TypeAgendaItem){
        AgendaItem *agendaItem = static_cast<AgendaItem*>(item);
        this->agendaItemDraw(painter, option, agendaItem);
    }
}


QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(index.data());
    TranscriptionItem *item = wrapper->ptr;
    bool isCurrent;
    if (current == index) {
        isCurrent = true;
    }
    else{
        isCurrent = false;
    }
    if (item->getType() == TranscriptionItem::TypeSpeech){
        Speech *speech = static_cast<Speech*>(item);
        return speechSizeHint(option, editing, isCurrent, speech);
    }
    else {
        qDebug() << "Agenda Item Size Hint";
        AgendaItem *agendaItem = static_cast<AgendaItem*>(item);
        return agendaItemSizeHint(option, editing, isCurrent, agendaItem);
    }
}
	
QWidget *ListViewDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem & option,
     const QModelIndex & index ) const {
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(index.data());
    TranscriptionItem *item = wrapper->ptr;
    TranscriptionItemEditor *editor;
    if (item->getType() == TranscriptionItem::TypeSpeech){
        editor = this->newSpeechEditor(parent);
    }
    else if (item->getType() == TranscriptionItem::TypeAgendaItem){
        editor = this->newAgendaItemEditor(parent);
    }
    editor->setIndex(index);
    QObject::connect( editor, SIGNAL(close(QWidget *)), this, SLOT( editorClose(QWidget *) ) );
    QObject::connect( this, SIGNAL(commitData(QWidget *)), this, SIGNAL( closeEditor(QWidget *) ) );
    QObject::connect( editor, SIGNAL(closeIndex(QModelIndex)), this, SLOT( display(QModelIndex) ) );
    QObject::connect( editor, SIGNAL(cancelled(QWidget *)), this, SIGNAL( closeEditor(QWidget *) ) );
    editor->setGeometry(option.rect);
    return editor;
}

void ListViewDelegate::editorClose(QWidget *editor) {
    emit commitData(editor);
}

void ListViewDelegate::setEditorData(QWidget *editor_,
                                     const QModelIndex &index) const {
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(index.data());
    TranscriptionItem *item = wrapper->ptr;
    if (item->getType() == TranscriptionItem::TypeSpeech){
        Speech *speech = static_cast<Speech*>(item);
        this->setSpeechEditorData(editor_, speech);
    }
    else if (item->getType() == TranscriptionItem::TypeAgendaItem){
        AgendaItem *agendaItem = static_cast<AgendaItem*>(item);
        this->setAgendaItemEditorData(editor_, agendaItem);
    }
}

void ListViewDelegate::setModelData(QWidget *editor_, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(index.data());
    TranscriptionItem *item = wrapper->ptr;
    TranscriptionModel* newModel = static_cast<TranscriptionModel*>(model);
    if (item->getType() == TranscriptionItem::TypeSpeech){
        Speech *speech = static_cast<Speech*>(item);
        this->setSpeechModelData(editor_, speech);
    }
    else if (item->getType() == TranscriptionItem::TypeAgendaItem){
        AgendaItem *agendaItem = static_cast<AgendaItem*>(item);
        this->setAgendaItemModelData(editor_, agendaItem);
    }
    newModel->transcriptionItemDataChanged(index);
 }
 
void ListViewDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }

void ListViewDelegate::currentEditing(QModelIndex index)
{
    current = index;
    editing = true;
    emit sizeHintChanged(index);
}

void ListViewDelegate::display(QModelIndex index)
{
    editing = false;
    current = index;
    emit sizeHintChanged(index);
}


void ListViewDelegate::speechDraw(QPainter *&painter, const QStyleOptionViewItem &option, Speech* speech)const{
    if(option.state & QStyle::State_Selected){
        painter->fillRect(option.rect, option.palette.highlight());
    }
    QTextDocument *qtext = new QTextDocument();
    qtext->setHtml(speech->getSpeech());
    QString speechText = qtext->toPlainText();
    QString startTimeTxt = "Start Time : "+speech->getStartTime().toString(Qt::TextDate);
    QString endTimeTxt = "End Time : "+speech->getEndTime().toString(Qt::TextDate);
    QRect rStartTime = option.rect.adjusted(2, 2, 200, 29);
    painter->drawText(rStartTime.left(), rStartTime.top(),
                      rStartTime.width(), rStartTime.height(),
                      Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap,
                      startTimeTxt, &rStartTime);
    QRect rEndTime = option.rect.adjusted(201, 2, 200, 29);
    painter->drawText(rEndTime.left(), rEndTime.top(), rEndTime.width(),
                      rEndTime.height(), Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap,
                      endTimeTxt, &rEndTime);
    QRect rPersonName = option.rect.adjusted(2, 30, 200, -2);
    painter->drawText(rPersonName.left(), rPersonName.top(),
                      rPersonName.width(), rPersonName.height(),
                      Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap,
                      speech->getPerson()->getName(), &rPersonName);
    QRect rSpeech = option.rect.adjusted(201, 30, -2, -2);
    painter->drawText(rSpeech.left(), rSpeech.top(),
                      rSpeech.width(), rSpeech.height(),
                      Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap, speechText, &rSpeech);

}

QSize ListViewDelegate::speechSizeHint(const QStyleOptionViewItem &option,
                           bool editing, bool current, Speech *speech)const{
    if ((editing == true) && (current == true)){
        return QSize(option.rect.width(), 350);
    }
    else if ((editing == false) && (current == true)){
        QTextDocument *qtext = new QTextDocument();
        qtext->setHtml(speech->getSpeech());
        QString text = qtext->toPlainText();
        int numberoflines = (text.length() / 65 ) ;
        return QSize(option.rect.width(), 50 + (25 * numberoflines));
    }
    else {
        return QSize(option.rect.width(), 50);
    }
}

void ListViewDelegate::setSpeechEditorData(QWidget *editor_, Speech *speech)const{
    SpeechEditor *editor = static_cast<SpeechEditor*>(editor_);
    editor->setPerson(speech->getPerson());
    editor->setSpeech(speech->getSpeech());
    editor->setStartTime(speech->getStartTime());
    editor->setEndTime(speech->getEndTime());
    editor->setComplete(false);
}

void ListViewDelegate::setSpeechModelData(QWidget *editor_, Speech* speech)const{
    SpeechEditor *editor = static_cast<SpeechEditor*>(editor_);
    speech->setStartTime(editor->getStartTime());
    speech->setEndTime(editor->getEndTime());
    speech->setPerson(editor->getPerson());
    speech->setComplete(editor->getComplete());
    speech->setSpeech(editor->getSpeech());
}


TranscriptionItemEditor * ListViewDelegate::newSpeechEditor(QWidget *parent)const{
    SpeechEditor *editor = new SpeechEditor(parent);
    editor->setPersonsModel(personsModel);
    return editor;
}

TranscriptionItemEditor * ListViewDelegate::newAgendaItemEditor(QWidget *parent)const{
    AgendaItemEditor *editor = new AgendaItemEditor(parent);
    editor->setAgendaItemMap(agendaItemMap);
    return editor;
}

void ListViewDelegate::agendaItemDraw(QPainter *& painter, const QStyleOptionViewItem &option, AgendaItem* agendaItem)const{
    QString agendaItemTxt = agendaItem->getTitle();
    QString startTimeTxt = "Start Time : "+agendaItem->getStartTime().toString(Qt::TextDate);
    QString endTimeTxt = "End Time : "+agendaItem->getEndTime().toString(Qt::TextDate);
    QRect rStartTime = option.rect.adjusted(2, 2, 200, 29);
    painter->drawText(rStartTime.left(), rStartTime.top(), rStartTime.width(),
                      rStartTime.height(), Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap,
                      startTimeTxt, &rStartTime);

    QRect rEndTime = option.rect.adjusted(201, 2, 200, 29);
    painter->drawText(rEndTime.left(), rEndTime.top(), rEndTime.width(),
                      rEndTime.height(), Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap,
                      endTimeTxt, &rEndTime);

    QRect rText = option.rect.adjusted(2, 30, 200, -2);
    painter->drawText(rText.left(), rText.top(), rText.width(), rText.height(),
                      Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap,
                      "Agenda Item : "+agendaItemTxt, &rText);
}

QSize ListViewDelegate::agendaItemSizeHint(const QStyleOptionViewItem &option,
                           bool editing, bool current, AgendaItem* /*agendaItem*/)const{
    if ((editing == true) && (current == true)){
        return QSize(option.rect.width(), 100);
    }
    else {
        return QSize(option.rect.width(), 50);
    }
}

void ListViewDelegate::setAgendaItemEditorData(QWidget *editor_, AgendaItem* agendaItem)const{
    AgendaItemEditor *editor = static_cast<AgendaItemEditor*>(editor_);
    editor->setStartTime(agendaItem->getStartTime());
    editor->setEndTime(agendaItem->getEndTime());
}

void ListViewDelegate::setAgendaItemModelData(QWidget *editor_, AgendaItem* agendaItem)const{
    AgendaItemEditor *editor = static_cast<AgendaItemEditor*>(editor_);
    agendaItem->setStartTime(editor->getStartTime());
    agendaItem->setEndTime(editor->getEndTime());
    agendaItem->setTitle(editor->getAgendaItem());
}

void ListViewDelegate::setPersonsModel(PersonsModel* personsModel_){
    personsModel = personsModel_;
}

void ListViewDelegate::setAgendaItemMap(QMap<QString, QString>* agendaItemMap_){
    agendaItemMap = agendaItemMap_;
    qDebug() << "WWWWWWWWWWWWWWWW";
    QMapIterator<QString, QString> i(*agendaItemMap);
    while (i.hasNext()) {
         i.next();
         qDebug() << "ZZZZZZZZZZZZZZZ" << i.value() << i.key();
    }
}
