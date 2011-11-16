#ifndef SPEECH_HPP
#define SPEECH_HPP

#include <QObject>
#include <QTime>
#include "transcriptionItem.hpp"

class Speech : public TranscriptionItem{
public:
    explicit Speech(QTime, QTime, QString, QString, int, bool);
    void setPersonName(QString);
    void setSpeech(QString);
    void setId(int id);
    QString getPersonName();
    QString getSpeech();
    int getId();
    void setStartTime(QTime);
    void setEndTime(QTime);
    QTime getStartTime();
    QTime getEndTime();
    void setComplete(bool);
    bool isComplete();
    ItemType getType();
protected:
    QString personName;
    QString speech;
    int id;
    bool complete;
};

#endif // SPEECH_HPP
