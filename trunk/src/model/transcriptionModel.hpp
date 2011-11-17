#ifndef TRANSCRIPIONMODEL_HPP
#define TRANSCRIPIONMODEL_HPP

#include <QAbstractListModel>
#include <QList>

#include "transcriptionItem.hpp"

class TranscriptionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TranscriptionModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertItem (int row, TranscriptionItem *item);
    bool setData(const QModelIndex & index, TranscriptionItem &item, int role);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
public slots:
    void loadTranscriptionItems(QList<TranscriptionItem *>*);
private:
    QList<TranscriptionItem *> *items;
};

#endif // TRANSCRIPIONMODEL_HPP
