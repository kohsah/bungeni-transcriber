#ifndef PERSONMODEL_HPP
#define PERSONMODEL_HPP

#include <QStandardItemModel>

class PersonsModel : public QStandardItemModel {
    Q_OBJECT
public:
    PersonsModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void insertItem(QModelIndex &parent, PlaylistItem *item);
};

#endif // PERSONMODEL_HPP
