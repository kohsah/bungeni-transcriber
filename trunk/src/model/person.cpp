#include "person.hpp"

Person::Person(QString firstName_, QString middleName_, QString surname_,
               QString title_, QString uri_, QString id_){
    firstName = firstName_;
    middleName = middleName_;
    surname = surname;
    title = title_;
    uri = uri_;
    id = id_;
}

void Person::setFirstName(QString firstName_){
    firstName = firstName_;
}

void Person::setMiddleName(QString middleName_){
    middleName = middleName_;
}

void Person::setSurname(QString surname_){
    surname = surname_;
}

void Person::setTitle(QString title_){
    title = title_;
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
    return firstName;
}

QString Person::getMiddleName(){
    return middleName;
}

QString Person::getSurname(){
    return surname;
}

QString Person::getTitle(){
    return title;
}

QString Person::getUri(){
    return uri;
}

QString Person::getId(){
    return id;
}
