#!/bin/sh
# displays time
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

while [ "$?" = "0" ]
do
#	TIME=$(date | cut -c12-19)
#	echo "{" > displayText.json
#	echo "\"func\" : \"displayText\"," >> displayText.json
#	echo "\"val\" : \"$TIME\"" >> displayText.json
#	echo "}" >> displayText.json

	echo -e \
$(cat displayClear.json displaySetPos.json) \
"{\"func\":\"displayText\",\"val\":\""$(date | cut -c12-19)"\"}" \
$(cat displayShow.json disconnect.json) \
| nc -n $IP_ADDR $PORT
done

