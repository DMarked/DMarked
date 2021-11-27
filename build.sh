#/usr/bin/env bash

if [[ $EUID -ne 0 ]]; then
   echo "Error:This script must be run as root!" 1>&2
   exit 1
fi

debuild -uc -us -tc -b
