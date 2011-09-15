/*
 * customTextEdit.hpp
 * This file is part of <program name>
 *
 * Copyright (C) 2009 - miano
 *
 * <program name> is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * <program name> is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with <program name>; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */
 
 


#ifndef _CUSTOM_LINE_EDIT_H_
#define _CUSTOM_LINE_EDIT_H_

#include <QTextEdit>

class customTextEdit : public QTextEdit
{
    Q_OBJECT;
public:
	customTextEdit(QWidget *parent);

protected:
	virtual void keyPressEvent( QKeyEvent *keyEvent );
};

#endif

