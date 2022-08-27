Name:           wordperfect8
Version:        0.09
Release:        1%{?dist}
Summary:        WordPerfect for UNIX (TM) Character Terminals
License:        Abandonware
URL:            https://github.com/taviso/wpunix
#Source0:       https://github.com/taviso/wpunix/archive/refs/heads/main.tar.gz
Source0:        https://github.com/taviso/wpunix/archive/refs/tags/v%{version}.tar.gz
Source1:        https://archive.download.redhat.com/pub/redhat/linux/7.2/en/os/i386/RedHat/RPMS/libc-5.3.12-27.i386.rpm
Source2:        https://archive.download.redhat.com/pub/redhat/linux/5.2/en/os/i386/RedHat/RPMS/glibc-2.0.7-29.i386.rpm
Source3:        https://archive.org/download/corel-wpunix-8/COREL_WPUNIX_1.iso
Requires:       glibc(x86-32)

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
* Sat Aug 13 2022 Tavis Ormandy <taviso@gmail.com>
- Renamed to match deb.
* Sat Aug 13 2022 Tavis Ormandy <taviso@gmail.com>
- Initial Version
