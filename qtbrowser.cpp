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

    connect(this,        SIGNAL(sendSignal(TObject*)),
            ui->widget, SLOT(receiveTObj(TObject*)));
}

QTBrowser::~QTBrowser()
{
    delete ui;
}

void QTBrowser::customMenuRequested(QPoint pos){
    QModelIndex index= ui->treeView->indexAt(pos);

    QMenu* menu = new QMenu(this);
    QAction* preview_item_action = new QAction("Preview", this);
    QAction* remove_item_action  = new QAction("Remove", this);

    menu->addAction(preview_item_action);
    menu->addAction(remove_item_action);

    menu->popup(ui->treeView->viewport()->mapToGlobal(pos));

    connect(preview_item_action, &QAction::triggered, [this, index]() { preview_item(index); });
    connect(remove_item_action,  &QAction::triggered, [this, index]() { remove_tree_item(index); });
}


void QTBrowser::on_applyFilterButton_clicked()
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

void QTBrowser::on_actionOpen_File_triggered()
{
    qDebug() << "Open file pressed";
    QString file_name = QFileDialog::getOpenFileName(this, tr("Select"), "/home/", tr("ROOT files (*.root)"));

    if(file_name.length()) {
        add_root_file_as_tree(file_name);
    }
}

void QTBrowser::on_treeView_doubleClicked(const QModelIndex &index)
{
    TObjectContainer* o = index.data(Qt::UserRole + 1).value<TObjectContainer*>();
    if(o) {
        qDebug() << o->getName();
        sendSignal(o->getObject());
    }
}

// removes the idx item and its children.
void QTBrowser::remove_tree_item(QModelIndex idx) {
    proxy_model->removeColumn(0, idx);
    proxy_model->removeRow(idx.row(), idx.parent());
}

void QTBrowser::preview_item(QModelIndex idx)
{
    TObjectContainer* o = idx.data(Qt::UserRole + 1).value<TObjectContainer*>();
    if(o && o->getObject()) {
        qDebug() << o->getName();
        PreviewQRootCanvas* preview = new PreviewQRootCanvas;
        preview->setAttribute(Qt::WA_DeleteOnClose);
        preview->show();
        preview->preview((TH1*)o->getObject());
    }
}

//___________________________ TREE TRAVERSAL ___________________________
void QTBrowser::add_root_file_as_tree(QString file_path){
    TFile* f = TFile::Open(file_path.toStdString().c_str());

    QStandardItem *parentItem = new QStandardItem(file_path);
    model->appendRow(parentItem);

    for (auto i : *(f->GetListOfKeys())) {
        visit(((TKey*)i), "", parentItem);
    }
}

void QTBrowser::visit(TKey* td, QString current_path, QStandardItem* parent) {
    TIter list(((TDirectory*)(td->ReadObj()))->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)list())) {
        TClass *cl1 = gROOT->GetClass(key->GetClassName());

        TObject* o = key->ReadObj();

        TObjectContainer *object = new TObjectContainer(o);
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
            visit(key, current_path + "/" + QString(key->GetTitle()), item);
        }
    }
}
