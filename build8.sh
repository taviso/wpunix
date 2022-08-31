#!/bin/bash
#
# This is a script to build a deb of WordPerfect 7.0 for UNIX.
#
# Tavis Ormandy <taviso@gmail.com>, July 2022
#

set -x
set -e

umask 2

declare pkgname="${1:-wordperfect8_i386}"
declare libcver="5.3.12"
declare libmver="5.0.6"
declare prefix="/opt/wp80"

declare wpurl="https://archive.org/details/corel-wpunix-8/"

declare tmpfile="$(mktemp)"
declare topdir="${PWD}"

declare libcurl="https://archive.download.redhat.com/pub/redhat/linux/5.2/en/os/i386/RedHat/RPMS/libc-5.3.12-27.i386.rpm"
declare ldsourl="https://archive.download.redhat.com/pub/redhat/linux/5.2/en/os/i386/RedHat/RPMS/glibc-2.0.7-29.i386.rpm"

test -f "${topdir}/COREL_WPUNIX_1.iso" || {
    echo "You need to download COREL_WPUNIX_1.iso from ${wpurl}"
    exit 1
}

# Required tools.
echo Checking for required tools...
type patchelf
type wget
type objcopy
type rpm2cpio
type cpio
type awk
type install
type bsdtar
type cpp

# Setup directory structure.
mkdir -m0755 -p "${pkgname}/DEBIAN"
mkdir -m0755 -p "${pkgname}/${prefix}"
mkdir -m0755 -p "${pkgname}/usr/bin"
mkdir -m0755 -p build

# Make sure the directories have sane permissions.
find "${pkgname}" -type d -exec chmod 0755 {} \;

# Copy over the static files.
cp control "${pkgname}/DEBIAN/control"
cp wp8.sh "${pkgname}/usr/bin/wp"

chmod 0755 "${pkgname}/usr/bin/wp"

# Fetch any missing distfiles.
test -f "${libcurl##*/}" || wget "${libcurl}"
test -f "${ldsourl##*/}" || wget "${ldsourl}"

pushd build

# Extract libraries.
rpm2cpio < "../${ldsourl##*/}" | cpio -d -i '*/ld-*'
rpm2cpio < "../${libcurl##*/}" | cpio -d -i '*/lib[cm].*'

cp ../libcompat.so lib/
cp ../libwppatch.so lib/

