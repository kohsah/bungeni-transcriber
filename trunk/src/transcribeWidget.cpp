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


TranscribeWidget * TranscribeWidget::instance = NULL;

TranscribeWidget::TranscribeWidget() : QMainWindow()
{
    //Setup UI 
    ui.setupUi(this);
    delegate = new ListViewDelegate(this);
    QObject::connect( ui.addButton, SIGNAL(clicked()), this, SLOT(addSpeech()) );
    QObject::connect( ui.removeButton, SIGNAL(clicked()), this, SLOT(removeSpeech()) );
    //QObject::connect( ui.agendaItemButton, SIGNAL(clicked()), this, SLOT(addAgendaItem()) );
    QObject::connect( ui.table, SIGNAL(doubleClicked(QModelIndex)), delegate, SLOT(currentEditing(QModelIndex)));
    QObject::connect( ui.table, SIGNAL(clicked(QModelIndex)), delegate, SLOT(display(QModelIndex)));
    QObject::connect( ui.table, SIGNAL(clicked(QModelIndex)), this, SLOT(selection(QModelIndex)));
    this->setupModelView();
    fileName="";
    
    const char * const vlc_args[] = {
              "-I", "dummy", /* Don't use any interface */
              "--ignore-config", /* Don't use VLC's config */
              "--extraintf=logger"};

    video=new QFrame(this);
    QPalette palette = video->palette();
    palette.setColor( backgroundRole(), QColor( 0, 0, 0 ) );
    video->setPalette( palette );
    video->setAutoFillBackground( true );
    
    ui.gridLayout_2->addWidget(video, 0, 0, 1, 3);
    
    controls = new ControlsWidget();
    ui.gridLayout_2->addWidget(controls, 1, 0, 1, 3);
    
    playlist = new PlaylistWidget();
    ui.gridLayout_2->addWidget(playlist, 2, 0, 1, 3);
    
    currentMediaFile = "";
    
    QObject::connect( playlist, SIGNAL(playMediaFile(QString)), this, SLOT(playFile(QString)));
    QObject::connect( playlist, SIGNAL(playMediaFile(QString)), this, SLOT(loadMetaData(QString)));
    
    QObject::connect( playlist, SIGNAL(loadTranscriptFile(int,QString)), this, SLOT(loadNextFileSlot(int,QString)));
    
    QObject::connect( controls, SIGNAL(playSignal()), this, SLOT(play()));
    QObject::connect( controls, SIGNAL(stopSignal()), this, SLOT(stop()));
    QObject::connect( controls, SIGNAL(fasterSignal()), this, SLOT(playFaster()));
    QObject::connect( controls, SIGNAL(slowerSignal()), this, SLOT(playSlower()));
    QObject::connect( controls, SIGNAL(nextSignal()), playlist, SLOT(next()));
    QObject::connect( controls, SIGNAL(prevSignal()), playlist, SLOT(prev()));
    this->createActions();
    this->createMenus();
    
    _isPlaying=false;
    poller=new QTimer(this);

    //Initialize an instance of vlc
    //a structure for the exception is needed for this initalization
    libvlc_exception_init(&_vlcexcep);

    //create a new libvlc instance
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args,&_vlcexcep);  
    raise (&_vlcexcep);
    
    // Create a media player playing environement 
    _mp = libvlc_media_player_new (_vlcinstance, &_vlcexcep);
    raise (&_vlcexcep);

    //connect the two sliders to the corresponding slots 
    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    connect(controls, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));

    poller->start(100); //start timer to trigger every 100 ms the updateInterface slot
}

TranscribeWidget::~TranscribeWidget()
{
    /* Stop playing */
    libvlc_media_player_stop (_mp, &_vlcexcep);

    /* Free the media_player */
    libvlc_media_player_release (_mp);

    libvlc_release (_vlcinstance);
    raise (&_vlcexcep);
}

void TranscribeWidget :: stop()
{
    /* Stop playing */
    libvlc_media_player_stop (_mp, &_vlcexcep);
    raise(&_vlcexcep);
    /* Free the media_player */
   // libvlc_media_player_release (_mp);
    qDebug() << "Stop " << currentMediaFile;
    _isPlaying = false;
    controls->changeIcon(false);
}


void TranscribeWidget::play()
{
    qDebug() << "Playing file =" << currentMediaFile;
    qDebug() << "is playing(Before)" << _isPlaying;
    if (currentMediaFile != "")
    {
        qDebug() << "Enter current file != NULL ";
        if ( _isPlaying == true )
        {
            libvlc_media_player_pause (_mp, &_vlcexcep);
            raise(&_vlcexcep);
            controls->changeIcon(false);
            _isPlaying = false;
        }
        else
        {
            libvlc_media_player_play (_mp, &_vlcexcep );
            raise(&_vlcexcep);
            
            _file_duration = libvlc_media_player_get_length( _mp, &_vlcexcep);
            raise(&_vlcexcep);
            
            _isPlaying = true;
            controls->changeIcon(true);
        }
    }
    qDebug() << "is playing(after)" << _isPlaying;

}


