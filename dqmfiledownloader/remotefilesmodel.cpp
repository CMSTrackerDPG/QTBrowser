#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

#include "remotefilesmodel.h"

RemoteFilesModel::RemoteFilesModel(QObject* parent)
{
    this->parent = parent;
}

void RemoteFilesModel::fillModelFromFile(QString path)
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
        m_remoteFiles.push_back(FileContainer(display_name, path));
    }

}

bool RemoteFilesModel::canFetchMore(const QModelIndex& /*parent*/) const
{
#if QT_VERSION >= 0x050000
    return false;
#else
    if (m_fileCount < m_remoteFiles.size())
        return true;
    else
        return false;
#endif
}

void RemoteFilesModel::fetchMore(const QModelIndex& /*parent*/)
{
#if QT_VERSION >= 0x050000
#else
    int remainder = m_remoteFiles.size() - m_fileCount;
    int items_to_fetch = qMin(2000, remainder);

    beginInsertRows(QModelIndex(), m_fileCount, m_fileCount+items_to_fetch-1);

    m_fileCount += items_to_fetch;

    endInsertRows();

    emit numberPopulated(items_to_fetch);
#endif
}

int RemoteFilesModel::rowCount(const QModelIndex& /*parent*/) const
{
    // Lazy loading of the model
    // when trying to display a large amount of files
    // the application takes WAY too long(1.5k entries ~1minute).
    // This only occurs in qt4 for some reason.

    //TODO:
    // a downside to this is that filtering with a proxy model
    // only filters the currently loaded entries. Which makes a
    // search function in a non fully loaded model very pointless.
    // Solution: *)could be to implement fetchMore in a proxy model
    //           *)write a manual filter that goes over vector comparing
    //             elements and then creating a model from them.
#if QT_VERSION >= 0x050000
    return m_remoteFiles.size();
#else
    return m_fileCount;
#endif
}

QVariant RemoteFilesModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_remoteFiles.size() || index.row() < 0)
        return QVariant();


    int row = index.row();
    if(role == Qt::DisplayRole) {
        return m_remoteFiles[row].getName();
    }
    if(role == Qt::ToolTipRole) {
        return m_remoteFiles[row].getPath();
    }

    return QVariant();
}

QString RemoteFilesModel::getFilepath(const QModelIndex& index) const
{
    int row = index.row();
    return m_remoteFiles[row].getPath();
}
