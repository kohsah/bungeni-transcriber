#ifndef PERSONMODEL_HPP
#define PERSONMODEL_HPP

#include <QAbstractTableModel>
#include "person.hpp"

class PersonsModel : public QAbstractTableModel{
    Q_OBJECT
public:
    PersonsModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                             int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                      int role = Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void loadPersonData(QList<Person *>* persons_);
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
private:
    QList<Person *>* persons;
};

#endif // PERSONMODEL_HPP
