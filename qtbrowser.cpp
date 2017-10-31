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

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->close();
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    model = new QStandardItemModel;

    proxy_model = new LeafSortFilterProxyModel;
    proxy_model->setSourceModel(model);

    ui->treeView->setModel(proxy_model);

    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)),
            this,         SLOT(customMenuRequested(QPoint)));

    add_root_file_as_tree("/home/fil/projects/dqmPlotter/f1.root");

    //TODO: Figure out how to dynamically change the Plugins

    // TODO: sendSignal needs a better name
    connect(this,       SIGNAL(sendSignal(TObjectContainer&)),
            ui->widget, SLOT(receiveTObj(TObjectContainer&)));
}

QTBrowser::~QTBrowser()
{
    delete ui;
}

// removes the idx item and its children.
void QTBrowser::remove_tree_item(QModelIndex idx) {
    proxy_model->removeColumn(0, idx);
    proxy_model->removeRow(idx.row(), idx.parent());
}

void QTBrowser::previewItem(QModelIndex idx)
{
    TObjectContainer* o = idx.data(Qt::UserRole + 1).value<TObjectContainer*>();
    if(o && o->getObject()) {
        qDebug() << o->getTitle();
        PreviewQRootCanvas* preview = new PreviewQRootCanvas;
        preview->setAttribute(Qt::WA_DeleteOnClose);
        preview->show();
        preview->preview((TH1*)o->getObject());
    }
}

//___________________________ EVENT HANDLERS ___________________________
void QTBrowser::customMenuRequested(QPoint pos){
    QModelIndex index= ui->treeView->indexAt(pos);

    QMenu* menu = new QMenu(this);
    QAction* preview_item_action = new QAction("Preview", this);
    QAction* remove_item_action  = new QAction("Remove", this);

    menu->addAction(preview_item_action);
    menu->addAction(remove_item_action);

    menu->popup(ui->treeView->viewport()->mapToGlobal(pos));

    connect(preview_item_action, &QAction::triggered, [this, index]() { previewItem(index); });
    connect(remove_item_action,  &QAction::triggered, [this, index]() { remove_tree_item(index); });
}

void QTBrowser::on_actionSettings_triggered()
{
    SettingsDialog* settings_dialog = new SettingsDialog;
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->show();
}

void QTBrowser::on_actionDownload_Remote_Files_triggered()
{
    DQMFileDownloader* file_downloader = new DQMFileDownloader(this);
    file_downloader->setAttribute(Qt::WA_DeleteOnClose);
    file_downloader->show();
}

void QTBrowser::on_filterLineEdit_returnPressed()
{
    QString filter = ui->filterLineEdit->text();
    proxy_model->setFilterRegExp(filter);

    QModelIndexList indexes = proxy_model->match(proxy_model->index(0,0), Qt::DisplayRole, "*", -1, Qt::MatchWildcard|Qt::MatchRecursive);
    if(filter.length()) { // so that we dont expand everything when the query is deleted
        foreach (QModelIndex index, indexes)
            ui->treeView->expand(index);
    } else {
        foreach (QModelIndex index, indexes)
            ui->treeView->collapse(index);
    }
}

//TODO: Generally not happy with how the filtering is done w.r.t. to
//      already loaded files.
void QTBrowser::on_actionOpen_File_triggered()
{
    QString dl_path = SettingsManager::getInstance().getSetting(SETTING::DOWNLOAD_PATH);
    QString file_name = QFileDialog::getOpenFileName(this, tr("Select"), dl_path, tr("ROOT files (*.root)"));

    if(file_name.length()) {
        add_root_file_as_tree(file_name);
        on_filterLineEdit_returnPressed();
    }
}

void QTBrowser::on_treeView_doubleClicked(const QModelIndex &index)
{
    TObjectContainer* o = index.data(Qt::UserRole + 1).value<TObjectContainer*>();
    if(o) {
        qDebug() << o->getTitle();
        sendSignal(*o);
    }
}

//___________________________ TREE TRAVERSAL ___________________________
void QTBrowser::add_root_file_as_tree(QString file_path){
    TFile* f = TFile::Open(file_path.toStdString().c_str());

    QStandardItem* parent_item = new QStandardItem(file_path);

    for (auto i : *(f->GetListOfKeys())) {
        // TODO: file_path is just being passed in visit
        //       so that a tooltip for the current file showing
        //       sohwing the filename+currentpath in the file
        //       can be displayed in the plugins
        //       This can probably be done more elegantly.
        visit(((TKey*)i), "", parent_item, file_path);
    }

    model->appendRow(parent_item);
}

void QTBrowser::visit(TKey* td, QString current_path, QStandardItem* parent, QString file_path) {
    TIter list(((TDirectory*)(td->ReadObj()))->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)list())) {
        TClass *cl1 = gROOT->GetClass(key->GetClassName());

        TObject* o = key->ReadObj();

        TObjectContainer *object = new TObjectContainer(o, file_path, current_path);
        QStandardItem *item = new QStandardItem(o->GetTitle());

        item->setData(QVariant::fromValue(object));  // this defaults to Qt::UserRole + 1
        parent->appendRow(item);

        if (cl1->InheritsFrom("TH1")) {

            QIcon icon;
            QString th3 = "/home/fil/projects/roottreeviewer/icons/h3_t.xpm";
            QString th2 = "/home/fil/projects/roottreeviewer/icons/h2_t.xpm";
            QString th1 = "/home/fil/projects/roottreeviewer/icons/h1_t.xpm";

            if      (dynamic_cast<TH3*>(o))   icon = QIcon(th3);
            else if (dynamic_cast<TH2*>(o))   icon = QIcon(th2);
            else                              icon = QIcon(th1);

            item->setIcon(icon);
        }

        if(cl1->InheritsFrom("TDirectory")) {
            visit(key, current_path + "/" + QString(key->GetTitle()), item, file_path);
        }
    }
}
