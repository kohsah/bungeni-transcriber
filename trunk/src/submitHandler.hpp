#ifndef SUBMITHANDLER_HPP
#define SUBMITHANDLER_HPP
#include <QObject>
#include <QNetworkReply>
#include <QSslError>
#include "model/take.hpp"

class SubmitHandler : QObject {
    Q_OBJECT
public:
    SubmitHandler();
    void submit(Take*);
public slots:
    void networkError(QNetworkReply::NetworkError);
    void networkSslErrors(QList<QSslError>);
    void submitFinished(QNetworkReply *);
};

#endif // SUBMITHANDLER_HPP
