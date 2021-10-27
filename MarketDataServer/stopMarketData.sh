#!/bin/sh

echo "Stopping marketdata server ..."

PID="$(pidof ${PWD}/MarketData)"

if [ -z "$PID" ] 
then
    echo "Market data already stopped"
else
    kill -2 ${PID}
    wait ${PID}
fi
