#! /bin/bash

if [ -z "$1" ]; then
address=127.0.0.1
else
address=$1
fi

if [ -z "$2" ]; then
port=56882
else
port=$2
fi

cat unfinished_packet1.json writeLed_Ready_Off.json | nc $address $port
cat unfinished_packet2.json writeLed_Ready_Off.json | nc $address $port
cat unfinished_packet3.json writeLed_Ready_Off.json | nc $address $port

cat unknown_tag.json  				| nc $address $port
cat unknown_func.json  				| nc $address $port        

cat writeInputMode_wrong_object.json  		| nc $address $port
cat writeInputMode_wrong_val.json  		| nc $address $port    
cat writeInputThreshold_wrong_object.json   	| nc $address $port
cat writeInputThreshold_wrong_value.json	| nc $address $port
cat readInput_wrong_object.json   		| nc $address $port

cat writeDac_wrong_object.json   		| nc $address $port
cat writeDac_wrong_tag.json 			| nc $address $port
cat writeDac_wrong_value.json			| nc $address $port

cat writeRelay_wrong_object.json 		| nc $address $port
cat writeRelay_wrong_value1.json 		| nc $address $port
cat writeRelay_wrong_value2.json 		| nc $address $port

cat writeLed_extra_arg.json 			| nc $address $port
cat writeLed_wrong_type1.json 			| nc $address $port
cat writeLed_wrong_type2.json   		| nc $address $port
cat writeLed_wrong_type3.json 			| nc $address $port

