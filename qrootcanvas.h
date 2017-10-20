#ifndef QROOTCANVAS_H
#define QROOTCANVAS_H

#include <QWidget>
#include <TApplication.h>
#include <TH1.h>

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QPushButton;
class QTimer;
class TCanvas;

class QRootCanvas : public QWidget
{
    Q_OBJECT

public:
    QRootCanvas( QWidget *parent = 0);
    virtual ~QRootCanvas() {}
    TCanvas* getCanvas(); // TODO: make private
    void draw(TH1& plot);

public slots:
    void handle_root_events();

protected:
    virtual void    mouseMoveEvent( QMouseEvent *e );
    virtual void    mousePressEvent( QMouseEvent *e );
    virtual void    mouseReleaseEvent( QMouseEvent *e );
    virtual void    paintEvent( QPaintEvent *e );
    virtual void    resizeEvent( QResizeEvent *e );

    TCanvas* canvas;
    QTimer*  root_timer;
};


#endif // QROOTCANVAS_H
