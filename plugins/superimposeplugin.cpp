#include "superimposeplugin.h"
#include "ui_superimposeplugin.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMenu>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>

#include <TDirectoryFile.h>
#include "tobjectcontainer.h"

SuperimposePlugin::SuperimposePlugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SuperimposePlugin)
{
    ui->setupUi(this);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this,           SLOT(customMenuRequested(QPoint)));
}

SuperimposePlugin::~SuperimposePlugin()
{
    delete ui;
}

void SuperimposePlugin::receiveTObjectContainer(TObjectContainer& container)
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
        displayCheckedInPreview();
        superimposeCheckedItems();
    }
}

void SuperimposePlugin::displayCheckedInPreview()
{
    ui->widget->clear();

    std::vector<TH1*> plots;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        TObjectContainer* o = ui->listWidget->item(i)->data(Qt::UserRole).value<TObjectContainer*>();

        auto item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked) {
            plots.push_back((TH1*)o->getObject()->Clone());
        }
    }

    ui->widget_2->draw(plots);
}

void SuperimposePlugin::superimposeCheckedItems()
{
    ui->widget->clear();

    std::vector<TH1*> plots;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        TObjectContainer* o = ui->listWidget->item(i)->data(Qt::UserRole).value<TObjectContainer*>();

        auto item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked) {
            plots.push_back((TH1*)o->getObject()->Clone());
        }
    }

    if(!plots.size()) return;

    if(is_multiaxis_enable)
        ui->widget->superimpose_multiaxis(plots, plot_title.toStdString());
    else
        ui->widget->superimpose(plots, plot_title.toStdString());
}

void SuperimposePlugin::customMenuRequested(QPoint pos){
//    QModelIndex index= ui->listWidget->indexAt(pos);
    QMenu* menu = new QMenu(this);
    QAction* remove_selection_action  = new QAction("Remove Selected Items", this);
    menu->addAction(remove_selection_action);
    menu->popup(ui->listWidget->viewport()->mapToGlobal(pos));
//    connect(remove_selection_action, &QAction::triggered, [this]() { removeSelectedFromList(); });
}


void SuperimposePlugin::on_listWidget_itemChanged(QListWidgetItem* /*item*/)
{
    displayCheckedInPreview();
    superimposeCheckedItems();
}

void SuperimposePlugin::removeSelectedFromList()
{
    for(auto& e : ui->listWidget->selectedItems()) {
        delete e;
    }
    displayCheckedInPreview();
    superimposeCheckedItems();
}

void SuperimposePlugin::on_pushButton_clicked()
{
    plot_title = QInputDialog::getText(this,"Superimpose","Enter plot title");
    superimposeCheckedItems();
}

void SuperimposePlugin::on_pushButton_2_clicked()
{
    auto filename = QFileDialog::getSaveFileName(this, "stuff", "/home/");
    ui->widget->saveAs(filename.toStdString());
}

void SuperimposePlugin::on_checkBox_clicked()
{
    is_multiaxis_enable = (ui->checkBox->checkState() == Qt::Checked);
    superimposeCheckedItems();
}
