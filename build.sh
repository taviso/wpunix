#!/bin/bash
#
# This is a script to build a deb of WordPerfect 7.0 for UNIX.
#
# Tavis Ormandy <taviso@gmail.com>, July 2022
#

set -x
set -e

declare pkgname="wordperfect7.0_i386"
declare libcver="5.4.46"
declare libmver="5.0.9"
declare prefix="/opt/wp70"

declare wpurl="https://winworldpc.com/product/wordperfect/7x-unix"

declare tmpfile="$(mktemp)"
declare topdir="${PWD}"

declare libcurl="https://mirrors.edge.kernel.org/pub/linux/libs/libc5/old/libc-${libcver}.bin.tar.gz"
declare ldsourl="http://ftp.us.debian.org/debian/pool/main/g/glibc/libc6_2.24-11+deb9u4_i386.deb"

# Precompiled with gcc2.7/glibc2.0.7/binutils2.9.1
declare wp7curl="https://lock.cmpxchg8b.com/files/libcompat-wp7.tar.gz"

test -f "${topdir}/COREL_WPUNIX.iso" || {
    echo "You need to download COREL_WPUNIX.iso from ${wpurl}"
    exit 1
}

# Required tools.
echo Checking for required tools...
type patchelf
type wget
type objcopy
type ar
type awk
type dpkg-deb
type install

# Setup directory structure.
mkdir -m0755 -p "${pkgname}/DEBIAN"
mkdir -m0755 -p "${pkgname}/${prefix}"
mkdir -m0755 -p "${pkgname}/usr/bin"
mkdir -m0755 -p build

# Make sure the directories have sane permissions.
find "${pkgname}" -type d -exec chmod 0755 {} \;

# Copy over the static files.
cp control "${pkgname}/DEBIAN/control"
cp wp.sh "${pkgname}/usr/bin/wp"

chmod 0755 "${pkgname}/usr/bin/wp"

# Fetch any missing distfiles.
test -f "${libcurl##*/}" || wget "${libcurl}"
test -f "${ldsourl##*/}" || wget "${ldsourl}"
test -f "${wp7curl##*/}" || wget "${wp7curl}"

pushd build

# Extract libraries.
ar xv "../${ldsourl##*/}" data.tar.xz

tar -xvf "../${wp7curl##*/}"
tar -xvf "../${libcurl##*/}" lib/

tar -xvf data.tar.xz ./lib/

ln -fs libc.so.${libcver} lib/libc.so.5
ln -fs libm.so.${libmver} lib/libm.so.5

# Weaken the symbols we want to patch.
objcopy -W _lxstat -W _fxstat -W _xstat lib/libc.so.${libcver} lib/libc.so.${libcver}

# Extract original WP distribution.
7z x "${topdir}/COREL_WPUNIX.iso" LINUX SOLARIS shared

rm LINUX/_{D,P,T}
ln -sf ../SOLARIS/_{D,P,T} LINUX/
ln -sf ../shared LINUX/

# These are only needed during install.
patchelf --set-interpreter "${topdir}/build/lib/ld-linux.so.2" LINUX/_I/wpdecom LINUX/_I/wxar
patchelf --set-rpath '$ORIGIN/../../lib' LINUX/_I/wpdecom LINUX/_I/wxar
patchelf --add-needed libcompat.so LINUX/_I/wpdecom LINUX/_I/wxar
chmod +x LINUX/_I/wxar LINUX/_I/wpdecom

# wxar works in the pwd, so this wrapper makes a tempdir.
function wxar()
{
    local tmpdir="$(mktemp -d)"
    local basedir="$(pwd)"
    local mode="${1}"
    local dest="${2}"

    pushd "${tmpdir}" > /dev/null
        "${basedir}/LINUX/_I/wxar" "${basedir}/${3}"
    popd > /dev/null

    mkdir -p --mode=0755 "${dest}"
    install -D --mode="${mode}" "${tmpdir}"/* "${dest}"
    rm -rf "${tmpdir}"
}

# Remove rockridge junk from 7z.
find -name YMTRANS.TBL -delete

# Decompress files
find LINUX -type f -exec LINUX/_I/wpdecom {} ${tmpfile} \; -exec cp ${tmpfile} {} \;
find SOLARIS -type f -exec LINUX/_I/wpdecom {} ${tmpfile} \; -exec cp ${tmpfile} {} \;

egrep -hv '^# ' shared/ship53 shared/ship7 | cpp -DALL -DLINUX | egrep -v '^# ' > install.txt

# a means archive file
# b means backup (treat like copy)
# c means copy
awk '$4 ~ /^[bc].[bc]/ {
    system(sprintf("install -D --mode=0%s LINUX/%s/%s root/%s/%s",$5,$3,$2,$6,$7));
}' install.txt

awk '$4 ~ /^[bc].a/ {
    printf("0%s LINUX/%s/%s root/%s\n",$5,$3,$2,$6);
}' install.txt | while read mode src dst; do
    wxar "${mode}" "${dst}" "${src}"
done

# Any extras
install -D --mode=0755 LINUX/_B/wpexc root/shbin10/wpexc

# My updated xterm terminal definitions.
cp -f ../xterm.trs root/shlib10/xterm.trs
cp -f ../xterm.trs root/shlib10/xterm-256color.trs

# Set default language.
printf "us\n" > root/shlib10/.def.lang
printf "us\n" > root/wplib/.def.lang

ln -fs wptpl7c.us root/wplib/wptpl.us

# This sets the language.
cp root/wpmacros/us/* root/wpmacros/
cp root/wpexpdocs/us/* root/wpexpdocs/
cp root/wplearn/us/* root/wplearn/

# Cleanup some junk
rm -f root/install.ftp
rm -f root/install.wp
rm -f root/readme.ftp

# Normalize permissions
find root/ -type f -perm /ugo+x -exec chmod 0755 {} \;
find root/ -type f -not -perm /ugo+x -exec chmod 0644 {} \;
find root/ -type d -exec chmod 0755 {} \;

find root -type f -perm -u+x -exec patchelf --set-interpreter "${prefix}/lib/ld-linux.so.2" {} \;
find root/ -type f -perm -u+x -exec patchelf --set-rpath "${prefix}/lib" {} \;
find root/ -type f -perm -u+x -exec patchelf --add-needed libcompat.so {} \;

# Copy over the runtime libraries
mv lib root

# Normalize permissions
find root/ -type f -perm /ugo+x -exec chmod 0755 {} \;
find root/ -type f -not -perm /ugo+x -exec chmod 0644 {} \;
find root/ -type d -exec chmod 0755 {} \;

# Install into prefix
mv root/* "${topdir}/${pkgname}/${prefix}/"

# Return to topdir
popd

# Install default configuration.
install -D --mode=0600 /dev/null ${pkgname}/${prefix}/shlib10/.wp7.lm
install -D --mode=0600 /dev/null ${pkgname}/${prefix}/wplib/.wp7c.set

# Create deb file
dpkg-deb --build --root-owner-group "${pkgname}"

# Cleanup
rm -rf build "${tmpfile}" "${pkgname}"

ls -l "${pkgname}.deb"
