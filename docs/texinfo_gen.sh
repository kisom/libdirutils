#!/bin/sh

DOCNAME="libdirutils"

which pandoc 2>/dev/null 1>/dev/null
if [ ! $? -eq 0 ]; then
        echo "[!] pandoc couldn't be found"
        exit
fi

if [ -e ${DOCNAME}.texi ]; then
        rm -f ${DOCNAME}.texi
fi

pandoc -f latex -o ${DOCNAME}_body.texi ${DOCNAME}.tex
if [ ! $? -eq 0 ]; then
        echo "[!] LaTeX -> texinfo conversion failed"
        exit
fi
cat header.texi ${DOCNAME}_body.texi footer.texi > ${DOCNAME}.texi
rm ${DOCNAME}_body.texi
