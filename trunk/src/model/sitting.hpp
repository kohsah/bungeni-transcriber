#ifndef SITTING_HPP
#define SITTING_HPP
#include <QString>
#include <QDateTime>
#include <QList>
#include "playlistItem.hpp"
#include "take.hpp"

class Sitting : public PlaylistItem {
    Q_OBJECT
public:
    Sitting(QString, QDateTime, QDateTime);
    void appendChild(Take *child);
    PlaylistItem::ItemType getType();
    QString getAknFilePath();
    void setAknFilePath(QString);
    const QList<PlaylistItem*>* getTakes() const;
private:
    QString aknFilePath;
};

#endif // SITTING_HPP
