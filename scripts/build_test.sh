#/bin/sh
# this script is designed to be run as either pre-commit or pre-push.

make
if [ ! $? -eq 0 ]; then
        echo "[+] build failed"
        exit 1
fi

make check
if [ ! $? -eq 0 ]; then
        echo "[+] unit tests failed"
        exit 1
fi

echo "[+] build and unit tests passed."
exit 0
