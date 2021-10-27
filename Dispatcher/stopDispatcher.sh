#!/bin/sh

echo "Stop dispatcher ..."

ID="$(pidof ${PWD}/Dispatcher)"
if [ -z "$PID" ] 
then
    echo "Dispatcher already stopped"
else
    kill -2 ${PID}
    wait ${PID}
fi
