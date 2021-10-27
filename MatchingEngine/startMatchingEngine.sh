#!/bin/bash

echo "Starting matching engine ..."

source "${PWD}/env.sh"

nohup ${EXECUTION_FOLDER}/MatchingEngine > ${PWD}/MatchingEngine.out 2>&1 &
