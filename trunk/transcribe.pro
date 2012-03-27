#Change INCLUDEPATH and LIBPATH below to where you have installed VLC
#this will go away soon
INCLUDEPATH = /home/undesa/libraries/vlc/include
LIBPATH = /home/undesa/libraries/vlc/lib
LIBS += -lvlc

HEADERS += src/transcribeWidget.hpp \
    src/controlsWidget.hpp \
    src/listViewDelegate.hpp \
    src/customTextEdit.hpp \
    src/speechEditor.hpp \
    src/agendaItemEditor.hpp \
    src/playlistWidget.hpp \
    src/network/formpost/formpost.hpp \
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
    src/model/playlistItem.hpp \
    src/model/sitting.hpp \
    src/model/take.hpp \
    src/takeEditorWidget.hpp \
    src/sittingEditorWidget.hpp \
    src/addSittingWidget.hpp \
    src/aknHandler.hpp \
    src/personsWidget.hpp \
    src/model/person.hpp \
    src/model/personsModel.hpp \
    src/personLineEdit.hpp

FORMS += src/ui/transcribe.ui \
    src/ui/speechEditor.ui \
    src/ui/uploadProgress.ui \
    src/ui/controls.ui \
    src/ui/about.ui \
    src/ui/serverSettings.ui \
    src/ui/agendaItemEditor.ui \
    src/ui/takeEditor.ui \
    src/ui/sittingEditor.ui \
    src/ui/addSittingWidget.ui \
    src/ui/personsWidget.ui

SOURCES += src/transcribeWidget.cpp \
    src/controlsWidget.cpp \
    src/main.cpp \
    src/listViewDelegate.cpp \
    src/customTextEdit.cpp \
    src/speechEditor.cpp \
    src/agendaItemEditor.cpp \
    src/playlistWidget.cpp \
    src/network/formpost/formpost.cpp \
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
    src/model/playlistItem.cpp \
    src/sittingEditorWidget.cpp \
    src/takeEditorWidget.cpp \
    src/addSittingWidget.cpp \
    src/aknHandler.cpp \
    src/personsWidget.cpp \
    src/model/person.cpp \
    src/model/personsModel.cpp \
    src/personLineEdit.cpp
UI_DIR      =  src/ui/
RESOURCES   =  src/resources.qrc           
CONFIG  +=  qt debug qxt
QT += network xml
QXT += core gui

















