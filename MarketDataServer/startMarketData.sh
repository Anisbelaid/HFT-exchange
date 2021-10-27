#!/bin/bash

echo "Starting market data server ..."

source "${PWD}/env.sh"

nohup ${EXECUTION_FOLDER}/MarketData > ${PWD}/MarketData.out 2>&1 &
