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

QTBrowser::QTBrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTBrowser)
{
    rootapp = new TApplication("ROOT Application", 0, 0);

    ui->setupUi(this);

    connect(ui->widget_3, SIGNAL(sendTObjectContainer(TObjectContainer&)),
            ui->widget,   SLOT(receiveTObjectContainer(TObjectContainer&)));
}

QTBrowser::~QTBrowser()
{
    delete ui;
}

void QTBrowser::on_actionSettings_triggered()
{
    SettingsDialog* settings_dialog = new SettingsDialog;
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->show();
}



