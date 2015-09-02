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


/*! \class DummyPlugin Erweiterung_Dummy.h
	\brief Die Erweiterung für die Pluginfabrik.
*/

/*! \class Erweiterung_Dummy Erweiterung_Dummy.h
	\brief Die Erweiterung für die Steuerung.
*/

/*! \class Dummy Dummy.h
	\brief Ein %Dummy

	Diese Erweiterung dient nur zum Testen der Anbindung an den GPSD.\n
	Es wird immer die selbe Koordinate gesendet.
*/

/*!
	\fn Dummy::Daten(const QString &daten)
	\brief Teilt der Steuerung mit, das neue Koordinaten empfangen worden sind.

	Die Übermittlung erfolgt als NMEA Datensatz.
*/

/*!
	\fn Dummy::MeldungSenden(Meldung meldung)
	\brief Teilt der Steuerung mit, wenn eine %Meldung vorliegt.

	Hierüber werden Debug-, Fehler- und Informationsmeldungen verschickt.
*/

/*!
	\fn Dummy::Beendet()
	\brief Teilt der Steuerung mit, wenn die Hardware abgeschaltet ist.
*/

/*!
  \brief Erstelllt das Dummyobjekt.

  Der 2. Parameter wird nicht ausgewertet, da es beim %Dummy nichts zu konfigurieren gibt.\n
  Da hier keine Hardware zu Grunde liegt wirde jede Sekunde der Emfang simuliert.
  \param[in,out] eltern Das Elternobjekt.
*/
Dummy::Dummy(QObject *eltern, const QSettings *) : QObject(eltern)
{
	QTimer *Uhr= new QTimer(this);
	connect(Uhr,SIGNAL(timeout()),this,SLOT(DatenSenden()));
	Uhr->start(1000);
}
void Dummy::DatenSenden()
{
	QDateTime Zeit=QDateTime::currentDateTime().toUTC();
	static bool GPRMC=true;
	QString Antwort;
	if (GPRMC)
	{
		Antwort=QString("$GPRMC,%1,A,%2,%3,%4,%5,%6,%7,%8,0.0,E,A").arg(Zeit.toString("HHmmss.zzz"))
																   .arg(BREITE).arg(BREITE_RICHTUNG)
																   .arg(LAENGE).arg(LAENGE_RICHTUNG)
																   .arg(GESCHWINDIGKEIT).arg(KURS)
																   .arg(Zeit.toString("ddMMyy"));
		GPRMC=false;
	}
	else
	{
		Antwort=QString("$GPGGA,%1,%2,%3,%4,%5,1,%6,0.0,%7,M,0.0,M,,").arg(Zeit.toString("HHmmss"))
																		.arg(BREITE).arg(BREITE_RICHTUNG)
																		.arg(LAENGE).arg(LAENGE_RICHTUNG)
																		.arg(ANZAHL_SATELITEN).arg(HOEHE_DER_ANTENNE);
		GPRMC=true;
	}
	Q_EMIT Daten(Antwort.append("\r\n"));
}

/*!
	\brief Beendet die GPS Hardware.
*/
void Dummy::Beenden()
{
	Q_EMIT Beendet();
}
