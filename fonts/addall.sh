#!/bin/bash
#
# This is a script to merge an ADD file into an ALL file using dosemu.
#
declare dosflags="-quiet -dumb"
declare addflags=""
declare allfile=""

if test $# -lt 2; then
    echo "usage: addall <allfile> <afmfile...>" 1>&2
    exit 1
fi

allfile=${1} && shift

for i in ${@}; do
    echo "Translating ${i} to .ADD..."

    # First clean up any old files
    rm -f *.TYP *.KRN *.MAP *.SPC *.ADD

    # Translate the AFM file
    if ! ./afm2add ${i}; then
        echo "Translation of ${i} failed, skipping." 1>&2
        continue
    fi

    # Try to merge it into the ALL file.
    for addfile in *.ADD; do
        echo "Attempting to merge ${addfile} into ${allfile}..."
        if ! dosemu ${dosflags} -E "IN G:/ ADDALL ${addflags} ${addfile} ${allfile}"; then
            echo "Merge failed." 1>&2
            exit 1
        else
            echo "Seems successful."
        fi
    done
done
