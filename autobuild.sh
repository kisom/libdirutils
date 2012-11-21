#!/bin/sh
# performs a full clean, autoreconf, configure with appropriate options,
# make, and make check: it starts from scratch, with a clean dev checkout
# and performs everything necessary to run the unit tests.

echo "[+] performing autobuild" ;          \
        scripts/clean.sh                   \
        scripts/prebuild.sh             && \
        scripts/regen.sh                && \
        make clean                      && \
        make                            && \
        make check
