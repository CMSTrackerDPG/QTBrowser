#include <QFileDialog>
#include <QDebug>

#include "settings/settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings/settingsmanager.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    auto& instance = SettingsManager::getInstance();
    ui->lineEdit_user_certificate->setText(instance.getSetting(SETTING::USER_CERTIFICATE_PATH));
    ui->lineEdit_user_key->setText(instance.getSetting(SETTING::USER_KEY_PATH));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    auto& instance = SettingsManager::getInstance();
    instance.writeSettings(SETTING::USER_CERTIFICATE_PATH, ui->lineEdit_user_certificate->text());
    instance.writeSettings(SETTING::USER_KEY_PATH, ui->lineEdit_user_key->text());
}


void SettingsDialog::on_pushButton_clicked()
{
    // user certificate path
    QString file_name = QFileDialog::getOpenFileName(this, tr("Select"), "/home/", tr("PEM certificate (*.pem);; Certificate (*.cert)"));
    ui->lineEdit_user_certificate->setText(file_name);
}

void SettingsDialog::on_pushButton_2_clicked()
{
    // user key path
    QString file_name = QFileDialog::getOpenFileName(this, tr("Select"), "/home/", tr("PEM certificate (*.pem);; Certificate (*.cert)"));
    ui->lineEdit_user_key->setText(file_name);
}
