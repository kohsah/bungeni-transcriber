/********************************************************************
 * transcribeWidget.cpp
 ********************************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2011 - UNDESA <www.parliaments.info>
 *
 *
 * Author - Miano Njoka <miano@parliaments.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 ***********************************************************************/


#include <QMessageBox>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMetaEnum>
#include <QProgressDialog>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPalette>
#include "transcribeWidget.hpp"
#include "about.hpp"
#include "hotkeyWidget.hpp"
#include "settings.hpp"
#include "model/speech.hpp"
#include "model/agendaItem.hpp"

TranscribeWidget * TranscribeWidget::instance = NULL;

TranscribeWidget::TranscribeWidget() : QMainWindow()
{
    // Initilise global vars
    currentMediaFile = "";
    fileName="";
    _isPlaying=false;

    //Setup UI
    ui.setupUi(this);
    delegate = new ListViewDelegate(this);
    // Connect UI elements to their respective slots
    QObject::connect(ui.addButton, SIGNAL(clicked()), this, SLOT(addSpeech()));
    QObject::connect(ui.removeButton, SIGNAL(clicked()),
                     this, SLOT(removeTranscriptionItem()));
    QObject::connect(ui.agendaItemButton, SIGNAL(clicked()),
                     this, SLOT(addAgendaItem()));
    QObject::connect(ui.table, SIGNAL(doubleClicked(QModelIndex)),
                     delegate, SLOT(currentEditing(QModelIndex)));
    QObject::connect(ui.table, SIGNAL(clicked(QModelIndex)),
                     delegate, SLOT(display(QModelIndex)));
    QObject::connect(ui.table, SIGNAL(clicked(QModelIndex)),
                     this, SLOT(selection(QModelIndex)));

    // Create and add video frame to UI
    video=new QFrame(this);
    QPalette palette = video->palette();
    palette.setColor( backgroundRole(), QColor( 0, 0, 0 ) );
    video->setPalette( palette );
    video->setAutoFillBackground( true );
    ui.gridLayout_2->addWidget(video, 0, 0, 1, 3);

    // Create and add playback controls.
    controls = new ControlsWidget();
    ui.gridLayout_2->addWidget(controls, 1, 0, 1, 3);

    // Create and add playlist widget
    playlist = new PlaylistWidget();
    ui.gridLayout_2->addWidget(playlist, 2, 0, 1, 3);

    // Connect playlist and control widgets
    QObject::connect( controls, SIGNAL(playSignal()), this, SLOT(play()));
    QObject::connect( controls, SIGNAL(stopSignal()), this, SLOT(stop()));
    QObject::connect( controls, SIGNAL(fasterSignal()), this, SLOT(playFaster()));
    QObject::connect( controls, SIGNAL(slowerSignal()), this, SLOT(playSlower()));
    QObject::connect( controls, SIGNAL(nextSignal()), playlist, SLOT(next()));
    QObject::connect( controls, SIGNAL(prevSignal()), playlist, SLOT(prev()));
    QObject::connect(controls, SIGNAL(sliderMoved(int)),
                     this, SLOT(changePosition(int)));
    QObject::connect( playlist, SIGNAL(playMediaFile(QString)),
                      this, SLOT(playFile(QString)));


    this->setupModelView();
    qRegisterMetaType<QList<TranscriptionItem*>* >("QList<TranscriptionItem*>*");
    QObject::connect( playlist, SIGNAL(loadTranscriptionItems(QList<TranscriptionItem*>*)),
                      model, SLOT(loadTranscriptionItems(QList<TranscriptionItem*>*)));
    this->createActions();
    this->createMenus();

    //create a new libvlc instance
    _vlcinstance=libvlc_new(0, NULL);
    // Create a media player playing environement
    _mp = libvlc_media_player_new (_vlcinstance);

    poller=new QTimer(this);
    QObject::connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    poller->start(100);
}

TranscribeWidget::~TranscribeWidget()
{
    /* Stop playing */
    libvlc_media_player_stop (_mp);
    /* Free the media_player */
    libvlc_media_player_release (_mp);
    libvlc_release (_vlcinstance);
    qDebug() << "Bungeni Transcribe : Exiting";
}

