#!/bin/sh
exec setarch i686 -R /opt/wp80/wpbin/wp -c ${COLUMNS:-$(tput cols)} -r ${LINES:-$(tput lines)} -t ${WPTERM:-xterm} "${@}"
