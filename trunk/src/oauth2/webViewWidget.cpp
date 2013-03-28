/********************************************************************
 * webView.cpp
 ********************************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - 2013 - UNDESA <www.parliaments.info>
 *
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
 ***********************************************************************/
#include <QGridLayout>
#include <QDebug>
#include "webViewWidget.hpp"

WebViewWidget :: WebViewWidget(QUrl authorizationURL, QUrl redirectURI_)
{
    ui.setupUi(this);
    redirectURI = redirectURI_;
    connect(ui.view, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onLinkChanged(const QUrl&)));
    ui.view->load(authorizationURL);
    ui.webViewCurrentURL->setEnabled(false);
}

void WebViewWidget :: onLinkChanged(const QUrl& url)
{
    if (url.toString(QUrl::RemoveQuery) == this->redirectURI.toString()){
        emit authorized(url.queryItemValue(QString("code")));
    }
    ui.webViewCurrentURL->setText(url.toString());
    ui.webViewCurrentURL->setCursorPosition(0);
}
