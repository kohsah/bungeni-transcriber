#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include "take.hpp"
#include "../takeEditorWidget.hpp"
#include "playlistModel.hpp"
#include "../playlistWidget.hpp"
Take::Take(QString name_, QDateTime start_,
           QDateTime end_, QString mediaLocation_) :
    PlaylistItem(name_, start_, end_){
    mediaLocation = mediaLocation_;
    index = QModelIndex();
    playlist = NULL;
}

void Take::appendChild(){
    qDebug() << "BUNGENI TRANSCRIBE ERROR : attempt to add a child to a take";
}
void Take::menu(QWidget * playlistWidget, QModelIndex index_){
    QMenu contextMenu;
    index = index_;
    playlist = playlistWidget;
    QAction *editAct = new QAction(tr("&Edit"), playlistWidget);
    editAct->setStatusTip(tr("Edit take"));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editTake()));
    contextMenu.addAction(editAct);

    QAction *removeAct = new QAction(tr("&Remove"), playlistWidget);
    removeAct->setStatusTip(tr("Remove take"));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(removeTake()));
    contextMenu.addAction(removeAct);

    contextMenu.exec(QCursor::pos());
}

void Take::editTake(){
    TakeEditorWidget *takeEditor = new TakeEditorWidget();
    takeEditor->setStartTime(start);
    takeEditor->setEndTime(end);
    takeEditor->setTakeName(name);
    takeEditor->setMediaLocation(mediaLocation);
    int result = takeEditor->exec();
    if (result == QDialog::Accepted){
        name = takeEditor->getTakeName();
        start = takeEditor->getStartTime();
        end = takeEditor->getEndTime();
        mediaLocation = takeEditor->getMediaLocation();
        PlaylistModel *model = static_cast<PlaylistWidget*>(playlist)->getModel();
        model->customDataChanged(index, index);
    }
    delete takeEditor;
}

void Take::removeTake(){
    QMessageBox msgBox;
    msgBox.setInformativeText("Are you sure you want to remove the item from the playlist");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int result = msgBox.exec();
    if (result == QMessageBox::Yes){

    }
}
