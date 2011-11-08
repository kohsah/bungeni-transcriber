#ifndef PLAYLISTITEM_HPP
#define PLAYLISTITEM_HPP
#include <QString>
#include <QDateTime>
#include <QList>
#include <QVariant>
#include <QModelIndex>
class PlaylistItem
{
public:
    PlaylistItem(QString, QDateTime, QDateTime);
    ~PlaylistItem();

    void appendChild(PlaylistItem *child);

    PlaylistItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PlaylistItem *parent();
    void menu();
protected:
    QList<PlaylistItem*> childItems;

private:
     QString name;
     QDateTime start;
     QDateTime end;
     PlaylistItem *parentItem;
};

#endif // PLAYLISTITEM_HPP
