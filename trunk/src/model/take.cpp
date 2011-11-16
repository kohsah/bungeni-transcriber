#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include "take.hpp"
#include "../takeEditorWidget.hpp"
#include "playlistModel.hpp"
#include "../playlistWidget.hpp"
Take::Take(QString name_, QDateTime start_,
           QDateTime end_, QString mediaLocation_) :
    PlaylistItem(name_, start_, end_){
    mediaLocation = mediaLocation_;
}

void Take::appendChild(){
    qDebug() << "BUNGENI TRANSCRIBE ERROR : attempt to add a child to a take";
}

PlaylistItem::ItemType Take::getType(){
    return PlaylistItem::TypeTake;
}

QString Take::getMediaLocation(){
    return mediaLocation;
}
void Take::setMediaLocation(QString mediaLocation_){
    mediaLocation = mediaLocation_;
}
