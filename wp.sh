#!/bin/sh
exec /opt/wp70/wpbin/wp -c ${COLUMNS:-$(tput cols)} -r ${LINES:-$(tput lines)} -t xterm "${@}"
