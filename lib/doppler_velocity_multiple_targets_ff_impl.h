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

#ifndef INCLUDED_CWRADAR_DOPPLER_VELOCITY_MULTIPLE_TARGETS_FF_IMPL_H
#define INCLUDED_CWRADAR_DOPPLER_VELOCITY_MULTIPLE_TARGETS_FF_IMPL_H

#include <cwradar/doppler_velocity_multiple_targets_ff.h>

namespace gr 
{
	namespace cwradar 
	{
		class doppler_velocity_multiple_targets_ff_impl : public doppler_velocity_multiple_targets_ff
		{
			private:
				size_t d_vlen; //Half of the FFT lengt (because it's divided in posivite and negative frequencies).
				double d_samples_per_sec; //sample rate.
				double d_radar_freq; //radar frequency.
				double d_signal_min_dB; //Power Threshold in dBm.
				double d_angle_of_approach; //Relative angle between the target movement direction and the line of sight of the radar.
				double d_lowpass_cut_freq; //Low-Pass Cutoff Frequency for the fft.
				double d_highpass_cut_freq; //High-Pass Cutoff Frequency for the fft.
				int d_max_num_of_targets; //Max number of targets to be able to detect.
				double d_doppler_signal_bw; //Bandwidth of the doppler signal in Hz.
            
			public:
			
				doppler_velocity_multiple_targets_ff_impl(size_t vlen, double samples_per_sec, double radar_freq, double signal_min_dB, double angle_of_approach, double lowpass_cut_freq, double highpass_cut_freq, int max_num_of_targets, double doppler_signal_bw);
      ~doppler_velocity_multiple_targets_ff_impl();

			// Where all the action really happens
			int work(int noutput_items,
				gr_vector_const_void_star &input_items,
				gr_vector_void_star &output_items);
				
			//Callback functions for seting variables in gnuradio-companion:
			void set_samples_per_sec(double _samples_per_sec_) { d_samples_per_sec = _samples_per_sec_; }
			void set_radar_freq(double _radar_freq_) { d_radar_freq = _radar_freq_; }
			void set_signal_min_dB(double _signal_min_dB_) { d_signal_min_dB = _signal_min_dB_; }
			void set_angle_of_approach(double _angle_of_approach_) { d_angle_of_approach = _angle_of_approach_; }
			void set_lowpass_cut_freq(double _lowpass_cut_freq_) { d_lowpass_cut_freq = _lowpass_cut_freq_; }
			void set_highpass_cut_freq(double _highpass_cut_freq_) { d_highpass_cut_freq = _highpass_cut_freq_; }
			void set_max_num_of_targets(int _max_num_of_targets_) { d_max_num_of_targets = _max_num_of_targets_; }
			void set_doppler_signal_bw(double _doppler_signal_bw_) { d_doppler_signal_bw = _doppler_signal_bw_; }
			
			double samples_per_sec() const { return d_samples_per_sec; }
			double radar_freq() const { return d_radar_freq; }
			double signal_min_dB() const { return d_signal_min_dB; }
			double angle_of_approach() const { return d_angle_of_approach; }
			double lowpass_cut_freq() const { return d_lowpass_cut_freq; }
			double highpass_cut_freq() const { return d_highpass_cut_freq; }
			int max_num_of_targets() const { return d_max_num_of_targets; }
			double doppler_signal_bw() const { return d_doppler_signal_bw; }
			
			//Function to calculate the signal amplitude in dB:
			double signal_dB(double signal_fft); //convert the value of fft in to dB
			double signal_volt(double signal_fft); //conver the value of fft to natural units (volts)
			
		};

	} // namespace cwradar
} // namespace gr

#endif /* INCLUDED_CWRADAR_DOPPLER_VELOCITY_MULTIPLE_TARGETS_FF_IMPL_H */

