#!/bin/bash

export CC=gcc
export CXX=g++
export LOGFILE=mplint_po.log

source ./tools/ci/scripts/init.sh

aptget_install gcc g++ git \
    make autoconf automake autopoint gettext

export CXXFLAGS="-std=gnu++11"

git clone https://gitlab.com/manaplus/mplint.git
cd mplint
run_configure_simple
run_make
cd ..
run_mplint $*

source ./tools/ci/scripts/exit.sh

exit 0