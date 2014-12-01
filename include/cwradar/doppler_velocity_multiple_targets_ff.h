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


#ifndef INCLUDED_CWRADAR_DOPPLER_VELOCITY_MULTIPLE_TARGETS_FF_H
#define INCLUDED_CWRADAR_DOPPLER_VELOCITY_MULTIPLE_TARGETS_FF_H

#include <cwradar/api.h>
#include <gnuradio/sync_block.h>

namespace gr 
{
	namespace cwradar 
	{
		/*!
		* \brief <+description of block+>
		* \ingroup cwradar
		*
		*/
		class CWRADAR_API doppler_velocity_multiple_targets_ff : virtual public gr::sync_block
		{
			public:
				typedef boost::shared_ptr<doppler_velocity_multiple_targets_ff> sptr;

				/*!
				* \brief Return a shared_ptr to a new instance of cwradar::doppler_velocity_multiple_targets_ff.
				*
				* To avoid accidental use of raw pointers, cwradar::doppler_velocity_multiple_targets_ff's
				* constructor is in a private implementation
				* class. cwradar::doppler_velocity_multiple_targets_ff::make is the public interface for
				* creating new instances.
				*/
				static sptr make(size_t vlen, double samples_per_sec, double radar_freq, double signal_min_dB, double angle_of_approach, double lowpass_cut_freq, double highpass_cut_freq, int max_num_of_targets, double doppler_signal_bw);
				
				virtual void set_samples_per_sec(double _samples_per_sec_) = 0;
				virtual void set_radar_freq(double _radar_freq_) = 0;
				virtual void set_signal_min_dB(double _signal_min_dB_) = 0;
				virtual void set_angle_of_approach(double _angle_of_approach_) = 0;
				virtual void set_lowpass_cut_freq(double _lowpass_cut_freq_) = 0;
				virtual void set_highpass_cut_freq(double _highpass_cut_freq_) = 0;
				virtual void set_max_num_of_targets(int _max_num_of_targets_) = 0;
				virtual void set_doppler_signal_bw(double _doppler_signal_bw_) = 0;

				virtual double samples_per_sec() const = 0;
				virtual double radar_freq() const = 0;
				virtual double signal_min_dB() const = 0;
				virtual double angle_of_approach() const = 0;
				virtual double lowpass_cut_freq() const = 0;
				virtual double highpass_cut_freq() const = 0;
				virtual int max_num_of_targets() const = 0;
				virtual double doppler_signal_bw() const = 0;
		};

	} // namespace cwradar
} // namespace gr

#endif /* INCLUDED_CWRADAR_DOPPLER_VELOCITY_MULTIPLE_TARGETS_FF_H */

