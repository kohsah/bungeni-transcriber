#ifndef TAKE_HPP
#define TAKE_HPP
#include <QModelIndex>
#include <QString>
#include <QDateTime>
#include "playlistItem.hpp"

class Take : public PlaylistItem {
    Q_OBJECT
public:
    Take(QString, QDateTime, QDateTime, QString);
    void appendChild();
    void menu(QWidget*, QModelIndex);
public slots:
    void editTake();
    void removeTake();
private:
    QString mediaLocation;
    QModelIndex index;
    QWidget *playlist;
};
#endif // TAKE_HPP
