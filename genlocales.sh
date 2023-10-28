#!/bin/sh

language=$(echo "$LANG" | cut -d_ -f1)
mkdir -p po/ "locale/$language"/LC_MESSAGES
touch "po/$language"

xgettext -o "po/$language.po" sgrandr.c sgrandr-cr.c sgrandr.h \
-k_ --package-name=sgrandr --package-version=0.1 \
--copyright-holder=ItzSelenux -j

for po in po/*; do
  lng=$(echo "$po" | cut -d/ -f2 | cut -d. -f1 )
  msgfmt -o "locale/$lng/LC_MESSAGES/sgrandr.mo" "$po"
done