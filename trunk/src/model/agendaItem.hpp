#ifndef AGENDAITEM_HPP
#define AGENDAITEM_HPP
#include <QString>

#include "transcriptionItem.hpp"

class AgendaItem : public TranscriptionItem
{
public:
    explicit AgendaItem(QTime, QTime, QString, int, int id = 0);
    void setTitle(QString title);
    void setId(int id);
    void setDocId(int id);
    QString getTitle();
    int getId();
    int getDocId();
    virtual void setStartTime(QTime);
    virtual void setEndTime(QTime);
    virtual QTime getStartTime();
    virtual QTime getEndTime();
    ItemType getType();
protected:
    QString title;
    int id;
    int docId;
};
#endif // AGENDAITEM_HPP
