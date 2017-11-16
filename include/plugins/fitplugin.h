#ifndef FITPLUGIN_H
#define FITPLUGIN_H

#include <QWidget>
#include <TApplication.h>
#include <QListWidgetItem>

#include "iplugin.h"

namespace Ui {
class FitPlugin;
}

class FitPlugin : public QWidget, public IPlugin
{
    Q_OBJECT

public:
    FitPlugin(QWidget *parent = 0);
    ~FitPlugin();
    void fit_plot(TH1* obj);

public slots:
    void receiveTObjectContainer(TObjectContainer& container) override;

private slots:
    void on_comboBox_fitOpt_currentIndexChanged(int index);
    void on_lineEdit_to_textChanged(const QString &arg1);
    void on_lineEdit_from_textChanged(const QString &arg1);
    void on_button_settitle_clicked();
    void on_button_saveas_clicked();

private:
    void fitCheckedItems();
    Ui::FitPlugin *ui;
    TH1* current_obj {nullptr};
    QString plot_title = "";
};

#endif // FITPLUGIN_H
