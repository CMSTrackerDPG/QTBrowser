#include <QFileDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include <TEnv.h>
#include <TFile.h>

#include "include/dqmfiledownloader.h"
#include "ui_dqmfiledownloader.h"
#include "include/settings/settingsdialog.h"
#include "include/settings/settingsmanager.h"

DQMFileDownloader::DQMFileDownloader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DQMFileDownloader)
{
    ui->setupUi(this);
}

DQMFileDownloader::~DQMFileDownloader()
{
    delete ui;
}

bool DQMFileDownloader::downloadTFileFromUrl(QString download_path, QString url)
{
    TFile* f = TFile::Open(url.toUtf8().constData());
    if(f) {
        f->Cp(download_path.toUtf8().constData());
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
    auto& inst = SettingsManager::getInstance();

    //TODO: this is garbage
//    bool is_settings_set = inst.getSetting(SETTING::USER_CERTIFICATE_PATH).compare("") &&
//                           inst.getSetting(SETTING::USER_KEY_PATH).compare("");

//    bool is_valid_cert_key; // TODO: find resource on server that can be used to verify
//                            //       if the certificate+key are valid

//    return is_settings_set;
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

    if(!download_base_path.compare("")) return;

    sm.writeSettings(SETTING::DOWNLOAD_PATH, download_base_path);

    for(auto& e : ui->listView->selectionModel()->selectedIndexes()) {
        auto real_idx = proxy_remote_files_model->mapToSource(e);
        QString name = current_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = current_model->getFilepath(real_idx);
        QString download_path =  download_base_path + "/" + name;
        downloadTFileFromUrl(download_path, url);
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

    for(auto& e : ui->listView->selectionModel()->selectedIndexes()) {
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
    if(!dropdowntext.compare("Online")) {
        if(!ONLINE_remote_files_model) {
            ONLINE_remote_files_model = new RemoteFilesModel(this);
            ONLINE_remote_files_model->fillModelFromFile("data/online.txt");
        }
        current_model = ONLINE_remote_files_model;

    } else if(!dropdowntext.compare("Relval")) {
        if(!RELVAL_remote_files_model) {
            RELVAL_remote_files_model = new RemoteFilesModel(this);
            RELVAL_remote_files_model->fillModelFromFile("data/relval.txt");
        }

        current_model = RELVAL_remote_files_model;

    } else if(!dropdowntext.compare("Offline")) {
        qDebug() << "Too bad, not supported";
    } else {
        qDebug() << "[BUG] DQMFileDownloader::on_comboBox_currentIndexChanged How did you manage to get here";
    }

    if(current_model) {
        proxy_remote_files_model = new QSortFilterProxyModel(this);
        proxy_remote_files_model->setSourceModel(current_model);

        ui->listView->setModel(proxy_remote_files_model);
        ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}
