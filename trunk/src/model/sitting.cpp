#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include "sitting.hpp"
#include "playlistModel.hpp"
#include "../playlistWidget.hpp"
#include "../takeEditorWidget.hpp"
#include "../sittingEditorWidget.hpp"

Sitting::Sitting(QString name_, QDateTime start_, QDateTime end_) :
    PlaylistItem(name_, start_, end_){
    index = QModelIndex();
    playlist = NULL;
}

void Sitting::appendChild(Take *child){
    childItems.append(child);
}

void Sitting::menu(QWidget * playlistWidget, QModelIndex index_){
    QMenu contextMenu;
    index = index_;
    playlist = playlistWidget;
    QAction *editAct = new QAction(tr("&Edit"), playlistWidget);
    editAct->setStatusTip(tr("Edit selected sitting"));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editSitting()));
    contextMenu.addAction(editAct);

    QAction *addTakeAct = new QAction(tr("&Add Take"), playlistWidget);
    addTakeAct->setStatusTip(tr("Add a take to the selected sitting"));
    connect(addTakeAct, SIGNAL(triggered()), this, SLOT(addTake()));
    contextMenu.addAction(addTakeAct);

    QAction *removeAct = new QAction(tr("&Remove"), playlistWidget);
    removeAct->setStatusTip(tr("Remove selected sitting"));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(removeSitting()));
    contextMenu.addAction(removeAct);

    contextMenu.exec(QCursor::pos());
}

void Sitting::editSitting(){
    SittingEditorWidget *sittingEditor = new SittingEditorWidget();
    sittingEditor->setStartDateTime(start);
    sittingEditor->setEndDateTime(end);
    sittingEditor->setSittingName(name);
    int result = sittingEditor->exec();
    if (result == QDialog::Accepted) {
        name = sittingEditor->getSittingName();
        start = sittingEditor->getStartDateTime();
        end = sittingEditor->getEndDateTime();
        PlaylistModel *model = static_cast<PlaylistWidget*>(playlist)->getModel();
        model->customDataChanged(index, index);
    }
    delete sittingEditor;
}

void Sitting::addTake(){
    TakeEditorWidget *addTakeWidget = new TakeEditorWidget();
    addTakeWidget->setStartTime(QDateTime::currentDateTime());
    addTakeWidget->setEndTime(QDateTime::currentDateTime());
    int result = addTakeWidget->exec();
    if (result == QDialog::Accepted){
        QString takeName = addTakeWidget->getTakeName();
        QDateTime startTime = addTakeWidget->getStartTime();
        QDateTime endTime = addTakeWidget->getEndTime();
        QString mediaLocation = addTakeWidget->getMediaLocation();
        Take * newTake = new Take(takeName, startTime, endTime, mediaLocation);
        PlaylistModel *model = static_cast<PlaylistWidget*>(playlist)->getModel();
        model->insertItem(index, newTake);
    }
    delete addTakeWidget;
}

void Sitting::removeSitting(){
    QMessageBox msgBox;
    msgBox.setInformativeText("Are you sure you want to remove the item from the playlist");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int result = msgBox.exec();
    if (result == QMessageBox::Yes){
        //pass
    }
}
