#!/bin/sh

SIZE=`stat -c %s $1`
echo $(($SIZE/512))