void TranscribeWidget::skipForward(int sec)
{
}

void TranscribeWidget::skipBackward(int sec)
{
}

void TranscribeWidget::playFaster()
{
    float currentRate = libvlc_media_player_get_rate( _mp, &_vlcexcep);
    raise(&_vlcexcep);
    qDebug() << "Current rate is " << currentRate;
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
        libvlc_media_player_set_rate( _mp, newRate, &_vlcexcep);
        raise(&_vlcexcep);
        qDebug() << "New rate is " << newRate;
}
void TranscribeWidget::playSlower()
{
    float currentRate = libvlc_media_player_get_rate( _mp, &_vlcexcep);
    raise(&_vlcexcep);
    qDebug() << "Current rate is " << currentRate;
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
        libvlc_media_player_set_rate( _mp, newRate, &_vlcexcep);
        raise(&_vlcexcep);
        qDebug() << "New rate is " << newRate;
    
}

void TranscribeWidget::loadMetaData(QString file)
{
    _file_duration = libvlc_media_player_get_length( _mp, &_vlcexcep) / 1000;
     raise(&_vlcexcep);
     qDebug() << "file duration = " << _file_duration ;
     
}

int TranscribeWidget::getFileDuration()
{
    return _file_duration;
}

// End of file reached callback
/*
static void end_reached_callback(const libvlc_event_t *, void *)
{
    qDebug() << "End Reached Callback success";
    //TranscribeWidget *instance = TranscribeWidget::getInstance();
  //  instance->endReached();
   // _isPlaying=false;
}
*/
/*
static void media_preparsed_callback(const libvlc_event_t *, void *)
{
    qDebug() << "Media Pre-parsed success";
}
*/

void TranscribeWidget::playFile(QString file)
{
    //the file has to be in one of the following formats /perhaps a little bit outdated)
    /*
    [file://]filename              Plain media file
    http://ip:port/file            HTTP URL
    ftp://ip:port/file             FTP URL
    mms://ip:port/file             MMS URL
    screen://                      Screen capture
    [dvd://][device][@raw_device]  DVD device
    [vcd://][device]               VCD device
    [cdda://][device]              Audio CD device
    udp:[[<source address>]@[<bind address>][:<bind port>]]
    */
    qDebug() << "File playing is" << file;
    
    currentMediaFile = file;
    
    
    
    /* Create a new LibVLC media descriptor */
    _m = libvlc_media_new (_vlcinstance, file.toAscii(), &_vlcexcep);
    raise(&_vlcexcep);
    
    libvlc_media_player_set_media (_mp, _m, &_vlcexcep);
    raise(&_vlcexcep);
    /*
     /!\ Please note /!\
    
    passing the widget to the lib shows vlc at which position it should show up
    vlc automatically resizes the video to the ´given size of the widget
    and it even resizes it, if the size changes at the playing
    */
    /* Get our media instance to use our window */
    #if defined(Q_OS_WIN)
        libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(video->winId()), &_vlcexcep );
        //libvlc_media_player_set_hwnd(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0
    #elif defined(Q_OS_MAC)
        libvlc_media_player_set_drawable(_mp, video->winId(), &_vlcexcep );
        //libvlc_media_player_set_agl (_mp, _videoWidget->winId(), &_vlcexcep); // for vlc 1.0
    #else //Linux
        libvlc_media_player_set_drawable(_mp, video->winId(), &_vlcexcep );
        //libvlc_media_player_set_xwindow(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0
    #endif
    raise(&_vlcexcep);

    /* Play */
    libvlc_media_player_play (_mp, &_vlcexcep );
    raise(&_vlcexcep);

    _isPlaying=true;
    controls->changeIcon(true);
    
    //length hack
    //libvlc takes its time before it parses file metadata
    //so we have to wait for it so as to get the file length
    //need to change this to use events
    
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->start(1500);
    
    QObject::connect( controls, SIGNAL(playSignal()), timer, SLOT(start()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(getLength()));
}

void TranscribeWidget::changeVolume(int newVolume)
{
    libvlc_exception_clear(&_vlcexcep);
    libvlc_audio_set_volume (_vlcinstance,newVolume , &_vlcexcep);
    raise(&_vlcexcep);
}

