#include "previewqrootcanvas.h"
#include "ui_previewqrootcanvas.h"

PreviewQRootCanvas::PreviewQRootCanvas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreviewQRootCanvas)
{
    ui->setupUi(this);
}

PreviewQRootCanvas::~PreviewQRootCanvas()
{
    delete ui;
}

void PreviewQRootCanvas::preview(TH1* plot)
{
    ui->widget->draw(*plot);
}
