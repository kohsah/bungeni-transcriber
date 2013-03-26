/**********************************************************
 * oauth2.cpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 -2013 - UNDESA
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

#include "oauth2.hpp"
#include "qjson/parser.h"

OAuth2 :: OAuth2(QObject *parent){
    linkStatus = false;
    authorizationCodeURL = QUrl();
    accessTokenURL = QUrl();
    clientSecret = QString();
    clientID = QString();
    manager = new QNetworkAccessManager(this);
}

void OAuth2 :: setClientID(QString clientID){
    this->clientID = clientID;
}

void OAuth2 :: setAuthorizationCodeURL(QUrl authorizationCodeURL_){
    this->authorizationCodeURL = authorizationCodeURL_;
}

void OAuth2 :: setAccessTokenURL(QUrl accessTokenURL_){
    this->accessTokenURL = accessTokenURL_;
}

void OAuth2 :: setClientSecret(QString clientSecret){
    this->clientSecret = clientSecret;
}

void OAuth2 :: setRedirectURI(QUrl uri){
    this->redirectURI = uri;
}

QUrl OAuth2 :: createAuthorizationCodeURL(){
    if (this->authorizationCodeURL.isEmpty()){
        // AuthorizationServerURL has not been set
        qDebug() << "Error: AuthorizationURL has not been set";
        return QUrl();
    }
    else {
        QUrl aURL = QUrl(this->authorizationCodeURL);
        aURL.addQueryItem("client_id", this->clientID);
        aURL.addQueryItem("client_secret", this->clientSecret);
        aURL.addQueryItem("response_type", "code");
        return aURL;
    }
}

void OAuth2 :: link(){
    if (linkStatus == true){
        // Already linked. Do nothing
    }
    else {
        QUrl aURL = this->createAuthorizationCodeURL();
        webView = new WebViewWidget(aURL, this->redirectURI);
        connect(webView, SIGNAL(authorized(QString)), this, SLOT(onAuthCode(QString)));
        webView->show();
        linkStatus = true;
    }
}

void OAuth2 :: onAuthCode(QString authCode) {
    authorizationCode = authCode;
    webView->close();
    initAccessToken();
}

void OAuth2 :: initAccessToken(){
    QNetworkRequest request;
    QUrl accessUrl = QUrl(this->accessTokenURL);
    accessUrl.addQueryItem("grant_type", "authorization_code");
    accessUrl.addQueryItem("client_id", this->clientID);
    accessUrl.addQueryItem("code", this->authorizationCode);
    request.setUrl(accessUrl);

    reply = manager->get(request);
    qDebug() << "INITIALISING ACCESS TOKEN" << accessUrl;
    networkData.clear();
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(networkError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(networkSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(onAccessTokenReadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(onAccessTokenReadyRead()));
}

void OAuth2 :: onAccessTokenReadyRead(){
    networkData.append(reply->readAll());
}

void OAuth2 :: onAccessTokenReadFinished(){
    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(this->networkData, &ok).toMap();
    if (!ok) {
        return;
    }
    this->accessToken = result["access_token"].toString();
    this->accessTokenExpiryInSeconds = result["expiry"].toInt();
    this->refreshToken = result["refresh_token"].toString();
}

void OAuth2 :: networkError(QNetworkReply::NetworkError){

}

void OAuth2 :: networkSslErrors(QList<QSslError>){

}
