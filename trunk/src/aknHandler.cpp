#include <QFile>
#include <QMessageBox>
#include "aknHandler.hpp"

AknHandler::AknHandler():QObject(){

}

bool AknHandler::saveSittingToFile(Sitting *sitting, QString filePath){
    QXmlStreamWriter *writer = new QXmlStreamWriter();
    QFile newfile(filePath);
    if (!newfile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, tr("Error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(filePath)
                             .arg(newfile.errorString()));
        return false;
    }
    writer->setDevice(&newfile);
    writer->writeStartDocument();
    writer->writeStartElement("akomaNtoso");
    writer->writeDefaultNamespace("http://www.akomantoso.org/2.0");
    writer->writeStartElement("debate");
    writer->writeStartElement("meta");

    writer->writeStartElement("identification");
    writer->writeAttribute("source", "Bungeni Transcriber");
    this->writeFRBRWork(writer, sitting);
    this->writeFRBRExpression(writer, sitting);
    this->writeFRBRManifestation(writer, sitting);
    writer->writeEndElement();  //end identification


    writer->writeEndElement();  //end meta
    writer->writeEndElement();  //end debate
    writer->writeEndElement();  //end akomaNtoso
    writer->writeEndDocument(); //end document
    return true;
}

void AknHandler::writeFRBRWork(QXmlStreamWriter* writer, Sitting* sitting) {
    writer->writeStartElement("FRBRWork");
    writer->writeStartElement("FRBRthis");
    writer->writeAttribute("value", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRuri");
    writer->writeAttribute("value", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRdate");
    writer->writeAttribute("date", "#test");
    writer->writeAttribute("name", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRauthor");
    writer->writeAttribute("href", "#test");
    writer->writeAttribute("author", "#test");
    writer->writeEndElement();
}

void AknHandler::writeFRBRExpression(QXmlStreamWriter* writer, Sitting* sitting) {
    writer->writeStartElement("FRBRExpression");
    writer->writeStartElement("FRBRthis");
    writer->writeAttribute("value", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRuri");
    writer->writeAttribute("value", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRdate");
    writer->writeAttribute("date", "#test");
    writer->writeAttribute("name", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRauthor");
    writer->writeAttribute("href", "#test");
    writer->writeAttribute("author", "#test");
    writer->writeEndElement();
    writer->writeEndElement();
}

void AknHandler::writeFRBRManifestation(QXmlStreamWriter* writer, Sitting* sitting) {
    writer->writeStartElement("FRBRManifestation");
    writer->writeStartElement("FRBRthis");
    writer->writeAttribute("value", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRuri");
    writer->writeAttribute("value", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRdate");
    writer->writeAttribute("date", "#test");
    writer->writeAttribute("name", "#test");
    writer->writeEndElement();

    writer->writeStartElement("FRBRauthor");
    writer->writeAttribute("href", "#test");
    writer->writeAttribute("author", "#test");
    writer->writeEndElement();
}

Sitting* AknHandler::loadSittingFromFile(QString filePath){

}
