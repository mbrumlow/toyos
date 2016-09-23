#!/bin/sh

SIZE=`stat -c %s $1`
PAGES=$((($SIZE >> 12) + 1))
PAD=$((($PAGES*4096) - $SIZE))
truncate -s +$PAD $1

