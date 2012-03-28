#ifndef PERSONLINEEDIT_HPP
#define PERSONLINEEDIT_HPP
#include <QLineEdit>
#include <QStringList>
#include <QSortFilterProxyModel>
#include "model/person.hpp"
#include "model/personsModel.hpp"
class QListView;
class QStringListModel;
class QModelIndex;
class PersonSortFilterProxyModel;

class PersonLineEdit : public QLineEdit {
    Q_OBJECT
public:
    PersonLineEdit(QWidget *parent = 0);
    void setPersonsModel(PersonsModel* personsModel_);
    Person* getPerson();
    void setPerson(Person*);
public slots:
    void setCompleter(const QString &text);
    void completeText(const QModelIndex &index);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    virtual void focusInEvent(QFocusEvent *e);
private:
    Person *person;
    QListView *listView;
    PersonSortFilterProxyModel *filterModel;

};

class PersonSortFilterProxyModel : public QSortFilterProxyModel{
    Q_OBJECT
public:
    PersonSortFilterProxyModel(QObject *parent);
    bool filterAcceptsRow(int, const QModelIndex &) const;
    bool filterAcceptsColumn(int sourceRow, const QModelIndex &sourceParent) const;
    void setPersonsModel(PersonsModel*);
};
#endif // PERSONLINEEDIT_HPP
