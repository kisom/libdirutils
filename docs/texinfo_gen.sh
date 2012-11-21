#!/bin/sh

DOCNAME="libdirutils"

if [ -e ${DOCNAME}.texi ]; then
        rm -f ${DOCNAME}.texi
fi

pandoc -f latex -o ${DOCNAME}_body.texi ${DOCNAME}.tex
cat header.texi ${DOCNAME}_body.texi footer.texi > ${DOCNAME}.texi
rm ${DOCNAME}_body.texi
