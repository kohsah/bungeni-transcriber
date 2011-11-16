#ifndef PLAYLISTITEM_HPP
#define PLAYLISTITEM_HPP
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QVariant>
#include <QModelIndex>
class PlaylistItem : public QObject
{
    Q_OBJECT
public:
    PlaylistItem(QString, QDateTime, QDateTime);
    ~PlaylistItem();

    virtual void appendChild(PlaylistItem *child);

    PlaylistItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PlaylistItem *parent();
    enum ItemType {
        RootItem, TypeSitting, TypeTake
    };
    virtual PlaylistItem::ItemType getType();
    QDateTime getStartDateTime();
    QDateTime getEndDateTime();
    QString getName();
    void setStartDateTime(QDateTime);
    void setEndDateTime(QDateTime);
    void setName(QString);
protected:
    QList<PlaylistItem*> childItems;
    QString name;
    QDateTime start;
    QDateTime end;
    PlaylistItem *parentItem;
};

#endif // PLAYLISTITEM_HPP
