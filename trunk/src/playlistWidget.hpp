/**********************************************************
 * playlistWidget.hpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - UNDESA
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
 
 
#ifndef _PLAYLISTWIDGET_H_
#define _PLAYLISTWIDGET_H_

#include <QStandardItemModel>
#include <QTreeView>
#include <QWidget>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QSpacerItem>

#include "addSittingWidget.hpp"
#include "model/playlistModel.hpp"
#define setupSmallButton( aButton ){  \
    aButton->setMaximumSize( QSize( 26, 26 ) ); \
    aButton->setMinimumSize( QSize( 26, 26 ) ); \
    aButton->setIconSize( QSize( 20, 20 ) ); \
    aButton->setFocusPolicy( Qt::NoFocus ); \
}

class PlaylistWidget : public QWidget
{
    Q_OBJECT
    public:
        PlaylistWidget();
        ~PlaylistWidget();
        PlaylistModel * getModel();
        void clearPlaylist();
        int getSelected();
    private:
        PlaylistModel *model;
        void setupModelView();
        QTreeView *treeView;
        QGridLayout *layout;
        QPushButton *addToPlaylistButton;
        QPushButton *removeFromPlaylistButton;
        QSpacerItem *horizontalSpacer;
        AddSittingWidget *addToPlaylist;
        int current;
        void takeMenu();
        void sittingMenu();
    public slots:
        void addItemToPlaylist();
        void addToPlaylistDialog();
        void play(int logicalIndex);
        void play(const QModelIndex&);
        void prev();
        void next();
        void contextualMenu(const QPoint&);
        void editTake();
        void removeTake();
        void editSitting();
        void addTake();
        void removeSitting();
    signals:
        void playMediaFile(QString);
        void loadTranscriptFile(int, QString);
};
#endif
