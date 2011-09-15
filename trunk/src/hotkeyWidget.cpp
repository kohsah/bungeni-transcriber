/********************************************************************
 * hotkeyWidget.cpp
 ********************************************************************
 * This file is part of Bungeni Transcribe
 *
 * Copyright (C) 2009 - UNDESA <www.parliaments.info>
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
 
#include "hotkeyWidget.hpp"

#include <QGridLayout> 
#include <QPushButton>
 
HotkeyWidget :: HotkeyWidget()
{ 
        int line = 0;

        QGridLayout *gLayout = new QGridLayout;
        setObjectName(QString::fromUtf8("preferenceDialog"));
    	resize(400, 300);
        setWindowTitle("Hotkey Settings");
        control = new transcribeKeySelectorControl();
       
        QPushButton *save = new QPushButton("Save");
        QPushButton *cancel = new QPushButton("Close");
        
        gLayout->addWidget(control,1,0,1,2);
        
        gLayout->addWidget(save,2,0);
        gLayout->addWidget(cancel,2,1);
        
        QObject::connect(save, SIGNAL(clicked()), this, SLOT(apply()));
       // QObject::connect(save, SIGNAL(clicked()), control, SLOT(close()));
        QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
        
        
        setLayout( gLayout );
}

void HotkeyWidget :: apply()
{
    control->doApply();
    this->close();
}
