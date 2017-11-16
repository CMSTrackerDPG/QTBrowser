#include "qtbrowser.h"
#include "ui_qtbrowser.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>

#include <TROOT.h>
#include <TFile.h>
#include <TH3.h>
#include <TH2.h>

#include "dqmfiledownloader.h"
#include "previewqrootcanvas.h"
#include "container/tobjectcontainer.h"
#include "settings/settingsmanager.h"
#include "settings/settingsdialog.h"

#include "plugins/superimposeplugin.h"
#include "plugins/concatinateplugin.h"
#include "plugins/fitplugin.h"
#include "plugins/previewplugin.h"

QTBrowser::QTBrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTBrowser)
{
    ui->setupUi(this);
    filedownloader   = ui->widget_2;
    filebrowser_tree = ui->widget_3;

    // When a download finishes, qtbrowser catches the signal
    // switches the tab to the treeview, and tells the treeview
    // to load the downloaded file in its view
    connect(filedownloader, SIGNAL(on_finishedDownloadFile(QString)),
            this,           SLOT(on_finishedDownloadFile(QString)));
}

QTBrowser::~QTBrowser()
{
    delete ui;

//   -------------- ROOT cleanup --------------
// Note: Cleaning up properly causes ROOT to segfault
// https://root-forum.cern.ch/t/break-segmentation-fault-after-quitting-the-canvas/21627
// >This is a long-standing problem in ROOT(and nobody cares).

//    rootapp->Terminate(0);
//    delete rootapp;
//    rootapp=nullptr;
}

void QTBrowser::on_finishedDownloadFile(QString file_path)
{
    ui->tabWidget->setCurrentIndex(1);
    filebrowser_tree->addTFileToTree(file_path);
}

void QTBrowser::on_actionSettings_triggered()
{
    SettingsDialog* settings_dialog = new SettingsDialog;
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->show();
}

void QTBrowser::setup_plugin()
{
    ui->splitter->addWidget(active_plugin.get());
    connect(filebrowser_tree, SIGNAL(sendTObjectContainer(TObjectContainer&)),
            active_plugin.get(), SLOT(receiveTObjectContainer(TObjectContainer&)));
}

void QTBrowser::runROOTApp() {
    if(rootapp == nullptr) {
        rootapp = new TApplication("ROOT Application", 0, 0);
    }
}

void QTBrowser::on_actionSuperimpose_triggered()
{
    runROOTApp();
    active_plugin.reset(new SuperimposePlugin());
    setup_plugin();
}

void QTBrowser::on_actionConcatinate_triggered()
{
    runROOTApp();
    active_plugin.reset(new ConcatinatePlugin());
    setup_plugin();
}

void QTBrowser::on_actionFit_triggered()
{
    runROOTApp();
    active_plugin.reset(new FitPlugin());
    setup_plugin();
}

void QTBrowser::on_actionPreview_triggered()
{
    runROOTApp();
    active_plugin.reset(new PreviewPlugin());
    setup_plugin();
}
