#ifndef PERSON_HPP
#define PERSON_HPP
#include <QString>

class Person{
public:
    Person(QString name=QString(), QString id=QString(), QString uri=QString());
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
