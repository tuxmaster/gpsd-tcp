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

#ifndef DUMMY_H
#define DUMMY_H

#include <QtCore>

class Dummy : public QObject
{
	Q_OBJECT
	public:
		explicit Dummy(QObject *eltern = 0);

	Q_SIGNALS:
		void	Daten(const QString &daten);

	private Q_SLOTS:
		void	DatenSenden();

};

#endif // DUMMY_H
