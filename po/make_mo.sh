#!/bin/sh

echo "* Generating cs.mo..."
mkdir -p ../lang/cs/LC_MESSAGES
msgfmt -c -v -o ../lang/cs/LC_MESSAGES/sus107-dt.mo cs.po

#echo "* Generating en.mo..."
#mkdir -p ../lang/en/LC_MESSAGES
#msgfmt -c -v -o ../lang/en/LC_MESSAGES/sus107-dt.mo en.po
