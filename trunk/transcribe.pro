#INCLUDEPATH for libvlc on Ubuntu. May be different on other distros
INCLUDEPATH = /usr/include/vlc
LIBS += -lvlc

HEADERS +=  src/transcribeWidget.hpp \
            src/controlsWidget.hpp \
            src/listViewDelegate.hpp \
            src/customTextEdit.hpp \
            src/inplaceEditor.hpp \
            src/agendaItemEditor.hpp \
            src/playlistWidget.hpp \
            src/network/formpost/formpost.hpp \
            src/addToPlaylistWidget.hpp \
            src/qxtspanslider/qxtspanslider_p.h \
            src/qxtspanslider/qxtspanslider.h \
            src/about.hpp \
            src/hotkeyWidget.hpp \
            src/util/vlc_keys.h \
            src/util/util.hpp \
            src/settings.hpp \
            src/transcribeKeySelector.hpp \
            src/model/agendaItem.hpp \
            src/model/speech.hpp \
            src/model/transcriptionModel.hpp 
FORMS   +=  src/ui/transcribe.ui \
            src/ui/inplace_editor.ui \
            src/ui/uploadProgress.ui \
            src/ui/controls.ui \
            src/ui/addToPlaylist.ui \
            src/ui/about.ui \
            src/ui/serverSettings.ui \
            src/ui/agendaItemEditor.ui
SOURCES +=  src/transcribeWidget.cpp \
            src/controlsWidget.cpp \
            src/main.cpp \
            src/listViewDelegate.cpp \
            src/customTextEdit.cpp \
            src/inplaceEditor.cpp \
            src/agendaItemEditor.cpp \
            src/playlistWidget.cpp \
            src/network/formpost/formpost.cpp \
            src/addToPlaylistWidget.cpp \
            src/qxtspanslider/qxtspanslider.cpp \
            src/about.cpp \
            src/hotkeyWidget.cpp \
            src/util/util.cpp \
            src/settings.cpp \
            src/transcribeKeySelector.cpp \
            src/model/agendaItem.cpp \
            src/model/speech.cpp \
            src/model/transcriptionItem.hpp \
            src/model/transcriptionModel.cpp
UI_DIR      =  src/ui/
RESOURCES   =  src/resources.qrc           
CONFIG  +=  qt debug
QT += network xml
