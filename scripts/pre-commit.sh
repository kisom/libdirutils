#!/bin/sh

scripts/build_test.sh
build_status=$?

branch="$(git branch | awk '{ print $2; }')"
if [ "$branch" = "master" -o "$branch" = "release" ]; then
    if [ ${build_status} -ne 0 ]; then
            echo "[!] failed build check: refusing to commit to ${branch}"
            exit 1
    fi
    echo "[+] accepting commit to ${branch}"
    exit 0
fi

if [ ${build_status} -ne 0 ]; then
    echo "[!] *** WARNING: build test failed. you should proceed with"
    echo "        caution. you will not be allowed to commit to master!"
fi
exit 0
