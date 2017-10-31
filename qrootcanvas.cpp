#include "qrootcanvas.h"

#include <QTimer>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QShowEvent>
#include <QLayout>
#include <QPushButton>
#include <QDebug>

#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TFrame.h>
#include <TTimer.h>
#include <TLegend.h>
#include <THStack.h>

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

void QRootCanvas::draw(TH1* plot)
{
    canvas->cd();
    plot->Draw();
}

void QRootCanvas::clear()
{
    canvas->cd();
    canvas->Clear();
}

void QRootCanvas::superimpose(std::vector<TH1*> plots, std::string title)
{
    canvas->cd();
    canvas->Clear();

    std::vector<Int_t> basic_colors = { kBlue, kGreen, kCyan, kMagenta, kRed };
    std::vector<Int_t> colors;
    for(auto c : basic_colors) colors.push_back(c);
    for(auto c : basic_colors) colors.push_back(c+2);
    for(auto c : basic_colors) colors.push_back(c-7);
    for(auto c : basic_colors) colors.push_back(c-4);
    for(auto c : basic_colors) colors.push_back(c-9);
    auto legend = new TLegend(0.65,0.8,0.85,0.9);

    THStack* hs = new THStack("", title.c_str());

    canvas->cd();
    int idx = 0;
    for(auto& elem : plots) {
        elem->SetLineColor(colors[idx]);
        legend->AddEntry(elem, elem->GetTitle());
        hs->Add(elem);
        idx++;
    }

    hs->Draw("nostack");
    legend->Draw();
    canvas->Update();
}

void QRootCanvas::concatinatePlots(std::vector<TH1*> plots, std::string title)
{
    int total_bins = 0;
    int total_range = 0;
    for(auto& plot : plots) {
        total_range += int(plot->GetXaxis()->GetXmax()) * (plot->FindLastBinAbove(0.1) / float(plot->GetNbinsX() - 2));
        total_bins  += plot->FindLastBinAbove(0.);
    }

    // FIXME: producing memory leak here
    auto* merged = new TH1F("",title.c_str(), total_bins, 0, total_range);

    int offset = 0;
    for(auto& plot : plots) {
        int last_idx = plot->FindLastBinAbove(0.);

        for(int bin = 0; bin<last_idx; bin++) {
            merged->SetBinContent(bin+offset, plot->GetBinContent(bin));
        }

        offset += last_idx;
    }
    canvas->cd();
    merged->Draw();
}

void QRootCanvas::saveAs(std::string filename)
{
    canvas->cd();
    canvas->SaveAs(filename.c_str());
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