void TranscribeWidget :: stop()
{
    /* Stop playing */
    libvlc_media_player_stop (_mp);
    _isPlaying = false;
    controls->changeIcon(false);
}


void TranscribeWidget::play()
{
    qDebug() << "Bungeni Transcribe : play(), media file -> " << currentMediaFile;
    if (currentMediaFile != "")
    {
        if ( _isPlaying == true )
        {
            qDebug() << "Bungeni Transcribe : play(), Playback paused";
            libvlc_media_player_pause (_mp);
            controls->changeIcon(false);
            _isPlaying = false;
        }
        else
        {
            qDebug() << "Bungeni Transcribe : play(), Playback continued";
            libvlc_media_player_play (_mp);
            _isPlaying = true;
            controls->changeIcon(true);
        }
    }
}


void TranscribeWidget::skipForward(int sec)
{
}

void TranscribeWidget::skipBackward(int sec)
{
}

void TranscribeWidget::playFaster()
{
    float currentRate = libvlc_media_player_get_rate( _mp);
    float newRate=1.0;
    if (currentRate < 0.75)
        newRate = 0.75;
    else if ((currentRate >= 0.75) && (currentRate < 1.0))
        newRate = 1.0;
    else if ((currentRate >= 1.0) && (currentRate < 1.5))
        newRate = 1.5;
    else if ((currentRate >= 1.5) && (currentRate < 2.0))
        newRate = 2.0;
    else if (currentRate >= 2.0)
        newRate = 3.0;
    libvlc_media_player_set_rate( _mp, newRate);
    qDebug() << "Bungeni Transcribe : playFaster(), new rate is " << newRate << "x";
}

void TranscribeWidget::playSlower()
{
    float currentRate = libvlc_media_player_get_rate( _mp);
    float newRate=1.0;
    if (currentRate < 0.75)
        newRate = 0.5;
    else if ((currentRate >= 0.75) && (currentRate < 1.0))
        newRate = 0.5;
    else if ((currentRate >= 1.0) && (currentRate < 1.5))
        newRate = 0.75 ;
    else if ((currentRate >=1.5) && (currentRate < 2.0))
        newRate = 1.0;
    else if ((currentRate >= 2.0) && (currentRate < 3.0))
        newRate = 1.5;
    else if (currentRate >= 3.0)
        newRate = 2.0;
    libvlc_media_player_set_rate( _mp, newRate);
    qDebug() << "Bungeni Transcribe : playSlower(), new rate is " << newRate << "x";

}

int TranscribeWidget::getFileDuration()
{
    qDebug() << "Bungeni Transcribe : getFileDuration(), file_duration ->" << _file_duration;
    return _file_duration;
}

void TranscribeWidget::setFileDuration()
{
    _file_duration = libvlc_media_player_get_length( _mp ) / 1000;
    controls->setDuration(_file_duration);
    qDebug() << "Bungeni Transcribe : setFileDuration(), File duration = " << _file_duration;
}

void TranscribeWidget::playFile(QString file)
{
    currentMediaFile = file;
    /* Create a new LibVLC media descriptor */
    _m = libvlc_media_new_path (_vlcinstance, file.toAscii());
    libvlc_media_player_set_media (_mp, _m);
    /* Get our media instance to use our window */
    libvlc_media_player_set_xwindow(_mp, video->winId());
    /* Play */
    libvlc_media_player_play (_mp );
    _isPlaying = true;
    controls->changeIcon(true);
    //length hack
    //libvlc takes its time before it parses file metadata
    //so we have to wait for it so as to get the file length
    //need to change this to use events
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->start(1500);
    QObject::connect( controls, SIGNAL(playSignal()), timer, SLOT(start()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(setFileDuration()));
    qDebug() << "Bungeni Transcribe : playFile(), Now playing " << currentMediaFile;
}

void TranscribeWidget::changeVolume(int newVolume)
{
    libvlc_audio_set_volume (_mp,newVolume);
    qDebug() << "Bungeni Transcribe : changeVolume, New volume" << newVolume;
}

void TranscribeWidget::changePosition(int newPosition)
{
    if(!_isPlaying){
        qWarning() << "Bungeni Transcribe : changePosition(), Position not changed, no file is currently being played";
        return;
    }
    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL){
        qWarning() << "Bungeni Transcribe : changePositconst QModelIndex &parent = 0ion(), Position not changed, no media";
        return;
    }
    float pos=(float)(newPosition)/(float)_file_duration;
    if (pos<=1.0)
    {
        libvlc_media_player_set_position (_mp, pos);
    }
    qDebug() << "Bungeni Transcribe : changePosition(), new position" << pos;
}

