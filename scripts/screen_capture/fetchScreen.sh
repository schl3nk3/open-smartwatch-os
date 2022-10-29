#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ];
then
    echo "usage: fetchScreen.sh <IP> <OUTPUT-IMAGE>"
    exit
fi

TEMPFILE=temp.raw

curl http://$1/api/screenserver --output $TEMPFILE
magick -size 240x240 -depth 8 rgb:$TEMPFILE $2

if [ -f $TEMPFILE ];
then
    rm $TEMPFILE
fi
