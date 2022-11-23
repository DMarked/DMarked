#/usr/bin/env bash
if [[ $EUID -ne 0 ]]; then
   echo "Error:This script must be run as root!" 1>&2
   exit 1
fi
if ! type debuild >/dev/null 2>&1; then
   apt install devscripts
fi

debuild -uc -us -tc -b
