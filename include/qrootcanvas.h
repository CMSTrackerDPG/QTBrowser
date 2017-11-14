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

    void draw(TH1* plot);
    void draw(std::vector<TH1*> plots);

    void clear();
    void saveAs(std::string filename);

    //TODO: maybe make superimpse/_mutliaxis into one function that has a bool multiaxis param
    void superimpose(std::vector<TH1*> plots, std::string title = "");
    void superimpose_multiaxis(std::vector<TH1*> plots, std::string title = "");

    void concatinatePlots(std::vector<TH1*> plots, std::string title = "");

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
