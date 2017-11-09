#include "qtbrowser.h"
#include "ui_qtbrowser.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>

#include <TROOT.h>
#include <TFile.h>
#include <TH3.h>
#include <TH2.h>

#include "tobjectcontainer.h"
#include "previewqrootcanvas.h"
#include "settings/settingsmanager.h"

#include "settings/settingsdialog.h"
#include "dqmfiledownloader/dqmfiledownloader.h"
#include "plugins/superimposeplugin.h"
#include "plugins/concatinateplugin.h"

QTBrowser::QTBrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTBrowser)
{
    rootapp = new TApplication("ROOT Application", 0, 0);
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
    delete rootapp;
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

void QTBrowser::addPlugin(QString name)
{
    removeActivePlugin();

    if(name.compare("Superimpose") == 0)
        active_plugin = new SuperimposePlugin();
    if(name.compare("Concatinate") == 0)
        active_plugin = new ConcatinatePlugin();

    ui->splitter->addWidget(active_plugin);
    connect(filebrowser_tree, SIGNAL(sendTObjectContainer(TObjectContainer&)),
            active_plugin,    SLOT(receiveTObjectContainer(TObjectContainer&)));
}

void QTBrowser::removeActivePlugin()
{
    if(active_plugin != nullptr) {
        disconnect(filebrowser_tree, SIGNAL(sendTObjectContainer(TObjectContainer&)),
                   active_plugin,    SLOT(receiveTObjectContainer(TObjectContainer&)));

        delete active_plugin;
    }
}


void QTBrowser::on_actionSuperimpose_triggered()
{
    addPlugin("Superimpose");
}

void QTBrowser::on_actionConcatinate_triggered()
{
    addPlugin("Concatinate");
}