void TranscribeWidget::endReached()
{
    _isPlaying = false;
    controls->changeIcon(false);
    qDebug() << "Bungeni Transcribe : endReached(), End of media file";
}

void TranscribeWidget::updateInterface()
{
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
        return;
    float pos=libvlc_media_player_get_position (_mp);
    int sliderPos=(int)(pos*(float)(_file_duration));
    controls->updateSlider(sliderPos);
}

void TranscribeWidget::removeTranscriptionItem()
{
    QMessageBox msgBox;
    msgBox.setText("Do you want to remove the transcript?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;

    if (indexes.isEmpty() != true)
    {
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes)
        {
            QModelIndexList indexes = selectionModel->selectedIndexes();
            QModelIndex index;
            foreach(index, indexes) {
                model->removeRow(index.row(),QModelIndex());
            }
        }
    }
}

void TranscribeWidget::addSpeech()
{
    Speech *newSpeech;
    if (playlist->getSelected()>=0)
    {
        if (model->rowCount() > 0){
            TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(
                        model->data(model->index(model->rowCount()-1)));
            TranscriptionItem *lastItem = wrapper->ptr;
            newSpeech = new Speech(lastItem->getEndTime().addSecs(1),
                                   lastItem->getEndTime().addSecs(120),
                                   "Speech Text", "Name of Person", 0, true);
        }
        else {
            newSpeech = new Speech(QTime(0,0,0), QTime(0,2,0),
                                    "Speech Text", "Name of Person", 0, true);
        }
        model->insertItem(model->rowCount(), newSpeech);
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),"Please add/select a file from the playlist");
    }
}

void TranscribeWidget::addAgendaItem()
{
    AgendaItem *newAgendaItem;
    if (playlist->getSelected()>=0)
    {
        if (model->rowCount() > 0){
            TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(
                        model->data(model->index(model->rowCount()-1)));
            TranscriptionItem *lastItem = wrapper->ptr;
            newAgendaItem = new AgendaItem(lastItem->getEndTime().addSecs(1),
                                   lastItem->getEndTime().addSecs(120),
                                   "Agenda Item", 0);
        }
        else {
            newAgendaItem = new AgendaItem(QTime(0,0,0), QTime(0,2,0),
                                    "Agenda Item", 0);
        }
        //model->insertItem(model->rowCount(), newAgendaItem);
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),"Please add/select a file from the playlist");
    }
}

void TranscribeWidget::setupModelView(){
    model = new TranscriptionModel();
    ui.table->setItemDelegate(delegate);
    ui.table->setAlternatingRowColors(true);
    //filterModel = new QSortFilterProxyModel();
    //filterModel->setSourceModel(model);
    //filterModel->setDynamicSortFilter(true);
    //filterModel->sort(2, Qt::AscendingOrder);
    ui.table->setModel(model);

    //selectionModel = new QItemSelectionModel(filterModel);
    selectionModel = new QItemSelectionModel(model);
    ui.table->setSelectionModel(selectionModel);
    //Set selection
    ui.table->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui.table->setSelectionRectVisible(true);
}


