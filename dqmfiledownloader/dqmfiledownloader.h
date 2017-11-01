#ifndef DQMFILEDOWNLOADER_H
#define DQMFILEDOWNLOADER_H

#include <QMainWindow>
#include <QSortFilterProxyModel>

#include "remotefilesmodel.h"

namespace Ui {
class DQMFileDownloader;
}

class DQMFileDownloader : public QMainWindow
{
    Q_OBJECT
public:
    explicit DQMFileDownloader(QWidget *parent = 0);
    ~DQMFileDownloader();

    // this is static so that QtConcurrent accepts it without having to write a wrapper
    static void download_tfile_from_url(QString download_path, QString url);

private slots:
    void on_pushButton_clicked();
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_actionPreferences_triggered();
    void on_lineEdit_returnPressed();

private:
    void setupCertificates();
    bool isValidSettings();

    Ui::DQMFileDownloader *ui;
    RemoteFilesModel      *remote_files_model;
    QSortFilterProxyModel *proxy_remote_files_model;
};

#endif // DQMFILEDOWNLOADER_H
