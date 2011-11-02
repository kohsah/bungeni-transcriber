#ifndef TESTTRANSCRIBEWIDGET_H
#define TESTTRANSCRIBEWIDGET_H
#include <QObject>
#include <QtTest/QTest>

 class TestTranscribeWidget: public QObject
 {
     Q_OBJECT
 private slots:
     void testWidget();
 };
//QTEST_MAIN(TestTranscribeWidget)
#endif
