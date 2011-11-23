#ifndef AKNPARSER_HPP
#define AKNPARSER_HPP
#include <QObject>
#include <QString>
#include <QXmlStreamWriter>
#include "model/sitting.hpp"
class AknHandler : QObject{
    Q_OBJECT
public:
    AknHandler();
    bool saveSittingToFile(Sitting*, QString);
    Sitting* loadSittingFromFile(QString);
private:
    void writeSpeeches(QXmlStreamWriter*, Sitting*);
    void writeFRBRWork(QXmlStreamWriter*, Sitting*);
    void writeFRBRExpression(QXmlStreamWriter*, Sitting*);
    void writeFRBRManifestation(QXmlStreamWriter*, Sitting*);
};

#endif // AKNPARSER_HPP
