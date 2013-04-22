#ifndef TAKESDOWNLOADMANAGER_HPP
#define TAKESDOWNLOADMANAGER_HPP

#include <QSemaphore>

class takesDownloadManager : public QObject
{
    Q_OBJECT
    public:
        takesDownloadManager(QList<QUrl *>);
    private:
        QSemaphore *lock;
};

#endif // TAKESDOWNLOADMANAGER_HPP
