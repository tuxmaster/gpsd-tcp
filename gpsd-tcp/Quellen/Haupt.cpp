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

#include <QtCore>

#include <signal.h>

#include "Vorgaben.h"
#include "Steuerung.h"

void Signalsteuerung(int signal)
{
	if(signal==SIGTERM)
	{
		QCoreApplication::quit();
	}
}

int main(int argc, char *argv[])
{
		QCoreApplication Qt(argc, argv);
		signal(SIGTERM,Signalsteuerung);

		QTranslator QtUebersetzung;
		QTranslator ProgrammUebersetzung;

		Qt.setApplicationVersion(VERSION);
		Qt.setApplicationName(PROGRAMM);

		QtUebersetzung.load(QString("qt_%1").arg(QLocale::system().name()),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
		ProgrammUebersetzung.load(QString("%1_%2").arg(PROGRAMM).arg(QLocale::system().name()),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
		if((ProgrammUebersetzung.isEmpty()) && (!QLocale::system().name().startsWith("de")))
			ProgrammUebersetzung.load(QString("%1_en").arg(PROGRAMM),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
		else
			Qt.installTranslator(&QtUebersetzung);
		Qt.installTranslator(&ProgrammUebersetzung);

		Steuerung s;
		return Qt.exec();
}
