#!/bin/bash

echo "Starting dataBase..."

source "${PWD}/env.sh"

nohup ${EXECUTION_FOLDER}/Persistence > ${PWD}/Persistence.out 2>&1 &
