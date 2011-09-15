/**********************************************************
 * transcribeKeySelector.hpp
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
#ifndef _TRANSCRIBE_KEY_SELECTOR_H_
#define _TRANSCRIBE_KEY_SELECTOR_H_

#include <QWidget>

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QSpinBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVector>
#include <QDialog>
#include <QSettings>

#include "util/util.hpp"

class QTreeWidget;
class QTreeWidgetItem;
class QGroupBox;
class QGridLayout;
class QDialogButtonBox;
class QVBoxLayout;

class transcribeKeyInputDialog : public QDialog
{
Q_OBJECT
public:
    transcribeKeyInputDialog( QTreeWidget *, QString, QWidget * );
    int keyValue;
    bool conflicts;
private:
    QTreeWidget *table;
    void checkForConflicts( int i_vlckey );
    void keyPressEvent( QKeyEvent *);
    void wheelEvent( QWheelEvent *);
    QLabel *selected;
    QVBoxLayout *vLayout;
    QDialogButtonBox *buttonBox;
};

class transcribeKeySelectorControl  : public QWidget
{
    Q_OBJECT
public:
    transcribeKeySelectorControl();
    virtual ~transcribeKeySelectorControl() {};
    virtual void hide() { table->hide(); if( label ) label->hide(); }
    virtual void show() { table->show(); if( label ) label->show(); }
    
private:
    void finish();
    QLabel *label;
    QTreeWidget *table;
    KeyShortcutEdit *shortcutValue;
private slots:
    void setTheKey();
    void selectKey( QTreeWidgetItem * = NULL );
    void select1Key();
public slots:
    void doApply();
};
#endif
