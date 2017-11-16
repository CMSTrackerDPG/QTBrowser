#include "tfiletreeviewer.h"
#include "ui_tfiletreeviewer.h"

#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QKeyEvent>
#include <QSignalMapper>

#include <TROOT.h>
#include <TFile.h>
#include <TKey.h>
#include <TH3.h>
#include <TH2.h>
#include <TClass.h>

#include "previewqrootcanvas.h"
#include "container/tobjectcontainer.h"
#include "settings/settingsmanager.h"

TFileTreeViewer::TFileTreeViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TFileTreeViewer)
{
    ui->setupUi(this);

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->close();
    model = new QStandardItemModel;

    proxy_model = new LeafSortFilterProxyModel;
    proxy_model->setSourceModel(model);

    ui->treeView->setModel(proxy_model);

    this->addTFileToTree("/home/fil/projects/dqmPlotter/f1.root");
}

TFileTreeViewer::~TFileTreeViewer()
{
    delete ui;
}

// removes the idx item and its children.
void TFileTreeViewer::removeTreeItem(QModelIndex idx) {
    proxy_model->removeColumn(0, idx);
    proxy_model->removeRow(idx.row(), idx.parent());
}

void TFileTreeViewer::previewItem(QModelIndex idx)
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

void TFileTreeViewer::on_filterLineEdit_returnPressed()
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

void TFileTreeViewer::on_treeView_doubleClicked(const QModelIndex &index)
{
    TObjectContainer* o = index.data(Qt::UserRole + 1).value<TObjectContainer*>();
    if(o) {
        qDebug() << o->getTitle();
        sendTObjectContainer(*o);
    }
}

void TFileTreeViewer::on_pushButton_clicked()
{
    QString dl_path = SettingsManager::getInstance().getSetting(SETTING::DOWNLOAD_PATH);
    QString file_name = QFileDialog::getOpenFileName(this, tr("Select"), dl_path, tr("ROOT files (*.root)"));

    if(file_name.length()) {
        addTFileToTree(file_name);
        on_filterLineEdit_returnPressed();
    }
}

//___________________________ TREE TRAVERSAL ___________________________
void TFileTreeViewer::addTFileToTree(QString file_path){
    TFile* f = TFile::Open(file_path.toUtf8().constData());

    if(!f) {
        qDebug() << "TFileTreeViewer::addTFileToTree: Could not open " << file_path;
        return;
    }

    QStandardItem* parent_item = new QStandardItem(file_path);

    for (auto i : *(f->GetListOfKeys())) {
        // file_path is just being passed in visit
        // so that a tooltip for the current file showing
        // sohwing the filename+currentpath in the file
        // can be displayed in the plugins
        visit(((TKey*)i), "", parent_item, file_path);
    }

    model->appendRow(parent_item);
}

void TFileTreeViewer::visit(TKey* td, QString current_path, QStandardItem* parent, QString file_path) {
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
            QString th3 = "/home/fil/projects/QTBrowser/icons/h3_t.xpm";
            QString th2 = "/home/fil/projects/QTBrowser/icons/h2_t.xpm";
            QString th1 = "/home/fil/projects/QTBrowser/icons/h1_t.xpm";

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

void TFileTreeViewer::on_pushButton_3_clicked()
{
    //preview selected
    previewItem(ui->treeView->currentIndex());
}

void TFileTreeViewer::on_pushButton_2_clicked()
{
    //remove selected
    removeTreeItem(ui->treeView->currentIndex());
}

void TFileTreeViewer::on_pushButton_4_clicked()
{
    on_filterLineEdit_returnPressed();
}
