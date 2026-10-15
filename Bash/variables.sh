#!/bin/bash

CURRENT_FILE_NAME=${0##*/}
# Relative to the command prompt
CURRENT_DIRECTORY=${0%/*}
# Relative to the command prompt
CURRENT_FULL_PATH=${0}

RELATIVE_FILE_NAME=${CURRENT_FILE_NAME}
CURRENT_ABS_FILE_NAME="$(cd "$(dirname "${RELATIVE_FILE_NAME}")" && pwd)/$(basename "${RELATIVE_FILE_NAME}")"

ARGUMENTS=${@}
echo ${CURRENT_ABS_DIR}
