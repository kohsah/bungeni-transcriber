#include <QtTest/QtTest>
#include "TestTranscribeWidget.hpp"
int main(int argc, char *argv[])
{

  TestTranscribeWidget ct;
  QTest::qExec(&ct, argc, argv);
  return 0;
}
