%if 0%{?fedora}
%global _my_debug CONFIG+=debug
%endif
Name:		gpsd-tcp
Version:	0.1.0
Release:	1%{?dist}
Summary:	Additional GPS sources	
Summary(de):	Zusätzliche GPS Quellen

License:	GPLv3
URL:		https://github.com/tuxmaster/gpsd-tcp
Source0:	%{name}-%{version}.tar.xz

BuildRequires:	systemd, doxygen, graphviz
%if 0%{?rhel}
BuildRequires:	application(qt5-linguist.desktop)
%else
BuildRequires:	cmake(Qt5LinguistTools)
%endif
BuildRequires:	pkgconfig(Qt5SerialPort) => 5.5, pkgconfig(libsystemd)
%{?systemd_requires}

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
Requires:	%{name}%{?_isa} = %{version}-%{release}

%description devel
The files for develop additional modules.

%description -l de devel 
Die Dateien um neue Module zu entwickeln.

%package dummy
Summary:	The dummy module
Summary(de):	Das Testmodul
Requires:	%{name}%{?_isa} = %{version}-%{release}

%description dummy
The dummy module for testing.

%description -l de dummy
Ein Dummy der zum Testen dient.

%package EM7345
Summary:	The EM7345 module
Summary(de):	Das EM7345 Module
Requires:	%{name}%{?_isa} = %{version}-%{release}

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
%autosetup

%build
qmake-qt5 %{?_my_debug}
make %{?_smp_mflags}
doxygen
sed -i 's|XXX|%{_libdir}/%{name}|g' gpsd-tcp.conf

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
%doc LICENSE README.md Änderungen
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
* Sun Mar 24 2019 terrortux <gott@terrortux.de> 0.1.0-1
- Add change log
- More qt5 specific
- Spec file support for RHEL >=7 and Fedora >=28

* Sun Oct 4 2015 terrortux <gott@terrortux.de> 0.0.1-1
- start
