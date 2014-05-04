#!/bin/bash

cd output

for file in *.yaml; do enscript -B -p "`basename -s .yaml $file`".ps $file; done
for file in *.ps; do ps2pdf $file "`basename -s .ps $file`".pdf; done
pdfunite *.pdf output.pdf

