Name:           wordperfect8
Version:        0.10
Release:        1%{?dist}
Summary:        WordPerfect for UNIX (TM) Character Terminals
License:        Abandonware
URL:            https://github.com/taviso/wpunix
#Source0:       https://github.com/taviso/wpunix/archive/refs/heads/main.tar.gz
Source0:        https://github.com/taviso/wpunix/archive/v%{version}/%{name}-%{version}.tar.gz
Source1:        https://archive.download.redhat.com/pub/redhat/linux/5.2/en/os/i386/RedHat/RPMS/libc-5.3.12-27.i386.rpm
Source2:        https://archive.download.redhat.com/pub/redhat/linux/5.2/en/os/i386/RedHat/RPMS/glibc-2.0.7-29.i386.rpm
Source3:        https://archive.org/download/corel-wpunix-8/COREL_WPUNIX_1.iso
ExclusiveArch:  i686
BuildRequires:  bsdtar
BuildRequires:  gcc
BuildRequires:  gdb
BuildRequires:  glibc-devel%{_isa}
BuildRequires:  glibc-static%{_isa}
BuildRequires:  make
BuildRequires:  patchelf
# just to avoid patching build script, not used for build
BuildRequires:  wget
Provides:       bundled(glibc) = 2.0.7-27
Provides:       bundled(libc) = 5.3.12-29

# Instructions:
# $ spectool -g -R wordperfect8.spec
# $ QA_RPATHS=$(( 0x0001|0x0010 )) rpmbuild --target i386 -bb wordperfect8.spec
#

%define __strip /bin/true

%description
WordPerfect is a word processing application for character terminals.

%global debug_package %{nil}

%prep
#%setup -q -n wpunix-main
%setup -q -n wpunix-%{version}

cp %{SOURCE1} %{SOURCE2} %{SOURCE3} .

%build
make %{name}_i386

%install
cp -r %{name}_i386/opt %{buildroot}
cp -r %{name}_i386/usr %{buildroot}

%files
/opt
/usr/bin/wp

%changelog
* Tue Sep 27 2022 Dominik Mierzejewski <dominik@greysector.net> - 0.10-1
- make version match actual tag in github release
- fix typo in libc download URL
- use a nicer source download URL
- add missing BuildRequires and appropriate ExclusiveArch

* Sat Aug 13 2022 Tavis Ormandy <taviso@gmail.com>
- Renamed to match deb.
* Sat Aug 13 2022 Tavis Ormandy <taviso@gmail.com>
- Initial Version
