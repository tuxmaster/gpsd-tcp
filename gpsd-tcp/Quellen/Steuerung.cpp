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

#include <QtNetwork>

#include "Vorgaben.h"
#include "Pluginfabrik.h"

#include <sys/socket.h>
#include <systemd/sd-journal.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>


/*! \class Pluginfabrik Pluginfabrik.h
	\brief Das Interface für die Verwaltung der Erweiterungen.
*/

/*!
	\fn virtual Plugin* Pluginfabrik::plugin(QObject *eltern)
	\brief Liefert eine Erweiterung.
	\param[in,out] eltern Das Elternobjekt.
	\return Der Zeiger auf die Erweiterung.
*/

/*! \class Plugin Plugin.h
	\brief Das Interface für die Erweiterungen
*/

/*!
	\fn virtual const QString Plugin::Version() const
	\brief Liefert die Versionsnummer als Text.
	\return Die Version als Text.
*/

/*!
	\fn virtual const QString Plugin::Name() const
	\brief Liefert den Namen.
	\return Den Namen.
*/

/*!
	\fn virtual QObject* Plugin::Erweiterung(QObject *eltern,const QSettings *konfiguration)
	\brief Liefert ein Zeiger auf die Erweiterung.
	\param[in,out] eltern Das Elternobjekt.
	\param[in] konfiguration Die Programmkonfiguration.
	\return Den Zeiger auf die Erweiterung.
*/

/*! \class Meldung Meldung.h
	\brief Eine %Meldung.

	Jede %Meldung hat 3 Parameter.\n
	ID, Text, Priorität.\n
	\b ID Bescheibt die UUID der %Meldung.\n
	Dies macht den Autausch über Sprachgrenzen einfacher, man nur die Nummer braucht.\n
	\b Text Die eigentliche %Meldung.\n
	\b Priorität Damit man festlegen kann, ab welchem Schweregrd man die Meldungen sehen möchte.
*/

/*!
	\fn Meldung::Meldung(const QString &id, const QString &text)
	\brief Erzeugt eine neue %Meldung.

	Die %Meldung wird mit der Priorität \b LOG_INFO erzeugt.
	\param[in] id Die UUID der %Meldung.
	\param[in] text Der Text der %Meldung.
*/

/*!
	\fn Meldung::Meldung(const QString &id, const QString &text, const int &prioritaet)
	\brief Erzeugt eine neue %Meldung.

	Die %Meldung wird mit der Priorität \b LOG_INFO erzeugt.
	\param[in] id Die UUID der %Meldung.
	\param[in] text Der Text der %Meldung.
	\param[in] prioritaet Die Priorität der %Meldung.
*/

/*!
	\fn const QString &Meldung::TextHolen() const
	\brief Liefert den Text der %Meldung.
	\return Den Text der %Meldung.
*/

/*!
	\fn const QString &Meldung::IDHolen() const
	\brief Liefert die UUID der %Meldung.
	\return Die UUID der %Meldung.
*/

/*!
	\fn const int &Meldung::PrioritaetHolen() const
	\brief Liefert die Priorität der %Meldung.
	\return Die Priorität der %Meldung.
*/

/*!
	\fn static const QString &Meldung::Textprio(const int &prio)
	\brief Liefert das Wort für die Priorität der %Meldung.
	\param[in] prio Den Wert der Priorität.
	\return Das Wort.
*/

/*!
  \class Steuerung Steuerung.h
  \brief Die eigentliche Ablaufsteuerung.
*/

/*!
	\fn void Steuerung::SensorenAbschalten()
	\brief Läßst den Senosor sich abschalten.

	Das Signal muss an jeden Sensor weiter geleitet werden,
	damit dieser die Hardware sauber beenden kann.
*/

int Steuerung::SIGTERM_Socked[2];

/*!
  \brief Erstelllt das Steuerungsobjekt.
  \param[in,out] eltern Das Elternobjekt.
*/
Steuerung::Steuerung(QObject *eltern) : QObject(eltern)
{
	K_Klienten=Q_NULLPTR;
	K_Einstellungen=new QSettings(KONFIGDATEI,QSettings::IniFormat,this);
	//connect(QCoreApplication::instance(),SIGNAL(aboutToQuit()),this,SLOT(beenden()));
	if (::socketpair(AF_UNIX, SOCK_STREAM, 0, SIGTERM_Socked))
	{
		Melden(Meldung("c16ef51833f54acf9ea2594d20783ef6",tr("Kann Prozesssteuerung nicht aufbauen."),LOG_CRIT));
		return;
	}
	K_SocketBeenden = new QSocketNotifier(SIGTERM_Socked[1], QSocketNotifier::Read, this);
	connect(K_SocketBeenden, SIGNAL(activated(int)),this,SLOT(beenden()));
	QTimer::singleShot(0,this,SLOT(loslegen()));
}
Steuerung::~Steuerung()
{
	if(K_Klienten)
	{
		for( auto Klient : *K_Klienten)
			Klient->disconnectFromHost();
	}
	delete K_Klienten;
}

