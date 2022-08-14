Name:           wpunix7c
Version:        0.05
Release:        1%{?dist}
Summary:        WordPerfect for UNIX (TM) Character Terminals
License:        Abandonware
URL:            https://github.com/taviso/wpunix
Source0:        https://github.com/taviso/wpunix/archive/refs/tags/v%{version}.tar.gz
Source1:        https://archive.download.redhat.com/pub/redhat/linux/7.2/en/os/i386/RedHat/RPMS/libc-5.3.12-27.i386.rpm
Source2:        https://archive.download.redhat.com/pub/redhat/linux/5.2/en/os/i386/RedHat/RPMS/glibc-2.0.7-29.i386.rpm
Source3:        https://archive.org/download/corel-word-perfect-7-for-unix-linux-7.0-1998-06-english-cd/Corel%20WordPerfect%207%20for%20Unix%20Linux%20%287.0%29%20%281998-06%29%20%5BEnglish%5D%20%28CD%29.zip/Corel%20WordPerfect%207%20for%20Unix%20Linux%20%287.0%29%20%281998-06%29%20%5BEnglish%5D%20%28CD%29%2FISO%2FCOREL_WPUNIX.iso
Requires:       glibc(x86-32)

%define __strip /bin/true

%description
WordPerfect is a word processing application.

%global debug_package %{nil}

%prep
%setup -q -n wpunix-%{version}
cp %{_topdir}/SOURCES/libc-5.3.12-27.i386.rpm .
cp %{_topdir}/SOURCES/glibc-2.0.7-29.i386.rpm .
cp %{_topdir}/SOURCES/Corel%20WordPerfect%207%20for%20Unix%20Linux%20%287.0%29%20%281998-06%29%20%5BEnglish%5D%20%28CD%29%2FISO%2FCOREL_WPUNIX.iso COREL_WPUNIX.iso

%build
make wordperfect7.0_i386
rm -f wordperfect7.0_i386/opt/wp70/shbin10/ptrinst
rm -f wordperfect7.0_i386/opt/wp70/shbin10/wpxtool
rm -f wordperfect7.0_i386/opt/wp70/shared/wpinstg

%install
cp -r wordperfect7.0_i386/opt %{buildroot}
cp -r wordperfect7.0_i386/usr %{buildroot}

%files
/opt
/usr/bin/wp

%changelog
* Sat Aug 13 2022 Tavis Ormandy <taviso@gmail.com>
- Initial Version
