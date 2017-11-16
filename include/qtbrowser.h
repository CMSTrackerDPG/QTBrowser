#ifndef QTBROWSER_H
#define QTBROWSER_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QStandardItemModel>

#include <TKey.h>
#include <TApplication.h>

#include "plugins/iplugin.h"
#include "tfiletreeviewer.h"
#include "dqmfiledownloader.h"

namespace Ui {
class QTBrowser;
}

class QTBrowser : public QMainWindow
{
    Q_OBJECT

public:
    explicit QTBrowser(QWidget *parent = 0);
    ~QTBrowser();


public slots:
    void on_finishedDownloadFile(QString file_path);

private slots:
    void on_actionSettings_triggered();
    void on_actionSuperimpose_triggered();
    void on_actionConcatinate_triggered();
    void on_actionFit_triggered();

    void on_actionPreview_triggered();

private:
    void runROOTApp();
    void setup_plugin();

    Ui::QTBrowser *ui;
    TApplication* rootapp = nullptr;
    DQMFileDownloader* filedownloader;
    TFileTreeViewer* filebrowser_tree;
    std::unique_ptr<QWidget> active_plugin = nullptr;
};

#endif // QTBROWSER_H
