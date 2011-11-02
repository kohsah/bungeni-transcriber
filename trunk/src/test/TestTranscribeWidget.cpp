#include <QApplication>
#include "../transcribeWidget.hpp"
#include "TestTranscribeWidget.hpp"

void TestTranscribeWidget::testWidget()
{
    int argc; char *argv[10];
    QApplication a(argc, argv);
    TranscribeWidget *p = TranscribeWidget::getInstance();
    p->show();
    a.exec();
    //QVERIFY(1 == 1);
}
