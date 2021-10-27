#!/bin/bash

echo "Starting gateway engine ..."

source "${PWD}/env.sh"

nohup ${EXECUTION_FOLDER}/Gateway > ${PWD}/Gateway.out 2>&1 &