mv usr/i486-linux-libc5/lib/* lib/
rm -rf usr

ln -fs libc.so.${libcver} lib/libc.so.5
ln -fs libm.so.${libmver} lib/libm.so.5

# Weaken the symbols we want to patch.
objcopy -W geteuid      \
        -W getuid       \
        -W getcwd       \
        -W _lxstat      \
        -W _fxstat      \
        -W _xstat       \
        -W _init        \
        lib/libc.so.${libcver} lib/libc.so.${libcver}

# Extract original WP distribution.
bsdtar xf "${topdir}/COREL_WPUNIX_1.iso" linux/ solaris/ sco/ shared/

rm -rf linux/{dat,prn,trm,lng}
ln -sf ../solaris/{dat,prn,trm} linux/
ln -sf ../sco/lng linux/
ln -sf ../shared linux/

# These are only needed during install.
# This loader is too old to support DT_RPATH :)
patchelf --set-interpreter "${topdir}/build/lib/ld-linux.so.2" linux/ins/wpdecom
patchelf --set-interpreter "${topdir}/build/lib/ld-linux.so.2" linux/ins/wxar
patchelf --add-needed "${topdir}/build/lib/libcompat.so" linux/ins/wpdecom
patchelf --add-needed "${topdir}/build/lib/libcompat.so" linux/ins/wxar
patchelf --replace-needed "libm.so.5" "${topdir}/build/lib/libm.so.5" linux/ins/wpdecom
patchelf --replace-needed "libm.so.5" "${topdir}/build/lib/libm.so.5" linux/ins/wxar
patchelf --replace-needed "libc.so.5" "${topdir}/build/lib/libc.so.5" linux/ins/wpdecom
patchelf --replace-needed "libc.so.5" "${topdir}/build/lib/libc.so.5" linux/ins/wxar
chmod +x linux/ins/wxar linux/ins/wpdecom

# wxar works in the pwd, so this wrapper makes a tempdir.
function wxar()
{
    local tmpdir="$(mktemp -d)"
    local basedir="$(pwd)"
    local mode="${1}"
    local dest="${2}"

    pushd "${tmpdir}" > /dev/null
        "${basedir}/linux/ins/wxar" "${basedir}/${3}"
    popd > /dev/null

    mkdir -p --mode=0755 "${dest}"
    install -D --mode="${mode}" "${tmpdir}"/* "${dest}"
    rm -rf "${tmpdir}"
}

# Remove leftover rockridge junk
find -name YMTRANS.TBL -delete

# Decompress files
find linux -type f -exec linux/ins/wpdecom {} ${tmpfile} \; -exec cp ${tmpfile} {} \;
find solaris -type f -exec linux/ins/wpdecom {} ${tmpfile} \; -exec cp ${tmpfile} {} \;
find sco -type f -exec linux/ins/wpdecom {} ${tmpfile} \; -exec cp ${tmpfile} {} \;

egrep -hv '^#[ s]' shared/ship | cpp -Wno-endif-labels -DALL -Dlinux | egrep -v '^# ' > install.txt

# a means archive file
# b means backup (treat like copy)
# c means copy
awk '$4 ~ /^[bc].[bc]/ {
    system(sprintf("install -D --mode=0%s linux/%s/%s root/%s/%s",$5,$3,$2,$6,$7));
}' install.txt

awk '$4 ~ /^[bc].a/ {
    printf("0%s linux/%s/%s root/%s\n",$5,$3,$2,$6);
}' install.txt | while read mode src dst; do
    wxar "${mode}" "${dst}" "${src}"
done

# Any extras
install -D --mode=0755 linux/bin/wpexc root/shbin10/wpexc

# My updated xterm terminal definitions.
cp -f ../xterm.trs root/shlib10/xterm.trs
cp -f ../xterm.trs root/shlib10/xterm-256color.trs
cp -f ../xterm-nomouse.trs root/shlib10/xterm-nomouse.trs

# Add some sample macros.
cp -f ../macro/macros/*.wpm root/wpmacros/

# Add macro include files.
mkdir --mode=0755 root/wpmacros/include
cp -f ../macro/macros/*.h root/wpmacros/include/

# Ghostscript Printer Resources
cp -f ../gscript.all root/shlib10/gscript.all

# Set default language.
printf "us\n" > root/shlib10/.def.lang
printf "us\n" > root/wplib/.def.lang

ln -fs wptplc.us root/wplib/wptpl.us

# This sets the language.
ln -rs root/wpmacros/us/* root/wpmacros/
ln -rs root/wpexpdocs/us/* root/wpexpdocs/
ln -rs root/wplearn/us/* root/wplearn/

# Cleanup some junk
rm -f root/install.ftp
rm -f root/install.wp
rm -f root/readme.ftp

# These dont seem useful.
rm -f root/shbin10/ptrinst
rm -f root/shbin10/wpxtool
rm -f root/shared/wpinstg
rm -f root/shared/mm
rm -f root/shared/ship
rm -f root/shlib10/readme.ftp

# Normalize permissions
find root/ -type f -perm /ugo+x -exec chmod 0755 {} \;
find root/ -type f -not -perm /ugo+x -exec chmod 0644 {} \;
find root/ -type d -exec chmod 0755 {} \;

find root/ -type f -perm -u+x -exec patchelf --set-interpreter "${prefix}/lib/ld-linux.so.2" {} \;
find root/ -type f -perm -u+x -exec patchelf --add-needed "${prefix}/lib/libcompat.so" {} \;
find root/ -type f -perm -u+x -exec patchelf --replace-needed libm.so.5 "${prefix}/lib/libm.so.5" {} \;
find root/ -type f -perm -u+x -exec patchelf --replace-needed libc.so.5 "${prefix}/lib/libc.so.5" {} \;

# Load our patch module into wp
patchelf --add-needed "${prefix}/lib/libwppatch.so" root/wpbin/wp

# Fix the tools we used during installation.
patchelf --replace-needed "${topdir}/build/lib/libm.so.5" "${prefix}/lib/libm.so.5" root/shared/wpdecompress root/shared/wxar
patchelf --replace-needed "${topdir}/build/lib/libc.so.5" "${prefix}/lib/libc.so.5" root/shared/wpdecompress root/shared/wxar
patchelf --remove-needed "${topdir}/build/lib/libcompat.so" root/shared/wpdecompress root/shared/wxar

# Copy over the runtime libraries
mv lib root

# Normalize those permissions
chmod -R 0755 root/lib

# Add any extra utility programs.
install -D --mode=0755 ../macro/mactool root/shbin10/mactool
install -D --mode=0755 ../util/grsutil root/shbin10/grsutil

# Remove any debugging symbols from our code.
strip root/shbin10/mactool
strip root/shbin10/grsutil
strip root/lib/libwppatch.so

# Install into prefix
mv root/* "${topdir}/${pkgname}/${prefix}/"

# Return to topdir
popd

# Install default configuration.
install -D --mode=0600 /dev/null ${pkgname}/${prefix}/shlib10/.wp8.lm
install -D --mode=0644 wp7c.set ${pkgname}/${prefix}/wplib/.wp8c.set

# Cleanup
rm -rf build "${tmpfile}"
