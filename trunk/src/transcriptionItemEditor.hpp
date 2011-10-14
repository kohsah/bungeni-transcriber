#ifndef TRANSCRIPTIONITEMEDITOR_HPP
#define TRANSCRIPTIONITEMEDITOR_HPP

#include <QWidget>
#include <QModelIndex>
class TranscriptionItemEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TranscriptionItemEditor(QWidget *parent = 0);
    virtual void setIndex(const QModelIndex& ) = 0;
signals:
    void close(QWidget *);
    void cancelled(QWidget *);
    void closeIndex(QModelIndex);
};

#endif // TRANSCRIPTIONITEMEDITOR_HPP
