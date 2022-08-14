Name:           wordperfect7
Version:        0.06
Release:        1%{?dist}
Summary:        WordPerfect for UNIX (TM) Character Terminals
License:        Abandonware
URL:            https://github.com/taviso/wpunix
Source0:        https://github.com/taviso/wpunix/archive/refs/tags/v%{version}.tar.gz
Source1:        https://archive.download.redhat.com/pub/redhat/linux/7.2/en/os/i386/RedHat/RPMS/libc-5.3.12-27.i386.rpm
Source2:        https://archive.download.redhat.com/pub/redhat/linux/5.2/en/os/i386/RedHat/RPMS/glibc-2.0.7-29.i386.rpm
Source3:        https://archive.org/download/corel-word-perfect-7-for-unix-linux-7.0-1998-06-english-cd/Corel%20WordPerfect%207%20for%20Unix%20Linux%20%287.0%29%20%281998-06%29%20%5BEnglish%5D%20%28CD%29.zip/Corel%20WordPerfect%207%20for%20Unix%20Linux%20%287.0%29%20%281998-06%29%20%5BEnglish%5D%20%28CD%29%2FISO%2FCOREL_WPUNIX.iso
Requires:       glibc(x86-32)

# Instructions:
# $ spectool -g -R wordperfect7.spec
# $ QA_RPATHS=$(( 0x0001|0x0010 )) rpmbuild --target i386 -bb wordperfect7.spec
#

%define __strip /bin/true

%description
WordPerfect is a word processing application for character terminals.

%global debug_package %{nil}

%prep
%setup -q -n wpunix-%{version}

cp %{SOURCE1} %{SOURCE2} .
cp %{SOURCE3} COREL_WPUNIX.iso

%build
make %{name}.0_i386

%install
cp -r %{name}.0_i386/opt %{buildroot}
cp -r %{name}.0_i386/usr %{buildroot}

%files
/opt
/usr/bin/wp

%changelog
* Sat Aug 13 2022 Tavis Ormandy <taviso@gmail.com>
- Renamed to match deb.
* Sat Aug 13 2022 Tavis Ormandy <taviso@gmail.com>
- Initial Version
