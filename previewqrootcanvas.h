#ifndef PREVIEWQROOTCANVAS_H
#define PREVIEWQROOTCANVAS_H

#include <QDialog>
#include <TH1.h>

namespace Ui {
class PreviewQRootCanvas;
}

class PreviewQRootCanvas : public QDialog
{
    Q_OBJECT

public:
    explicit PreviewQRootCanvas(QWidget *parent = 0);
    ~PreviewQRootCanvas();
    void preview(TH1* plot);

private:
    Ui::PreviewQRootCanvas *ui;
};

#endif // PREVIEWQROOTCANVAS_H
