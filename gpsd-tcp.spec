Name:		gpsd-tcp		
Version:	0.0.1
Release:	1%{?dist}
Summary:	Additional GPS sources	
Summary(de):	Zusätzliche GPS Quellen

Group:		Applications/Communications	
License:	GPLv3
URL:		https://github.com/tuxmaster/gpsd-tcp
Source0:	https://github.com/tuxmaster/gpsd-tcp/releases/v%{version}.tar.gz

BuildRequires:	systemd, doxygen, graphviz, qt5-qtserialport-devel >= 5.5, systemd-devel, qt5-linguist	

Requires(post):		systemd
Requires(preun):	systemd
Requires(postun):	systemd

%description
Provides an service that can deliver GPS information's
from various devices.

%description -l de
Stellt ein Dienst bereit, der verschiedene GPS Quellen
für den gpsd bereit stellen kann. Dieser lässt sich
über Module erweitert.

%package devel
Summary:	Files for development
Summary(de):	Dateien für die Entwicklung
Requires:	%{name}-%{version}

%description devel
The files for develop additional modules.

%description -l de devel 
Die Dateien um neue Module zu entwickeln.

%package dummy
Summary:	The dummy module
Summary(de):	Das Testmodul
Requires:	%{name}-%{version}

%description dummy
The dummy module for testing.

%description -l de dummy
Ein Dummy der zum Testen dient.

%package EM7345
Summary:	The EM7345 module
Summary(de):	Das EM7345 Module
Requires:	%{name}-%{version}

%description EM7345
The module for the EM7345 LTE modem.

%description -l de EM7345
Das Modul für das EM7345 LTE Modem.

%post
%systemd_post gpsd-tcp.service

%preun
%systemd_preun gpsd-tcp.service

%postun
%systemd_postun_with_restart gpsd-tcp.service

%postun dummy
%systemd_postun_with_restart gpsd-tcp.service

%postun EM7345
%systemd_postun_with_restart gpsd-tcp.service

%prep
%autosetup -n %{name}

%build
qmake-qt5
make %{?_smp_mflags}
doxygen
sed 's|XXX|%{_libdir}/%{name}|g' gpsd-tcp.conf > gpsd-tcp.conf.neu
mv gpsd-tcp.conf.neu gpsd-tcp.conf

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}%{_defaultdocdir}/%{name}
mkdir -p %{buildroot}%{_libdir}/%{name}
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_sysconfdir}
mkdir -p %{buildroot}%{_qt5_translationdir}
mkdir -p %{buildroot}%{_includedir}/%{name}
mkdir -p %{buildroot}%{_unitdir}
cp -r Dokumentation %{buildroot}%{_defaultdocdir}/%{name}/
cp Plug-Ins/*.so %{buildroot}%{_libdir}/%{name}/
cp gpsd-tcp/gpsd-tcp %{buildroot}%{_bindir}/
cp gpsd-tcp.conf %{buildroot}%{_sysconfdir}/
cp gpsd-tcp/Uebersetzungen/*.qm %{buildroot}%{_qt5_translationdir}/
cp Plug-Ins/EM7345/Uebersetzungen/*.qm %{buildroot}%{_qt5_translationdir}/
cp gpsd-tcp/Quellen/Pluginfabrik.h %{buildroot}%{_includedir}/%{name}/
cp gpsd-tcp/Quellen/Plugin.h %{buildroot}%{_includedir}/%{name}/
cp gpsd-tcp/Quellen/Meldung.h %{buildroot}%{_includedir}/%{name}/
cp gpsd-tcp.service  %{buildroot}%{_unitdir}/


%files
%exclude %{_defaultdocdir}/%{name}/Dokumentation
%doc LICENSE README.md
%{_bindir}/gpsd-tcp
%config(noreplace) %{_sysconfdir}/gpsd-tcp.conf
%{_qt5_translationdir}/gpsd-tcp_*
%{_unitdir}/*

%files devel
%{_defaultdocdir}/%{name}/Dokumentation
%{_includedir}/%{name}

%files dummy
%{_libdir}/%{name}/libdummy.so

%files EM7345
%{_libdir}/%{name}/libem7345.so
%{_qt5_translationdir}/EM7345*

%changelog
* Sun Oct 4 2015 terrortux <gott@terrortux.de> 0.0.1-1
- start
