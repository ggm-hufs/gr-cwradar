#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/gabriel/gr-cwradar/lib
export PATH=/home/gabriel/gr-cwradar/build/lib:$PATH
export LD_LIBRARY_PATH=/home/gabriel/gr-cwradar/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-cwradar 
