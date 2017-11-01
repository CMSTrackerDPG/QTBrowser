#ifndef SUPERIMPOSEPLUGIN_H
#define SUPERIMPOSEPLUGIN_H

#include <QWidget>
#include <TApplication.h>
#include <QListWidgetItem>

#include "iplugin.h"

namespace Ui {
class SuperimposePlugin;
}

class SuperimposePlugin : public QWidget, public IPlugin
{
    Q_OBJECT

public:
    SuperimposePlugin(QWidget *parent = 0);
    ~SuperimposePlugin();

public slots:
    void receiveTObj(TObjectContainer& container) override;

private slots:
    void customMenuRequested(QPoint pos);
    void on_listWidget_itemChanged(QListWidgetItem *item);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_checkBox_clicked();

private:
    void superimposeCheckedItems();
    void removeSelectedFromList();

    Ui::SuperimposePlugin *ui;

    QString plot_title = "";
    bool is_multiaxis_enable = false;
};


#endif // SUPERIMPOSEPLUGIN_H
