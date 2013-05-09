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
#include <QSignalMapper>
#include <QMenu>
#include <QFileDialog>
#include "playlistWidget.hpp"
#include "model/take.hpp"
#include "model/sitting.hpp"
#include "takeEditorWidget.hpp"
#include "sittingEditorWidget.hpp"
#include "aknHandler.hpp"

PlaylistWidget :: PlaylistWidget() : QWidget()
{
    this->setupModelView();
    //QObject::connect( addToPlaylistButton, SIGNAL(clicked()), this, SLOT(addToPlaylistDialog()));
    QObject::connect( refreshPlaylistButton, SIGNAL(clicked()), this, SLOT(refreshPlaylistClicked()));
    QObject::connect( treeView, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( itemClicked( const QModelIndex & ) ) );
    QObject::connect( this, SIGNAL( currentTakeIndex(const QModelIndex&) ), model, SLOT( setCurrentTakeIndex( const QModelIndex & ) ) );
    submitHandler = new SubmitHandler();
}

PlaylistWidget :: ~PlaylistWidget()
{
}

void PlaylistWidget :: refreshPlaylistClicked(){
    emit refreshPlaylist();
}

/*void PlaylistWidget :: addToPlaylistDialog()
{
    addToPlaylist = new AddSittingWidget();
    addToPlaylist->show();
    QObject::connect( addToPlaylist, SIGNAL(accepted()), this, SLOT(addItemToPlaylist()));
    
}*/

/*void PlaylistWidget :: addItemToPlaylist()
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
}*/

PlaylistModel * PlaylistWidget :: getModel()
{
    return model;
}

void PlaylistWidget :: itemClicked(const QModelIndex & index)
{
    PlaylistItem *item = static_cast<PlaylistItem*>(index.internalPointer());
    if (item && item->getType() == PlaylistItem::TypeTake) {
        Take *take = static_cast<Take*>(item);
        emit loadTranscriptionItems(take->getItems());
        emit playMediaFile(take->getMediaLocation());
        emit currentTakeIndex(index);
    }
    /*
    emit loadTranscriptFile(current, model->data(index).toString());
    qDebug() << "other play executed";
    current = index.row();
    QString mediaFilePath = model->data(model->index(index.row(), 1)).toString();
    
    emit playMediaFile(mediaFilePath); */
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
        qDebug() << "getSelected" << "less than one";
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

void PlaylistWidget :: setupModelView()
{
    model = new PlaylistModel();
    treeView = new QTreeView();
    treeView->setModel(model);
    QSizePolicy sizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView->setSelectionBehavior( QAbstractItemView::SelectRows );
    treeView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    connect(treeView,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(contextualMenu(const QPoint &)));

    /*addToPlaylistButton = new QPushButton("");
    addToPlaylistButton->setSizePolicy( sizePolicy );
    setupSmallButton( addToPlaylistButton );
    addToPlaylistButton->setIcon(QIcon(":/pixmaps/add.png"));
    
    removeFromPlaylistButton = new QPushButton("");
    removeFromPlaylistButton->setSizePolicy( sizePolicy );
    setupSmallButton( removeFromPlaylistButton );
    removeFromPlaylistButton->setIcon(QIcon(":/pixmaps/remove.png")); */

    refreshPlaylistButton = new QPushButton("");
    refreshPlaylistButton->setSizePolicy( sizePolicy );
    setupSmallButton( refreshPlaylistButton );
    refreshPlaylistButton->setIcon(QIcon(":/pixmaps/refresh.png"));
    
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    layout = new QGridLayout();
    layout->addWidget(treeView, 0, 0, 1, 3);
    layout->addWidget(refreshPlaylistButton, 1, 0);
    //layout->addWidget(addToPlaylistButton, 1, 0);
    //layout->addWidget(removeFromPlaylistButton, 1, 1);
    layout->addItem(horizontalSpacer, 1, 2);
    
    setLayout(layout);
}

void PlaylistWidget::contextualMenu(const QPoint & point){
    QModelIndex index = treeView->indexAt(point);
    PlaylistItem *item = static_cast<PlaylistItem*>(index.internalPointer());
    if (item && item->getType() == PlaylistItem::TypeSitting){
        this->sittingMenu();
    }
    else if (item && item->getType() == PlaylistItem::TypeTake){
        this->takeMenu();
    }
}


void PlaylistWidget::takeMenu(){
    QMenu contextMenu;
    QAction *submitAct = new QAction(tr("&Submit"), this);
    connect(submitAct, SIGNAL(triggered()), this, SLOT(submitTake()));
    submitAct->setStatusTip(tr("Submit speeches"));
    contextMenu.addAction(submitAct);
    /*
    QAction *removeAct = new QAction(tr("&Remove"), this);
    connect(removeAct, SIGNAL(triggered()), this, SLOT(removeTake()));
    removeAct->setStatusTip(tr("Remove take"));
    contextMenu.addAction(removeAct);*/

    contextMenu.exec(QCursor::pos());
}

void PlaylistWidget::submitTake(){
    QModelIndex index = treeView->selectionModel()->currentIndex();
    Take *item = static_cast<Take*>(index.internalPointer());
    submitHandler->submit(item);
}

void PlaylistWidget::editTake(){
    QModelIndex index = treeView->selectionModel()->currentIndex();
    Take *item = static_cast<Take*>(index.internalPointer());
    Sitting *sitting = static_cast<Sitting*>(item->parent());
    TakeEditorWidget *takeEditor = new TakeEditorWidget();
    takeEditor->setParentTimes(sitting->getStartDateTime(), sitting->getEndDateTime());
    takeEditor->setStartTime(item->getStartDateTime());
    takeEditor->setEndTime(item->getEndDateTime());
    takeEditor->setTakeName(item->getName());
    takeEditor->setMediaLocation(item->getMediaLocation());
    int result = takeEditor->exec();
    if (result == QDialog::Accepted){
        item->setName(takeEditor->getTakeName());
        item->setStartDateTime(takeEditor->getStartTime());
        item->setEndDateTime(takeEditor->getEndTime());
        item->setMediaLocation(takeEditor->getMediaLocation());
        model->customDataChanged(index, index);
    }
    delete takeEditor;
}

void PlaylistWidget::removeTake(){
    QMessageBox msgBox;
    msgBox.setInformativeText("Are you sure you want to remove the item from the playlist");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int result = msgBox.exec();
    if (result == QMessageBox::Yes){

    }
}

void PlaylistWidget::sittingMenu(){
    QMenu contextMenu;

    QModelIndex index = treeView->selectionModel()->currentIndex();
    Sitting *sitting = static_cast<Sitting*>(index.internalPointer());
    /*
    QAction *editAct = new QAction(tr("&Edit"), this);
    editAct->setStatusTip(tr("Edit selected sitting"));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editSitting()));
    contextMenu.addAction(editAct);
    */
    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setStatusTip(tr("Edit selected sitting"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveSitting()));
    contextMenu.addAction(saveAct);

    QString f = sitting->getAknFilePath();
    if(!f.isNull()){
        QAction *saveAsAct = new QAction(tr("&Save As"), this);
        saveAsAct->setStatusTip(tr("Save as"));
        connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsSitting()));
        contextMenu.addAction(saveAsAct);
    }
    /*
    QAction *addTakeAct = new QAction(tr("&Add Take"), this);
    addTakeAct->setStatusTip(tr("Add a take to the selected sitting"));
    connect(addTakeAct, SIGNAL(triggered()), this, SLOT(addTake()));
    contextMenu.addAction(addTakeAct);

    QAction *removeAct = new QAction(tr("&Remove"), this);
    removeAct->setStatusTip(tr("Remove selected sitting"));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(removeSitting()));
    contextMenu.addAction(removeAct);
    */
    contextMenu.exec(QCursor::pos());
}

