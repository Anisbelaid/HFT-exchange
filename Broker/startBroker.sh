#!/bin/bash

echo "Starting Broker..."

source "${PWD}/env.sh"

nohup ${EXECUTION_FOLDER}/Broker > ${PWD}/Broker.out 2>&1 &
