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

cat \
getVersions.json \
getErrorCounters.json \
resetStats.json \
\
displayClear.json \
displayEnable_Off.json \
displayEnable_On.json \
displaySetPos.json \
displayText.json \
displayShow.json \
\
readButtons.json \
\
readInput1.json \
readInput2.json \
readInput3.json \
readInput4.json \
\
writeDac1.json \
writeDac1Off.json \
\
writeDac2.json \
\
writeInput1Mode_Counter.json \
writeInput1Mode_ADC.json \
\
writeInputPullup_1_On.json \
writeInputPullup_1_Off.json \
\
writeInputPullup_2_On.json \
writeInputPullup_2_Off.json \
\
writeInputShunt.json \
\
writeInputThreshold.json \
\
writeLed_Alarm_On.json \
writeLed_Alarm_Off.json \
\
writeLed_Ready_On.json \
writeLed_Ready_Off.json \
\
writeLed_RS232_A_On.json \
writeLed_RS232_B_On.json \
\
writeLed_Status_On.json \
writeLed_Status_Off.json \
\
writeLed_Uplink_On.json \
writeLed_Uplink_Off.json \
\
writeRelay1On.json \
writeRelay1Off.json \
\
writeRelay2On.json \
writeRelay2Off.json \
disconnect.json \
| nc -n $address $port 
