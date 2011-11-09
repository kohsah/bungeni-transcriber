#ifndef SITTING_HPP
#define SITTING_HPP
#include <QString>
#include <QDateTime>
#include "playlistItem.hpp"
#include "take.hpp"
#include "../addTakeWidget.hpp"
class Sitting : public PlaylistItem {
    Q_OBJECT
public:
    Sitting(QString, QDateTime, QDateTime);
    void appendChild(Take *child);
    void menu(QWidget *, QModelIndex);
public slots:
    void editSitting();
    void addTakeDialog();
    void removeSitting();
    void addTake();
private:
    AddTakeWidget *addTakeWidget;
    QString name;
    QModelIndex index;
    QWidget *playlist;
};

#endif // SITTING_HPP
