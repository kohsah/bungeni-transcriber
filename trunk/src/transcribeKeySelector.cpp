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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif


#include "util/vlc_keys.h"
#include "transcribeKeySelector.hpp"
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
#include <QGridLayout>
#include <QSettings>
#include <QDialogButtonBox>
class QTreeWidget;
class QTreeWidgetItem;
class QGroupBox;

class QVBoxLayout;


transcribeKeyInputDialog::transcribeKeyInputDialog( QTreeWidget *_table,
                                QString keyToChange,
                                QWidget *_parent ) :
                                QDialog( _parent ), keyValue(0)
{
    setModal( true );
    conflicts = false;

    table = _table;
    setWindowTitle( "Hotkey for "  + keyToChange );

    vLayout = new QVBoxLayout( this );
    selected = new QLabel("Press the new keys for " + keyToChange );
    vLayout->addWidget( selected , Qt::AlignCenter );

    buttonBox = new QDialogButtonBox;
    QPushButton *ok = new QPushButton( "OK" );
    QPushButton *cancel = new QPushButton( "Cancel" );
    buttonBox->addButton( ok, QDialogButtonBox::AcceptRole );
    buttonBox->addButton( cancel, QDialogButtonBox::RejectRole );
    ok->setDefault( true );

    vLayout->addWidget( buttonBox );
    buttonBox->hide();

    QObject::connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
    QObject::connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

void transcribeKeyInputDialog::checkForConflicts( int i_vlckey )
{
     QList<QTreeWidgetItem *> conflictList =
         table->findItems( VLCKeyToString( i_vlckey ), Qt::MatchExactly, 1 );

    if( conflictList.size() )
    {
        QLabel *warning = new QLabel("Warning: the key is already assigned to \"" +
          conflictList[0]->text( 0 ) + "\"" );
        vLayout->insertWidget( 1, warning );
        buttonBox->show();

        conflicts = true;
    }
    else accept();
}

void transcribeKeyInputDialog::keyPressEvent( QKeyEvent *e )
{
    if( e->key() == Qt::Key_Tab ||
        e->key() == Qt::Key_Shift ||
        e->key() == Qt::Key_Control ||
        e->key() == Qt::Key_Meta ||
        e->key() == Qt::Key_Alt ||
        e->key() == Qt::Key_AltGr )
        return;
   
    if( ( e->modifiers() &  Qt::ControlModifier ) || ( e->modifiers() &  Qt::AltModifier ) || ( e->modifiers() &  Qt::ShiftModifier ) )
    {
    	int i_vlck = qtEventToVLCKey( e );
    	selected->setText( "Key: " + VLCKeyToString( i_vlck ) );
    	checkForConflicts( i_vlck );
    	keyValue = i_vlck;
    }
    
}

void transcribeKeyInputDialog::wheelEvent( QWheelEvent *e )
{
    int i_vlck = qtWheelEventToVLCKey( e );
    selected->setText( "Key: " + VLCKeyToString( i_vlck ) );
    checkForConflicts( i_vlck );
    keyValue = i_vlck;
}

transcribeKeySelectorControl::transcribeKeySelectorControl() : QWidget()

{
    
    QGridLayout *gLayout = new QGridLayout( );

    label = new QLabel("Select an action to change the associated hotkey");
 
    /* Deactivated for now
    QLabel *searchLabel = new QLabel( qtr( "Search" ) );
    QLineEdit *actionSearch = new QLineEdit;*/

    table = new QTreeWidget;
    table->setColumnCount(2);
    table->headerItem()->setText( 0,  "Action"  );
    table->headerItem()->setText( 1,  "Shortcut"  );

    shortcutValue = new KeyShortcutEdit;
    shortcutValue->setReadOnly(true);

    QPushButton *clearButton = new QPushButton(  "Clear" );
    QPushButton *setButton = new QPushButton(  "Set" );
    setButton->setDefault( true );
    finish();

    gLayout->addWidget( label, 0, 0, 1, 4 );
  /* deactivated for now
    gLayout->addWidget( searchLabel, 1, 0, 1, 2 );
    gLayout->addWidget( actionSearch, 1, 2, 1, 2 ); */
    gLayout->addWidget( table, 2, 0, 1, 4 );
    gLayout->addWidget( clearButton, 3, 0, 1, 1 );
    gLayout->addWidget( shortcutValue, 3, 1, 1, 2 );
    gLayout->addWidget( setButton, 3, 3, 1, 1 );

   // gLayout->addWidget( keyContainer, 0, 0, 1, 2 );
   this->setLayout(gLayout);

    QObject::connect( clearButton, SIGNAL(clicked()), shortcutValue, SLOT(clear()) );
    QObject::connect( clearButton, SIGNAL(clicked()), this, SLOT(setTheKey()) );
 //   BUTTONACT( setButton, setTheKey() );
    QObject::connect( setButton, SIGNAL( clicked() ), this, SLOT(setTheKey()) );
}
/*
void transcribeKeySelectorControl::finish()
{
 
    table->setColumnCount( 2 );
    table->setAlternatingRowColors( true );

    module_t *p_main = module_Find( p_this, "main" );
    assert( p_main );

    unsigned confsize;
    module_config_t *p_config;

    p_config = module_GetConfig (p_main, &confsize);
	int j = 0;
    for (size_t i = 0; i < confsize; i++)
    {
        module_config_t *p_item = p_config + i;
        if( p_item->i_type & CONFIG_ITEM && p_item->psz_name
            && strstr( p_item->psz_name , "trans-" )
            && !EMPTY_STR( p_item->psz_text ) )
        {
            j++;
        }
    }
    QString keys = "trans-play";
    if (j==0)
    {
    	  config_PutInt( p_this,
                           qtu( keys ),
                           65 );
    }
    qDebug("j : %d",j);
    for (size_t i = 0; i < confsize; i++)
    {
        module_config_t *p_item = p_config + i;

        if( p_item->i_type & CONFIG_ITEM && p_item->psz_name
            && strstr( p_item->psz_name , "trans-" )
            && !EMPTY_STR( p_item->psz_text ) )
        {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem();
            treeItem->setText( 0, qtr( p_item->psz_text ) );
            treeItem->setData( 0, Qt::UserRole,
                               QVariant( qfu( p_item->psz_name ) ) );
            treeItem->setText( 1, VLCKeyToString( p_item->value.i ) );
            treeItem->setData( 1, Qt::UserRole, QVariant( p_item->value.i ) );
            table->addTopLevelItem( treeItem );
        }
    }
    module_PutConfig (p_config);
    module_Put (p_main);

    table->resizeColumnToContents( 0 );

    CONNECT( table, itemDoubleClicked( QTreeWidgetItem *, int ),
             this, selectKey( QTreeWidgetItem * ) );
    CONNECT( table, itemSelectionChanged (),
             this, select1Key() );

    CONNECT( shortcutValue, pressed(), this, selectKey() );
}
*/

void transcribeKeySelectorControl::finish()
{
 
    table->setColumnCount( 2 );
    table->setAlternatingRowColors( true );
     
    QSettings settings("transcribe.conf", QSettings::IniFormat);
     settings.beginGroup("Hotkey");
     QStringList keys = settings.childKeys();
     keys = settings.childKeys();
     for (int i = 0; i < keys.size(); ++i)
     {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem();
            treeItem->setText( 0, keys.at(i) );
            treeItem->setData( 0, Qt::UserRole, QVariant(keys.at(i)) );
            treeItem->setText( 1, VLCKeyToString( settings.value(keys.at(i)).toInt() ));
            treeItem->setData( 1, Qt::UserRole, QVariant( settings.value(keys.at(i)) ) );
            table->addTopLevelItem( treeItem );   
    }
    settings.endGroup();

    table->resizeColumnToContents( 0 );

    QObject::connect( table, SIGNAL(itemDoubleClicked( QTreeWidgetItem *, int )),
             this, SLOT(selectKey( QTreeWidgetItem * )) );
    QObject::connect( table, SIGNAL(itemSelectionChanged ()),
             this, SLOT(select1Key()) );

    QObject::connect( shortcutValue, SIGNAL(pressed()), this, SLOT(selectKey()) );
}

/* Show the key selected from the table in the keySelector */
void transcribeKeySelectorControl::select1Key()
{
    QTreeWidgetItem *keyItem = table->currentItem();
    shortcutValue->setText( keyItem->text( 1 ) );
    shortcutValue->setValue( keyItem->data( 1, Qt::UserRole ).toInt() );
}

void transcribeKeySelectorControl::selectKey( QTreeWidgetItem *keyItem )
{
    /* This happens when triggered by ClickEater */
    if( keyItem == NULL ) keyItem = table->currentItem();

    /* This can happen when nothing is selected on the treeView
       and the shortcutValue is clicked */
    if( !keyItem ) return;

    /* Launch a small dialog to ask for a new key */
    transcribeKeyInputDialog *d = new transcribeKeyInputDialog( table, keyItem->text( 0 ), this );
    d->exec();

    if( d->result() == QDialog::Accepted )
    {
        int newValue = d->keyValue;
        shortcutValue->setText( VLCKeyToString( newValue ) );
        shortcutValue->setValue( newValue );

        if( d->conflicts )
        {
            QTreeWidgetItem *it;
            for( int i = 0; i < table->topLevelItemCount() ; i++ )
            {
                it = table->topLevelItem(i);
                if( ( keyItem != it )
                        && ( it->data( 1, Qt::UserRole ).toInt() == newValue ) )
                {
                    it->setData( 1, Qt::UserRole, QVariant( -1 ) );
                    it->setText( 1, "Unset");
                }
            }
            /* We already made an OK once. */
            setTheKey();
        }
    }
    delete d;
}

void transcribeKeySelectorControl::setTheKey()
{
    table->currentItem()->setText( 1, shortcutValue->text() );
    table->currentItem()->setData( 1, Qt::UserRole, shortcutValue->getValue() );
}

void transcribeKeySelectorControl::doApply()
{
    QTreeWidgetItem *it;
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Hotkey");
    for( int i = 0; i < table->topLevelItemCount() ; i++ )
    {
        it = table->topLevelItem(i);
        if( it->data( 1, Qt::UserRole ).toInt() >= 0 )
            settings.setValue( it->data( 0, Qt::UserRole ).toString() , it->data( 1, Qt::UserRole ).toInt() );
    }
    settings.endGroup();
}
