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

#ifndef MELDUNG_H
#define MELDUNG_H

#include <QtCore>

#include <syslog.h>

class Meldung
{
	public:
		Meldung();
		Meldung(const QString& id,const QString &text)
		{
			K_ID=id;
			K_Text=text;
			K_Prioritaet=LOG_INFO;
		}
		Meldung(const QString& id,const QString &text,const int &prioritaet) :Meldung(id,text)
		{
			K_Prioritaet=prioritaet;
		}
		static const QString Textprio(const int &prio)
		{
			QString Rueckgabe;
			switch (prio)
			{
				case LOG_EMERG:
					Rueckgabe=QObject::tr("Notfall");
					break;
				case LOG_ALERT:
					Rueckgabe=QObject::tr("Alarm");
					break;
				case LOG_CRIT:
					Rueckgabe=QObject::tr("Kritisch");
					break;
				case LOG_ERR:
					Rueckgabe=QObject::tr("Fehler");
					break;
				case LOG_WARNING:
					Rueckgabe=QObject::tr("Warnung");
					break;
				case LOG_NOTICE:
					Rueckgabe=QObject::tr("Hinweis");
					break;
				case LOG_INFO:
					Rueckgabe=QObject::tr("Information");
					break;
				case LOG_DEBUG:
					Rueckgabe=QObject::tr("Debug");
					break;
				default:
					Rueckgabe=QObject::tr("Unbekannt");
					break;
			}
			return Rueckgabe;
		}
		const QString	&TextHolen() const {return K_Text;}
		const QString	&IDHolen() const {return K_ID;}
		const int		&PrioritaetHolen() const {return K_Prioritaet;}
	private:
		QString			K_ID;
		QString			K_Text;
		int				K_Prioritaet;
};
static QDebug operator<<(QDebug debug, const Meldung &m)
{
	QDebugStateSaver saver(debug);
	debug.nospace() << '(' << m.IDHolen() << ", " << m.TextHolen() << ", "<< Meldung::Textprio(m.PrioritaetHolen())<<')';
	return debug;
}
#endif // MELDUNG_H