void TranscribeWidget::createMenus()
{
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(addToPlaylistAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(getTakesAct);
    fileMenu->addAction(postTakesAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(preferencesAct);
    editMenu->addAction(hotkeyAct);

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAct);
}

void TranscribeWidget::preferences()
{
    SettingsDialog *settings = new SettingsDialog(this);
    settings->show();
}

void TranscribeWidget::createActions()
{
    addToPlaylistAct = new QAction("&Add to Playlist", this);
    // addToPlaylistAct->setShortcuts(QKeySequence::New);
    addToPlaylistAct->setStatusTip("Add an existing or new item to playlist");
    connect(addToPlaylistAct, SIGNAL(triggered()), playlist, SLOT(addToPlaylistDialog()));

    saveAct = new QAction("&Save", this);
    //   saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip("Save the document to disk");
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction("Save As", this);
    saveAsAct->setStatusTip("Save the document to disk");
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    preferencesAct = new QAction("Preferences", this);
    preferencesAct->setStatusTip("Bungeni Transcribe Preferences");
    connect(preferencesAct, SIGNAL(triggered()), this, SLOT(preferences()));

    hotkeyAct = new QAction("Hotkey Settings", this);
    hotkeyAct->setStatusTip("Hotkey Settings");
    connect(hotkeyAct, SIGNAL(triggered()), this, SLOT(hotkeySettings()));

    getTakesAct = new QAction("Get Takes", this);
    getTakesAct->setStatusTip("Get Assigned Takes from Bungeni Portal Server");
    connect(getTakesAct, SIGNAL(triggered()), this, SLOT(takes()));

    postTakesAct = new QAction("Post Takes", this);
    postTakesAct->setStatusTip("Post Transcribed Takes to Bungeni Portal Server");
    connect(postTakesAct, SIGNAL(triggered()), this, SLOT(post()));

    exitAct = new QAction("E&xit", this);
    //exitAct->setShortcut("Ctrl+Q");
    exitAct->setStatusTip("Exit the application");
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction("&About", this);
    aboutAct->setStatusTip("Show the application's About box");
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));


}

void TranscribeWidget::hotkeySettings()
{
    HotkeyWidget *hotkey = new HotkeyWidget();
    hotkey->show();
}

void TranscribeWidget::about()
{
    aboutWidget *about = new aboutWidget();
    about->show();
}

void TranscribeWidget::jumpPosition(int change)
{
    if(!_isPlaying)
        return;

    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
        return;
    float curtime = libvlc_media_player_get_time(_mp) / 1000;
    float newTime = curtime + change;

    if (newTime > 0)
    {
        libvlc_media_player_set_time (_mp, newTime*1000);
    }
}

void TranscribeWidget::keyPressEvent( QKeyEvent *keyEvent )
{
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Hotkey");
    QStringList keys = settings.childKeys();
    int values[30];
    int j=0, i=0;
    int event = qtEventToVLCKey(keyEvent);
    bool found = false;
    keys = settings.childKeys();
    if (event > 0)
    {
        for (i = 0; i < keys.size(); ++i)
        {
            values[i]=  settings.value(keys.at(i)).toInt();
        }
        while ((found == false) && (j <= i))
        {
            if( values[j] == event)
            {
                qDebug() << "accept" << event;

                if (keys.at(j) == "Faster")
                {
                    this->playFaster();
                }
                else if (keys.at(j) == "Add Transcript")
                {
                    this->addSpeech();
                }
                else if (keys.at(j) == "Jump backward 10 Sec")
                {
                    this->jumpPosition(-10);
                }
                else if (keys.at(j) == "Jump backward 2 Sec")
                {
                    this->jumpPosition(-2);
                }
                else if (keys.at(j) == "Jump backward 30 Sec")
                {
                    this->jumpPosition(-30);
                }
                else if (keys.at(j) == "Jump forward 10 Sec")
                {
                    this->jumpPosition(10);
                }
                else if (keys.at(j) == "Jump forward 2 Sec")
                {
                    this->jumpPosition(2);
                }
                else if (keys.at(j) == "Jump forward 30 Sec")
                {
                    this->jumpPosition(30);
                }
                else if (keys.at(j) == "Play")
                {
                    this->play();
                }
                else if (keys.at(j) == "Remove Transcript")
                {
                    this->removeTranscriptionItem();
                }
                else if (keys.at(j) == "Slower")
                {
                    this->playSlower();
                }
                else if (keys.at(j) == "Stop")
                {
                    this->stop();
                }
                found = true;
                keyEvent->accept();
            }
            j++;
        }

        if (found == false)
        {
            qDebug() << "ignore" << event;
            //QTextEdit::keyPressEvent(e);
            keyEvent->ignore();
        }
        settings.endGroup();
    }
}