void PlaylistWidget::saveSitting(){
    QModelIndex index = treeView->selectionModel()->currentIndex();
    Sitting *sitting = static_cast<Sitting*>(index.internalPointer());
    QString f = sitting->getAknFilePath();
    if (f.isNull()){
        QString aknFilePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Akoma Ntoso (*.akn);;Any File (*.*)"));
        sitting->setAknFilePath(aknFilePath);
    }
    AknHandler* akn = new AknHandler();
    akn->saveSittingToFile(sitting, sitting->getAknFilePath());
}

void PlaylistWidget::saveAsSitting(){

}


void PlaylistWidget::editSitting(){
    QModelIndex index = treeView->selectionModel()->currentIndex();
    Sitting *item = static_cast<Sitting*>(index.internalPointer());
    SittingEditorWidget *sittingEditor = new SittingEditorWidget();
    sittingEditor->setStartDateTime(item->getStartDateTime());
    sittingEditor->setEndDateTime(item->getEndDateTime());
    sittingEditor->setSittingName(item->getName());
    int result = sittingEditor->exec();
    if (result == QDialog::Accepted) {
        item->setName(sittingEditor->getSittingName());
        item->setStartDateTime(sittingEditor->getStartDateTime());
        item->setEndDateTime(sittingEditor->getEndDateTime());
        model->customDataChanged(index, index);
    }
    delete sittingEditor;
}

void PlaylistWidget::addTake(){
    QModelIndex index = treeView->selectionModel()->currentIndex();
    Take *item = static_cast<Take*>(index.internalPointer());
    Sitting *sitting = static_cast<Sitting*>(item->parent());
    TakeEditorWidget *addTakeWidget = new TakeEditorWidget();
    addTakeWidget->setParentTimes(sitting->getStartDateTime(), sitting->getEndDateTime());
    addTakeWidget->setStartTime(QDateTime::currentDateTime());
    addTakeWidget->setEndTime(QDateTime::currentDateTime());
    int result = addTakeWidget->exec();
    if (result == QDialog::Accepted){
        QString takeName = addTakeWidget->getTakeName();
        QDateTime startTime = addTakeWidget->getStartTime();
        QDateTime endTime = addTakeWidget->getEndTime();
        QString mediaLocation = addTakeWidget->getMediaLocation();
        Take * newTake = new Take(takeName, startTime, endTime, mediaLocation);
        model->insertItem(index, newTake);
    }
    delete addTakeWidget;
}

void PlaylistWidget::removeSitting(){
    QMessageBox msgBox;
    msgBox.setText("Remove Take");
    msgBox.setInformativeText("Are you sure you want to remove the item from the playlist");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int result = msgBox.exec();
    if (result == QMessageBox::Yes){

    }
}
