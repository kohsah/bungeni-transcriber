#include <QDebug>
#include "take.hpp"

Take::Take(QString name_, QDateTime start_, QDateTime end_) :
    PlaylistItem(name_, start_, end_){
}

void Take::appendChild(Take *child){
    qDebug() << "BUNGENI TRANSCRIBE ERROR : attempt to add a child to a take";
}
