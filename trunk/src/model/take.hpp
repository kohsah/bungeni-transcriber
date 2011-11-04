#ifndef TAKE_HPP
#define TAKE_HPP
#include <QString>
#include <QDateTime>
#include "playlistItem.hpp"

class Take : public PlaylistItem
{
public:
    Take(QString, QDateTime, QDateTime);
    void appendChild(Take *child);
};
#endif // TAKE_HPP