void Steuerung::loslegen()
{
	K_Protokoll=ProtokollTextNachZahl(K_Einstellungen->value("Protokollebene","Info").toString());
	if (K_Protokoll >= Protokolltiefe::Info)
		Melden(Meldung("a475b92d2cc84b63a233e7a027442c5f",tr("Starte ...")));

	if(!TCPstarten())
		return;

	if(!ModulLaden(K_Einstellungen->value("Modul",MODUL).toString(),K_Einstellungen->value("Modulpfad",MODULE).toString()))
		return;

	if(!KontextWechseln(K_Einstellungen->value("Benutzer",BENUTZER).toString(),K_Einstellungen->value("Gruppe",GRUPPE).toString()))
		return;

	if(K_Protokoll >=Protokolltiefe::Info)
		Melden(Meldung("a91b6e29651945378c619e50a629f8cf",tr("Bereit für die Anfragen."),LOG_INFO));
}
/*!
	\brief Wird von der Unix Signalsteuerung beim Signal SIGTERM aufgerufen.

	Dies ist nötig um die Unix Signal korrekt an Qt zu übermitteln.
*/
void Steuerung::beenden()
{
	K_SocketBeenden->setEnabled(false);
	char tmp;
	::read(SIGTERM_Socked[1], &tmp, sizeof(tmp));
	Q_EMIT SensorenAbschalten();
	Melden(Meldung("3c9ac521e2e6487995ac623d35b06d70",tr("Beende ..."),LOG_INFO));
	K_SocketBeenden->setEnabled(true);
}

