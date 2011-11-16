#ifndef AGENDAITEM_HPP
#define AGENDAITEM_HPP
#include <QString>

#include "transcriptionItem.hpp"

class AgendaItem : public TranscriptionItem
{
public:
    explicit AgendaItem(QTime, QTime, QString, int);
    void setTitle(QString title);
    void setId(int id);
    QString getTitle();
    int getId();
    virtual void setStartTime(QTime);
    virtual void setEndTime(QTime);
    virtual QTime getStartTime();
    virtual QTime getEndTime();
    ItemType getType();
protected:
    QString title;
    int id;
};
#endif // AGENDAITEM_HPP
