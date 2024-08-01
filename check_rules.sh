#!/bin/sh

if [ "$#" -ne 1 ] ; then
  echo "Usage: $0 RULES_FILE.lua" >&2
  exit 1
fi

TFILE="$$_$(basename $1 .lua).lua"

echo "require 'civlike.rules'; local function _() : Rules" > $TFILE
cat $1 >> $TFILE
echo "end" >> $TFILE

tl check -q $TFILE

rm $TFILE