void TranscribeWidget::changePosition(int newPosition)
{
    if(!_isPlaying)
        return;
        
    libvlc_exception_clear(&_vlcexcep);
    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp, &_vlcexcep);
    libvlc_exception_clear(&_vlcexcep);
    if (curMedia == NULL)
        return;
   
    float pos=(float)(newPosition)/(float)_file_duration;
    if (pos <=1.0)
    {
        libvlc_media_player_set_position (_mp, pos, &_vlcexcep);
        raise(&_vlcexcep);
    }
}

void TranscribeWidget::endReached()
{
    _isPlaying = false;
    controls->changeIcon(false);
    //this->stop();
}

void TranscribeWidget::updateInterface()
{
    
    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp, &_vlcexcep);
    libvlc_exception_clear(&_vlcexcep);
    if (curMedia == NULL)
        return;

    float pos=libvlc_media_player_get_position (_mp, &_vlcexcep);
    
    //qDebug() << "Update Interface" << _file_duration;
    int siderPos=(int)(pos*(float)(_file_duration));
   // ui.positionSlider->setValue(siderPos);
   controls->updateSlider(siderPos);  
}

void TranscribeWidget::selection(QModelIndex)
{
    if (ui.table->isSelectionRectVisible ())
    {
        qDebug() << "selection rect is visible";
    }
    else
    {
        qDebug() << "selection rect is not visible";
    }
}


void TranscribeWidget::getLength()
{
    //in seconds
    
      _file_duration = libvlc_media_player_get_length( _mp, &_vlcexcep) / 1000;
     raise(&_vlcexcep);
     qDebug() << "get length file duration = " << _file_duration ;
     
     controls->setDuration(_file_duration);
}


void TranscribeWidget::raise(libvlc_exception_t * ex)
{
    if (libvlc_exception_raised (ex))
    {
         fprintf (stderr, "error: %s\n", libvlc_exception_get_message(ex));
         exit (-1);
    }
}

/*
void TranscribeWidget::keyPressEvent( QKeyEvent *keyEvent )
{
   qDebug() << "ignore - Transcribe Widget";
   keyEvent->ignore();
   
}
*/
void TranscribeWidget::removeSpeech()
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
/*
void TranscribeWidget::refresh(const QModelIndex & index)
{
    qDebug( "Refresh entered" );
    emit refreshWidget(index);
    //delegate->display(index);
}
*/
void TranscribeWidget::addSpeech()
{
	qDebug( "Add Transcript entered" );
	int i_length = _file_duration;
	if (playlist->getSelected()>=0)
	{
	    model->insertRows(model->rowCount(), 1, QModelIndex());
	    model->setData(model->index(model->rowCount()-1, 1, QModelIndex()), "Name of Person");
        model->setData(model->index(model->rowCount()-1, 0, QModelIndex()), "Speech Text");
        model->setData(model->index(model->rowCount()-1, 4, QModelIndex()), false);
        model->setData(model->index(model->rowCount()-1, 5, QModelIndex()), true);
	    if (model->rowCount() == 1)
        {  
      		    model->setData(model->index(0, 2, QModelIndex()), 0);
      		    if (i_length < 120)
     			    model->setData(model->index(0, 3, QModelIndex()), i_length);
     		    else
     			    model->setData(model->index(0, 3, QModelIndex()), 120);
     			qDebug( "model->rowCount() = 1" );
        }
        else
        {
     	    int end = model->data(model->index(model->rowCount()-2, 3, QModelIndex())).toInt();
     	    if ((end+120) < i_length)
     	    {
     		    model->setData(model->index(model->rowCount()-1, 2, QModelIndex()), QVariant(end));
			    model->setData(model->index(model->rowCount()-1, 3, QModelIndex()), QVariant(end+120));
		    }
		    else
		    {
			    model->setData(model->index(model->rowCount()-1, 2, QModelIndex()), QVariant(end));
			    model->setData(model->index(model->rowCount()-1, 3, QModelIndex()), QVariant(i_length));
		    }
		    qDebug( "model->rowCount() > 1" );
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),"Please add/select a file from the playlist");
    }
}
/*
void TranscribeWidget::addAgendaItem()
{
	qDebug( "Add Agenda Item entered" );
	int i_length = _file_duration;
	if (playlist->getSelected()>=0)
	{
	    model->insertRows(model->rowCount(), 1, QModelIndex());
        model->setData(model->index(model->rowCount()-1, 5, QModelIndex()), false);
        model->setData(model->index(model->rowCount()-1, 6, QModelIndex()), "");
	    if (model->rowCount() == 1)
        {  
      		    model->setData(model->index(0, 2, QModelIndex()), 0);
      		    if (i_length < 120)
     			    model->setData(model->index(0, 3, QModelIndex()), i_length);
     		    else
     			    model->setData(model->index(0, 3, QModelIndex()), 120);
     			qDebug( "model->rowCount() = 1" );
        }
        else
        {
     	    int end = model->data(model->index(model->rowCount()-2, 3, QModelIndex())).toInt();
     	    if ((end+120) < i_length)
     	    {
     		    model->setData(model->index(model->rowCount()-1, 2, QModelIndex()), QVariant(end));
			    model->setData(model->index(model->rowCount()-1, 3, QModelIndex()), QVariant(end+120));
		    }
		    else
		    {
			    model->setData(model->index(model->rowCount()-1, 2, QModelIndex()), QVariant(end));
			    model->setData(model->index(model->rowCount()-1, 3, QModelIndex()), QVariant(i_length));
		    }
		    qDebug( "model->rowCount() > 1" );
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),"Please add/select a file from the playlist");
    }
}
*/
void TranscribeWidget::setupModelView()
{
    model = new QStandardItemModel(0,8,this);
    // Column 0 - Person Name
    // Column 1 - Speech
    // Column 2 - Start Time
    // Column 3 - End Time
    // Column 4 - Incomplete or not
    // Column 5 - Boolean - Speech(true) or Agenda Item(false)
    // Column 6 - Agenda Item string
    // Column 7 - Agenda Item ID
    
    
	ui.table->setItemDelegate(delegate);
	ui.table->setAlternatingRowColors(true);
	
	filterModel = new QSortFilterProxyModel();
    filterModel->setSourceModel(model);
	filterModel->setDynamicSortFilter(true);
	//Sort by start time
    filterModel->sort(2, Qt::AscendingOrder);
    ui.table->setModel(filterModel);
     	
    selectionModel = new QItemSelectionModel(filterModel);
	ui.table->setSelectionModel(selectionModel);
	//Set selection
    ui.table->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui.table->setSelectionRectVisible(true);
    
}


