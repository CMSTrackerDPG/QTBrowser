#include "plugins/fitplugin.h"
#include "ui_fitplugin.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMenu>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>
#include <QCompleter>

#include <TDirectoryFile.h>
#include "container/tobjectcontainer.h"

FitPlugin::FitPlugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FitPlugin)
{
    ui->setupUi(this);
    ui->comboBox_fitOpt->setEditable(true);
    ui->comboBox_fitOpt->completer()->setCompletionMode(QCompleter::PopupCompletion);

    ui->lineEdit_from->setValidator(new QDoubleValidator(0, 100, 2, this));
    ui->lineEdit_to->setValidator(new QDoubleValidator(0, 100, 2, this));
}

FitPlugin::~FitPlugin()
{
    delete ui;
}

void FitPlugin::fit_plot(TH1* obj)
{
    std::string option = ui->comboBox_fitOpt->currentText().toUtf8().constData();

    double xmin = 0;
    double xmax = 0;

    QString mintxt = ui->lineEdit_from->text();
    if(mintxt!="") xmin = mintxt.toDouble();

    QString maxtxt = ui->lineEdit_to->text();
    if(maxtxt!="") xmax = maxtxt.toDouble();

    if(plot_title!="")
        obj->SetTitle(plot_title.toUtf8().constData());

    if(mintxt=="" || maxtxt=="") {
        ui->widget->fit(obj, option);
    } else {
        ui->widget->fit(obj, option, xmin, xmax);
    }
}

void FitPlugin::receiveTObjectContainer(TObjectContainer& container)
{
    TObject* obj = container.getObject();
    bool is_directory = (obj->IsA() == TDirectoryFile::Class());

    if(current_obj) {
        delete current_obj;
    }

    if(!is_directory) {
        current_obj = (TH1*) obj->Clone();
        // set the 2 text options to "" so that
        // we dont fit on a new plot with old ranges
        // because if the new plot does not have values
        // in the spec'd ranges, it just shows a white canvas
        // which might make people thing that it doesnt work.
        ui->lineEdit_from->setText("");
        ui->lineEdit_to->setText("");
        plot_title="";
        fit_plot(current_obj);
    }
}

void FitPlugin::on_comboBox_fitOpt_currentIndexChanged(int /*index*/)
{
    fit_plot(current_obj);
}

void FitPlugin::on_lineEdit_to_textChanged(const QString& /*arg1*/)
{
    fit_plot(current_obj);
}

void FitPlugin::on_lineEdit_from_textChanged(const QString& /*arg1*/)
{
    fit_plot(current_obj);
}

void FitPlugin::on_button_settitle_clicked()
{
    plot_title = QInputDialog::getText(this,"Superimpose","Enter plot title");
    fit_plot(current_obj);
}

void FitPlugin::on_button_saveas_clicked()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save Dialog", "/home/");
    ui->widget->saveAs(std::string(filename.toUtf8().constData()));
}
