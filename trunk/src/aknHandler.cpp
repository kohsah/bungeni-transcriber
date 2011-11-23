#include <QFile>
#include <QMessageBox>
#include "aknHandler.hpp"
#include "model/speech.hpp"

AknHandler::AknHandler():QObject(){

}

bool AknHandler::saveSittingToFile(Sitting *sitting, QString filePath){
    QXmlStreamWriter *writer = new QXmlStreamWriter();
    writer->setAutoFormatting(true);
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

    writer->writeStartElement("debateContent");
    writer->writeStartElement("debateSection");
    writer->writeAttribute("name", "#default");
    writer->writeAttribute("id", "#id");
    this->writeSpeeches(writer, sitting);
    writer->writeEndElement();  //end debateSection
    writer->writeEndElement();  //end debateContent

    writer->writeEndElement();  //end debate
    writer->writeEndElement();  //end akomaNtoso

    writer->writeEndDocument(); //end document
    return true;
}

void AknHandler::writeSpeeches(QXmlStreamWriter* writer, Sitting* sitting){
    const QList<PlaylistItem*>* takesList = sitting->getTakes();
    Take* take;
    QList<TranscriptionItem*>* transcriptionItemsList;
    TranscriptionItem* transcriptionItem;
    for (int i=0; i < takesList->size(); i++){
        take = static_cast<Take*>(takesList->at(i));
        transcriptionItemsList = take->getItems();
        for (int j=0; j<transcriptionItemsList->size(); j++){
            transcriptionItem = transcriptionItemsList->at(j);
            if(transcriptionItem->getType()==TranscriptionItem::TypeSpeech){
                Speech* speech = static_cast<Speech*>(transcriptionItem);
                writer->writeStartElement("speech");
                writer->writeAttribute("by", speech->getPersonName());
                writer->writeStartElement("p");
                writer->writeCharacters(speech->getSpeech());
                writer->writeEndElement();
                writer->writeEndElement();
            }
        }
    }
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
    writer->writeEndElement(); //end FRBRthis

    writer->writeStartElement("FRBRuri");
    writer->writeAttribute("value", "#test");
    writer->writeEndElement(); //end FRBRuri

    writer->writeStartElement("FRBRdate");
    writer->writeAttribute("date", "#test");
    writer->writeAttribute("name", "#test");
    writer->writeEndElement(); //end FRBRdate

    writer->writeStartElement("FRBRauthor");
    writer->writeAttribute("href", "#test");
    writer->writeAttribute("author", "#test");
    writer->writeEndElement(); //end FRBRauthor

    writer->writeEndElement(); //end FRBRManifestation
}

Sitting* AknHandler::loadSittingFromFile(QString filePath){

}
