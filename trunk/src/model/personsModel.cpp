#include "personsModel.hpp"

PersonsModel::PersonsModel(QObject* parent) : QAbstractTableModel(parent){
    persons = new QList<Person *>();
}

int PersonsModel::rowCount(const QModelIndex &parent) const{
    return persons->size();
}

QVariant PersonsModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid()){
        return QVariant();
    }
    Person *person = persons->at(index.row());
    if (role == Qt::DisplayRole){
        if (index.column() == 0)
            return QVariant(person->getName());
        else if (index.column() == 1)
            return QVariant(person->getId());
        else if (index.column() == 2)
            return QVariant(person->getUri());
    }
    else{
        return QVariant();
    }
}

QVariant PersonsModel::headerData(int section, Qt::Orientation orientation,
                         int role) const{
    QVariant headers[3] = {"Name", "ID", "URI"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headers[section];
    return QVariant();
}

Qt::ItemFlags PersonsModel::flags(const QModelIndex &index) const{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool PersonsModel::setData(const QModelIndex &index, const QVariant &value,
                  int role){
    if (index.isValid()) {
        Person *person = static_cast<Person*>(index.internalPointer());
        if (role == Qt::EditRole && person->setData(index, value))
            return true;
    }
    return false;
}

bool PersonsModel::insertRows(int position, int rows, const QModelIndex &index){
    beginInsertRows(QModelIndex(), position, position+rows-1);
    Person *person;
    for (int row = 0; row < rows; ++row) {
        person = new Person();
        persons->insert(position, person);
    }
    endInsertRows();
    return true;
}

bool PersonsModel::removeRows(int position, int rows, const QModelIndex &index){
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
        delete persons->at(position);
        persons->removeAt(position);
    }
    endRemoveRows();
    return true;
}

int PersonsModel::columnCount(const QModelIndex&) const {
    return 3;
}

QModelIndex PersonsModel::index(int row, int column, const QModelIndex &parent) const{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    return createIndex(row, column, persons->at(row));
}

void PersonsModel::loadPersonData(QList<Person *>* persons_){
    this->beginResetModel();
    persons = persons_;
    this->endResetModel();
}
