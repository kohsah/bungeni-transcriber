#include <QMenu>
#include <QCursor>
#include "playlistItem.hpp"

PlaylistItem::PlaylistItem(QString name_, QDateTime start_,
                           QDateTime end_) : QObject(){
    name = name_;
    start = start_;
    end = end_;
}

PlaylistItem::~PlaylistItem(){
    qDeleteAll(childItems.begin(), childItems.end());
    childItems.clear();
}

void PlaylistItem::appendChild(PlaylistItem *child){
    child->parentItem = this;
    childItems.append(child);
}

PlaylistItem * PlaylistItem::child(int row){
    if(row < childItems.count())
        return childItems.at(row);
    return NULL;
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

PlaylistItem::ItemType PlaylistItem::getType(){
    return PlaylistItem::RootItem;
}

QDateTime PlaylistItem::getStartDateTime(){
    return start;
}

QDateTime PlaylistItem::getEndDateTime(){
    return end;
}

QString PlaylistItem::getName(){
    return name;
}


void PlaylistItem::setStartDateTime(QDateTime start_){
    start = start_;
}

void PlaylistItem::setEndDateTime(QDateTime end_){
    end = end_;
}

void PlaylistItem::setName(QString name_){
    name = name_;
}
