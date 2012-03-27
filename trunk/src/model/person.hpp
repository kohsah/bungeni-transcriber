#ifndef PERSON_HPP
#define PERSON_HPP
#include <QString>
#include <QModelIndex>
class Person{
public:
    Person(QString name_=QString("Name of Person"), QString id_=QString("null"), QString uri_=QString("/Null"));
    QString getName();
    QString getUri();
    QString getId();
    void setName(QString);
    void setUri(QString);
    void setId(QString);
    bool setData(const QModelIndex &index, const QVariant &value);
private:
    QString name;
    QString uri;
    QString id;
};

#endif // PERSON_HPP
