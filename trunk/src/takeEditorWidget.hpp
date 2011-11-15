#ifndef TAKEEDITORWIDGET_HPP
#define TAKEEDITOREWIDGET_HPP

#include <QObject>
#include <QString>
#include <QDateTime>
#include "ui/ui_takeEditor.h"
class TakeEditorWidget : public QDialog
{
    Q_OBJECT
public:
    TakeEditorWidget();
    QString getTakeName();
    QString getMediaLocation();
    QDateTime getStartTime();
    QDateTime getEndTime();
    void setTakeName(QString);
    void setStartTime(QDateTime);
    void setEndTime(QDateTime);
    void setMediaLocation(QString);
private:
    Ui::addTakeDialog ui;
public slots:
    void locateMediaLocation();
};
#endif // TakeEditorWidget_HPP
