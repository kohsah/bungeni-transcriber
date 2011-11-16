#ifndef TAKE_HPP
#define TAKE_HPP
#include <QModelIndex>
#include <QString>
#include <QDateTime>
#include "playlistItem.hpp"

class Take : public PlaylistItem {
    Q_OBJECT
public:
    Take(QString, QDateTime, QDateTime, QString);
    void appendChild();
    PlaylistItem::ItemType getType();
    QString getMediaLocation();
    void setMediaLocation(QString);
private:
    QString mediaLocation;
};
#endif // TAKE_HPP
