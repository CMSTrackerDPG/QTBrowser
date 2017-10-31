#include "concatinateplugin.h"
#include "ui_concatinateplugin.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMenu>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>

#include <TDirectoryFile.h>


ConcatinatePlugin::ConcatinatePlugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConcatinatePlugin)
{
    ui->setupUi(this);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this,           SLOT(customMenuRequested(QPoint)));
}

ConcatinatePlugin::~ConcatinatePlugin()
{
    delete ui;
}

void ConcatinatePlugin::receiveTObj(TObjectContainer& container)
{
    TObject* obj = container.getObject();
    bool is_directory = (obj->IsA() == TDirectoryFile::Class());

    if(!is_directory) {
        TObjectContainer* object = new TObjectContainer(obj, container.getParentFileName());
        QListWidgetItem* item = new QListWidgetItem(obj->GetTitle(), ui->listWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(object));  // this defaults to Qt::UserRole + 1
        item->setToolTip(container.getParentFileName());

        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked); // initialize check state
        ui->listWidget->addItem(item);
        concatinateCheckedItems();
    }
}

void ConcatinatePlugin::concatinateCheckedItems() {
    ui->widget->clear();

    std::vector<TH1*> plots;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        TObjectContainer* o = ui->listWidget->item(i)->data(Qt::UserRole).value<TObjectContainer*>();

        auto item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked) {
            plots.push_back((TH1*)o->getObject()->Clone());
        }
    }

    ui->widget->concatinatePlots(plots, "title");
}

void ConcatinatePlugin::removeSelectedFromList()
{
    for(auto& e : ui->listWidget->selectedItems()) {
        delete e;
    }
    concatinateCheckedItems();
}

void ConcatinatePlugin::customMenuRequested(QPoint pos){
//    QModelIndex index= ui->listWidget->indexAt(pos);
    QMenu* menu = new QMenu(this);
    QAction* remove_selection_action  = new QAction("Remove Selected Items", this);
    menu->addAction(remove_selection_action);
    menu->popup(ui->listWidget->viewport()->mapToGlobal(pos));
    connect(remove_selection_action, &QAction::triggered, [this]() { removeSelectedFromList(); });
}


void ConcatinatePlugin::on_pushButton_clicked()
{
    //up button
    int currentRow = ui->listWidget->currentRow();
    if (currentRow == 0) return;
    QListWidgetItem * currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow - 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow - 1);
    concatinateCheckedItems();
}

void ConcatinatePlugin::on_pushButton_2_clicked()
{
    //down button
    int currentRow = ui->listWidget->currentRow();
    if (currentRow > ui->listWidget->count()-1) return;
    QListWidgetItem * currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow + 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow + 1);
    concatinateCheckedItems();
}

void ConcatinatePlugin::on_listWidget_itemChanged(QListWidgetItem *item)
{
    concatinateCheckedItems();
}

void ConcatinatePlugin::on_pushButton_3_clicked()
{
    //set title
    plot_title = QInputDialog::getText(this,"Concatinate","Enter plot title");
    concatinateCheckedItems();
}

void ConcatinatePlugin::on_pushButton_4_clicked()
{
    //save as
    auto filename = QFileDialog::getSaveFileName(this, "stuff", "/home/");
    ui->widget->saveAs(filename.toStdString());
}