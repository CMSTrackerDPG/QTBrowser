#ifndef QTBROWSER_H
#define QTBROWSER_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QStandardItemModel>

#include <TKey.h>
#include <TApplication.h>

#include "plugins/iplugin.h"

namespace Ui {
class QTBrowser;
}

class QTBrowser : public QMainWindow
{
    Q_OBJECT

public:
    explicit QTBrowser(QWidget *parent = 0);
    ~QTBrowser();

signals:

private slots:
    void on_actionSettings_triggered();

private:
    Ui::QTBrowser *ui;
    TApplication* rootapp;
    QWidget* active_plugin;
    QWidget* treeview;
};

#endif // QTBROWSER_H
