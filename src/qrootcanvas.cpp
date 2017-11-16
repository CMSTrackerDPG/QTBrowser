#include "qrootcanvas.h"
#include <memory>

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
#include <TGaxis.h>

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

void QRootCanvas::draw(TH1* plot)
{
    canvas->cd();
    plot->Draw();
}

void QRootCanvas::draw(std::vector<TH1*> plots)
{
    canvas->Clear();
    canvas->Divide(plots.size(), 1);
    std::vector<TH1*> copies;
    for(auto& elem : plots){
        copies.push_back((TH1*)elem->Clone());
    }

    int i = 1;
    for(auto& elem : copies) {
        canvas->cd(i++);
        elem->Draw();
    }
    canvas->Update();
}

void QRootCanvas::clear()
{
    canvas->cd();
    canvas->Clear();
}


void QRootCanvas::superimpose(std::vector<TH1*>& plots, std::string title, bool is_multiaxis)
{
    if(is_multiaxis)
        superimpose_with_multiaxis(plots, title);
    else
        superimpose_without_multiaxis(plots, title);
}

void QRootCanvas::superimpose_without_multiaxis(std::vector<TH1*>& plots, std::string title)
{
    canvas->cd();
    canvas->Clear();

    std::vector<Int_t> basic_colors = { kBlue+2, kRed, kCyan+1, kMagenta, kGreen};
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

void QRootCanvas::superimpose_with_multiaxis(std::vector<TH1*>& plots, std::string title)
{
    canvas->cd();

    std::vector<Int_t> basic_colors = { kRed, kCyan+1, kMagenta, kGreen, kBlue };
    std::vector<Int_t> colors;
    for(auto c : basic_colors) colors.push_back(c);
    for(auto c : basic_colors) colors.push_back(c+2);
    for(auto c : basic_colors) colors.push_back(c-7);
    for(auto c : basic_colors) colors.push_back(c-4);
    for(auto c : basic_colors) colors.push_back(c-9);
    auto legend = new TLegend(0.65,0.8,0.85,0.9);


    // How all of this works:
    // We find the plot that has the largest X value on the x axis, so that all the
    // other plots essentially fit into that one. that plot its plots[largest_plot_idx]
    int maxx = 0, largest_plot_idx = 0;

    for(int i=0; i<plots.size(); i++) {
        if(plots[i]->GetXaxis()->GetXmax() > maxx) {
            maxx = plots[i]->GetXaxis()->GetXmax();
            largest_plot_idx = i;
        }
    }
    plots[largest_plot_idx]->SetStats(false);
    legend->AddEntry(plots[largest_plot_idx], plots[largest_plot_idx]->GetTitle());

    // Draw the Plot, disable all the axis labels
    // Then we draw that plot before anything else, and remove it from
    // @arg plots, so that we can now iterate over @arg plots
    // and superimpose on top of the largest one
    plots[largest_plot_idx]->SetTitle(title.c_str());
    plots[largest_plot_idx]->GetXaxis()->SetTitle("");
    plots[largest_plot_idx]->GetYaxis()->SetTitle("");
    plots[largest_plot_idx]->Draw();
    legend->Draw();

    canvas->Update();
    plots.erase(plots.begin() + largest_plot_idx);

    float scale;
    int offset=0;
    int ctr=0;
    for(auto& plot : plots) {
        auto curr_color = colors[ctr];

        plot->SetStats(false);
        scale = gPad->GetUymax()/plot->GetMaximum();
        plot->SetLineColor(curr_color);


        // YAxis stuff
        //offset the axis after the second plot by 5% of the maximum of Yaxis
        offset = (float(gPad->GetUxmax())*0.05)*ctr;

        TGaxis* Yaxis = new TGaxis(gPad->GetUxmax()-offset, gPad->GetUymin(),
                                   gPad->GetUxmax()-offset, gPad->GetUymax(),
                                   0, plot->GetMaximum(),
                                   505,"+L");

        Yaxis->SetLineColor(curr_color);
        Yaxis->SetLabelColor(curr_color);
        Yaxis->Draw();

        // XAxis stuff
        offset = (float(gPad->GetUymax())*0.05)*ctr;
        TGaxis* Xaxis = new TGaxis(gPad->GetUxmin(), gPad->GetUymax() - offset,
                                   gPad->GetUxmax(), gPad->GetUymax() - offset,
                                   plot->GetXaxis()->GetXmin(), plot->GetXaxis()->GetXmax(),
                                   505,"");

        Xaxis->SetLineColor(curr_color);
        Xaxis->SetLabelColor(curr_color);
        Xaxis->Draw();

        plot->Scale(scale); // Y rescale
        plot->SetBins(plot->GetNbinsX(), 0, maxx); // X rescale

        // Histogram stuff
        plot->Draw("][ same hist");

        // Legend stuff
        legend->AddEntry(plot, plot->GetTitle());
        legend->Draw();

        ctr++;
    }
}

void QRootCanvas::concatinatePlots(std::vector<TH1*>& plots, std::string title)
{
    int total_bins = 0;
    int total_range = 0;
    for(auto& plot : plots) {
        total_range += int(plot->GetXaxis()->GetXmax()) * (plot->FindLastBinAbove(0.1) / float(plot->GetNbinsX() - 2));
        total_bins  += plot->FindLastBinAbove(0.);
    }

    auto merged(TH1F("",title.c_str(), total_bins, 0, total_range));

    int offset = 0;
    for(auto& plot : plots) {
        int last_idx = plot->FindLastBinAbove(0.);

        for(int bin = 0; bin<last_idx; bin++) {
            merged.SetBinContent(bin+offset, plot->GetBinContent(bin));
        }
        offset += last_idx;
    }
    canvas->cd();
    merged.Clone()->Draw();
}

void QRootCanvas::fit(TH1* plot, std::string options)
{
    canvas->Clear();
    plot->Fit(options.c_str());
}

void QRootCanvas::fit(TH1* plot, std::string options, double xmin, double xmax)
{
    canvas->Clear();
    plot->Fit(options.c_str(), "", "", xmin, xmax);
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
