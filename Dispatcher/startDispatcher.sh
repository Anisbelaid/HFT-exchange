#!/bin/bash

echo "Starting Dispatcher ..."

source "${PWD}/env.sh"

nohup ${EXECUTION_FOLDER}/Dispatcher > ${PWD}/Dispatcher.out 2>&1 &
