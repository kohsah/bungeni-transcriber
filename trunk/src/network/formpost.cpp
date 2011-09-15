#include "formpost.hpp"

FormPostPlugin::FormPostPlugin()
:QWidget(0)
{
    userAgentS="";
    encodingS="utf-8";
    refererS="";
    http=new QNetworkAccessManager(this);
}

QString FormPostPlugin::userAgent() {
    return userAgentS;
}

void FormPostPlugin::setUserAgent(QString agent) {
    userAgentS=agent;
}

QString FormPostPlugin::referer() {
    return refererS;
}

void FormPostPlugin::setReferer(QString ref) {
    refererS=ref;
}

QString FormPostPlugin::encoding() {
    return encodingS;
}

void FormPostPlugin::setEncoding(QString enc) {
    if (enc=="utf-8" || enc=="ascii") {
        encodingS=enc;    
    }
}

QByteArray FormPostPlugin::strToEnc(QString s) {
    if (encodingS=="utf-8") {
        return s.toUtf8();
    } else {
        return s.toAscii();
    }
}

void FormPostPlugin::addField(QString name, QString value) {
    fieldNames.append(name);
    fieldValues.append(value);
}

void FormPostPlugin::addFile(QString fieldName, QByteArray file, QString name, QString mime) {
    files.append(file);
    fileFieldNames.append(fieldName);
    fileNames.append(name);
    fileMimes.append(mime);
}

void FormPostPlugin::addFile(QString fieldName, QString fileName, QString mime) {
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QByteArray file=f.readAll();
    f.close();
    QString name;
    if (fileName.contains("/")) {
        int pos=fileName.lastIndexOf("/");
        name=fileName.right(fileName.length()-pos-1);
    } else if (fileName.contains("\\")) {
    int pos=fileName.lastIndexOf("\\");
    name=fileName.right(fileName.length()-pos-1);
    } else {
        name=fileName;  
    }
    addFile(fieldName,file,name,mime);
}

QNetworkReply * FormPostPlugin::postData(QString url) {
    QString host;
    host=url.right(url.length()-url.indexOf("://")-3);
    host=host.left(host.indexOf("/"));
    QString crlf="\r\n";
    qsrand(QDateTime::currentDateTime().toTime_t());
    QString b = QVariant(qrand()).toString()+QVariant(qrand()).toString()
                                            +QVariant(qrand()).toString();
    QString boundary="---------------------------"+b;
    QString endBoundary=crlf+"--"+boundary+"--"+crlf;
    QString contentType="multipart/form-data; boundary="+boundary;
    boundary="--"+boundary+crlf;
    QByteArray bond=boundary.toAscii();
    QByteArray send;
    bool first=true;
  
    for (int i=0; i<fieldNames.size(); i++) {
        send.append(bond);
        if (first) {
            boundary=crlf+boundary;
            bond=boundary.toAscii();
            first=false;
        }
        send.append(QString("Content-Disposition: form-data; name=\""
                    +fieldNames.at(i)+"\""+crlf).toAscii());
        if (encodingS=="utf-8") send.append(QString("Content-Transfer-Encoding: 8bit"
                                            +crlf).toAscii());
        send.append(crlf.toAscii());
        send.append(strToEnc(fieldValues.at(i)));
    }
    for (int i=0; i<files.size(); i++) {
        send.append(bond);
        send.append(QString("Content-Disposition: form-data; name=\""
                            +fileFieldNames.at(i)+"\"; filename=\""
                            +fileNames.at(i)+"\""+crlf).toAscii());
        send.append(QString("Content-Type: "+fileMimes.at(i)+crlf+crlf).toAscii());
        send.append(files.at(i));  
    }

    send.append(endBoundary.toAscii());
  
    fieldNames.clear();
    fieldValues.clear();
    fileFieldNames.clear();
    fileNames.clear();
    fileMimes.clear();
    files.clear();

  
    connect(http,SIGNAL(finished(QNetworkReply *)),this,SLOT(readData(QNetworkReply *)));
    QNetworkRequest request;
    request.setRawHeader("Host", host.toAscii());
    if (userAgentS!="") request.setRawHeader("User-Agent", userAgentS.toAscii());
    if (refererS!="") request.setRawHeader("Referer", refererS.toAscii());
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toAscii());
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(send.size()).toString());
    request.setUrl(QUrl(url));
    QNetworkReply * reply=http->post(request,send);
    return reply;
}

void FormPostPlugin::readData(QNetworkReply * r) {
    this->data = r->readAll();
}

QByteArray FormPostPlugin::response() {
    return this->data;
}

Q_EXPORT_PLUGIN2(formpost, FormPostPlugin);


