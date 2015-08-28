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

#include "EM7345.h"
#include "Vorgaben.h"

#include <syslog.h>

EM7345::EM7345(QObject *eltern, const QSettings *konfiguration) : QObject(eltern)
{
	K_Konfiguration=konfiguration;
	K_Anschluss=K_Konfiguration->value("EM7345/Anschluss",ANSCHLUSS).toString();
	QTimer::singleShot(0,this,SLOT(starten()));
}
void EM7345::starten()
{
	Q_EMIT MeldungSenden(Meldung("a42c182ded374fcb86fd8bd605d9cfa6",tr("%1 benutze Anschluss %2").arg(NAME).arg(K_Anschluss),LOG_INFO));
}