void Steuerung::Melden(Meldung meldung) const
{
	if(meldung.PrioritaetHolen()>=K_Protokoll)
		sd_journal_send(QString("MESSAGE=%1").arg(meldung.TextHolen().simplified()).toUtf8().constData(),QString("MESSAGE_ID=%1").arg(meldung.IDHolen()).toUtf8().constData(),
						QString("PRIORITY=%1").arg(meldung.PrioritaetHolen()).toUtf8().constData(),QString("VERSION=%1").arg(VERSION).toUtf8().constData(),NULL);
	if(meldung.PrioritaetHolen()==LOG_CRIT)
		QCoreApplication::quit();
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
void Steuerung::NeuerKlient(QObject *dienst)
{
	QTcpSocket* Klient =dynamic_cast<QTcpServer*> (dienst)->nextPendingConnection();
	Melden(Meldung("a32a5261338d422d8e27dc832e1c6e90",tr("Verbindung von %1.").arg(Klient->peerAddress().toString()),LOG_INFO));
	K_Klienten->append(Klient);
	connect(Klient, SIGNAL(disconnected()), K_Klientloescher, SLOT(map()));
	K_Klientloescher->setMapping(Klient,Klient);
}
void Steuerung::DatenVerteilen(const QString &daten)
{
	Melden(Meldung("1bf20681b80a42c191b906560bb6647a",tr("Daten empfangen vom Modul: %1").arg(daten),LOG_DEBUG));
	for( auto Klient : *K_Klienten)
	{
		if(Klient->state()==QAbstractSocket::ConnectedState)
		{
			Melden(Meldung("6733089a635b45f7ae0d27c3ecaf518b",tr("Sende an Klient %1").arg(Klient->peerAddress().toString()),LOG_DEBUG));
			Klient->write(daten.toLatin1());
		}
	}
}
void Steuerung::KlientLoeschen(QObject *klient)
{
	Melden(Meldung("fcea41ff29354995b415d9dce03fcdf7",tr("Verbindung von %1 getrennt.").arg(dynamic_cast<QTcpSocket*> (klient)->peerAddress().toString()),LOG_INFO));
	K_Klienten->removeOne(dynamic_cast<QTcpSocket*> (klient));
}
bool Steuerung::KontextWechseln(const QString &nutzer, const QString &gruppe)
{
	QString Fehlertext;
	int NutzerID;
	int GruppeID;
	group *Gruppe=getgrnam(gruppe.toUtf8().constData());
	int FehlerGruppe=errno;
	passwd *Nutzer=getpwnam(nutzer.toUtf8().constData());
	int FehlerNutzer=errno;
	if( Gruppe==NULL)
	{
		Fehlertext=trUtf8("Gruppenname: %1 konnte nicht aufgelöst werden.").arg(gruppe);
		if (FehlerGruppe !=0)
			Fehlertext.append(QString("\n%1").arg(strerror(FehlerGruppe)));
		Melden(Meldung("3dfabfae6cf244b690d9d41b0b293593",Fehlertext,LOG_CRIT));
		return false;
	}
	if( Nutzer==NULL)
	{
		Fehlertext=trUtf8("Nutzername: %1 konnte nicht aufgelöst werden.").arg(nutzer);
		if (FehlerNutzer!=0)
			Fehlertext.append(QString("\n%1").arg(strerror(FehlerNutzer)));
		Melden(Meldung("a9a1690d05ef442cbd2625d3065bbdb9",Fehlertext,LOG_CRIT));
		return false;
	}
	GruppeID=Gruppe->gr_gid;
	NutzerID=Nutzer->pw_uid;

	Melden(Meldung("b044be0993314c3384c95cce368c207e",tr("Starte als Nutzer: %1(%2) Gruppe: %3(%4)").arg(nutzer).arg(NutzerID).arg(gruppe).arg(GruppeID),LOG_DEBUG));
	setgroups(0, NULL);
	if(setgid(GruppeID)!=0)
		{
				int Fehler=errno;
				Melden(Meldung("5acf2a01d541456499e684e160bb8ebf",tr("Konnte nicht in den Gruppenkontext %1 wechseln.\n%2").arg(gruppe).arg(strerror(Fehler)),LOG_CRIT));
				return false;
		}
	if(setuid(NutzerID)!=0)
	{
		int Fehler=errno;
		Melden(Meldung("b74f03cfa9d34ed6aa9f40325ff6f4c5",tr("Konnte nicht in den Benutzerkontext %1 wechseln.\n%2").arg(nutzer).arg(strerror(Fehler)),LOG_CRIT));
		return false;
	}
	return true;
}
bool Steuerung::TCPstarten()
{
	bool gueltigerDienst=false;
	K_Klienten = new QList<QTcpSocket*>;
	int Anschluss=0;
	QString Adresse;
	QTcpServer *Datendienst=Q_NULLPTR;
	K_Klientensammler=new QSignalMapper(this);
	K_Klientloescher=new QSignalMapper(this);
	for( auto Dienst : K_Einstellungen->childGroups())
	{
		if(Dienst.toUpper().startsWith("DIENST"))
		{
			Anschluss=K_Einstellungen->value(QString("%1/Anschluss").arg(Dienst),0).toInt();
			if ((Anschluss ==0) || (Anschluss >65535))
			{
				Melden(Meldung("ba896cda507d4a79a34c6b7db175b64e",tr("Anschlussnummer %1 ist ungültig. Ignoriere %2.").arg(Anschluss).arg(Dienst),LOG_ERR));
				continue;
			}
			Adresse=K_Einstellungen->value(QString("%1/Adresse").arg(Dienst),"").toString();
			if ((Adresse.isEmpty()) || (QHostAddress(Adresse).isNull()))
			{
				Melden(Meldung("bd4b290c2a7a4627a8d2129338b58798",tr("Adresse %1 ist ungültig. Ignoriere %2.").arg(Adresse).arg(Dienst),LOG_ERR));
				continue;
			}
			Melden(Meldung("81964998fd0f4f6cb7c82ffc5b7bdf27",tr("Erstelle: %1 Adresse: %2 Anschluss: %3").arg(Dienst).arg(Adresse).arg(Anschluss),LOG_DEBUG));

			//Starten der Dienste
			Datendienst=new QTcpServer(this);
			if(!Datendienst->listen(QHostAddress(Adresse),Anschluss))
			{
				Melden(Meldung("d91c632a84b54f3cb634485cf007d485",tr("Konnte %1 nicht starten.\n%2").arg(Dienst).arg(Datendienst->errorString()),LOG_ERR));
				Datendienst->deleteLater();
			}
			else
			{
				gueltigerDienst=true;
				connect(Datendienst, SIGNAL(newConnection()), K_Klientensammler, SLOT(map()));
				K_Klientensammler->setMapping(Datendienst,Datendienst);
				Melden(Meldung("fefe966c7e594a48bd0365e961a2c30c",tr("Lausche für %1 auf %2 Anschluss %3").arg(Dienst).arg(Adresse).arg(Anschluss),LOG_INFO));
			}

		}
	}
	connect(K_Klientensammler,SIGNAL(mapped(QObject*)),this,SLOT(NeuerKlient(QObject*)));
	connect(K_Klientloescher,SIGNAL(mapped(QObject*)),this,SLOT(KlientLoeschen(QObject*)));
	if(!gueltigerDienst)
	{
		Melden(Meldung("217dfdf7f21949b08a37ae5dd20d88f6",tr("Es konnte kein Dienst gestartet werden."),LOG_ERR));
		QCoreApplication::quit();
	}
	return gueltigerDienst;
}
bool Steuerung::ModulLaden(const QString modulname, const QString &pfad)
{
	Melden(Meldung("35b01b5da0cc44dcb04822de636f620b",tr("Modulpfad: %1").arg(pfad),LOG_DEBUG));
	Melden(Meldung("9ac70e7688f8437eb1b72e6737b4c445",tr("Modul: %1").arg(modulname),LOG_DEBUG));

	QString Datei;
	QDirIterator Verzeichis(pfad,QDirIterator::Subdirectories);
	bool ModulGefunden=false;
	while (Verzeichis.hasNext())
	{
		Datei=Verzeichis.next();
		if (Verzeichis.fileInfo().isFile())
		{
			Melden(Meldung("de0b1026c38040708422dbbb6b512d61",tr("Untersuche Datei %1 auf eine Erweiterung").arg(Datei),LOG_DEBUG));
			QPluginLoader ErweiterungLesen(Datei);
			QObject *Erweiterung = ErweiterungLesen.instance();
			if(Erweiterung)
			{
				Melden(Meldung("64016fc20c8e434a98418514639ab699",tr("Untersuche Datei %1 auf eine gpsd-tcp Erweiterung").arg(Datei),LOG_DEBUG));
				Pluginfabrik* gpsd_tcpd_Erweiterung= qobject_cast<Pluginfabrik*>(Erweiterung);
				if(gpsd_tcpd_Erweiterung)
				{
					Melden(Meldung("e04e03ea37294065b95590b9052c8081",tr("%1 ist eine gpsd-tcp Erweiterung").arg(Datei),LOG_DEBUG));
					if(gpsd_tcpd_Erweiterung->plugin(this)->Name() == modulname )
					{
						Melden(Meldung("38395376945042999326095790667f8e",tr("Modul %1 gefunden.").arg(modulname),LOG_DEBUG));
						ModulGefunden=true;
						connect(gpsd_tcpd_Erweiterung->plugin(this)->Erweiterung(this,K_Einstellungen),SIGNAL(Daten(const QString&)),this,SLOT(DatenVerteilen(QString)));
						connect(gpsd_tcpd_Erweiterung->plugin(this)->Erweiterung(this,K_Einstellungen),SIGNAL(MeldungSenden(Meldung)),this,SLOT(Melden(Meldung)));
						connect(this,SIGNAL(SensorenAbschalten()),gpsd_tcpd_Erweiterung->plugin(this)->Erweiterung(this,K_Einstellungen),SLOT(Beenden()));
						connect(gpsd_tcpd_Erweiterung->plugin(this)->Erweiterung(this,K_Einstellungen),SIGNAL(Beendet()),this,SLOT(SensorenAbgeschaltet()));
						break;
					}
				}
			}
			else
				Melden(Meldung("5f0663b236b24fb28a5b7066f93ddd1f",tr("Konnte %1 nicht verarbeiten.").arg(Datei),LOG_DEBUG));
		}
	}
	if (!ModulGefunden)
		Melden(Meldung("a19f89878e354485aa86f4e54cd0e76d",tr("Das Modul %1 konnte nicht gefunden werden.").arg(modulname),LOG_CRIT));
	return ModulGefunden;
}
void Steuerung::SensorenAbgeschaltet()
{
	Melden(Meldung("88d3484ea9ea4739bd0e345db5ac0caa",tr("Sensor abgeschaltet"),LOG_DEBUG));
	QCoreApplication::quit();
}

/*!
  \brief Emfängt das Unix Signal SIGTERM.

  Anschließend wird es via Unix Socked an das Programm übergeben.
*/
void Steuerung::Signal_SIGTERM_Verwaltung(int)
{
	char a = 1;
	::write(SIGTERM_Socked[0], &a, sizeof(a));
}
