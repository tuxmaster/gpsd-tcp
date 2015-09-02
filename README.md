# gpsd-tcp
Daemon to feed the gpsd with position values form various sources via Plug-Ins.

#Requirements/Voraussetzungen
- Qt >= 5.4.2
- systemd >= 216

For/Für Fedora >= 21<br>
yum install  qt5-qtserialport-devel qt5-qtbase-devel qt5-qttools-devel systemd-devel

#Build/Erstellen
qmake-qt5 gpsd-tcp.pro<br>
make

#Optional for the documentation/Option für die Dokumentation
make -f gpsd-tcp/Makefile Dokumentation
