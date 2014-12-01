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

#ifndef INCLUDED_CWRADAR_VECTOR_FLIP_FF_IMPL_H
#define INCLUDED_CWRADAR_VECTOR_FLIP_FF_IMPL_H

#include <cwradar/vector_flip_ff.h>

namespace gr {
  namespace cwradar {

    class vector_flip_ff_impl : public vector_flip_ff
    {
     private:
      size_t d_vlen; //The local variable to save the parameter that receives the constructor.

     public:
      vector_flip_ff_impl(double vlen);
      ~vector_flip_ff_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace cwradar
} // namespace gr

#endif /* INCLUDED_CWRADAR_VECTOR_FLIP_FF_IMPL_H */

