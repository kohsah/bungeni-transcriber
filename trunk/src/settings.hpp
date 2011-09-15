#ifndef _TRANSCRIBE_SETTINGS_H_
#define _TRANSCRIBE_SETTINGS_H_


#include "ui_serverSettings.h"
#include <QDialog>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(QWidget * parent = 0);
    virtual ~SettingsDialog();
private:
    Ui::Dialog ui;
public slots:
    void saveData();
    void close();
};

#endif
