#INCLUDEPATH for libvlc on Ubuntu. May be different on other distros
INCLUDEPATH = /usr/include/vlc
LIBS += -lvlc

HEADERS += src/transcribeWidget.hpp \
    src/controlsWidget.hpp \
    src/listViewDelegate.hpp \
    src/customTextEdit.hpp \
    src/speechEditor.hpp \
    src/agendaItemEditor.hpp \
    src/playlistWidget.hpp \
    src/network/formpost/formpost.hpp \
    src/addToPlaylistWidget.hpp \
    src/about.hpp \
    src/hotkeyWidget.hpp \
    src/util/vlc_keys.h \
    src/util/util.hpp \
    src/settings.hpp \
    src/transcriptionItemEditor.hpp \
    src/transcribeKeySelector.hpp \
    src/model/agendaItem.hpp \
    src/model/speech.hpp \
    src/model/transcriptionModel.hpp \
    src/model/transcriptionItem.hpp \
    src/model/playlistModel.hpp \
    src/model/sitting.hpp \
    src/model/take.hpp \
    src/model/playlistItem.hpp

FORMS += src/ui/transcribe.ui \
    src/ui/speechEditor.ui \
    src/ui/uploadProgress.ui \
    src/ui/controls.ui \
    src/ui/addToPlaylist.ui \
    src/ui/about.ui \
    src/ui/serverSettings.ui \
    src/ui/agendaItemEditor.ui

SOURCES += src/transcribeWidget.cpp \
    src/controlsWidget.cpp \
    src/main.cpp \
    src/listViewDelegate.cpp \
    src/customTextEdit.cpp \
    src/speechEditor.cpp \
    src/agendaItemEditor.cpp \
    src/playlistWidget.cpp \
    src/network/formpost/formpost.cpp \
    src/addToPlaylistWidget.cpp \
    src/about.cpp \
    src/hotkeyWidget.cpp \
    src/util/util.cpp \
    src/settings.cpp \
    src/transcribeKeySelector.cpp \
    src/model/agendaItem.cpp \
    src/model/speech.cpp \
    src/transcriptionItemEditor.cpp \
    src/model/transcriptionModel.cpp \
    src/model/sitting.cpp \
    src/model/take.cpp \
    src/model/playlistModel.cpp \
    src/model/playlistItem.cpp
UI_DIR      =  src/ui/
RESOURCES   =  src/resources.qrc           
CONFIG  +=  qt debug qxt
QT += network xml
QXT += core gui
