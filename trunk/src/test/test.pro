include(../../transcribe.pro)
MYHEADERS = $$join(HEADERS, " ../../", ../../)
HEADERS = $$MYHEADERS
SOURCES -= src/main.cpp
MYSOURCES = $$join(SOURCES, " ../../", ../../)
SOURCES = $$MYSOURCES
MYUI_DIR = $$join(UI_DIR, " ../../", ../../)
UI_DIR = $$MYUI_DIR
MYFORMS = $$join(FORMS, " ../../", ../../)
FORMS = $$MYFORMS
MYRESOURCES = $$join(RESOURCES, " ../../", ../../)
RESOURCES = $$MYRESOURCES
HEADERS +=  TestTranscribeWidget.hpp
SOURCES +=  TestTranscribeWidget.cpp \
            mainTest.cpp
QT += testlib
