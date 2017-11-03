#ifndef LEAFSORTFILTERPROXYMODEL_H
#define LEAFSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class LeafSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    LeafSortFilterProxyModel() : QSortFilterProxyModel() {
    }

    bool filterAcceptsRow(int row, const QModelIndex &parent) const override {
        QModelIndex index = sourceModel()->index(row, 0, parent);

        if (!index.isValid())
            return false;

        if (index.data().toString().contains(filterRegExp()))
            return true;

        int rows = sourceModel()->rowCount(index);
        for (row = 0; row < rows; row++)
            if (filterAcceptsRow(row, index))
                return true;

        return false;
    }
};

#endif // LEAFSORTFILTERPROXYMODEL_H
