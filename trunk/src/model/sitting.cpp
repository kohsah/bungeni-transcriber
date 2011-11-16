#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include "sitting.hpp"
#include "playlistModel.hpp"
#include "../playlistWidget.hpp"
#include "../takeEditorWidget.hpp"
#include "../sittingEditorWidget.hpp"

Sitting::Sitting(QString name_, QDateTime start_, QDateTime end_) :
    PlaylistItem(name_, start_, end_){
}

void Sitting::appendChild(Take *child){
    childItems.append(child);
}

PlaylistItem::ItemType Sitting::getType(){
    return PlaylistItem::TypeSitting;
}
