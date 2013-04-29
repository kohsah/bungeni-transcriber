#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <stdio.h>
#include <QDebug>

#include "takesDownloadManager.hpp"

TakesDownloadManager::TakesDownloadManager(QObject *parent)
     : QObject(parent)
 {
 }

 void TakesDownloadManager::append(QModelIndex takeIndex, const QUrl &url)
 {
     if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextDownload()));
     downloadQueue.enqueue(url.toString());
     urlTakeMap[url.toString()] = takeIndex;
}

QString TakesDownloadManager::saveFileName(QString url)
{
     QString basename = QFileInfo(url).fileName();

     if (basename.isEmpty())
         basename = "download";

     if (QFile::exists(basename)) {
         // already exists, don't overwrite
         int i = 0;
         basename += '.';
         while (QFile::exists(basename + QString::number(i)))
             ++i;

         basename += QString::number(i);
     }

     return basename;
}

void TakesDownloadManager::startNextDownload()
{
    if (downloadQueue.isEmpty()) {
         emit finished();
         return;
    }

    QString url = downloadQueue.dequeue();

    QString filename = saveFileName(url);
    output.setFileName(filename);
    if (!output.open(QIODevice::WriteOnly)) {
         startNextDownload();
         return;
     }

     QNetworkRequest request(url);
     currentDownload = manager.get(request);
     connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
             SLOT(downloadProgress(qint64,qint64)));
     connect(currentDownload, SIGNAL(finished()),
             SLOT(downloadFinished()));
     connect(currentDownload, SIGNAL(readyRead()),
             SLOT(downloadReadyRead()));
 }

void TakesDownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
     // calculate the download speed
     double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
     QString unit;
     if (speed < 1024) {
         unit = "bytes/sec";
     } else if (speed < 1024*1024) {
         speed /= 1024;
         unit = "kB/s";
     } else {
         speed /= 1024*1024;
         unit = "MB/s";
     }
}

void TakesDownloadManager::downloadFinished()
 {
     output.close();

     if (currentDownload->error()) {
         qDebug() << "Error";
     } else {
         QModelIndex takeIndex = urlTakeMap[currentDownload->url().toString()];
         Take* take = static_cast<Take*>(takeIndex.internalPointer());
         take->setMediaLocation(QFileInfo(output).absoluteFilePath());
         emit takeFinished(takeIndex);
     }
     currentDownload->deleteLater();
     startNextDownload();
}

void TakesDownloadManager::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}