void TranscribeWidget::updateComplete( int state )
{
    int checked=0;
	if (state == Qt::Checked)
    {
        checked = 1;
        model->setData(model->index(currentIndex.row(),4), QVariant(checked));
        qDebug() << "Checked";
    }
    else
    {
        checked = 0;
        model->setData(model->index(currentIndex.row(),4), QVariant(checked));
        qDebug() << "Unchecked";
    }
    qDebug() << "Model Changed to " << model->data(model->index(currentIndex.row(), 4)).toInt();
}


/*
void  TranscribeWidget::play(QModelIndex _index)
{
	QModelIndex index = filterModel->mapToSource(_index);
	
	int temp;
	QVariant startTimeTxt;
	QTime start;
	startTimeTxt = model->data(model->index(index.row(), 2));
	start = QTime::fromString(timeSecondstoString(startTimeTxt.toInt()));
	//qDebug() << "Time : " << start.toString();
	if( THEPL->current.i_size > 0 )
	{
		
		if ( THEMIM->getIM()->hasInput() )
    		{
    			int64_t i_length = var_GetTime(  THEMIM->getInput() , "length" );
		   // qDebug() << "i_length : " << i_length;
        		int64_t i_time = (int64_t)
            			( QTime( 0, 0, 0 ).msecsTo( start ) ) * 1000;
            		//qDebug() << "i_time : " << i_time;
        		if (startTimeTxt.toInt() > 1)
        		{
        			
        			if (i_time > i_length) 
        				QMessageBox::warning(this, "Error", "Current Media does not have that timestamp. Please ensure that transcript file is for the right media file");
        			else
        				var_SetTime( THEMIM->getInput(), "time", i_time );
    			}
    			else
    			{
    				var_SetTime( THEMIM->getInput(), "time", (int64_t)(1000000) );
    			}
    		}
    		else
    		{
    			playlist_Play( THEPL );
    		}
    	}
    	else
    	{
    		QMessageBox::warning(this, "Error", "Please open a media file through main VLC menu");
    	}
}
*/
void TranscribeWidget::saveFile()
{
	if (fileName != "")
	{
		this->writeFile(fileName);
	}
	else
	{
		this->saveFileAs();
	}
}

