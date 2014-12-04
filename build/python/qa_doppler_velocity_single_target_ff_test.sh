#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/gabriel/gr-cwradar/python
export PATH=/home/gabriel/gr-cwradar/build/python:$PATH
export LD_LIBRARY_PATH=/home/gabriel/gr-cwradar/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/gabriel/gr-cwradar/build/swig:$PYTHONPATH
/usr/bin/python2 /home/gabriel/gr-cwradar/python/qa_doppler_velocity_single_target_ff.py 
