#include "plugins/previewplugin.h"
#include "ui_previewplugin.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMenu>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>
#include <QCompleter>

#include <TDirectoryFile.h>
#include "container/tobjectcontainer.h"

PreviewPlugin::PreviewPlugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPlugin)
{
    ui->setupUi(this);
}

PreviewPlugin::~PreviewPlugin()
{
    delete ui;
}

void PreviewPlugin::receiveTObjectContainer(TObjectContainer& container)
{
    TObject* obj = container.getObject();
    bool is_directory = (obj->IsA() == TDirectoryFile::Class());

    if(current_obj) {
        delete current_obj;
    }

    if(!is_directory) {
        current_obj = (TH1*) obj->Clone();
        ui->widget->draw(current_obj);
    }
}
