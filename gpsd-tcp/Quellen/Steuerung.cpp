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

#include "Steuerung.h"
#include "Vorgaben.h"
#include <systemd/sd-journal.h>

Steuerung::Steuerung(QObject *eltern) : QObject(eltern)
{
	connect(QCoreApplication::instance(),SIGNAL(aboutToQuit()),this,SLOT(beenden()));
	QTimer::singleShot(0,this,SLOT(loslegen()));
}
void Steuerung::loslegen()
{
	Melden(Meldung("a475b92d2cc84b63a233e7a027442c5f",tr("Starte ...")));
}
void Steuerung::beenden()
{
	Melden(Meldung("3c9ac521e2e6487995ac623d35b06d70",tr("Beende ...")));
}

void Steuerung::Melden(Meldung m)
{
	 sd_journal_send(QString("MESSAGE=%1").arg(m.TextHolen()).toUtf8().constData(),QString("MESSAGE_ID=%1").arg(m.IDHolen()).toUtf8().constData(),
					 QString("PRIORITY=%1").arg(m.PrioritaetHolen()).toUtf8().constData(),QString("VERSION=%1").arg(VERSION).toUtf8().constData(),NULL);
}
