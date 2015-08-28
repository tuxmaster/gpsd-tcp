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

#ifndef ERWEITERUNG_DUMMY_H
#define ERWEITERUNG_DUMMY_H

#include <QObject>
#include <QtPlugin>

#include "../../gpsd-tcp/Quellen/Pluginfabrik.h"

#include "Dummy.h"

class Erweiterung_Dummy : public Plugin
{
	public:
		virtual const QString	Version() const Q_DECL_OVERRIDE {return "0.0.1";}
		virtual const QString	Name()const Q_DECL_OVERRIDE {return "Dummy";}
		virtual QObject			*Erweiterung(QObject *eltern,const QSettings *konfiguration) Q_DECL_OVERRIDE {if (!K_Dummy) K_Dummy=new Dummy(eltern,konfiguration); return K_Dummy;}
	private:
		Dummy					*K_Dummy;
};

class DummyPlugin :public QObject, Pluginfabrik
{
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "de.terrortux.katlin.gpsd-tcp.Pluginfabrik")
		Q_INTERFACES(Pluginfabrik)

		public:
			~DummyPlugin()		{ if(K_Plugin) delete K_Plugin;}
			 Plugin				*plugin(QObject *) Q_DECL_OVERRIDE {if (!K_Plugin) K_Plugin=new Erweiterung_Dummy(); return K_Plugin; }
		private:
			Plugin				*K_Plugin=0;
};

#endif // ERWEITERUNG_DUMMY_H

