#ifndef __FORMPOSTINTERFACE_H__
#define __FORMPOSTINTERFACE_H__
#include <QByteArray>
#include <QString>
#include <QNetworkReply>

class FormPost
{
  public:
    virtual ~FormPost() {}
    virtual QString userAgent() = 0;
    virtual void setUserAgent(QString agent) = 0;
    virtual QString referer() = 0;
    virtual void setReferer(QString ref) = 0;
    virtual QString encoding() = 0;
    virtual void setEncoding(QString enc) = 0;
    virtual void addField(QString name, QString value) = 0;
    virtual void addFile(QString fieldName, QByteArray file, QString name, QString mime) = 0;
    virtual void addFile(QString fieldName, QString fileName, QString mime) = 0;
    virtual QNetworkReply * postData(QString url) = 0;
    virtual QByteArray response() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(FormPost,"com.tuckdesign.Plugin.FormPost/1.0");
QT_END_NAMESPACE

#endif

