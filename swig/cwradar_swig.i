/* -*- c++ -*- */

#define CWRADAR_API

%include "gnuradio.i"			// the common stuff


//load generated python docstrings
%include "cwradar_swig_doc.i"

%{
#include "cwradar/doppler_velocity_multiple_targets_ff.h"
#include "cwradar/doppler_velocity_single_target_ff.h"
#include "cwradar/vector_flip_ff.h"
%}


%include "cwradar/doppler_velocity_multiple_targets_ff.h"
GR_SWIG_BLOCK_MAGIC2(cwradar, doppler_velocity_multiple_targets_ff);
%include "cwradar/doppler_velocity_single_target_ff.h"
GR_SWIG_BLOCK_MAGIC2(cwradar, doppler_velocity_single_target_ff);
%include "cwradar/vector_flip_ff.h"
GR_SWIG_BLOCK_MAGIC2(cwradar, vector_flip_ff);
