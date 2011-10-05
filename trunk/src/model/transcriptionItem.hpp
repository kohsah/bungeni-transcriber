#ifndef TRANSCRIPTIONITEM_HPP
#define TRANSCRIPTIONITEM_HPP

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QMetaType>
class TranscriptionItem
{
public:
    virtual QWidget * getEditor(QWidget*) = 0;
    virtual void setStartTime(QTime) = 0;
    virtual void setEndTime(QTime) = 0;
    virtual QTime getStartTime() = 0;
    virtual QTime getEndTime() = 0;
protected:
    QTime startTime;
    QTime endTime;
};

typedef struct TranscriptionItemWrapper
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
