/****************************************************************************
**
** Copyright (C) J-P Nurmi <jpnurmi@gmail.com>. Some rights reserved.
**
** This file is part of the QxtGui module of the
** Qt eXTension library <http://libqxt.sourceforge.net>
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or any later version.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QXTSPANSLIDER_H
#define QXTSPANSLIDER_H

#include <QSlider>
#include "qxtpimpl.h"

class QxtSpanSliderPrivate;

class QxtSpanSlider : public QSlider
{
	Q_OBJECT
	QXT_DECLARE_PRIVATE(QxtSpanSlider);
	Q_PROPERTY(int lowerValue READ lowerValue WRITE setLowerValue)
	Q_PROPERTY(int upperValue READ upperValue WRITE setUpperValue)

public:
	explicit QxtSpanSlider(QWidget* parent = 0);
	explicit QxtSpanSlider(Qt::Orientation orientation, QWidget* parent = 0);
	virtual ~QxtSpanSlider();

	int lowerValue() const;
	int upperValue() const;

public slots:
	void setLowerValue(int lower);
	void setUpperValue(int upper);
	void setSpan(int lower, int upper);

signals:
	void spanChanged(int lower, int upper);
	void lowerValueChanged(int lower);
	void upperValueChanged(int upper);

#ifndef QXT_DOXYGEN_RUN
protected:
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void paintEvent(QPaintEvent* event);
#endif // QXT_DOXYGEN_RUN
};

#endif // QXTSPANSLIDER_H
