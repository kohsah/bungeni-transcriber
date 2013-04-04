#include <QDebug>
#include "playlistModel.hpp"

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractItemModel(parent){
    rootItem = new PlaylistItem("Root Item", QDateTime(), QDateTime());
    currentItemIndex = QPersistentModelIndex();
}


QVariant PlaylistModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid()) {
        return QVariant();
    }
    PlaylistItem *item = static_cast<PlaylistItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    else if ((role == Qt::BackgroundRole) && (currentItemIndex.row() == index.row())
             && (currentItemIndex.parent() == index.parent())) {
            return QVariant( QBrush( Qt::darkGray ) );
    }
    else {
        return QVariant();
    }
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation,
                     int role) const{
    QVariant headers[3] = {"Name", "Start Date/Time", "End Date/Time"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headers[section];
    return QVariant();
}

QModelIndex PlaylistModel::index(int row, int column,
                   const QModelIndex &parent) const{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    PlaylistItem * parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<PlaylistItem*>(parent.internalPointer());
    PlaylistItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
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

void PlaylistModel::insertItem(QModelIndex &parent, PlaylistItem *item){
    PlaylistItem * parentItem;
    if (parent.isValid())
        parentItem = static_cast<PlaylistItem*>(parent.internalPointer());
    else
        parentItem = rootItem;
    this->beginInsertRows(parent, parentItem->childCount(), parentItem->childCount());
    parentItem->appendChild(item);
    this->endInsertRows();
}

void PlaylistModel::customDataChanged(QModelIndex& top, QModelIndex& bottom){
    emit dataChanged(top, bottom);
}

void PlaylistModel::setCurrentTakeIndex(const QModelIndex& index){
    emit dataChanged(this->index(currentItemIndex.row(), 0, currentItemIndex.parent()),
                     this->index(currentItemIndex.row(), 2, currentItemIndex.parent()));
    currentItemIndex = QPersistentModelIndex(index);
    emit dataChanged(index, index);
}
