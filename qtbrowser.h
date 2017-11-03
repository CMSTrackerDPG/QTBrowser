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

private slots:
    void on_actionSettings_triggered();


    void on_actionSuperimpose_triggered();

    void on_actionConcatinate_triggered();

private:
    void addPlugin(QString name);
    void removeActivePlugin();

    Ui::QTBrowser *ui;
    TApplication* rootapp;
    QWidget* filebrowser_tree;
    QWidget* active_plugin;
};

#endif // QTBROWSER_H
