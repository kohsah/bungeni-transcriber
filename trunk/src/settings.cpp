
#include <QString>
#include <QSettings>
#include "settings.hpp"

SettingsDialog::SettingsDialog(QWidget * parent ) : QDialog(parent)
{
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    ui.setupUi(this);
    settings.beginGroup("Network");
    QObject::connect(ui.buttonBox, SIGNAL( accepted() ), this, SLOT( saveData() ) );
    QObject::connect(ui.buttonBox, SIGNAL( rejected() ), this, SLOT( close() ) );
    ui.hostname->setText(settings.value("hostname").toString());
    ui.portnumber->setText(settings.value("port").toString());
    ui.username->setText(settings.value("username").toString());
    
    
    //TO DO - Encrypt password.
    
    ui.password->setText(settings.value("password").toString());
    this->setModal(true);
    settings.endGroup();
}

void SettingsDialog::saveData()
{
    
        QString hostname = ui.hostname->text();
        QString port = ui.portnumber->text();
        QString username = ui.username->text();
        QString password = ui.password->text();
        QSettings settings("transcribe.conf", QSettings::IniFormat);
        settings.beginGroup("Network");
        //QStringList keys = settings.childKeys();
        settings.setValue("hostname", hostname);
        settings.setValue("port", port);
        settings.setValue("username", username);
        settings.setValue("password", password);
     settings.endGroup();
        this->hide();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::close()
{
    this->hide();
}
