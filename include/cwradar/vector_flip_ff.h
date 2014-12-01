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


#ifndef INCLUDED_CWRADAR_VECTOR_FLIP_FF_H
#define INCLUDED_CWRADAR_VECTOR_FLIP_FF_H

#include <cwradar/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace cwradar {

    /*!
     * \brief <+description of block+>
     * \ingroup cwradar
     *
     */
    class CWRADAR_API vector_flip_ff : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<vector_flip_ff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of cwradar::vector_flip_ff.
       *
       * To avoid accidental use of raw pointers, cwradar::vector_flip_ff's
       * constructor is in a private implementation
       * class. cwradar::vector_flip_ff::make is the public interface for
       * creating new instances.
       */
      static sptr make(double vlen);
    };

  } // namespace cwradar
} // namespace gr

#endif /* INCLUDED_CWRADAR_VECTOR_FLIP_FF_H */

