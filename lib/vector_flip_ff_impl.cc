/* -*- c++ -*- */
/* 
 * Copyright 2014 Gabriel Gómez Molina Moreno Catalán at 한국외국어대학교, 대한민국.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "vector_flip_ff_impl.h"

#include <algorithm>
#include <iterator>

namespace gr {
  namespace cwradar {

    vector_flip_ff::sptr
    vector_flip_ff::make(double vlen) //Function called by python through SWIG to create the object of the block.
    {
      return gnuradio::get_initial_sptr
        (new vector_flip_ff_impl(vlen));
    }

    /*
     * The private constructor
     */
    vector_flip_ff_impl::vector_flip_ff_impl(double vlen) //vlen comes from the GNU Radio block configuration.
      : gr::sync_block("vector_flip_ff",
              gr::io_signature::make(1, 1, vlen*sizeof(float)),  //Size of the block input port.
              gr::io_signature::make(1, 1, vlen*sizeof(float))), //Size of the block output port.
		d_vlen(vlen) //FFT Length.
    {}

    /*
     * Our virtual destructor.
     */
    vector_flip_ff_impl::~vector_flip_ff_impl()
    {
    }

    int
    vector_flip_ff_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0]; //Pointer to the block input stream.
        float *out = (float *) output_items[0]; //Pointer to the block output stream.

		size_t block_size = output_signature()->sizeof_stream_item (0);

        // Do <+signal processing+>
		for (int i = 0; i < d_vlen; i++) //Flip the array. The last element becomes the first one and the so on.
  		{
     		out[i] = in[d_vlen-i-1];
  		}
		
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace cwradar */
} /* namespace gr */

