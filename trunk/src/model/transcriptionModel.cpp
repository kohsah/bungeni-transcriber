#include "transcriptionModel.hpp"

TranscriptionModel::TranscriptionModel(QObject *parent) :
    QAbstractListModel(parent) {
}

QVariant TranscriptionModel::data(const QModelIndex &index, int role) const{
    return QVariant::fromValue(new TranscriptionItemWrapper(items.value(index.row())));
}

Qt::ItemFlags TranscriptionModel::flags(const QModelIndex &index) const{
    if (index.isValid())
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
    return Qt::ItemIsEditable;
}

bool TranscriptionModel::removeRows(int row, int count, const QModelIndex &parent){
    if (row >= items.size() || row + count <= 0)
        return false;

    int beginRow = qMax(0, row);
    int endRow = qMin(row + count - 1, items.size() - 1);

    beginRemoveRows(parent, beginRow, endRow);

    while (beginRow <= endRow) {
        items.removeAt(beginRow);
        ++beginRow;
    }

    endRemoveRows();
    return true;
}

bool TranscriptionModel::insertItem (int row, TranscriptionItem * item){
    int size = items.size();
    beginInsertRows(QModelIndex(), size, size);
    items.insert(size, item);
    endInsertRows();
    return true;
}

bool TranscriptionModel::setData(const QModelIndex & index,
                                 TranscriptionItem &item, int role = Qt::EditRole){
    items.replace(index.row(), &item);
    emit dataChanged(index, index);
    return true;
}

int TranscriptionModel::rowCount(const QModelIndex &parent) const
{
    return items.size();
}
