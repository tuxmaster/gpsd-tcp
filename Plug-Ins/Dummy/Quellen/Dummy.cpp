/*
	Copyright (C) 2015 Frank Büttner frank@familie-büttner.de

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "Dummy.h"
#include "Vorgaben.h"

Dummy::Dummy(QObject *eltern) : QObject(eltern)
{
	QTimer *Uhr= new QTimer(this);
	connect(Uhr,SIGNAL(timeout()),this,SLOT(DatenSenden()));
	Uhr->start(1000);
}
void Dummy::DatenSenden()
{
	QDateTime Zeit=QDateTime::currentDateTime();
	static bool GPRMC=true;
	QString Antwort;
	if (GPRMC)
	{
		Antwort=QString("$GPRMC,%1,A,%2,%3,%4,%5,%6,%7,%8,0.0,E,S\n").arg(Zeit.toString("HHmmss.z"))
																   .arg(BREITE).arg(BREITE_RICHTUNG)
																   .arg(LAENGE).arg(LAENGE_RICHTUNG)
																   .arg(GESCHWINDIGKEIT).arg(KURS)
																   .arg(Zeit.toString("ddMMyy"));
		GPRMC=false;
	}
	else
	{
		Antwort=QString("$GPGGA,%1,%2,%3,%4,%5,1,%6,0.0,%7,M,0.0,M,,\n").arg(Zeit.toString("HHmmss.z"))
																		.arg(BREITE).arg(BREITE_RICHTUNG)
																		.arg(LAENGE).arg(LAENGE_RICHTUNG)
																		.arg(ANZAHL_SATELITEN).arg(HOEHE_DER_ANTENNE);
		GPRMC=true;
	}
	Q_EMIT Daten(Antwort);
}
