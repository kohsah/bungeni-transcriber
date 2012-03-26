#include "person.hpp"

Person::Person(QString name_=QString(), QString id_=QString(), QString uri_=QString()){
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

void Person::setFirstName(QString firstName_){
    firstName = firstName_;
}

QString Person::getFirstName(){
    return name;
}

QString Person::getUri(){
    return uri;
}

QString Person::getId(){
    return id;
}

bool Person::setData(const QModelIndex &index, const QVariant &value){
    if(index.column() == 0){
        this->setName(value);
        return true;
    }
    else if (index.column() == 1){
        this->setUri(value);
        return true;
    }
    else if (index.column() == 1) {
        this->setUri(value);
        return true;
    }
    else{
        return false;
    }
}
