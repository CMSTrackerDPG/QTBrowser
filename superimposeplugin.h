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
    void receiveTObj(TObject* obj) override;

private slots:
    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_pushButton_clicked();

private:
    void superimpose_checked();

    Ui::SuperimposePlugin *ui;
};


#endif // SUPERIMPOSEPLUGIN_H
