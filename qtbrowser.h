#ifndef QTBROWSER_H
#define QTBROWSER_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QStandardItemModel>

#include <TKey.h>
#include <TApplication.h>

#include "leafsortfilterproxymodel.h"
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
    void sendSignal(TObjectContainer& obj);

private slots:
    void on_actionOpen_File_triggered();
    void on_treeView_doubleClicked(const QModelIndex &index);
    void customMenuRequested(QPoint pos);
    void on_actionSettings_triggered();
    void on_actionDownload_Remote_Files_triggered();
    void on_filterLineEdit_returnPressed();

private:
    void add_root_file_as_tree(QString file_path);
    void visit(TKey* td, QString current_path, QStandardItem* parent, QString file_path);
    void remove_tree_item(QModelIndex idx);
    void previewItem(QModelIndex idx);

    Ui::QTBrowser *ui;
    QStandardItemModel *model;
    LeafSortFilterProxyModel *proxy_model;
    TApplication* rootapp;
    QWidget* active_plugin;
};

#endif // QTBROWSER_H
