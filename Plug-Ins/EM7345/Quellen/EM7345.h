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

#ifndef EM7345_H
#define EM7345_H

#include <QtCore>
#include "../../gpsd-tcp/Quellen/Meldung.h"

class QSerialPort;
class EM7345 : public QObject
{
	Q_OBJECT
	public:
		explicit EM7345(QObject *eltern, const QSettings *konfiguration);

	public Q_SLOTS:
		void			Beenden();

	Q_SIGNALS:
		void			Daten(const QString &daten);
		void			MeldungSenden(Meldung meldung);
		void			Beendet();

	private Q_SLOTS:
		void			starten();
		void			DatenZumLesen();
		void			KeineDatenBekommen();

	private:
		const QSettings	*K_Konfiguration;
		QString			K_Anschluss;
		int				K_ID;
		QSerialPort*	K_Modem;
		bool			K_IDGesetzt;
		QTimer			*K_Datenwachhund;
		void			Pruefsumme(QString &daten);
};

#endif // EM7345_H
