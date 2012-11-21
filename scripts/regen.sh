#!/bin/sh

OSNAME="$(uname -s)"
if [ "x${OSNAME}" = "xOpenBSD" ]; then
        echo "[+] configuring for ${OSNAME}"
        ./configure LDFLAGS=-L/usr/local/lib
elif [ "x${OSNAME}" = "xDarwin" ]; then
        echo "[+] configuring for ${OSNAME}"
        ./configure LDFLAGS=-L/usr/local/lib
else
        echo "[+] using generic configuration"
        ./configure
fi
