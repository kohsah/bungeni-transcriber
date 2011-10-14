#ifndef AGENDAITEM_HPP
#define AGENDAITEM_HPP
#include <QString>

#include "transcriptionItem.hpp"
#include "../transcriptionItemEditor.hpp"

class AgendaItem : public TranscriptionItem
{
public:
    explicit AgendaItem(QTime, QTime, QString, int);
    void setTitle(QString title);
    void setId(int id);
    QString getTitle();
    int getId();
    virtual QWidget * getEditor(QWidget*);
    TranscriptionItemEditor *newEditor(QWidget *parent);
    virtual void setStartTime(QTime);
    virtual void setEndTime(QTime);
    virtual QTime getStartTime();
    virtual QTime getEndTime();
    void draw(QPainter *&, const QStyleOptionViewItem &);
    QSize sizeHint(const QStyleOptionViewItem &, bool, bool);
    void setEditorData(QWidget *);
    void setModelData(QWidget *);
protected:
    QString title;
    int id;
};
#endif // AGENDAITEM_HPP
