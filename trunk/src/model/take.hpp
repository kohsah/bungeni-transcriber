#ifndef TAKE_HPP
#define TAKE_HPP
#include <QModelIndex>
#include <QString>
#include <QDateTime>
#include <QList>
#include "playlistItem.hpp"
#include "transcriptionItem.hpp"

class Take : public PlaylistItem {
    Q_OBJECT
public:
    Take(QString, QDateTime, QDateTime, QString);
    void appendChild();
    PlaylistItem::ItemType getType();
    QString getMediaLocation();
    void setMediaLocation(QString);
    QList<TranscriptionItem*>* getItems();
private:
    QString mediaLocation;
    QList<TranscriptionItem*>* items;
};
#endif // TAKE_HPP
