#ifndef SITTING_HPP
#define SITTING_HPP
#include <QString>
#include <QDateTime>
#include "playlistItem.hpp"
#include "take.hpp"

class Sitting : public PlaylistItem {
    Q_OBJECT
public:
    Sitting(QString, QDateTime, QDateTime);
    void appendChild(Take *child);
    void menu(QWidget *, QModelIndex);
public slots:
    void editSitting();
    void addTake();
    void removeSitting();
private:
    QModelIndex index;
    QWidget *playlist;
};

#endif // SITTING_HPP
