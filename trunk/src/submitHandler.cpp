#include <QUrl>
#include <QMessageBox>
#include "submitHandler.hpp"
#include "model/sitting.hpp"
#include "model/transcriptionItem.hpp"
#include "model/speech.hpp"
#include "util/util.hpp"
#include "qjson/serializer.h"
#include "transcribeWidget.hpp"

SubmitHandler::SubmitHandler(){

}


void SubmitHandler::submit(Take *take){
    Sitting *sitting = static_cast<Sitting*>(take->parent());
    QUrl postUrl = QUrl(sitting->getBungeniUrl()+"/add-speeches");
    QList<TranscriptionItem*>* transcriptionItemsList = take->getItems();
    TranscriptionItem* transcriptionItem;
    Speech* speech;
    QVariantList items;
    for (int j=0; j<transcriptionItemsList->size(); j++){
        transcriptionItem = transcriptionItemsList->at(j);
        if(transcriptionItem->getType()==TranscriptionItem::TypeSpeech){
            QVariantMap item;
            speech = static_cast<Speech*>(transcriptionItem);
            item.insert("start_time", QTimeToSeconds(speech->getStartTime()));
            item.insert("endTime", QTimeToSeconds(speech->getEndTime()));
            item.insert("user_id", speech->getPerson()->getId());
            item.insert("speech",speech->getSpeech());
            items << item;
        }
    }
    TranscribeWidget *instance = TranscribeWidget::getInstance();
    if (items.size() > 0){
        QJson::Serializer serializer;
        bool ok;
        QByteArray json = "form.data=" + serializer.serialize(items, &ok)+"&form.actions.add=add";
        QNetworkAccessManager *m = new QNetworkAccessManager(this);
        QNetworkRequest req(postUrl);
        OAuth2 *oauth = instance->oauth;
        req.setRawHeader("Authorization", "Bearer " + oauth->getAccessToken().toAscii());
        QNetworkReply *r = m->post(req, json);
        connect(r, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(networkError(QNetworkReply::NetworkError)));
        connect(r, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(networkSslErrors(QList<QSslError>)));
        connect(m, SIGNAL(finished(QNetworkReply*)), this, SLOT(submitFinished(QNetworkReply*)));
    }
    else {
        QMessageBox::warning(instance, tr("No Speeches"),
            tr("There are no speeches to be submitted"), QMessageBox::Ok);
    }
}

void SubmitHandler::networkError(QNetworkReply::NetworkError){

}

void SubmitHandler::networkSslErrors(QList<QSslError> list){

}

void SubmitHandler::submitFinished(QNetworkReply *reply){
    TranscribeWidget *instance = TranscribeWidget::getInstance();
    QMessageBox::information(instance, tr("Success"),
       tr("Speeches successfully posted"));
}
