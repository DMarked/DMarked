#/usr/bin/env bash

# You need install `fakeroot`, to avoid run as root!

if ! type debuild >/dev/null 2>&1; then
   apt install devscripts
fi

debuild -uc -us -tc -b
