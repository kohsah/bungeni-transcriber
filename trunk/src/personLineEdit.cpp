#include <QKeyEvent>
#include <QListView>
#include <QStringListModel>
#include <QDebug>
#include "personLineEdit.hpp"

PersonLineEdit::PersonLineEdit(QWidget *parent) : QLineEdit(parent){
    listView = new QListView(this);
    listView->setWindowFlags(Qt::ToolTip);
    person = new Person();
    filterModel = new PersonSortFilterProxyModel(this);
    this->setFocusPolicy(Qt::StrongFocus);
    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(setCompleter(const QString &)));
    connect(listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeText(const QModelIndex &)));
}

void PersonLineEdit::focusOutEvent(QFocusEvent *e) {
    listView->hide();
    QLineEdit::focusOutEvent(e);
}

void PersonLineEdit::focusInEvent(QFocusEvent *e) {
    this->selectAll();
    QLineEdit::focusInEvent(e);
}

void PersonLineEdit::keyPressEvent(QKeyEvent *e) {
    if (!listView->isHidden()) {
        int key = e->key();
        int count = listView->model()->rowCount();
        QModelIndex currentIndex = listView->currentIndex();

        if (Qt::Key_Down == key) {
            int row = currentIndex.row() + 1;
            if (row >= count) {
                row = 0;
            }

            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        } else if (Qt::Key_Up == key) {
            int row = currentIndex.row() - 1;
            if (row < 0) {
                row = count - 1;
            }

            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        } else if (Qt::Key_Escape == key) {
            listView->hide();
        } else if (Qt::Key_Enter == key || Qt::Key_Return == key) {
            if (currentIndex.isValid()) {
                person = static_cast<Person*>(filterModel->mapToSource(currentIndex).internalPointer());
                setText(person->getName());
            }

            listView->hide();
        } else {
            listView->hide();
            QLineEdit::keyPressEvent(e);
        }
    } else {
        QLineEdit::keyPressEvent(e);
    }
}

void PersonLineEdit::setPerson(Person* person_){
    person = person_;
    setText(person->getName());
}

Person* PersonLineEdit::getPerson(){
    return person;
}

void PersonLineEdit::setPersonsModel(PersonsModel* personsModel_){
    filterModel->setSourceModel(personsModel_);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    listView->setModel(filterModel);
}

void PersonLineEdit::setCompleter(const QString &text){
    if (text.isEmpty()) {
        listView->hide();
        return;
    }

    if ((text.length() > 1) && (!listView->isHidden())) {
        return;
    }

    filterModel->setFilterRegExp(text);

    if (filterModel->rowCount() == 0) {
        return;
    }

    // Position the text edit
    listView->setMinimumWidth(width());
    listView->setMaximumWidth(width());

    QPoint p(0, height());
    int x = mapToGlobal(p).x();
    int y = mapToGlobal(p).y() + 1;

    listView->move(x, y);
    listView->show();
}

void PersonLineEdit::completeText(const QModelIndex &index) {
    if (index.isValid()){
        person = static_cast<Person*>(filterModel->mapToSource(index).internalPointer());
        setText(person->getName());
        listView->hide();
    }
}

PersonSortFilterProxyModel::PersonSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool PersonSortFilterProxyModel::filterAcceptsRow(int sourceRow,
         const QModelIndex &sourceParent) const{
     QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
     return (sourceModel()->data(index).toString().contains(filterRegExp()));
}

bool PersonSortFilterProxyModel::filterAcceptsColumn(int sourceColumn,
         const QModelIndex &sourceParent) const{
     if(sourceColumn == 0){
         return true;
     }
     return false;
}
