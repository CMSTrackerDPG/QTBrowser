#include "superimposeplugin.h"
#include "ui_superimposeplugin.h"

#include <QDebug>
#include <QListWidgetItem>
#include <TDirectoryFile.h>
#include "tobjectcontainer.h"

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

void SuperimposePlugin::receiveTObj(TObject* obj)
{
    bool is_directory = (obj->IsA() == TDirectoryFile::Class());

    if(!is_directory) {
        TObjectContainer* object = new TObjectContainer(obj);
        QListWidgetItem* item = new QListWidgetItem(obj->GetTitle(), ui->listWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(object));  // this defaults to Qt::UserRole + 1

        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked); // initialize check state
        ui->listWidget->addItem(item);
    }
    superimpose_checked();
}

void SuperimposePlugin::superimpose_checked()
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
    ui->widget->superimpose(plots);
}

void SuperimposePlugin::on_listWidget_itemChanged(QListWidgetItem* item)
{
    superimpose_checked();
}

void SuperimposePlugin::on_pushButton_clicked()
{
    qDebug() << "Button pressed";
    qDeleteAll(ui->listWidget->selectedItems());
    superimpose_checked();
}
