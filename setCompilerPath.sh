#!/bin/bash
WORKING_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
COMP_RELATIVE_DIR=cxx/linux/bin
COMPILER_DIR=${WORKING_DIR}/${COMP_RELATIVE_DIR}

export PATH=${PATH}:${COMPILER_DIR}
echo Added ${COMPILER_DIR} to the environment PATH
