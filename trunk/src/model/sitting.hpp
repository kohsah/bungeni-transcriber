#ifndef SITTING_HPP
#define SITTING_HPP
#include <QString>
#include <QDateTime>
#include "playlistItem.hpp"
#include "take.hpp"
class Sitting : public PlaylistItem {
public:
    Sitting(QString, QDateTime, QDateTime);
    void appendChild(Take *child);
};

#endif // SITTING_HPP
