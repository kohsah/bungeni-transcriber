/**********************************************************
 * oauth2.hpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - 2013 - UNDESA
 *
 * Author - Miano Njoka <miano@parliaments.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */
#ifndef OAUTH2_HPP
#define OAUTH2_HPP

#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>

#include "webViewWidget.hpp"

struct OAuth2Error {
    QString error_code;
    QString error_description;
};

class OAuth2 : public QObject {
    Q_OBJECT
public:
    OAuth2(QString accessToken = QString(), QString refreshToken = QString());
    void setClientSecret(QString);
    void setClientID(QString);
    void setAuthorizationCodeURL(QUrl);
    void setAccessTokenURL(QUrl);
    void setRedirectURI(QUrl);
    void link();
    void unlink();
    bool getLinkStatus();
    QString getAccessToken();
    QString getRefreshToken();
protected:
    WebViewWidget *webView;
    QUrl createAuthorizationCodeURL();
private:
    QString clientSecret;
    QString clientID;
    QString refreshToken;
    QString accessToken;
    QString authorizationCode;
    int accessTokenExpiryInSeconds;
    QUrl authorizationCodeURL;
    QUrl accessTokenURL;
    QUrl redirectURI;
    bool authorizationStatus;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray networkData;
    void initAccessToken(bool);
    QDateTime lastAccessTokenRefresh;
public slots:
    void onAuthCode(QString);
    void onAccessTokenReadyRead();
    void onAccessTokenReadFinished();
    void networkError(QNetworkReply::NetworkError);
    void networkSslErrors(QList<QSslError>);
signals:
    void linkSucceeded();
    void linkFailed(OAuth2Error&);
};

#endif
