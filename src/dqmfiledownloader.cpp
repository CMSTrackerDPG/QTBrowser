#include <QFileDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include <TEnv.h>
#include <TFile.h>

#include "include/dqmfiledownloader.h"
#include "ui_dqmfiledownloader.h"
#include "settings/settingsdialog.h"
#include "settings/settingsmanager.h"

DQMFileDownloader::DQMFileDownloader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DQMFileDownloader)
{
    ui->setupUi(this);

    ONLINE_remote_files_model = new RemoteFilesModel(this);
    ONLINE_remote_files_model->fillModelFromFile("data/online.txt");
    current_model = ONLINE_remote_files_model;

    proxy_remote_files_model = new QSortFilterProxyModel(this);
    proxy_remote_files_model->setSourceModel(current_model);

    ui->listView->setModel(proxy_remote_files_model);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

DQMFileDownloader::~DQMFileDownloader()
{
    delete ui;
}

bool DQMFileDownloader::downloadTFileFromUrl(QString download_path, QString url)
{
    TFile* f = TFile::Open(url.toStdString().c_str());
    if(f) {
        f->Cp(download_path.toStdString().c_str());
        f->Close();
    }
    return f;
}

void DQMFileDownloader::on_listView_doubleClicked(const QModelIndex &index)
{
    qDebug() << index;
}

void DQMFileDownloader::on_actionPreferences_triggered()
{
    SettingsDialog* settings_dialog = new SettingsDialog(this);
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->show();
}

void DQMFileDownloader::setupCertificates()
{
    auto& instance =  SettingsManager::getInstance();
    QString cert = instance.getSetting(SETTING::USER_CERTIFICATE_PATH);
    QString key = instance.getSetting(SETTING::USER_KEY_PATH);
    gEnv->SetValue("Davix.GSI.UserCert", cert.toUtf8().constData());
    gEnv->SetValue("Davix.GSI.UserKey", key.toUtf8().constData());
    gEnv->SetValue("Davix.GSI.GridMode", true);
    gEnv->SetValue("Davix.GSI.CACheck", false);
}

bool DQMFileDownloader::isValidSettings()
{
    auto sm = SettingsManager::getInstance();
    //TODO: figure out how to check if the fucking settings are valid;
    return true;
}

void DQMFileDownloader::on_lineEdit_returnPressed()
{
    QString filter = ui->lineEdit->text();
    proxy_remote_files_model->setFilterRegExp(filter);
}

// DOWNLOAD ONLY
void DQMFileDownloader::on_pushButton_clicked()
{
    //todo: extract function
    if(!isValidSettings()){
        QMessageBox::information(this,
                                 tr("DQMFileDownloader"),
                                 tr("Settings are not valid! (Edit->Settings)") );
        return;
    }

    setupCertificates();

    auto& sm = SettingsManager::getInstance();
    QString download_base_path = QFileDialog::getExistingDirectory(this, tr("Select"), sm.getSetting(SETTING::DOWNLOAD_PATH));

    if(download_base_path=="") return;

    qDebug() << download_base_path;

    for(auto& e : ui->listView->selectionModel()->selectedIndexes()){

        auto real_idx = proxy_remote_files_model->mapToSource(e);
        QString name = current_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = current_model->getFilepath(real_idx);
        qDebug() << 3;

        QString download_path =  download_base_path + "/" + name;
        downloadTFileFromUrl(download_path, url);
        qDebug() << 4;
    }

}


// DOWNLOAD AND OPEN IN TREEVIEW
void DQMFileDownloader::on_pushButton_2_clicked()
{
    //todo: extract function
    if(!isValidSettings()){
        QMessageBox::information(this,
                                 tr("DQMFileDownloader"),
                                 tr("Settings are not valid! (Edit->Settings)") );
        return;
    }

    setupCertificates();

    auto& sm = SettingsManager::getInstance();
    QString download_base_path = "data/downloads";

    sm.writeSettings(SETTING::DOWNLOAD_PATH, download_base_path);

    for(auto& e : ui->listView->selectionModel()->selectedIndexes()){

        auto real_idx = proxy_remote_files_model->mapToSource(e);
        QString name = current_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = current_model->getFilepath(real_idx);
        QString download_path =  download_base_path + "/" + name;

        bool download_success = downloadTFileFromUrl(download_path, url);
        if (download_success) {
            on_finishedDownloadFile(download_path);
        }
    }
}

void DQMFileDownloader::on_comboBox_currentIndexChanged(const QString& dropdowntext)
{
    if(dropdowntext == "Online") {
        if(!ONLINE_remote_files_model) {
            ONLINE_remote_files_model = new RemoteFilesModel(this);
            ONLINE_remote_files_model->fillModelFromFile("data/online.txt");
        }
        current_model = ONLINE_remote_files_model;

    } else if(dropdowntext == "Offline") {
        if(!OFFLINE_remote_files_model) {
            OFFLINE_remote_files_model = new RemoteFilesModel(this);
            OFFLINE_remote_files_model->fillModelFromFile("data/offline.txt");
        }
        current_model = OFFLINE_remote_files_model;

    } else if(dropdowntext == "Relval") {
        if(!RELVAL_remote_files_model) {
            RELVAL_remote_files_model = new RemoteFilesModel(this);
            RELVAL_remote_files_model->fillModelFromFile("data/relval.txt");
        }
        current_model = RELVAL_remote_files_model;
    } else {
        qDebug() << "[BUG] DQMFileDownloader::on_comboBox_currentIndexChanged This should not happen";
    }

    proxy_remote_files_model->setSourceModel(current_model);
    ui->listView->setModel(proxy_remote_files_model);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}
