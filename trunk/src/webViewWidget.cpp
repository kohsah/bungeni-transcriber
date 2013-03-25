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

WebViewWidget :: WebViewWidget()
{
    QGridLayout *gLayout = new QGridLayout;
    resize(400, 300);
    view = new QWebView(this);
    connect(view, SIGNAL(urlChanged(QUrl&)), this, SLOT(onLinkChanged(QUrl&)));
    gLayout->addWidget(view, 0, 0, 1, 1);
    setLayout(gLayout);
    view->load(QUrl("http://localhost:8081/oauth/authorise?client_secret=b055cafa5b528ed3ada2b9e91c9015344d841bc2&client_id=test&response_type=authorization_code"));
}

void WebViewWidget :: onLinkChanged(const QUrl & url)
{
    qDebug() << "LINK CHANGED";
}
