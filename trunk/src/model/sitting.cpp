#include "sitting.hpp"

Sitting::Sitting(QString name_, QDateTime start_, QDateTime end_) :
    PlaylistItem(name_, start_, end_){
}

void Sitting::appendChild(Take *child){
    childItems.append(child);
}
