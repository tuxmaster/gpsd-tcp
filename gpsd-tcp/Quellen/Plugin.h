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

#ifndef PLUGIN_H
#define PLUGIN_H
#include <QtCore>

class Plugin
{
		public:
			virtual					~Plugin(){}
			virtual const QString	Version() const =0;
			virtual const QString	Name()const =0;
};
Q_DECLARE_INTERFACE(Plugin, "de.terrortux.gpsd-tcp.Plugin")

#endif // PLUGIN_H

