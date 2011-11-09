#include <QMenu>
#include <QDebug>
#include "sitting.hpp"
#include "playlistModel.hpp"
#include "../playlistWidget.hpp"

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
    connect(addTakeAct, SIGNAL(triggered()), this, SLOT(addTakeDialog()));
    contextMenu.addAction(addTakeAct);

    QAction *removeAct = new QAction(tr("&Remove"), playlistWidget);
    removeAct->setStatusTip(tr("Remove selected sitting"));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(removeSitting()));
    contextMenu.addAction(removeAct);

    contextMenu.exec(QCursor::pos());
}

void Sitting::editSitting(){

}

void Sitting::removeSitting(){

}

void Sitting::addTakeDialog(){
    addTakeWidget = new AddTakeWidget();
    addTakeWidget->show();
    QObject::connect(addTakeWidget, SIGNAL(accepted()), this, SLOT(addTake()));
}

void Sitting::addTake(){
    QString takeName = addTakeWidget->getTakeName();
    QDateTime startTime = addTakeWidget->getStartTime();
    QDateTime endTime = addTakeWidget->getEndTime();
    QString mediaLocation = addTakeWidget->getMediaLocation();
    Take * newTake = new Take(takeName, startTime, endTime, mediaLocation);
    PlaylistModel *model = static_cast<PlaylistWidget*>(playlist)->getModel();
    model->insertItem(index, newTake);
}
