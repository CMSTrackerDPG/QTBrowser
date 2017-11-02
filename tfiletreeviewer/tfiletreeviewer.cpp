#include "tfiletreeviewer.h"
#include "ui_tfiletreeviewer.h"

TFileTreeViewer::TFileTreeViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TFileTreeViewer)
{
    ui->setupUi(this);
}

TFileTreeViewer::~TFileTreeViewer()
{
    delete ui;
}
