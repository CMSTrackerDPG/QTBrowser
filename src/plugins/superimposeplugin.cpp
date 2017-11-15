#include "plugins/superimposeplugin.h"
#include "ui_superimposeplugin.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMenu>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>

#include <TDirectoryFile.h>
#include "container/tobjectcontainer.h"

SuperimposePlugin::SuperimposePlugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SuperimposePlugin)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
    ui->preview_canvas->clear();

    std::vector<TH1*> plots;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        TObjectContainer* o = ui->listWidget->item(i)->data(Qt::UserRole).value<TObjectContainer*>();

        auto item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked) {
            plots.push_back((TH1*)o->getObject()->Clone());
        }
    }

    ui->preview_canvas->draw(plots);
}

void SuperimposePlugin::superimposeCheckedItems()
{
    ui->superimpose_canvas->clear();

    std::vector<TH1*> plots;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        TObjectContainer* o = ui->listWidget->item(i)->data(Qt::UserRole).value<TObjectContainer*>();

        auto item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked) {
            plots.push_back((TH1*)o->getObject()->Clone());
        }
    }

    if(!plots.size()) return;

    ui->superimpose_canvas->superimpose(plots, std::string(plot_title.toUtf8().constData()), is_multiaxis_enable);
}

void SuperimposePlugin::on_listWidget_itemChanged(QListWidgetItem* /*item*/)
{
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
    auto filename = QFileDialog::getSaveFileName(this, "Save Dialog", "/home/");
    ui->superimpose_canvas->saveAs(std::string(filename.toUtf8().constData()));
}

void SuperimposePlugin::on_checkBox_clicked()
{
    is_multiaxis_enable = (ui->checkBox->checkState() == Qt::Checked);
    superimposeCheckedItems();
}

void SuperimposePlugin::on_pushButton_3_clicked()
{
    for(auto& e : ui->listWidget->selectedItems()) {
        delete e;
    }
    displayCheckedInPreview();
    superimposeCheckedItems();
}
