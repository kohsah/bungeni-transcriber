#ifndef SPEECH_HPP
#define SPEECH_HPP

#include <QObject>
#include <QTime>
#include "transcriptionItem.hpp"
#include "../transcriptionItemEditor.hpp"
class Speech : public TranscriptionItem{
public:
    explicit Speech(QTime, QTime, QString, QString, int, bool);
    void setPersonName(QString);
    void setSpeech(QString);
    void setId(int id);
    QString getPersonName();
    QString getSpeech();
    int getId();
    QWidget * getEditor(QWidget*);
    TranscriptionItemEditor *newEditor(QWidget *parent);
    void setStartTime(QTime);
    void setEndTime(QTime);
    QTime getStartTime();
    QTime getEndTime();
    void setComplete(bool);
    bool isComplete();
    void draw(QPainter *&, const QStyleOptionViewItem&);
    QSize sizeHint(const QStyleOptionViewItem&, bool, bool);
    void setEditorData(QWidget*);
    void setModelData(QWidget *editor_);
protected:
    QString personName;
    QString speech;
    int id;
    bool complete;
};

#endif // SPEECH_HPP
