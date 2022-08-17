#!/bin/sh
exec /opt/wp80/wpbin/wp -c ${COLUMNS:-$(tput cols)} -r ${LINES:-$(tput lines)} -t xterm "${@}"
