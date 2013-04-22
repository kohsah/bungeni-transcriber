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
#include <QDialog>
#include <QProgressDialog>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPalette>
#include <QDesktopServices>
#include "transcribeWidget.hpp"
#include "about.hpp"
#include "hotkeyWidget.hpp"
#include "settings.hpp"
#include "personsWidget.hpp"
#include "model/speech.hpp"
#include "model/agendaItem.hpp"

#include "qjson/parser.h"

TranscribeWidget * TranscribeWidget::instance = NULL;

TranscribeWidget::TranscribeWidget() : QMainWindow()
{
    // Initilise global vars
    currentMediaFile = "";
    fileName="";
    _isPlaying=false;
    currentIndex = QModelIndex();
    //Setup UI
    ui.setupUi(this);
    delegate = new ListViewDelegate(this);
    this->setupModelView();
    if (!this->setupPersonsModel()){
        QMessageBox::warning(0, tr("Error"),
                             tr("Could not initialise persons model"));
    }
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
    QObject::connect( playlist, SIGNAL(loadTranscriptionItems(QList<TranscriptionItem*>*)),
                  model, SLOT(loadTranscriptionItems(QList<TranscriptionItem*>*)));
    QObject::connect( playlist, SIGNAL(currentTakeIndex(const QModelIndex&)),
                      this, SLOT(setCurrentTakeIndex(const QModelIndex&)));
    QObject::connect( playlist, SIGNAL(refreshPlaylist()),
                      this, SLOT(playlistRefresh()));
    this->createActions();
    this->createMenus();
    unsetenv ("DESKTOP_STARTUP_ID");
    //create a new libvlc instance
    _vlcinstance=libvlc_new(0, NULL);
    // Create a media player playing environement
    _mp = libvlc_media_player_new (_vlcinstance);

    poller=new QTimer(this);
    QObject::connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    poller->start(100);
    setupOAuth();
    manager = new QNetworkAccessManager(this);
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

void TranscribeWidget::setCurrentTakeIndex(const QModelIndex& current){
    currentIndex = current;
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
            QModelIndex index, mappedIndex;
            index = indexes[0];
            mappedIndex = filterModel->mapToSource(index);
            model->removeRow(mappedIndex.row(),QModelIndex());
        }
    }
}

void TranscribeWidget::addSpeech()
{
    Speech *newSpeech;
    if (currentIndex.isValid())
    {
        if (model->rowCount() > 0){
            TranscriptionItemWrapper *wrapper = qvariant_cast<TranscriptionItemWrapper*>(
                        model->data(model->index(model->rowCount()-1)));
            TranscriptionItem *lastItem = wrapper->ptr;
            newSpeech = new Speech(lastItem->getEndTime().addSecs(1),
                                   lastItem->getEndTime().addSecs(120),
                                   "Speech Text", new Person(), true);
        }
        else {
            newSpeech = new Speech(QTime(0,0,0), QTime(0,2,0),
                                    "Speech Text", new Person(), true);
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
            qDebug() << "NEW AGENDA ITEM CREATED";
        }
        model->insertItem(model->rowCount(), newAgendaItem);
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
    //filterModel = new TranscriptionSortModel();
    filterModel = new QSortFilterProxyModel();
    filterModel->setSourceModel(model);
    filterModel->sort(0, Qt::AscendingOrder);
    filterModel->setDynamicSortFilter(true);
    filterModel->setSortRole(Qt::UserRole);

    ui.table->setModel(filterModel);

    selectionModel = new QItemSelectionModel(filterModel);
    ui.table->setSelectionModel(selectionModel);
    ui.table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.table->setSelectionRectVisible(true);

}

QList<Person *>* readPersonsFile(QString filePath){
    QXmlStreamReader reader;
    QFile newfile(filePath);
    QList<Person*>* persons = new QList<Person *>();
    Person *person;
    if (!newfile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file1";
        return persons;
    }
    reader.setDevice(&newfile);
    if (reader.readNextStartElement()){
        if (reader.name() == "persons"){
            while (!reader.atEnd()){
                if ((reader.readNext() == QXmlStreamReader::StartElement) &&
                        (reader.name() == "person")){
                    person = new Person();
                    person->setId(reader.attributes().value("id").toString());
                    person->setName(reader.attributes().value("name").toString());
                    person->setUri(reader.attributes().value("href").toString());
                    persons->append(person);
                }
            }
        }
    }
    if (reader.hasError()){
         qDebug()<<"An error occured"<<reader.errorString()<<reader.lineNumber()<<reader.columnNumber()<<reader.characterOffset();
    }
    newfile.close();
    return persons;
}

bool TranscribeWidget::setupPersonsModel(){
    personsModel = new PersonsModel();
    QString applicationDataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDir applicationDataDir = QDir(applicationDataPath);
    if (!applicationDataDir.exists("bungeni_transcriber")){
        applicationDataDir.mkdir("bungeni_transcriber");
    }
    QFile personsDataFile(applicationDataDir.path()+"bungeni_transcriber/persons.xml");
    if (personsDataFile.exists()){
        //Load file into model
        personsModel->loadPersonData(readPersonsFile(QFileInfo(personsDataFile).filePath()));
        delegate->setPersonsModel(personsModel);
        return true;
    }
    else{
        //create file
        if (!personsDataFile.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(0, tr("Error"),
                                 tr("Cannot write file:\n%2.")
                                 .arg(personsDataFile.errorString()));
            return false;
        }
        personsDataFile.close();
        return true;
    }

}


void TranscribeWidget::createMenus()
{
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(addToPlaylistAct);
    /*fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(getTakesAct);
    fileMenu->addAction(postTakesAct); */
    fileMenu->addSeparator();
    if (!isLoggedIn()){
        fileMenu->addAction(loginAct);
    }
    else {
        fileMenu->addAction(logoutAct);
    }
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(bungeniSettingsAct);
    editMenu->addAction(hotkeyAct);
    editMenu->addAction(personsAct);

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAct);
}

