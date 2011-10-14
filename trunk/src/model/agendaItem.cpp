#include "agendaItem.hpp"
#include "../agendaItemEditor.hpp"
AgendaItem::AgendaItem(QTime startTime_, QTime endTime_,
                       QString title_, int id_) : TranscriptionItem(){
    startTime = startTime_;
    endTime = endTime_;
    title = title_;
    id = id_;
}

QWidget * AgendaItem::getEditor(QWidget *editor_){
    AgendaItemEditor *editor = static_cast<AgendaItemEditor*>(editor_);
    return editor;
}

TranscriptionItemEditor * AgendaItem::newEditor(QWidget *parent){
    AgendaItemEditor *editor = new AgendaItemEditor(parent);
    return editor;
}

void AgendaItem::setStartTime(QTime startTime_){
    startTime = startTime_;
}

void AgendaItem::setEndTime(QTime endTime_){
    endTime = endTime_;
}

void AgendaItem::setTitle(QString title_){
    title = title_;
}

void AgendaItem::setId(int id_){
    id = id_;
}

QString AgendaItem::getTitle(){
    return title;
}

int AgendaItem::getId(){
    return id;
}


QTime AgendaItem::getStartTime(){
    return startTime;
}

QTime AgendaItem::getEndTime(){
    return endTime;
}

void AgendaItem::draw(QPainter *& painter, const QStyleOptionViewItem &option){
    QString agendaItemTxt = this->getTitle();
    QString startTimeTxt = "Start Time : "+this->getStartTime().toString(Qt::TextDate);
    QString endTimeTxt = "End Time : "+this->getEndTime().toString(Qt::TextDate);
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

QSize AgendaItem::sizeHint(const QStyleOptionViewItem &option,
                           bool editing, bool current){
    if ((editing == true) && (current == true)){
        return QSize(option.rect.width(), 100);
    }
    else {
        return QSize(option.rect.width(), 50);
    }
}

void AgendaItem::setEditorData(QWidget *editor_){
    AgendaItemEditor *editor = static_cast<AgendaItemEditor*>(editor_);
    editor->setStartTime(this->getStartTime());
    editor->setEndTime(this->getEndTime());
}

void AgendaItem::setModelData(QWidget *editor_){
    AgendaItemEditor *editor = static_cast<AgendaItemEditor*>(editor_);
    //this->setStartTime(editor->getStartTime());
    //this->setEndTime(editor->getEndTime());
    this->setTitle(editor->getAgendaItem());
}
