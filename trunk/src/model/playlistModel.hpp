#ifndef PLAYLISTMODEL_HPP
#define PLAYLISTMODEL_HPP

#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>
#include <QModelIndex>

#include "playlistItem.hpp"
#include "transcriptionItem.hpp"
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
    void insertItem(QModelIndex &parent, PlaylistItem *item);
    void customDataChanged(QModelIndex&, QModelIndex&);
public slots:
    void setCurrentTakeIndex(const QModelIndex&);
private:
    PlaylistItem *rootItem;
    QModelIndex currentTakeIndex;
};

#endif // PLAYLISTMODEL_HPP
