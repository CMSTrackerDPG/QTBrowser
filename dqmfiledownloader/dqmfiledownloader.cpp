#include <QFileDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include <TEnv.h>
#include <TFile.h>

#include "dqmfiledownloader.h"
#include "ui_dqmfiledownloader.h"
#include "settings/settingsdialog.h"
#include "settings/settingsmanager.h"

DQMFileDownloader::DQMFileDownloader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DQMFileDownloader)
{
    ui->setupUi(this);

    ONLINE_remote_files_model = new RemoteFilesModel(this);
//    ONLINE_remote_files_model->fill_model_from_file("/home/fil/projects/QTBrowser/dqmfiledownloader/test.txt");

    proxy_remote_files_model = new QSortFilterProxyModel(this);
    proxy_remote_files_model->setSourceModel(ONLINE_remote_files_model);

    ui->listView->setModel(proxy_remote_files_model);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

DQMFileDownloader::~DQMFileDownloader()
{
    delete ui;
}

bool DQMFileDownloader::download_tfile_from_url(QString download_path, QString url)
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
        QString name = ONLINE_remote_files_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = ONLINE_remote_files_model->getFilepath(real_idx);
        QString download_path =  download_base_path + "/" + name;
        download_tfile_from_url(download_path, url);
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
    QString download_base_path = QFileDialog::getExistingDirectory(this, tr("Select"), sm.getSetting(SETTING::DOWNLOAD_PATH));

    if(!download_base_path.compare("")) return;

    sm.writeSettings(SETTING::DOWNLOAD_PATH, download_base_path);

    for(auto& e : ui->listView->selectionModel()->selectedIndexes()) {
        auto real_idx = proxy_remote_files_model->mapToSource(e);
        QString name = ONLINE_remote_files_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = ONLINE_remote_files_model->getFilepath(real_idx);
        QString download_path =  download_base_path + "/" + name;

        bool download_success = download_tfile_from_url(download_path, url);
        if (download_success) {
            on_finishedDownloadFile(download_path);
        }
    }
}
