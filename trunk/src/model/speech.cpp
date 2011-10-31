#include <QDebug>
#include "speech.hpp"
#include "../speechEditor.hpp"
Speech::Speech(QTime startTime_, QTime endTime_, QString speech_,
               QString personName_, int id_, bool complete_) : TranscriptionItem(){
    startTime = startTime_;
    endTime = endTime_;
    speech = speech_;
    personName = personName_;
    id = id_;
    complete = complete_;
}

TranscriptionItemEditor * Speech::newEditor(QWidget *parent){
    SpeechEditor *editor = new SpeechEditor(parent);
    return editor;
}

QWidget * Speech::getEditor(QWidget *editor_){
    SpeechEditor *editor = static_cast<SpeechEditor*>(editor_);
    return editor;
}
void Speech::setStartTime(QTime startTime_){
    startTime = startTime_;
}

void Speech::setEndTime(QTime endTime_){
    endTime = endTime_;
}

void Speech::setPersonName(QString personName_){
    personName = personName_;
}

void Speech::setId(int id_){
    id = id_;
}

void Speech::setComplete(bool complete_){
    complete = complete_;
}
void Speech::setSpeech(QString speech_){
    speech = speech_;
}

QString Speech::getPersonName(){
    return personName;
}

QString Speech::getSpeech(){
    return speech;
}

int Speech::getId(){
    return id;
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

void Speech::draw(QPainter *&painter, const QStyleOptionViewItem &option){
    QTextDocument *qtext = new QTextDocument();
    qtext->setHtml(this->getSpeech());
    QString speech = qtext->toPlainText();
    QString startTimeTxt = "Start Time : "+this->getStartTime().toString(Qt::TextDate);
    QString endTimeTxt = "End Time : "+this->getEndTime().toString(Qt::TextDate);
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
                      this->getPersonName(), &rPersonName);
    QRect rSpeech = option.rect.adjusted(201, 30, -2, -2);
    painter->drawText(rSpeech.left(), rSpeech.top(),
                      rSpeech.width(), rSpeech.height(),
                      Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap, speech, &rSpeech);
}

QSize Speech::sizeHint(const QStyleOptionViewItem &option,
                           bool editing, bool current){
    if ((editing == true) && (current == true)){
        return QSize(option.rect.width(), 350);
    }
    else if ((editing == false) && (current == true)){
        QTextDocument *qtext = new QTextDocument();
        qtext->setHtml(this->getSpeech());
        QString text = qtext->toPlainText();
        int numberoflines = (text.length() / 65 ) ;
        return QSize(option.rect.width(), 50 + (25 * numberoflines));
    }
    else {
        return QSize(option.rect.width(), 50);
    }
}

void Speech::setEditorData(QWidget *editor_){
    SpeechEditor *editor = static_cast<SpeechEditor*>(editor_);
    editor->setName(this->getPersonName());
    editor->setSpeech(this->getSpeech());
    editor->setStartTime(this->getStartTime());
    editor->setEndTime(this->getEndTime());
    editor->setComplete(false);
}

void Speech::setModelData(QWidget *editor_){
    SpeechEditor *editor = static_cast<SpeechEditor*>(editor_);
    //this->setStartTime(editor->getStartTime());
    //this->setEndTime(editor->getEndTime());
    this->setPersonName(editor->getName());
    this->setComplete(editor->getComplete());
    this->setSpeech(editor->getSpeech());
}
