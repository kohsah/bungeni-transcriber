#ifndef SPEECH_HPP
#define SPEECH_HPP

#include <QObject>
#include <QTime>
#include "transcriptionItem.hpp"
class Speech : public TranscriptionItem{
public:
    explicit Speech(QTime, QTime, QString, QString, int, bool);
    void setPersonName(QString);
    void setSpeech(QString title);
    void setId(int id);
    QString getPersonName();
    QString getSpeech();
    int getId();
    virtual QWidget * getEditor(QWidget*) = 0;
    virtual void setStartTime(QTime) = 0;
    virtual void setEndTime(QTime) = 0;
    virtual QTime getStartTime() = 0;
    virtual QTime getEndTime() = 0;
    void setComplete(bool);
    bool isComplete();
protected:
    QString personName;
    QString speech;
    int id;
    bool complete;
};

#endif // SPEECH_HPP
