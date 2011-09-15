/********************************************************************
 * controlsWidget.cpp
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
 
#include "controlsWidget.hpp" 


ControlsWidget::ControlsWidget()
{

    ui.setupUi(this);
    
    setSizePolicy( QSizePolicy::Preferred , QSizePolicy::Maximum );


    /** Slower and faster Buttons **/
    
    ui.slower->setAutoRaise( true );
    ui.slower->setMaximumSize( QSize( 26, 20 ) );
    ui.slower->setFocusPolicy( Qt::NoFocus );
    ui.slower->setIcon(QIcon(":/pixmaps/slower.png"));
   // BUTTON_SET_ACT_I( slowerButton, "", slower, qtr( "Slower" ), slower() );

    ui.faster->setAutoRaise( true );
    ui.faster->setMaximumSize( QSize( 26, 20 ) );
    ui.faster->setFocusPolicy( Qt::NoFocus );
    ui.faster->setIcon(QIcon(":/pixmaps/faster.png"));
   // BUTTON_SET_ACT_I( fasterButton, "", faster, qtr( "Faster" ), faster() );

  


    /** Play Buttons **/
    QSizePolicy sizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    sizePolicy.setHorizontalStretch( 0 );
    sizePolicy.setVerticalStretch( 0 );
    
    /* Play */
   
    ui.play->setSizePolicy( sizePolicy );
    ui.play->setMaximumSize( QSize( 32, 32 ) );
    ui.play->setMinimumSize( QSize( 32, 32 ) );
    ui.play->setIconSize( QSize( 26, 26 ) );
    ui.play->setFocusPolicy( Qt::NoFocus );
    ui.play->setIcon(QIcon(":/pixmaps/play.png"));

    /* Prev */
    
    ui.prev->setSizePolicy( sizePolicy );
    setupSmallButton( ui.prev );
    ui.prev->setIcon(QIcon(":/pixmaps/previous.png"));

    /* Stop */
    
    ui.stop->setSizePolicy( sizePolicy );
    setupSmallButton( ui.stop );
    ui.stop->setIcon(QIcon(":/pixmaps/stop.png"));
    
    /* next */
    ui.next->setSizePolicy( sizePolicy );
    setupSmallButton( ui.next );
    ui.next->setIcon(QIcon(":/pixmaps/next.png"));
    
    ui.positionSlider->setMaximum(POSITION_RESOLUTION);
    
    QObject::connect( ui.stop, SIGNAL(clicked()), this, SLOT(stop()));
    QObject::connect( ui.prev, SIGNAL(clicked()), this, SLOT(prev()));
    QObject::connect( ui.play, SIGNAL(clicked()), this, SLOT(play()));
    QObject::connect( ui.next, SIGNAL(clicked()), this, SLOT(next()));
    QObject::connect( ui.faster, SIGNAL(clicked()), this, SLOT(playFaster()));
    QObject::connect( ui.slower, SIGNAL(clicked()), this, SLOT(playSlower()));
    QObject::connect( ui.positionSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(sliderMoved(int)));
}

ControlsWidget::~ControlsWidget()
{}



void ControlsWidget::stop()
{
    emit stopSignal();
}

void ControlsWidget::playFaster()
{
    emit fasterSignal();
}

void ControlsWidget::playSlower()
{
    emit slowerSignal();
}

void ControlsWidget::play()
{
    emit playSignal();
}

void ControlsWidget::prev()
{
    emit prevSignal();
}

void ControlsWidget::next()
{
    emit nextSignal();
}

void ControlsWidget :: changeIcon(bool playing)
{
    if (playing)
    {
        ui.play->setIcon(QIcon(":/pixmaps/pause.png"));
    }
    else
    {
        ui.play->setIcon(QIcon(":/pixmaps/play.png"));
    }
}

//Duration of currently playing file in seconds
void ControlsWidget::setDuration(int sec)
{
    ui.positionSlider->setMaximum(sec);
}

void ControlsWidget::updateSlider(int pos)
{
    ui.positionSlider->setSliderPosition(pos);
}

