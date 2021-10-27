#!/bin/sh

echo "Stopping gateway ..."

PID="$(pidof ${PWD}/Gateway)"

if [ -z "$PID" ] 
then
    echo "Gateway already stopped"
else
    kill -2 ${PID}
    wait ${PID}
fi
