#!/bin/sh

echo "Stopping persistence ..."

PID="$(pidof ${PWD}/Persistence)"

if [ -z "$PID" ] 
then
    echo "Persistence already stopped"
else
    kill -2 ${PID}
    wait ${PID}
fi

