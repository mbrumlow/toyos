#!/bin/sh

SIZE=`stat -c %s $1`
BLK=`echo "($SIZE/512) + 1" | bc`
echo $BLK

