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
