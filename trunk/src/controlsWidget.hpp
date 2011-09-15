/**********************************************************
 * controlsWidget.hpp
 **********************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2011 - UNDESA
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
 */

#include "ui/ui_controls.h" 
#define setupSmallButton( aButton ){  \
    aButton->setMaximumSize( QSize( 26, 26 ) ); \
    aButton->setMinimumSize( QSize( 26, 26 ) ); \
    aButton->setIconSize( QSize( 20, 20 ) ); \
    aButton->setFocusPolicy( Qt::NoFocus ); \
}
#define POSITION_RESOLUTION 1000
class ControlsWidget : public QWidget
{
    Q_OBJECT
public:
    ControlsWidget();
    ~ControlsWidget();
    void changeIcon(bool playing);
    void setDuration(int sec);
    void updateSlider(int pos);
private:
    Ui::Controls ui;
protected slots:
    void play();
    void stop();
    void prev();
    void next();
    void playFaster();
    void playSlower();
    /*
    void updateVolume( int );
    void updateVolume( void );
    void updateInput();
    void faster();
    void slower();
    */
signals:
    void playSignal();
    void stopSignal();
    void prevSignal();
    void nextSignal();
    void fasterSignal();
    void slowerSignal();
    void sliderMoved(int);
};
