#ifndef QTBROWSER_H
#define QTBROWSER_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QStandardItemModel>

#include <TKey.h>
#include <TApplication.h>

#include "leafsortfilterproxymodel.h"
#include "iplugin.h"

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
    void sendSignal(TObject* obj);

private slots:
    void on_applyFilterButton_clicked();
    void on_actionOpen_File_triggered();
    void on_treeView_doubleClicked(const QModelIndex &index);
    void customMenuRequested(QPoint pos);

private:
    void add_root_file_as_tree(QString file_path);
    void visit(TKey* td, QString current_path, QStandardItem* parent);
    void remove_tree_item(QModelIndex idx);
    void preview_item(QModelIndex idx);

    Ui::QTBrowser *ui;
    QStandardItemModel *model;
    LeafSortFilterProxyModel *proxy_model;
    TApplication* rootapp;
};

#endif // QTBROWSER_H
