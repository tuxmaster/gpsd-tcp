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

#ifndef STEUERUNG_H
#define STEUERUNG_H

#include <QtCore>
#include "Meldung.h"

enum Protokolltiefe
{
	Fehler,
	Info,
	Debug
};

class QTcpSocket;
class QTcpServer;
class Steuerung : public QObject
{
	Q_OBJECT
	public:
		explicit			Steuerung(QObject *eltern = Q_NULLPTR);
							~Steuerung();

	public Q_SLOTS:
		void				beenden();

	private Q_SLOTS:
		void				loslegen();
		void				NeuerKlient(QObject *dienst);

	private:
		void				Melden(Meldung m)const;
		QSettings			*K_Einstellungen;
		Protokolltiefe		K_Protokoll;
		Protokolltiefe		ProtokollTextNachZahl(const QString &text) const;
		QString				K_Modulpfad;
		QString				K_Modul;
		QSignalMapper		*K_Klientensammler;
		QList<QTcpSocket*>	*K_Klienten;

};

#endif // STEUERUNG_H