void TranscribeWidget::saveFileAs()
{
	QString newfileName = QFileDialog::getSaveFileName(this, tr("File Save As"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
	fileName = newfileName;
	
	this->writeFile(newfileName);
	playlist->setTranscriptFileLocation(newfileName, -1);
}
void TranscribeWidget::openFile()
{
    QString newfileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
    newFile();
    if (!this->loadFile(newfileName))
    {
        QMessageBox::warning(this, tr("Error"),"Cannot Open File");
    }
}

void TranscribeWidget::loadNextFileSlot(int currentLogicalIndex, QString newfileName)
{
    qDebug() << "Load File Slot, newfilename = " << newfileName;
    qDebug() << "Load File Slot, filename = " << fileName;
    qDebug() << "Load File Slot, currentLogicalIndex = " << currentLogicalIndex;
    if ((fileName == "") && (model->rowCount() > 0 ))
    {
	    QMessageBox::warning(this, tr("Error"),"Please save the current transcripts before loading another");
        //this->saveFileAs();
        QString f = QFileDialog::getSaveFileName(this, tr("File Save As"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
	    playlist->setTranscriptFileLocation(f, currentLogicalIndex);
	    this->writeFile(f);
    }
    else if (fileName != "")
    {
        this->writeFile(fileName);
    }
   
    this->loadFile(newfileName);
}

bool TranscribeWidget::loadFile(QString newfileName)
{
	model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
	if (newfileName == "")
	{
	    fileName = newfileName;
	    return true;
	}
	fileName = newfileName;
	QFile newfile(newfileName);
	if (!newfile.open(QFile::ReadOnly | QFile::Text)) {
        	return false;
    	}
	QXmlStreamReader reader;
   	reader.setDevice(&newfile);
	
	if (!reader.atEnd()) {
         	reader.readNext();

         	if (reader.isStartElement()) {
             		if (reader.name() != "trs")
                 	{	
            			return false;	
                 	}	
         	}
     	}
     	
     	if (reader.hasError()) {
            			return false;	
	    }
 	     
     	QString str; 
     	QString person, startTimeTxt, endTimeTxt;
     	QTime q_endTime, q_startTime;
     	model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
	    int i_startTime, i_endTime;
        int row = 0;
     	while (!reader.atEnd()) {
        	reader.readNext();
        	if (reader.name() == "trs")
        	{
        		mediaLocation = reader.attributes().value("media").toString();
        		sittingName = reader.attributes().value("name").toString();
        	}
        	else if (reader.name() == "speech")
        	{	
        		
        		startTimeTxt = reader.attributes().value("startTime").toString();
        		endTimeTxt = reader.attributes().value("endTime").toString();
        		
        		q_startTime = QTime::fromString(startTimeTxt);
        		i_startTime = q_startTime.hour() * 3600 + q_startTime.minute()*60 + q_startTime.second();
        		
        		q_endTime = QTime::fromString(endTimeTxt);
        		i_endTime = q_endTime.hour() * 3600 + q_endTime.minute()*60 + q_endTime.second();
        		
        		person = reader.attributes().value("person").toString();
        		if (reader.isCDATA())
        		{
        		 
        	        str = reader.text().toString();
        	    }
        	    else
        	    {
        	        str = reader.readElementText();
        	    }
        	    qDebug() << str;
        		model->insertRows(row, 1, QModelIndex());
        		model->setData(model->index(row, 0, QModelIndex()), str);
                	model->setData(model->index(row, 1, QModelIndex()), person);
     			model->setData(model->index(row, 2, QModelIndex()), i_startTime);
     			model->setData(model->index(row, 3, QModelIndex()), i_endTime);
     			model->setData(model->index(row, 5, QModelIndex()), true);
     			row++;
     		}
    	}
    	
	newfile.close();
	return true;
}

void TranscribeWidget::newFile()
{
	int ret;
	if (fileName != "")
	{
		ret = QMessageBox::warning(this, tr("Transcribe"),
                   tr("The document has been modified.\n"
                      "Do you want to save your changes?"),
                   QMessageBox::Save | QMessageBox::Discard
                   | QMessageBox::Cancel,
                   QMessageBox::Save);
        switch (ret)
		{
			case QMessageBox::Save:
				saveFile();
				setupModelView();
			break;
			case QMessageBox::Discard:
				setupModelView();
			break;
			case QMessageBox::Cancel:
			break;
		}
    }       
    else
    {
        setupModelView();	
    } 
	
	
}

bool TranscribeWidget::writeFile(QString fileName)
{
    QXmlStreamWriter writer;
    QFile newfile(fileName);
    if (!newfile.open(QFile::WriteOnly | QFile::Text)) {
        	QMessageBox::warning(this, tr("error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(newfile.errorString()));
        	return false;
    }
    writer.setDevice(&newfile);
    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE trs>");
    writer.writeStartElement("trs");
    writer.writeAttribute("media", playlist->getMediaLocation());
    qDebug() << "Media = " << playlist->getMediaLocation();
    writer.writeAttribute("name", playlist->getSittingName());
    qDebug() << "Sitting = " << playlist->getSittingName();
    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
    {
        bool type = model->index( i , 5).data(Qt::DisplayRole).toBool();
        qDebug() << "Type, Speech or not" << type;
        if (type == true)	
        {
    	    writer.writeStartElement("speech");
    	    writer.writeAttribute("person", model->data(model->index(i, 1, QModelIndex()),Qt::DisplayRole).toString());
    	    QString name = model->data(model->index(i, 1, QModelIndex()),Qt::DisplayRole).toString();
    	    if (mphash.value(name, "") != "")
    	    {    
    	        writer.writeAttribute("person_id",mphash.value(name, ""));
    	    }
    	    else
    	    {
    	        writer.writeAttribute("person_id", "");
    	    }
    	    writer.writeAttribute("startTime", 
    	        TranscribeWidget::timeSecondstoString(model->data(model->index(i, 
    	                2, QModelIndex()),Qt::DisplayRole).toInt()));
    	    writer.writeAttribute("endTime", 
    	        TranscribeWidget::timeSecondstoString(model->data(model->index(i, 
    	                3, QModelIndex()),Qt::DisplayRole).toInt()));
            writer.writeCDATA(model->data(model->index(i, 0, QModelIndex()),Qt::DisplayRole).toString());
            //writer.writeCharacters( QDomDocument::createCDATASection ( model->data(model->index(i, 0, QModelIndex()),Qt::DisplayRole).toString() ) );
            writer.writeEndElement();    
        }
        else
        {
            writer.writeStartElement("agenda_item");
            writer.writeAttribute("startTime", 
    	        TranscribeWidget::timeSecondstoString(model->data(model->index(i, 
    	                2, QModelIndex()),Qt::DisplayRole).toInt()));
    	    writer.writeAttribute("endTime", 
    	        TranscribeWidget::timeSecondstoString(model->data(model->index(i, 
    	                3, QModelIndex()),Qt::DisplayRole).toInt()));
    	    writer.writeAttribute("id", 
    	        TranscribeWidget::model->data(model->index(i, 
    	                6, QModelIndex()),Qt::DisplayRole).toString());
    	    writer.writeCDATA(model->data(model->index(i, 7, QModelIndex()),Qt::DisplayRole).toString()); 
            writer.writeEndElement();
        }
    }
    writer.writeEndDocument();
    newfile.close();
    return true;
}

QTemporaryFile * TranscribeWidget::writeTemp()
{
    QXmlStreamWriter writer;
    QTemporaryFile * file = new QTemporaryFile();
    if (!file->open()) {
        	QMessageBox::warning(this, tr("error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file->errorString()));
        	return NULL;
    }
    qDebug( "temp file opened" );
    writer.setDevice(file);
    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE trs>");
    writer.writeStartElement("trs");
    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
    {
    	writer.writeStartElement("speech");
    	writer.writeAttribute("person", model->data(model->index(i, 1, QModelIndex()),Qt::DisplayRole).toString());
    	writer.writeAttribute("startTime", TranscribeWidget::timeSecondstoString(model->data(model->index(i, 2, QModelIndex()),Qt::DisplayRole).toInt()));
    	writer.writeAttribute("endTime", TranscribeWidget::timeSecondstoString(model->data(model->index(i, 3, QModelIndex()),Qt::DisplayRole).toInt()));
        writer.writeCharacters(model->data(model->index(i, 0, QModelIndex()),Qt::DisplayRole).toString());
        writer.writeEndElement();    
    }
    writer.writeEndDocument();
    qDebug( "file written" );
    file->close();
    return file;
}

void TranscribeWidget::post()
{
    if (fileName != "")
    {
        this->saveFile();
    }
    qDebug() << "POSTING TAKES";
    QString string;
    QXmlStreamWriter writer(&string);
    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE trs>");
    writer.writeStartElement("trs");

    QHashIterator<QString, QString> i(hash);
    QStandardItemModel * localmodel = playlist->getModel();
    int c=0;
    while (c <= localmodel->rowCount()){
        QString takeName = localmodel->data(localmodel->index(c, 0)).toString();
        QString sittingID = takeName.section("-", -1, -1);
        QString endTime = takeName.section("-", -2, -2);
        QString startTime = takeName.section("-", -3, -3);
        QString trsfileName = localmodel->data(localmodel->index(c, 2)).toString();
        QFile newfile(trsfileName);
        writer.writeStartElement("take");
        writer.writeAttribute("sittingID", sittingID);
        writer.writeAttribute("startTime", startTime);
        writer.writeAttribute("endTime", endTime);
	    if (newfile.open(QFile::ReadOnly | QFile::Text)) {
            QXmlStreamReader reader;
   	        reader.setDevice(&newfile);
     	    while (!reader.atEnd()) {
        	    reader.readNext();
        	    if ((reader.name() == "speech") || (reader.name() == "agenda_item"))
        	    {	
        		    writer.writeCurrentToken(reader);
        		    writer.writeCharacters(reader.readElementText());
        		    writer.writeEndElement();
     		    }
    	    }
        }	
        writer.writeEndElement();    
	    newfile.close();
	    c++;
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    qDebug() << string;

    FormPostPlugin * post = new FormPostPlugin();

    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString str = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/speechxml";
    post->addField("form.xml", string);
    post->addField("form.actions.post", "Post");
    settings.endGroup();
    QNetworkReply *reply = post->postData(str);

    connect( reply, SIGNAL(finished()), this, SLOT(postFinished()) );
}

void TranscribeWidget::postFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray ba = reply->readAll();
    qDebug() << ba;
    qDebug() << reply->url();
    QMessageBox::information(0, "Success", "Speeches successfully posted to Bungeni");
}



void TranscribeWidget::takes()
{  
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString name = settings.value("username").toString();
    qDebug() << "Name : " << name;
    QString password = settings.value("password").toString();
    QString url = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/login";
    qDebug() << "url : " << url;
    posta = new FormPostPlugin();
    posta->addField("login", name);
    posta->addField("password", password);
    posta->addField("actions.login", "Login");
    reply = posta->postData(url);
    connect( reply, SIGNAL(finished()), this, SLOT(getTakesLoginComplete()) );
    connect( reply, SIGNAL(finished()), this, SLOT(getMPList()) );
    settings.endGroup();
}

void TranscribeWidget::getMPList()
{
    QNetworkRequest request;
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString url = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/mpslistxml";
    request.setUrl(QUrl(url));
    qDebug() << url;
    reply = posta->http->get(request);
    connect( reply, SIGNAL(finished()), this, SLOT(MPListReply()) );
    settings.endGroup();
}

void TranscribeWidget::MPListReply()
{
    QXmlStreamReader reader;
    QByteArray data = posta->response();
    reader.addData(data);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name() == "mp")
        {
        	QString MPName = reader.attributes().value("name").toString();
        	QString MPid = reader.attributes().value("id").toString();
        	if ((MPName != "") && (MPid != ""))
        	{
        	    mphash.insert(MPName, MPid);
        	}
        }
    }
    QHashIterator<QString, QString> i(mphash);
    QStringList mplist;
    while (i.hasNext()) {
        i.next();
        mplist.append(i.key());
    }
    delegate->setMPList(mplist);
}

void TranscribeWidget::getTakesLoginComplete()
{
    QByteArray data = posta->response();
    QNetworkRequest request;
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString url = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/takesxml";
    request.setUrl(QUrl(url));
    qDebug() << url;
    reply = posta->http->get(request);
    connect( reply, SIGNAL(finished()), this, SLOT(takesReply()) );
    settings.endGroup();
}

void TranscribeWidget::takesReply()
{
    QByteArray data = posta->response();
    qDebug() << "Assignments" << data;
    QXmlStreamReader reader;
    QString sittingName;
    reader.addData(data);
    QStringList files;
    QString fileURL = "";
    QString sittingID = "";
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name() == "assignment")
        {
        	//mediaLocation = reader.attributes().value("media").toString();
        	qDebug() << "assignment";
        }
        else if (reader.name() == "sitting")
        {	
        	sittingName = reader.attributes().value("name").toString();
        	sittingID = reader.attributes().value("id").toString();
        	fileURL = reader.attributes().value("file").toString();
        	//qDebug() << streamName << "streams";
        	qDebug() << "sitting" << fileURL;
     	}
     	else if (reader.name() == "take")
     	{
     	    QString startTime = reader.attributes().value("startTime").toString();
     	    QString endTime = reader.attributes().value("endTime").toString();
        	
        	
        	if ((fileURL != "") && (startTime != "") && (endTime != ""))
        	{
        	    //files.insert(files.size(),take);
        	    QString takeName = sittingName + "-" + startTime + "-" + endTime + "-" +sittingID;
        	    QString takeURL = fileURL + "?t=" + startTime + "/" + endTime;
        	    qDebug() << "Take Name = " << takeName << ", Take URL" << takeURL;
        	    hash.insert(takeName, takeURL);
        	}
        	
     	}
     	else if (reader.name() == "agenda_item")
     	{
     	    QString agenda_item_title = reader.attributes().value("title").toString();
     	    QString agenda_item_id = reader.attributes().value("id").toString();
     	    if ((agenda_item_title != "") && (agenda_item_id != ""))
        	{
        	    agendahash.insert(agenda_item_title, agenda_item_id);
        	}
     	}
    }
   // QHash<QString, QString> hash;
    QNetworkRequest request;
    QNetworkAccessManager networkmanager;
    QEventLoop q;
    QTimer tT;
    
    tT.setSingleShot(true);
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
    connect( &networkmanager, SIGNAL(finished( QNetworkReply* )), this, SLOT(takesDownload( QNetworkReply* )) );
    connect(&networkmanager, SIGNAL(finished(QNetworkReply*)), &q, SLOT(quit()));
    
    QHashIterator<QString, QString> a(agendahash);
    QStringList agendalist;
    while (a.hasNext()) {
        a.next();
        agendalist.append(a.key());
    }
    delegate->setAgendaList(agendalist);
    
    QHashIterator<QString, QString> i(hash);
    QProgressDialog progress("Downloading Takes", "Abort", 0, hash.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    int counter = 1;
   // playlist_Clear(THEPL, true);
    playlist->clearPlaylist();
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
    progress.setValue(0);
    while (i.hasNext()) {
        i.next();
        

        if (progress.wasCanceled())
             break;
        request.setUrl(QUrl(i.value()));
        qDebug() << i.value();
        networkmanager.get(request);
        //connect( reply, SIGNAL(finished()), this, SLOT(takesDownload()) );
        tT.start(120000); // 120s timeout
        q.exec();
        if(tT.isActive()){
            // download complete
            tT.stop();
        } else {
            qDebug() << "Time out: Please check your network connection";
            break;
        }
        progress.setValue(counter);
        counter++;
    }
    progress.setValue(hash.size());
    /*
    foreach( QString file, files )
    {
        //playlist_Add( );
        //qDebug() << file;
        QNetworkRequest request;
        QNetworkAccessManager *networkmanager = new QNetworkAccessManager();
        request.setUrl(QUrl(file));
        networkmanager->get(request);
        //connect( reply, SIGNAL(finished()), this, SLOT(takesDownload()) );
        connect( networkmanager, SIGNAL(finished( QNetworkReply* )), this, SLOT(takesDownload( QNetworkReply* )) );
    }
    */

}

void TranscribeWidget::takesDownload( QNetworkReply * reply )
{
    QByteArray data = reply->readAll();
    QString url = reply->url().toString();
    //qDebug() << url;
    QString fileName = hash.key(url);
    QFile file("tmp/"+fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "error opening media file";
    }else
    {
       // QDataStream out(&file);
        //out << data;
        file.write(data);
    }  
    file.flush();
    file.close();
    QFileInfo fileInfo(file);
    QByteArray absfilepath = fileInfo.absoluteFilePath().toAscii(); 
    QString trsfile = "tmp/"+fileName + ".trs";
    playlist->addTakeToPlaylist(fileName, absfilepath, trsfile);
    qDebug() << "File has been downloaded -> " << fileName;
}


void TranscribeWidget::replyFinished(QNetworkReply* reply)
{
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
 
    // "200 OK" received?
    if (statusCodeV.toInt()==200)
        {
        QMessageBox::warning(0, "Get Worked", "Get Worked");
        }
    // Some http error or redirect
    else
        {
        QMessageBox::warning(0, "Error", "Error");
        }
        
    delete reply;   
}

void TranscribeWidget::slotError(QNetworkReply::NetworkError error)
{
    QString errorValue;
    QMetaObject meta = QNetworkReply::staticMetaObject;
    for (int i=0; i < meta.enumeratorCount(); ++i) {
        QMetaEnum m = meta.enumerator(i);
        if (m.name() == QLatin1String("NetworkError")) {
            errorValue = QLatin1String(m.valueToKey(error));
            break;
        }
    }
progressWidget->hide();
    QMessageBox::warning(0, "Upload Error", "Error code:"+errorValue);
}

void TranscribeWidget::progress ( qint64 bytesSent, qint64 bytesTotal )
{
   // int value = (int)((float) bytesSent / (float) bytesTotal * 100);
   // uiProgress.progressBar->setValue(value);
}

void TranscribeWidget::playItem(const QModelIndex& index)
{
    qDebug() << "success";
    QString currentFile = index.model()->index( index.row() , 0).data(Qt::DisplayRole).toString();
    //qDebug() << currentFile;
    if (fileName != "")
    {
        this->saveFile();
    }
    setupModelView();
    currentFile = currentFile + ".trs";
    if(!this->loadFile(currentFile))
    {
        fileName = currentFile;
        
    }
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
    //qDebug() << "Hotkey";
    qDebug() << "Hotkey";
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
        
    libvlc_exception_clear(&_vlcexcep);
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp, &_vlcexcep);
    libvlc_exception_clear(&_vlcexcep);
    if (curMedia == NULL)
        return;
    float curtime = libvlc_media_player_get_time(_mp, &_vlcexcep) / 1000;
    raise(&_vlcexcep);
    float newTime = curtime + change;
    
    if (newTime > 0)
    {
        libvlc_media_player_set_time (_mp, newTime*1000, &_vlcexcep);
        raise(&_vlcexcep);
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
    					this->removeSpeech();
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
