#ifndef ADDTAKEWIDGET_HPP
#define ADDTAKEWIDGET_HPP

#include <QObject>
#include <QString>
#include <QDateTime>
#include "ui/ui_addTake.h"
class AddTakeWidget : public QDialog
{
    Q_OBJECT
public:
    AddTakeWidget();
    QString getTakeName();
    QString getMediaLocation();
    QDateTime getStartTime();
    QDateTime getEndTime();
private:
    Ui::addTakeDialog ui;
public slots:
    void locateMediaLocation();
};
#endif // ADDTAKEWIDGET_HPP