void TranscribeWidget::bungeniSettings()
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
    /*
    saveAct = new QAction("&Save", this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip("Save the document to disk");
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction("Save As", this);
    saveAsAct->setStatusTip("Save the document to disk");
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    */
    bungeniSettingsAct = new QAction("Bungeni Settings", this);
    bungeniSettingsAct->setStatusTip("Bungeni Settings");
    connect(bungeniSettingsAct, SIGNAL(triggered()), this, SLOT(bungeniSettings()));

    personsAct = new QAction("Persons", this);
    personsAct->setStatusTip("Persons");
    connect(personsAct, SIGNAL(triggered()), this, SLOT(persons()));

    hotkeyAct = new QAction("Hotkey Settings", this);
    hotkeyAct->setStatusTip("Hotkey Settings");
    connect(hotkeyAct, SIGNAL(triggered()), this, SLOT(hotkeySettings()));

    loginAct = new QAction("Login", this);
    loginAct->setStatusTip("Log in to Bungeni");
    connect(loginAct, SIGNAL(triggered()), this, SLOT(login()));

    logoutAct = new QAction("Logout", this);
    logoutAct->setStatusTip("Log out of Bungeni");
    connect(logoutAct, SIGNAL(triggered()), this, SLOT(logout()));

    /*
    getTakesAct = new QAction("Get Takes", this);
    getTakesAct->setStatusTip("Get Assigned Takes from Bungeni Portal Server");
    connect(getTakesAct, SIGNAL(triggered()), this, SLOT(takes()));

    postTakesAct = new QAction("Post Takes", this);
    postTakesAct->setStatusTip("Post Transcribed Takes to Bungeni Portal Server");
    connect(postTakesAct, SIGNAL(triggered()), this, SLOT(post()));
    */
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

void TranscribeWidget::persons(){
    PersonsWidget * personsWidget = new PersonsWidget(this);
    personsWidget->setModel(personsModel);
    personsWidget->show();
}

void TranscribeWidget::setupOAuth(){
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString hostName = settings.value("hostname").toString();
    QString clientSecret = settings.value("clientSecret").toString();
    settings.endGroup();
    oauth = new OAuth2();
    oauth->setClientID(QString("bungeni_transcribe"));
    oauth->setClientSecret(clientSecret);
    oauth->setAuthorizationCodeURL(QUrl(hostName + QString("/oauth/authorize")));
    oauth->setRedirectURI(QUrl("http://localhost/"));
    oauth->setAccessTokenURL(QUrl(hostName + QString("/oauth/access-token")));
    if (isLoggedIn()){
        settings.beginGroup("Security");
        QString refreshToken = settings.value("refreshtoken").toString();
        settings.endGroup();
        oauth->setRefreshToken(refreshToken);
        //initiliase access token
        oauth->getAccessToken();
    }
    qDebug()<<"IS USER LOGGED IN"<<isLoggedIn();
    connect(oauth, SIGNAL(linkSucceeded()), this, SLOT(OAuthLinked()));
}

