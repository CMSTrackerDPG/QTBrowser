#ifndef REMOTEFILESMODEL_H
#define REMOTEFILESMODEL_H

#include <QAbstractListModel>
#include <vector>
#include "container/filecontainer.h"


class RemoteFilesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    RemoteFilesModel(QObject *parent);
    QString getFilepath(const QModelIndex& index) const;

// QAbstractItemModel interface
public:
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void fillModelFromFile(QString path);

signals:
    void numberPopulated(int number);


protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

private:
    std::vector<FileContainer> m_remoteFiles;
    QObject* parent;
    int m_fileCount = 0;
};

#endif // REMOTEFILESMODEL_H
