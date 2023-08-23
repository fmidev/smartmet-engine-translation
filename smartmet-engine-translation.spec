%define DIRNAME translation
%define LIBNAME smartmet-%{DIRNAME}
%define SPECNAME smartmet-engine-%{DIRNAME}
Summary: SmartMet Translation Engine
Name: %{SPECNAME}
Version: 23.8.23
Release: 1%{?dist}.fmi
License: FMI
Group: SmartMet/Engines
URL: https://github.com/fmidev/smartmet-engine-translation
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%if 0%{?rhel} && 0%{rhel} < 9
%define smartmet_boost boost169
%else
%define smartmet_boost boost
%endif

%define smartmet_fmt_min 8.1.1
%define smartmet_fmt_max 8.2.0

BuildRequires: %{smartmet_boost}-devel
BuildRequires: bzip2-devel
BuildRequires: fmt-devel >= %{smartmet_fmt_min}, fmt-devel < %{smartmet_fmt_max}
BuildRequires: gcc-c++
BuildRequires: gdal35-devel
BuildRequires: libatomic
BuildRequires: make
BuildRequires: rpm-build
BuildRequires: smartmet-library-macgyver-devel >= 23.7.28
BuildRequires: smartmet-library-spine-devel >= 23.8.23
BuildRequires: smartmet-library-timeseries-devel >= 23.7.28
BuildRequires: sqlite3pp-devel >= 1.0.9
BuildRequires: curl-devel >= 7.61.0
BuildRequires: smartmet-utils-devel >= 23.7.17
BuildRequires: zlib-devel
Requires: %{smartmet_boost}-date-time
Requires: %{smartmet_boost}-iostreams
Requires: %{smartmet_boost}-locale
Requires: %{smartmet_boost}-serialization
Requires: %{smartmet_boost}-system
Requires: %{smartmet_boost}-thread
Requires: fmt >= %{smartmet_fmt_min}, fmt < %{smartmet_fmt_max}
Requires: gdal35-libs
Requires: libatomic
Requires: smartmet-library-macgyver >= 23.7.28
Requires: smartmet-library-spine >= 23.8.23
Requires: smartmet-library-timeseries >= 23.7.28
Requires: smartmet-server >= 23.7.28
Requires: unixODBC

%if %{defined el7}
Requires: libpqxx < 1:7.0
BuildRequires: libpqxx-devel < 1:7.0
Requires: sqlite33-libs >= 3.30.1
BuildRequires: sqlite33-devel >= 3.30.1
BuildRequires: libspatialite43-devel
#TestRequires: libspatialite43-devel
#TestRequires: sqlite33-devel >= 3.30.1
#TestRequires: catch-devel >= 1.9.6
#TestRequires: smartmet-utils-devel >= 23.7.17
%else
%if 0%{?rhel} && 0%{rhel} >= 8
Requires: libpqxx >= 7.7.0 libpqxx < 1:7.8.0
Requires: sqlite-libs >= 3.22.0
BuildRequires: sqlite-devel >= 3.22.0
BuildRequires: libspatialite50-devel
BuildRequires: librttopo-devel
#TestRequires: libspatialite50-devel
#TestRequires: librttopo-devel
#TestRequires: sqlite-devel >= 3.22.0
#TestRequires: catch-devel >= 2.1.3
BuildRequires: libpqxx-devel >= 7.7.0 libpqxx-devel < 1:7.8.0
%else
Requires: libpqxx
BuildRequires: libpqxx-devel
BuildRequires: libspatialite-devel
BuildRequires: librttopo-devel
Requires: sqlite-libs >= 3.22.0
BuildRequires: sqlite-devel >= 3.22.0
#TestRequires: catch-devel
%endif
%endif

#TestRequires: make
#TestRequires: gcc-c++
#TestRequires: gdal35-devel
#TestRequires: bzip2-devel
#TestRequires: zlib-devel
#TestRequires: smartmet-library-macgyver
#TestRequires: smartmet-library-timeseries
#TestRequires: smartmet-library-timeseries-devel
#TestRequires: smartmet-library-spine
#TestRequires: smartmet-test-data

%if 0%{rhel} >= 8
Requires: libspatialite50
BuildRequires: libspatialite50-devel
#TestRequires: libspatialite50-devel
# pkg-config for libspatialite wants these
BuildRequires: minizip-devel
BuildRequires: freexl-devel
BuildRequires: proj90-devel
BuildRequires: libxml2-devel
#TestRequires: minizip-devel
#TestRequires: freexl-devel
#TestRequires: proj90-devel
#TestRequires: libxml2-devel
%else
Requires: libspatialite43
BuildRequires: libspatialite43-devel
#TestRequires: libspatialite43-devel
%endif

Provides: %{SPECNAME}

%description
SmartMet engine for fetching translations 

%package -n %{SPECNAME}-devel
Summary: SmartMet %{SPECNAME} development headers
Group: SmartMet/Development
Provides: %{SPECNAME}-devel
Requires: %{SPECNAME} = %{version}-%{release}
Requires: smartmet-library-spine-devel >= 23.8.8
%description -n %{SPECNAME}-devel
SmartMet %{SPECNAME} development headers.

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q -n %{SPECNAME}

%build -q -n %{SPECNAME}
make %{_smp_mflags}

%install
%makeinstall

%post
/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files -n %{SPECNAME}
%defattr(0775,root,root,0775)
%{_datadir}/smartmet/engines/%{DIRNAME}.so
%defattr(0664,root,root,0775)

%files -n %{SPECNAME}-devel
%defattr(0664,root,root,0775)
%{_includedir}/smartmet/engines/%{DIRNAME}

%changelog
* Mon Aug 14 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.8.14-1.fmi
- Initial version 

* Wed Aug 23 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.8.23-1.fmi
- Start using Translations-api from spine
