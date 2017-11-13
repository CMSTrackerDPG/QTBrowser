#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

#include "remotefilesmodel.h"

RemoteFilesModel::RemoteFilesModel(QObject* parent)
{
    this->parent = parent;
}

void RemoteFilesModel::fill_model_from_file(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    in.setCodec("UTF-8");

    // turn a string like:
    // https://cmsweb.cern.ch/dqm/online/data/browse/Original/00030xxxx/0003039xx/DQM_V0001_Scal_R000303995.root
    // into DQM_V0001_Scal_R000303995.root, and fill the model with that name + urlpath
    while(!in.atEnd()) {
        QString path = in.readLine();
        auto tmp = path.lastIndexOf("/");
        QString display_name = path.mid(tmp+1);
        remote_files.push_back(FileContainer(display_name, path));
    }

}

int RemoteFilesModel::rowCount(const QModelIndex& parent) const
{
    return remote_files.size();
}

QVariant RemoteFilesModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(role == Qt::DisplayRole) {
        return remote_files[row].getName();
    }
    if(role == Qt::ToolTipRole) {
        return remote_files[row].getPath();
    }

    return QVariant();
}

QString RemoteFilesModel::getFilepath(const QModelIndex& index) const
{
    int row = index.row();
    return remote_files[row].getPath();
}
