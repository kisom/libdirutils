#!/bin/sh
# copied from surfraw

set -e

#echo "[+] creating the damn texinfo file"
#cd docs && ./texinfo_gen.sh && cd ..

echo "[+] setting up automake"
# if AUTOMAKE + ACLOCAL are not set, explicity search
# for automake-1.{10,11,12} and aclocal-1.{10,11,12}
if [ -z "$AUTOMAKE_VERSION" ]; then
    if [ -x "`which automake-1.12 2>/dev/null`" ] &&
         [ -x "`which aclocal-1.12 2>/dev/null`" ]; then
        echo "    [*] found automake 1.12"
        AUTOMAKE=automake-1.12
        ACLOCAL=aclocal-1.12
        AUTOMAKE_VERSION=1.12
        export AUTOMAKE ACLOCAL
    elif [ -x "`which automake-1.11 2>/dev/null`" ] &&
         [ -x "`which aclocal-1.11 2>/dev/null`" ]; then
        echo "    [*] found automake 1.11"
        AUTOMAKE=automake-1.11
        ACLOCAL=aclocal-1.11
        AUTOMAKE_VERSION=1.11
        export AUTOMAKE ACLOCAL AUTOMAKE_VERSION
    elif [ -x "`which automake-1.10 2>/dev/null`" ] &&
         [ -x "`which aclocal-1.10 2>/dev/null`" ]; then
        echo "    [*] found automake 1.10"
        AUTOMAKE=automake-1.10
        ACLOCAL=aclocal-1.10
        AUTOMAKE_VERSION=1.10
        export AUTOMAKE ACLOCAL
     else
        echo "    [*] using default automake"
     fi
    # else use default
fi

echo "[+] setting up autoconf"
# if AUTOCONF is not set, explicity search for autoconf2.68
if [ -z "$AUTOCONF_VERSION" ]; then
    if   [ -x "`which autoconf-2.68 2>/dev/null`" ]; then
        AUTOCONF=autoconf-2.68
        AUTOCONF_VERSION=2.68
        echo "    [*] using autoconf 2.68"
        export AUTOCONF AUTOCONF_VERSION
    fi
    # else use default
fi

echo "[+] running autoreconf"
autoreconf -vi

