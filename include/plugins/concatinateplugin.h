#ifndef APPENDHISTOGRAMSPLUGIN_H
#define APPENDHISTOGRAMSPLUGIN_H

#include <QWidget>
#include <QListWidgetItem>

#include "iplugin.h"

namespace Ui {
class ConcatinatePlugin;
}

class ConcatinatePlugin : public QWidget, public IPlugin
{
    Q_OBJECT

public:
    explicit ConcatinatePlugin(QWidget *parent = 0);
    ~ConcatinatePlugin();

    // IPlugin interface
public slots:
    void receiveTObjectContainer(TObjectContainer& container) override;

private slots:
    void on_pushButton_clicked();
    void on_listWidget_itemChanged(QListWidgetItem *item);
    //TODO: rename
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

private:
    void concatinateCheckedItems();

    Ui::ConcatinatePlugin *ui;

    QString plot_title = "";
};

#endif // APPENDHISTOGRAMSPLUGIN_H
