LIBS += -lqjson
CONFIG += qt qjson
QT += network

SOURCES += \
    $$PWD/webViewWidget.cpp \
    $$PWD/oauth2.cpp \

HEADERS += \
    $$PWD/webViewWidget.hpp \
    $$PWD/oauth2.hpp \
