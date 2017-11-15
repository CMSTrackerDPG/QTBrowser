#ifndef DQMFILEDOWNLOADER_H
#define DQMFILEDOWNLOADER_H

#include <QMainWindow>
#include <QSortFilterProxyModel>

#include "models/remotefilesmodel.h"

namespace Ui {
class DQMFileDownloader;
}

class DQMFileDownloader : public QWidget
{
    Q_OBJECT
public:
    DQMFileDownloader(QWidget *parent = 0);
    ~DQMFileDownloader();
    bool downloadTFileFromUrl(QString name, QString url);

signals:
    void on_finishedDownloadFile(QString file_path);

private slots:
    void on_pushButton_clicked();
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_actionPreferences_triggered();
    void on_lineEdit_returnPressed();
    void on_pushButton_2_clicked();
    void on_comboBox_currentIndexChanged(const QString &dropdowntext);
    void on_pushButton_3_clicked();

private:
    void setupCertificates();
    bool isValidSettings();

    Ui::DQMFileDownloader *ui;
    RemoteFilesModel      *ONLINE_remote_files_model  = nullptr;
    RemoteFilesModel      *OFFLINE_remote_files_model = nullptr;
    RemoteFilesModel      *RELVAL_remote_files_model  = nullptr;
    RemoteFilesModel      *current_model              = nullptr;

    QSortFilterProxyModel *proxy_remote_files_model;
};

#endif // DQMFILEDOWNLOADER_H
