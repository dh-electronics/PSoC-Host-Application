#!/bin/sh
# displays some text
# Usage: ./DisplayText.sh ip_address port
# default port:	56882
# default ip: 	127.0.0.1

if [ -z "$2" ]; then
PORT=56882
else
PORT=$2
fi

if [ -z "$1" ]; then
IP_ADDR=127.0.0.1
else
IP_ADDR=$1
fi

cat displayClear.json \
displaySetPosZ1.json \
displayTextZ1-1.json \
displayTextZ1-2.json \
displaySetPosZ2.json \
displayTextZ2-1.json \
displayTextZ2-2.json \
displaySetPosZ3.json \
displayTextZ3-1.json \
displayTextZ3-2.json \
displaySetPosZ4.json \
displayTextZ4-1.json \
displayTextZ4-2.json \
displayShow.json \
disconnect.json \
| nc -n $IP_ADDR $PORT

