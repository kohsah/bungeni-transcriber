#include <QFileDialog>
#include "addTakeWidget.hpp"

AddTakeWidget::AddTakeWidget() : QDialog(){
    ui.setupUi(this);
    this->setModal(true);
    ui.startDateTime->setDateTime(QDateTime::currentDateTime());
    ui.endDateTime->setDateTime(QDateTime::currentDateTime());
    QObject::connect( ui.mediaLocation, SIGNAL(clicked()), this, SLOT(locateMediaLocation()));
}
QString AddTakeWidget::getTakeName(){
    return ui.takeName->text();
}
QString AddTakeWidget::getMediaLocation() {
    return ui.mediaLocation->text();
}

QDateTime AddTakeWidget::getStartTime(){
    return ui.startDateTime->dateTime();
}

QDateTime AddTakeWidget::getEndTime(){
    return ui.endDateTime->dateTime();
}

void AddTakeWidget::locateMediaLocation()
{
    QString newFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Media Files (*.ogv *.ogg);;Any File (*.*)"));
    ui.mediaLocation->setText(newFilePath);
}
