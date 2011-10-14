#ifndef TRANSCRIPTIONITEM_HPP
#define TRANSCRIPTIONITEM_HPP

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QMetaType>
#include <QPainter>
#include <QSize>
#include <QStyleOptionViewItem>
#include "../transcriptionItemEditor.hpp"
class TranscriptionItem
{
public:
    virtual QWidget * getEditor(QWidget*) = 0;
    virtual void setStartTime(QTime) = 0;
    virtual void setEndTime(QTime) = 0;
    virtual QTime getStartTime() = 0;
    virtual QTime getEndTime() = 0;
    virtual void draw(QPainter *&, const QStyleOptionViewItem&) = 0;
    virtual QSize sizeHint(const QStyleOptionViewItem&, bool, bool) = 0;
    virtual void setEditorData(QWidget *) = 0;
    virtual void setModelData(QWidget *) = 0;
    virtual TranscriptionItemEditor *newEditor(QWidget *) = 0;
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
