#!/bin/sh

echo "Stop matching engine ..."

ID="$(pidof ${PWD}/MatchingEngine)"
if [ -z "$PID" ] 
then
    echo "Matching engine already stopped"
else
    kill -2 ${PID}
    wait ${PID}
fi
