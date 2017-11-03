#ifndef TFILETREEVIEWER_H
#define TFILETREEVIEWER_H

#include <QWidget>
#include <QStandardItemModel>
#include <TKey.h>

#include "tfiletreeviewer/leafsortfilterproxymodel.h"
#include "tobjectcontainer.h"

namespace Ui {
class TFileTreeViewer;
}

class TFileTreeViewer : public QWidget
{
    Q_OBJECT

public:
    explicit TFileTreeViewer(QWidget *parent = 0);
    ~TFileTreeViewer();

signals:
    void sendTObjectContainer(TObjectContainer& obj);

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_filterLineEdit_returnPressed();
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_clicked();

private:
    void add_root_file_as_tree(QString file_path);
    void visit(TKey* td, QString current_path, QStandardItem* parent, QString file_path);
    void remove_tree_item(QModelIndex idx);
    void previewItem(QModelIndex idx);

    Ui::TFileTreeViewer *ui;
    QStandardItemModel *model;
    LeafSortFilterProxyModel *proxy_model;
};

#endif // TFILETREEVIEWER_H
