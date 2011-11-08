#ifndef PLAYLISTMODEL_HPP
#define PLAYLISTMODEL_HPP

#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>

#include "playlistItem.hpp"
#include "sitting.hpp"

class PlaylistModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    PlaylistModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void insertItem (Sitting *item);
private:
    PlaylistItem *rootItem;
};

#endif // PLAYLISTMODEL_HPP