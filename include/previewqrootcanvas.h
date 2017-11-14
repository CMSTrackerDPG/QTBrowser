#ifndef PREVIEWQROOTCANVAS_H
#define PREVIEWQROOTCANVAS_H

#include <QDialog>
#include <TH1.h>

#include "ui_previewqrootcanvas.h"

namespace Ui {
class PreviewQRootCanvas;
}

class PreviewQRootCanvas : public QDialog
{
    Q_OBJECT

public:
    explicit PreviewQRootCanvas(QWidget *parent = 0) : QDialog(parent), ui(new Ui::PreviewQRootCanvas){
        ui->setupUi(this);
    }

    ~PreviewQRootCanvas() {
        delete ui;
    }

    void preview(TH1* plot) {
        ui->widget->draw(plot);
    }

private:
    Ui::PreviewQRootCanvas *ui;
};

#endif // PREVIEWQROOTCANVAS_H
