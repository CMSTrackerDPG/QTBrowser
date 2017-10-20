#include "qrootcanvas.h"

#include <QTimer>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QShowEvent>

#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TFrame.h>
#include <TTimer.h>
#include <QLayout>
#include <QPushButton>
#include <QDebug>


QRootCanvas::QRootCanvas(QWidget *parent) :
    QWidget(parent, 0), canvas(0)
{
    setMouseTracking(kTRUE);
    int wid = gVirtualX->AddWindow((ULong_t)winId(), width(), height());
    canvas = new TCanvas("Root Canvas", width(), height(), wid);

    root_timer = new QTimer(this);
    QObject::connect(root_timer, SIGNAL(timeout()), this, SLOT(handle_root_events()) );
    root_timer->start( 20 );
}

TCanvas* QRootCanvas::getCanvas()
{
    return canvas;
}

void QRootCanvas::draw(TH1& plot)
{
    canvas->cd();
    plot.Draw();
}

void QRootCanvas::mouseMoveEvent(QMouseEvent *e)
{
    if (canvas) {
        if (e->buttons() & Qt::LeftButton) {
            canvas->HandleInput(kButton1Motion, e->x(), e->y());
        } else if (e->buttons() & Qt::MidButton) {
            canvas->HandleInput(kButton2Motion, e->x(), e->y());
        } else if (e->buttons() & Qt::RightButton) {
            canvas->HandleInput(kButton3Motion, e->x(), e->y());
        } else {
            canvas->HandleInput(kMouseMotion, e->x(), e->y());
        }
    }
}

void QRootCanvas::mousePressEvent( QMouseEvent *e )
{
    if (canvas) {
        switch (e->button()) {
        case Qt::LeftButton :
            canvas->HandleInput(kButton1Down, e->x(), e->y());
            break;
        case Qt::MidButton :
            canvas->HandleInput(kButton2Down, e->x(), e->y());
            break;
        case Qt::RightButton :
            // does not work properly on Linux...
            break;
        default:
            break;
        }
    }
}

void QRootCanvas::mouseReleaseEvent( QMouseEvent *e )
{
    if (canvas) {
        switch (e->button()) {
        case Qt::LeftButton :
            canvas->HandleInput(kButton1Up, e->x(), e->y());
            break;
        case Qt::MidButton :
            canvas->HandleInput(kButton2Up, e->x(), e->y());
            break;
        case Qt::RightButton :
            // does not work properly on Linux...
            break;
        default:
            break;
        }
    }
}

void QRootCanvas::resizeEvent( QResizeEvent * )
{
    if (canvas) {
        canvas->Resize();
        canvas->Modified();
    }
}

void QRootCanvas::paintEvent( QPaintEvent * )
{
    if (canvas) {
        canvas->Resize();
        canvas->Modified();
    }
}

void QRootCanvas::handle_root_events()
{
    gSystem->ProcessEvents();
    canvas->Update();
}
