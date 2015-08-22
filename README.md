# gpsd-tcp
Daemon to feed the gpsd with position values form various sources via Plug-Ins.

Requirements/Voraussetzungen
- Qt >= 5.4.2
- systemd >= 216

For/Für Fedora >=21
yum install  qt5-qtserialport-devel qt5-qtbase-devel systemd-devel

Build/Erstellen
qmake-qt5 gpsd-tcp.pro
