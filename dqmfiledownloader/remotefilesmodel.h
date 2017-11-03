#ifndef REMOTEFILESMODEL_H
#define REMOTEFILESMODEL_H

#include <QAbstractListModel>

#include "filecontainer.h"

class RemoteFilesModel : public QAbstractListModel
{
public:
    RemoteFilesModel(QObject *parent);
    QString getFilepath(const QModelIndex& index) const;

// QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void fill_model_from_file(QString path);

private:
    std::vector<FileContainer> remote_files;
    QObject *parent;
};

#endif // REMOTEFILESMODEL_H
