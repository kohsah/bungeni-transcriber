/**********************************************************
 * transcribeWidget.hpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2011 - UNDESA
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

#ifndef _TRANSCRIBEWIDGET_H_
#define _TRANSCRIBEWIDGET_H_

#include <QWidget>
#include <QTemporaryFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QNetworkReply>
#include <QHash>
#include <QFrame>
#include <QString>
#include <QTimer>
#include <QEvent>
#include <QInputEvent>

#include <vlc/vlc.h>

#include "ui/ui_transcribe.h"
#include "listViewDelegate.hpp"
#include "ui/ui_uploadProgress.h"
#include "controlsWidget.hpp"
#include "playlistWidget.hpp"
#include "model/transcriptionModel.hpp"
#include "model/personsModel.hpp"
#include "model/sitting.hpp"
#include "util/util.hpp"
#include "oauth2/oauth2.hpp"
#include "takesDownloadManager.hpp"

class UserDetails : public QObject
{
    Q_OBJECT
public:
    UserDetails();
    QString getLogin();
    void setLogin(QString);
private:
    QString login;
};


class TranscribeWidget : public QMainWindow
{
    Q_OBJECT
    public:
        static TranscribeWidget* getInstance()
        {
            if (!instance)
                instance = new TranscribeWidget();
            return instance;
        }
        int getFileDuration();
        void endReached();
        QStandardItemModel* getPersonsModel();
        //OAUTH
        OAuth2 *oauth;
    public slots:
        void updateInterface();
        void changeVolume(int newVolume);
        void changePosition(int newPosition);
        void playFile(QString file);
        void setFileDuration();
        void play();
        void stop();
        void about();
        void bungeniSettings();
        void hotkeySettings();
        void skipForward(int sec);
        void skipBackward(int sec);
        void playFaster();
        void playSlower();
        // Add transcription item slots
        void addAgendaItem();
        void addSpeech();
        // Remove transcription item
        void removeTranscriptionItem();
        // Persons widget slot
        void persons();
        void login();
        void logout();
        void setCurrentTakeIndex(const QModelIndex& current);
        void OAuthLinked();
        void playlistRefresh();
        //Bungeni
        void onWorkspaceReadFinished();
        void networkError(QNetworkReply::NetworkError);
        void networkSslErrors(QList<QSslError>);
        void onDebateReadFinished(QNetworkReply *);
        void onUserReadFinished(QNetworkReply *);
        void onAgendaItemsReadFinished(QNetworkReply *);
        void onAllUsersReadFinished(QNetworkReply *);
        void onTakesReadFinished(QNetworkReply *);
        void onTakeItemsReadFinished(QNetworkReply *);
        void takeFinished(QModelIndex);
        void setupOAuth();
    private:
        TranscribeWidget();
        ~TranscribeWidget();

        // Singleton instance
        static TranscribeWidget *instance;

        // LibVLC variables
        libvlc_instance_t *_vlcinstance;
        libvlc_media_player_t *_mp;
        libvlc_time_t _file_duration;
        libvlc_media_t *_m;
        // Models
        TranscriptionModel *model;
        QSortFilterProxyModel *filterModel;
        QItemSelectionModel *selectionModel;
        PersonsModel *personsModel;
        // Delegates
        ListViewDelegate *delegate;

        //UI elements
        Ui::Transcribe ui;
        QFrame * video;
        QTableView *table;
        QVBoxLayout *mainLayout;
        QPushButton *addButton;
        QPushButton *removeButton;
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *helpMenu;
        QAction *addToPlaylistAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *getTakesAct;
        QAction *postTakesAct;
        QAction *bungeniSettingsAct;
        QAction *exitAct;
        QAction *aboutAct;
        QAction *hotkeyAct;
        QAction *personsAct;
        QAction *loginAct;
        QAction *logoutAct;
        ControlsWidget *controls;
        PlaylistWidget *playlist;

        // UI setup routines
        void createActions();
        void createMenus();

        // Model Setup routines
        void setupModelView();
        bool setupPersonsModel();
        // Poller to update the user interface
        QTimer *poller;

        void keyPressEvent( QKeyEvent *keyEvent );
        void jumpPosition(int change);

        QModelIndex currentIndex;
        QString fileName;
        int currentFileDuration;
        QString sittingName;
        QString mediaLocation;
        QString currentMediaFile;
        bool _isPlaying;

        //OAUTH

        bool isLoggedIn();

        //Bungeni
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        QByteArray networkData;
        QModelIndex addSitting(QString, QDateTime, QDateTime, QString);
        UserDetails *currentUserDetails;
        QString getHostName();
        QString getClientSecret();
        void saveRefreshToken(QString);
        QString readRefreshToken();
        QMap<QString, QModelIndex> replySittingMap;
        QMap<QNetworkReply*, QModelIndex> replyTakeItemsMap;
        TakesDownloadManager *takesDownloadManager;
};

#endif
