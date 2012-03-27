#include "person.hpp"

Person::Person(QString name_, QString id_, QString uri_){
    name = name_;
    id = id_;
    uri = uri_;
}

void Person::setName(QString name_){
    name = name_;
}

void Person::setUri(QString uri_){
    uri = uri_;
}

void Person::setId(QString id_){
    id = id_;
}

QString Person::getUri(){
    return uri;
}

QString Person::getId(){
    return id;
}

QString Person::getName(){
    return name;
}

bool Person::setData(const QModelIndex &index, const QVariant &value){
    if(index.column() == 0){
        this->setName(value.toString());
        return true;
    }
    else if (index.column() == 1){
        this->setUri(value.toString());
        return true;
    }
    else if (index.column() == 2) {
        this->setId(value.toString());
        return true;
    }
    else{
        return false;
    }
}
