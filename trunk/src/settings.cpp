
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
    ui.hostName->setText(settings.value("hostName").toString());
    ui.clientSecret->setText(settings.value("clientSecret").toString());
    this->setModal(true);
    settings.endGroup();
}

void SettingsDialog::saveData()
{
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString hostName = ui.hostName->text();
    if (hostName.endsWith("/")){
        hostName.chop(1);
    }
    settings.setValue("hostName", hostName);
    settings.setValue("clientSecret", ui.clientSecret->text());
    settings.endGroup();
    this->hide();
}

void SettingsDialog::close()
{
    this->hide();
}
