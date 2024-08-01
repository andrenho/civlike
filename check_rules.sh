#!/bin/sh

if [ "$#" -ne 1 ] ; then
  echo "Usage: $0 RULES_FILE.lua" >&2
  exit 1
fi

TFILE="$(basename $0).$$.tl"

sed '0,/return/{s/return/require "civlike.rules"; local _ : Rules =/}' $1 > $TFILE

tl check -q $TFILE

# rm $TFILE
