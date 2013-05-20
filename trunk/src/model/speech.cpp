#include <QDebug>
#include "speech.hpp"
Speech::Speech(QTime startTime_, QTime endTime_, QString speech_,
               Person *person_, bool complete_, int id_) : TranscriptionItem(){
    startTime = startTime_;
    endTime = endTime_;
    speech = speech_;
    person = person_;
    complete = complete_;
    id = id_;
}

void Speech::setStartTime(QTime startTime_){
    startTime = startTime_;
}

void Speech::setEndTime(QTime endTime_){
    endTime = endTime_;
}

void Speech::setPerson(Person* person_){
    person = person_;
}

void Speech::setComplete(bool complete_){
    complete = complete_;
}
void Speech::setSpeech(QString speech_){
    speech = speech_;
}

void Speech::setId(int id_){
    id = id_;
}

int Speech::getId(){
    return id;
}

Person* Speech::getPerson(){
    return person;
}

QString Speech::getSpeech(){
    return speech;
}

QTime Speech::getStartTime(){
    return startTime;
}

QTime Speech::getEndTime(){
    return endTime;
}

bool Speech::isComplete(){
    return complete;
}

TranscriptionItem::ItemType Speech::getType(){
    return TranscriptionItem::TypeSpeech;
}
