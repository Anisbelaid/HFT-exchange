#!/bin/sh

echo "Stopping broker ..."

PID="$(pidof ${PWD}/Broker)"

if [ -z "$PID" ] 
then
    echo "Gateway already stopped"
else
    kill -2 ${PID}
    wait ${PID}
fi

