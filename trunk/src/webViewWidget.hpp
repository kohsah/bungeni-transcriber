#ifndef WEBVIEWWIDGET_HPP
#define WEBVIEWWIDGET_HPP

#include <QDialog>
#include <QtGui>
#include <QtWebKit>

class QWebView;

class WebViewWidget : public QDialog
{
    Q_OBJECT
    public:
        WebViewWidget();
    private:
        QWebView* view;
    signals:
        void authorizationCode(QString&);
    protected slots:
        void onLinkChanged(const QUrl&);
};

#endif // WEBVIEW_HPP
