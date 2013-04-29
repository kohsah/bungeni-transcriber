#ifndef TAKESDOWNLOADMANAGER_HPP
#define TAKESDOWNLOADMANAGER_HPP

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>

#include "model/take.hpp"

class TakesDownloadManager: public QObject
{
    Q_OBJECT
public:
    TakesDownloadManager(QObject *parent = 0);
    void append(QModelIndex, const QUrl &url);
signals:
    void takeFinished(QModelIndex);
    void finished();
private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager manager;
    QMap<QString, QModelIndex> urlTakeMap;
    QQueue<QString> downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
    QTime downloadTime;
    QString saveFileName(QString);
};
#endif // TAKESDOWNLOADMANAGER_HPP
