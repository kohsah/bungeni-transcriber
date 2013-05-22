#ifndef AGENDAITEM_HPP
#define AGENDAITEM_HPP
#include <QString>

#include "transcriptionItem.hpp"

class AgendaItem : public TranscriptionItem
{
public:
    explicit AgendaItem(QTime, QTime, int, int id = 0);
    void setId(int id);
    void setDocId(int id);
    int getId();
    int getDocId();
    virtual void setStartTime(QTime);
    virtual void setEndTime(QTime);
    virtual QTime getStartTime();
    virtual QTime getEndTime();
    ItemType getType();
protected:
    int id;
    int docId;
};
#endif // AGENDAITEM_HPP
