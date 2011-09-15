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

#include <vlc.h>

#include "ui/ui_transcribe.h"
#include "listViewDelegate.hpp"
#include "qxtspanslider/qxtspanslider.h"
#include "ui/ui_uploadProgress.h"
#include "network/formpost.hpp"
#include "controlsWidget.hpp"
#include "playlistWidget.hpp"

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
    public slots:  
        void addSpeech();
        void removeSpeech();
        void updateComplete( int state );
        void replyFinished(QNetworkReply*);
        void saveFileAs();
        void saveFile();
        void openFile();
        void newFile();
        void slotError(QNetworkReply::NetworkError);
        void getTakesLoginComplete();
        void progress ( qint64, qint64 );
        void takesReply();
        void takesDownload( QNetworkReply * reply );
        void playItem(const QModelIndex& );
        void postFinished();
        void updateInterface();
        void changeVolume(int newVolume);
        void changePosition(int newPosition);
        void playFile(QString file);
        void play();
        void stop();
        void about();
        void preferences();
        void hotkeySettings();
        void loadMetaData(QString);
        void loadNextFileSlot(int currentLogicalIndex,QString newfileName);
        void skipForward(int sec);
        void skipBackward(int sec);
        void playFaster();
        void playSlower();
        void getLength();
        void selection(QModelIndex);
        void takes();
        void getMPList();
        void MPListReply();
        void post();
        //void addAgendaItem();
    private:
        TranscribeWidget();
        ~TranscribeWidget();
        static TranscribeWidget *instance;
        QFrame * video;
        void raise(libvlc_exception_t * ex);
        Ui::Transcribe ui;
        bool _isPlaying;
        libvlc_exception_t _vlcexcep;
        libvlc_instance_t *_vlcinstance;
        libvlc_media_player_t *_mp;
        libvlc_media_t *_m;
        libvlc_time_t _file_duration;
        libvlc_event_manager_t * p_event_manager;
        QTableView *table;
        QVBoxLayout *mainLayout;
        QPushButton *addButton;
        QPushButton *removeButton;
        ListViewDelegate *delegate;
        QStandardItemModel *model;
        QSortFilterProxyModel *filterModel;
        QItemSelectionModel *selectionModel;
        void setupModelView();
        QxtSpanSlider *horizontalSlider; 
        QModelIndex currentIndex;
        QTemporaryFile * writeTemp();
        void refresh(const QModelIndex & index);
        bool writeFile(QString fileName);
        QWidget *progressWidget; 
        Ui::uploadProgress uiProgress;
        QString fileName;
        FormPostPlugin * posta;
        QNetworkReply *reply;
        QHash<QString, QString> hash;
        QHash<QString, QString> mphash;
        QHash<QString, QString> agendahash;
        QTimer *poller;
        bool loadFile(QString newfileName);
        void createActions();
        void createMenus();
        PlaylistWidget *playlist;
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *helpMenu;
        QAction *addToPlaylistAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *getTakesAct;
        QAction *postTakesAct;
        QAction *preferencesAct;
        QAction *exitAct;
        QAction *aboutAct;
        QAction *hotkeyAct;
        int currentFileDuration;
        QString sittingName;
        QString mediaLocation;
        QString currentMediaFile;
        ControlsWidget *controls;
        void keyPressEvent( QKeyEvent *keyEvent );
        void jumpPosition(int change);
};

#endif
