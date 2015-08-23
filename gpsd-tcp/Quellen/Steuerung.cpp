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
	K_Einstellungen=new QSettings(KONFIGDATEI,QSettings::IniFormat,this);
	connect(QCoreApplication::instance(),SIGNAL(aboutToQuit()),this,SLOT(beenden()));
	QTimer::singleShot(0,this,SLOT(loslegen()));
}
void Steuerung::loslegen()
{
	Melden(Meldung("a475b92d2cc84b63a233e7a027442c5f",tr("Starte ...")));
	K_Protokoll=ProtokollTextNachZahl(K_Einstellungen->value("Protokollebene","Info").toString());
	K_Modulpfad=K_Einstellungen->value("Modulpfad",MODULE).toString();
	K_Modul=K_Einstellungen->value("Modul",MODUL).toString();
	if (K_Protokoll==Protokolltiefe::Debug)
	{
		Melden(Meldung("35b01b5da0cc44dcb04822de636f620b",tr("Modulpfad: %1").arg(K_Modulpfad),LOG_DEBUG));
		Melden(Meldung("9ac70e7688f8437eb1b72e6737b4c445",tr("Modul: %1").arg(K_Modul),LOG_DEBUG));
	}
	int Anschluss=0;
	QString Adresse;
	QTcpServer *Datendienst=Q_NULLPTR;
	for( auto Dienst : K_Einstellungen->childGroups())
	{
		if(Dienst.toUpper().startsWith("DIENST"))
		{
			Anschluss=K_Einstellungen->value(QString("%1/Anschluss").arg(Dienst),0).toInt();
			if ((Anschluss ==0) || (Anschluss >65535))
			{
				if(K_Protokoll >=Protokolltiefe::Fehler)
					Melden(Meldung("ba896cda507d4a79a34c6b7db175b64e",trUtf8("Anschlussnummer %1 ist ungültig. Ignoriere %2.").arg(Anschluss).arg(Dienst),LOG_ERR));
				continue;
			}
			Adresse=K_Einstellungen->value(QString("%1/Adresse").arg(Dienst),"").toString();
			if ((Adresse.isEmpty()) || (QHostAddress(Adresse).isNull()))
			{
				if(K_Protokoll >=Protokolltiefe::Fehler)
					Melden(Meldung("bd4b290c2a7a4627a8d2129338b58798",trUtf8("Adresse %1 ist ungültig. Ignoriere %2.").arg(Adresse).arg(Dienst),LOG_ERR));
				continue;
			}
			if(K_Protokoll==Protokolltiefe::Debug)
				Melden(Meldung("81964998fd0f4f6cb7c82ffc5b7bdf27",tr("Erstelle: %1 Adresse: %2 Anschluss: %3").arg(Dienst).arg(Adresse).arg(Anschluss),LOG_DEBUG));

			//Starten der Dienste
			//Datendienst=new QTcpServer(this);
			if(K_Protokoll >=Protokolltiefe::Info)
			{
				Melden(Meldung("fefe966c7e594a48bd0365e961a2c30c",trUtf8("Lausche für %1 auf %2 Anschluss %3").arg(Dienst).arg(Adresse).arg(Anschluss),LOG_INFO));
			}

		}
	}
}
void Steuerung::beenden()
{
	Melden(Meldung("3c9ac521e2e6487995ac623d35b06d70",tr("Beende ...")));
}

void Steuerung::Melden(Meldung m) const
{
	 sd_journal_send(QString("MESSAGE=%1").arg(m.TextHolen()).toUtf8().constData(),QString("MESSAGE_ID=%1").arg(m.IDHolen()).toUtf8().constData(),
					 QString("PRIORITY=%1").arg(m.PrioritaetHolen()).toUtf8().constData(),QString("VERSION=%1").arg(VERSION).toUtf8().constData(),NULL);
}
Protokolltiefe Steuerung::ProtokollTextNachZahl(const QString &text) const
{
	if(text.toUpper().contains("INFO"))
		return Protokolltiefe::Info;
	else if((text.toUpper().contains("FEHLER")) || (text.toUpper().contains("ERROR")))
		return Protokolltiefe::Fehler;
	else if(text.toUpper().contains("DEBUG"))
		return Protokolltiefe::Debug;
	Melden(Meldung("45de11448ff94ec9a17b7a549bcac339",trUtf8("Ungültige Protokolltiefe %1, benutze Info.").arg(text),LOG_ERR));
	return Protokolltiefe::Info;
}
