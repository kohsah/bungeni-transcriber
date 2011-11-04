#include "playlistItem.hpp"
PlaylistItem::PlaylistItem(QString name_, QDateTime start_,
                           QDateTime end_){
    name = name_;
    start = start_;
    end = end_;
}

PlaylistItem::~PlaylistItem(){
    qDeleteAll(childItems.begin(), childItems.end());
    childItems.clear();
}

void PlaylistItem::appendChild(PlaylistItem *child){
    childItems.append(child);
}

PlaylistItem * PlaylistItem::child(int row){
    return childItems.at(row);
}

int PlaylistItem::childCount() const{
    return childItems.count();
}
int PlaylistItem::columnCount() const {
    return 3;
}

QVariant PlaylistItem::data(int column) const {
    if (column == 0)
        return QVariant(name);
    else if (column == 1)
        return QVariant(start);
    else if (column == 2)
        return QVariant(end);
    else
        return QVariant();

}

int PlaylistItem::row() const {
    return parentItem->childItems.indexOf(
                const_cast<PlaylistItem*>(this));
}

PlaylistItem *PlaylistItem::parent(){
    return parentItem;
}
