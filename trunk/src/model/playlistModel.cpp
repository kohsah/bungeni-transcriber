#include "playlistModel.hpp"

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractItemModel(parent){
}


QVariant PlaylistModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    PlaylistItem *item = static_cast<PlaylistItem*>(index.internalPointer());

    return item->data(index.column());
}
Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation,
                     int role) const{
    QVariant headers[3] = {"Group Name", "Start Date/Time", "End Date/Time"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headers[section];
    return QVariant();
}

QModelIndex PlaylistModel::index(int row, int column,
                   const QModelIndex &parent) const{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    PlaylistItem * parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<PlaylistItem*>(parent.internalPointer());
    PlaylistItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
QModelIndex PlaylistModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    PlaylistItem *childItem = static_cast<PlaylistItem*>(index.internalPointer());
    PlaylistItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
int PlaylistModel::rowCount(const QModelIndex &parent) const{
    PlaylistItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<PlaylistItem*>(parent.internalPointer());

    return parentItem->childCount();
}
int PlaylistModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return static_cast<PlaylistItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

void PlaylistModel::insertItem (Sitting *item){
    rootItem->appendChild(static_cast<PlaylistItem*>(item));
}