void TranscribeWidget::OAuthLinked(){
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Security");
    settings.setValue("refreshtoken", oauth->getRefreshToken());
    settings.endGroup();
    this->fileMenu->removeAction(loginAct);
    this->fileMenu->insertAction(exitAct, logoutAct);
}

bool TranscribeWidget::isLoggedIn(){
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Security");
    QString rToken = settings.value("refreshtoken").toString();
    settings.endGroup();
    if (rToken.isEmpty()){
        return false;
    }
    else{
        return true;
    }
}

void TranscribeWidget::login(){
    oauth->link();
}

void TranscribeWidget::logout(){
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Security");
    settings.remove("refreshtoken");
    settings.endGroup();
    oauth->unlink();
    this->fileMenu->removeAction(logoutAct);
    this->fileMenu->insertAction(exitAct, loginAct);
}

void TranscribeWidget::playlistRefresh(){
    if (isLoggedIn()){
        QSettings settings("transcribe.conf", QSettings::IniFormat);
        settings.beginGroup("Network");
        QString hostName = settings.value("hostname").toString();
        settings.endGroup();
        QNetworkRequest request;
        QUrl accessUrl = QUrl(hostName+"/api/workspace/my-documents/inbox/");
        accessUrl.addQueryItem("filter_type", "debate_record");
        request.setRawHeader("Authorization", "Bearer " + oauth->getAccessToken().toAscii());
        qDebug() << oauth->getAccessToken().toAscii();
        request.setUrl(accessUrl);
        reply = manager->get(request);
        networkData.clear();
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(networkError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(networkSslErrors(QList<QSslError>)));
        connect(reply, SIGNAL(finished()), this, SLOT(onWorkspaceReadFinished()));
    }
    else {
        QMessageBox::warning(this, tr("Not Logged In"),
            tr("Please log in before refreshing the playlist"),
            QMessageBox::Ok);
    }
}


void TranscribeWidget::addSitting(QString sittingName, QDateTime startTime, QDateTime endTime){
    Sitting* newSitting = new Sitting(sittingName, startTime, endTime);
    PlaylistModel *model = playlist->getModel();
    QModelIndex parent_index = QModelIndex();
    model->insertItem(parent_index, newSitting);
}

void TranscribeWidget::onDebateReadFinished(QNetworkReply *reply){
    QJson::Parser parser;
    bool ok;
    QByteArray data;
    data.append(reply->readAll());
    QVariantMap result = parser.parse(data, &ok).toMap();
    if (!ok) {
        QMessageBox::warning(this, tr("Playlist Refresh Error"),
            tr("An error occured while refreshing the playlist"), QMessageBox::Ok);
        return;
    }
    qDebug() << result;
    addSitting(result["title"].toString(), QDateTime::fromString(result["start_date"].toString(), Qt::ISODate), QDateTime::fromString(result["end_date"].toString(), Qt::ISODate));
}


void TranscribeWidget::onWorkspaceReadFinished(){
    QJson::Parser parser;
    bool ok;
    networkData.append(reply->readAll());
    QVariantMap result = parser.parse(networkData, &ok).toMap();
    if (!ok) {
        QMessageBox::warning(this, tr("Playlist Refresh Error"),
            tr("An error occured while refreshing the playlist"), QMessageBox::Ok);
        return;
    }
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString hostName = settings.value("hostname").toString();
    settings.endGroup();
    QList<QVariant> nodes = result["nodes"].toList();
    for (int i = 0; i < nodes.size(); ++i) {
        QMap<QString, QVariant> node = nodes.at(i).toMap();
        QNetworkAccessManager *m = new QNetworkAccessManager(this);
        qDebug() << hostName+"/api/workspace/my-documents/inbox/"+node["object_id"].toString();
        QNetworkRequest req(QUrl(hostName+"/api/workspace/my-documents/inbox/"+node["object_id"].toString()));
        req.setRawHeader("Authorization", "Bearer " + oauth->getAccessToken().toAscii());
        QNetworkReply *r = m->get(req);
        connect(r, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(networkError(QNetworkReply::NetworkError)));
        connect(r, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(networkSslErrors(QList<QSslError>)));
        connect(m, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDebateReadFinished(QNetworkReply*)));
     }
}

void TranscribeWidget::networkError(QNetworkReply::NetworkError){

}

void TranscribeWidget::networkSslErrors(QList<QSslError>){

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
