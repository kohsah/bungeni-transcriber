#ifndef PERSON_HPP
#define PERSON_HPP
#include <QString>

class Person{
public:
    Person(QString, QString, QString, QString, QString, QString);
    QString getFirstName();
    QString getMiddleName();
    QString getSurname();
    QString getTitle();
    QString getUri();
    QString getId();
    void setFirstName(QString);
    void setMiddleName(QString);
    void setSurname(QString);
    void setTitle(QString);
    void setUri(QString);
    void setId(QString);
private:
    QString firstName;
    QString middleName;
    QString surname;
    QString title;
    QString uri;
    QString id;
};

#endif // PERSON_HPP
