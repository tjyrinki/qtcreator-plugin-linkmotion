Name:       %ProjectName%

# >> macros
# << macros

Summary:    LinkMotion IVI example
Version:    1.0.0
Release:    1%{?dist}
Group:      User Interface/Desktops
License:    Proprietary
URL:        https://lm-cgit.nomovok.info/cgit.cgi/lmmw-ivi-sdkexample
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5QuickTest)
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  pkgconfig(Qt5Multimedia)
BuildRequires:  pkgconfig(Qt5Concurrent)
BuildRequires:  qt5-qtdeclarative-import-qtquick2plugin

%description
LinkMotion IVI example application

%package %ProjectName%-ivi-app
Group:      User Interface/Desktops
Requires:   %{name}
Summary:    LinkMotion IVI example application

%description %ProjectName%-ivi-app
LinkMotion IVI example application

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre
qmake -r CONFIG+=declarative_debug CONFIG+=qml_debug CONFIG+=force_debug_info
make

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
make install INSTALL_ROOT=%{buildroot}

# >> install post
# << install post

%files
%defattr(-,root,root,-)
/altdata/apps/*
# >> files
# << files

%changelog
* Thu Jul 14 2016 FirstName Surname <firstname.surname@example.com> - 1.0
- Initial release
