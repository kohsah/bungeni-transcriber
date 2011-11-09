/********************************************************************
 * playlistWidget.cpp
 ********************************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2011 - UNDESA <www.parliaments.info>
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
 
#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QAbstractItemView>
#include "playlistWidget.hpp"
#include "model/sitting.hpp"
PlaylistWidget :: PlaylistWidget() : QWidget()
{
    this->setupModelView();
    QObject::connect( addToPlaylistButton, SIGNAL(clicked()), this, SLOT(addToPlaylistDialog()));
    QObject::connect( treeView, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( play( const QModelIndex & ) ) );
}

PlaylistWidget :: ~PlaylistWidget()
{
}

void PlaylistWidget :: addToPlaylistDialog()
{
    addToPlaylist = new AddToPlaylistWidget();
    addToPlaylist->show();
    QObject::connect( addToPlaylist, SIGNAL(accepted()), this, SLOT(addItemToPlaylist()));
    
}

void PlaylistWidget :: addItemToPlaylist()
{
    if (addToPlaylist->addExisting())
    {
        // TODO
    }
    else
    {
        QString sittingName = addToPlaylist->getSittingName();
        QDateTime startTime = addToPlaylist->getStartDateTime();
        QDateTime endTime = addToPlaylist->getEndDateTime();
        Sitting *newSitting = new Sitting(sittingName, startTime, endTime);
        QModelIndex temp = QModelIndex();
        model->insertItem(temp, newSitting);
    }
}

PlaylistModel * PlaylistWidget :: getModel()
{
    return model;
}

void PlaylistWidget :: addTakeToPlaylist(QString sittingName, QString mediaFileLocation, QString trsFile)
{
    // TODO
}

void PlaylistWidget :: play(int logicalIndex)
{   /*
    qDebug() << "doubleclick" << logicalIndex;
    emit loadTranscriptFile(current, model->data(model->index(logicalIndex, 2)).toString());
    
    current = logicalIndex;
    QString mediaFilePath = model->data(model->index(logicalIndex, 1)).toString();
    
    //if (model->data(model->index(logicalIndex, 2)).toString() != "")
    
    emit playMediaFile(mediaFilePath);
    */
}

void PlaylistWidget :: play(const QModelIndex & index)
{   /*
    emit loadTranscriptFile(current, model->data(index).toString());
    qDebug() << "other play executed";
    current = index.row();
    QString mediaFilePath = model->data(model->index(index.row(), 1)).toString();
    
    emit playMediaFile(mediaFilePath); */
}

void PlaylistWidget :: setTranscriptFileLocation(QString transcriptFileLocation, int logicalIndex)
{ /*
    qDebug() << "Current Logical Index is " << current;
    if (logicalIndex < 0)
        model->setData(model->index(current, 2, QModelIndex()), transcriptFileLocation);
    else
        model->setData(model->index(logicalIndex, 2, QModelIndex()), transcriptFileLocation);
        */
}

int PlaylistWidget ::getSelected()
{
    if (model->rowCount()>0)
    {
        return current;
    }
    else
    {
        return -1;
    }
}

void PlaylistWidget :: next()
{ /*
    if (current < model->rowCount()-1)
    {
        QString mediaFilePath = model->data(model->index(current+1, 1)).toString();
        play(current+1);
    } */
}

void PlaylistWidget :: prev()
{/*
    if ( current > 0 )
    {
        QString mediaFilePath = model->data(model->index(current - 1, 1)).toString();
        play(current-1);
    } */
}

QString PlaylistWidget :: getMediaLocation()
{ /*
    return model->data(model->index(current, 1)).toString();
    */
    return QString();
}

QString PlaylistWidget :: getSittingName()
{
     //return model->data(model->index(current, 0)).toString();
    return QString();
}

void PlaylistWidget :: clearPlaylist()
{
    qDebug() << "Clear playlist";
   // model->removeRows(0,model->rowCount());
}

void PlaylistWidget :: setupModelView()
{
    model = new PlaylistModel();
    treeView = new QTreeView();
    treeView->setModel(model);
    QSizePolicy sizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(contextualMenu(const QPoint &)));

    addToPlaylistButton = new QPushButton("");
    addToPlaylistButton->setSizePolicy( sizePolicy );
    setupSmallButton( addToPlaylistButton );
    addToPlaylistButton->setIcon(QIcon(":/pixmaps/add.png"));
    
    removeFromPlaylistButton = new QPushButton("");
    removeFromPlaylistButton->setSizePolicy( sizePolicy );
    setupSmallButton( removeFromPlaylistButton );
    removeFromPlaylistButton->setIcon(QIcon(":/pixmaps/remove.png"));
    
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    layout = new QGridLayout();
    layout->addWidget(treeView, 0, 0, 1, 3);
    layout->addWidget(addToPlaylistButton, 1, 0);
    layout->addWidget(removeFromPlaylistButton, 1, 1);
    layout->addItem(horizontalSpacer, 1, 2);
    
    setLayout(layout);
}

void PlaylistWidget::contextualMenu(const QPoint & point){
    QModelIndex index = treeView->indexAt(point);
    PlaylistItem *item = static_cast<PlaylistItem*>(index.internalPointer());
    item->menu(this, index);
}
