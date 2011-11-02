#INCLUDEPATH for libvlc on Ubuntu. May be different on other distros
INCLUDEPATH = /home/undesa/vlc-1.1.11/include/vlc
LIBS += -lvlc

HEADERS +=  TestTranscribeWidget.hpp \
            ../transcribeWidget.hpp \
            ../controlsWidget.hpp \
            ../listViewDelegate.hpp \
            ../customTextEdit.hpp \
            ../speechEditor.hpp \
            ../agendaItemEditor.hpp \
            ../playlistWidget.hpp \
            ../network/formpost/formpost.hpp \
            ../addToPlaylistWidget.hpp \
            ../about.hpp \
            ../hotkeyWidget.hpp \
            ../util/vlc_keys.h \
            ../util/util.hpp \
            ../settings.hpp \
            ../transcriptionItemEditor.hpp \
            ../transcribeKeySelector.hpp \
            ../model/agendaItem.hpp \
            ../model/speech.hpp \
            ../model/transcriptionModel.hpp
            
FORMS   +=  ../ui/transcribe.ui \
            ../ui/speechEditor.ui \
            ../ui/uploadProgress.ui \
            ../ui/controls.ui \
            ../ui/addToPlaylist.ui \
            ../ui/about.ui \
            ../ui/serverSettings.ui \
            ../ui/agendaItemEditor.ui
SOURCES +=  TestTranscribeWidget.cpp \
            mainTest.cpp \
            ../transcribeWidget.cpp \
            ../controlsWidget.cpp \
            ../listViewDelegate.cpp \
            ../customTextEdit.cpp \
            ../speechEditor.cpp \
            ../agendaItemEditor.cpp \
            ../playlistWidget.cpp \
            ../network/formpost/formpost.cpp \
            ../addToPlaylistWidget.cpp \
            ../about.cpp \
            ../hotkeyWidget.cpp \
            ../util/util.cpp \
            ../settings.cpp \
            ../transcribeKeySelector.cpp \
            ../model/agendaItem.cpp \
            ../model/speech.cpp \
            ../model/transcriptionItem.hpp \ 
            ../transcriptionItemEditor.cpp \
            ../model/transcriptionModel.cpp 
UI_DIR      =  ../ui/
RESOURCES   =  ../resources.qrc           
CONFIG  +=  qt debug qxt
QT += network xml testlib
QXT += core gui
