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

    void superimpose(std::vector<TH1*>& plots, std::string title = "", bool is_multiaxis=false);
    void concatinatePlots(std::vector<TH1*>& plots, std::string title = "");

    void fit(TH1* plot, std::string options);
    void fit(TH1* plot, std::string options, double xmin, double xmax);


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

private:
    void superimpose_with_multiaxis(std::vector<TH1*>& plots, std::string title = "");
    void superimpose_without_multiaxis(std::vector<TH1*>& plots, std::string title);
};


#endif // QROOTCANVAS_H
