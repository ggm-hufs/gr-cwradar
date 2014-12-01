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
#include "doppler_velocity_multiple_targets_ff_impl.h"
#include <math.h>
#include <stdio.h>

#define C 299792458 //Definition of the speed of light.

namespace gr 
{
	namespace cwradar 
	{
		doppler_velocity_multiple_targets_ff::sptr
		doppler_velocity_multiple_targets_ff::make(size_t vlen, double samples_per_sec, double radar_freq, double signal_min_dB, double angle_of_approach, double lowpass_cut_freq, double highpass_cut_freq, int max_num_of_targets, double doppler_signal_bw)
		{
			return gnuradio::get_initial_sptr
				(new doppler_velocity_multiple_targets_ff_impl(vlen, samples_per_sec, radar_freq, signal_min_dB, angle_of_approach, lowpass_cut_freq, highpass_cut_freq, max_num_of_targets, doppler_signal_bw));
		}

		/*
		* The private constructor
		*/
		doppler_velocity_multiple_targets_ff_impl::doppler_velocity_multiple_targets_ff_impl(size_t vlen, double samples_per_sec, double radar_freq, double signal_min_dB, double angle_of_approach, double lowpass_cut_freq, double highpass_cut_freq, int max_num_of_targets, double doppler_signal_bw)
			: gr::sync_block("doppler_velocity_multiple_targets_ff",
              gr::io_signature::make(2, 2, vlen*sizeof(float)),
              gr::io_signature::make3(3, 3, max_num_of_targets*sizeof(float), max_num_of_targets*sizeof(int), sizeof(int) )),
			  d_vlen(vlen), //Half of the FFT lengt (because it's divided in posivite and negative frequencies).
			  d_samples_per_sec(samples_per_sec), //sample rate.
			  d_radar_freq(radar_freq), //radar frequency.
			  d_signal_min_dB(signal_min_dB), //Power Threshold in dBm.
			  d_angle_of_approach(angle_of_approach), //Relative angle between the target movement direction and the line of sight of the radar.
			  d_lowpass_cut_freq(lowpass_cut_freq), //Low-Pass Cutoff Frequency for the fft.
			  d_highpass_cut_freq(highpass_cut_freq), //High-Pass Cutoff Frequency for the fft.
			  d_max_num_of_targets(max_num_of_targets), //Max number of targets to be able to detect.
			  d_doppler_signal_bw(doppler_signal_bw) //Bandwidth of the doppler signal in Hz.
		{
            //Set the local variables of the parameters with its values from the constructor parameters.
			set_samples_per_sec(samples_per_sec);
			set_radar_freq(radar_freq);
			set_signal_min_dB(signal_min_dB);
			set_angle_of_approach(angle_of_approach);
			set_lowpass_cut_freq(lowpass_cut_freq);
			set_highpass_cut_freq(highpass_cut_freq);
			set_max_num_of_targets(max_num_of_targets);
			set_doppler_signal_bw(doppler_signal_bw);
			
		}

		/*
		* Our virtual destructor.
		*/
		doppler_velocity_multiple_targets_ff_impl::~doppler_velocity_multiple_targets_ff_impl()
		{
		}
		
		double 
    	doppler_velocity_multiple_targets_ff_impl::signal_dB(double signal_fft) //Function to calculate the signal power in dBm.
    	{
    		//Blackmanharris factor: Calculated from obeservation in GNU Radio FFT2SINK.
    		double BH_factor = 1.9733; //This is the stimated attenuation of the Blackman-Harris window function.
    		return 20*log10((signal_fft*BH_factor)/(d_vlen*2));
    	}
    	double
    	doppler_velocity_multiple_targets_ff_impl::signal_volt(double signal_fft) //Function to calculate the signal power in natural units.
    	{
    		//Blackmanharris factor: Calculated from obeservation in GNU Radio FFT2SINK.
    		double BH_factor = 1.9733; //This is the stimated attenuation of the Blackman-Harris window function.
    		return (signal_fft*BH_factor)/(d_vlen*2);
    	}

		int
		doppler_velocity_multiple_targets_ff_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
		{
			// Do <+signal processing+>
			float speed_mps_radar[d_max_num_of_targets];  //The speed measured by the radar in meters per second.
			float speed_kph_radar[d_max_num_of_targets];  //The speed measured by the radar in kilometer per hour.
			float speed_kph_target[d_max_num_of_targets]; //The speed of the target in kilometer per hour.
			int directions[d_max_num_of_targets]; //The vector with the speed of every target.
			float doppler_freq[d_max_num_of_targets]; //Variable to store the doppler frequencies.
			const float c = C; //Light speed. Difined with the constant C: #define C 299792458
			float lambda = c/(float)d_radar_freq; //Wavelength of the radar RF signal.
			int max_index_positive[d_max_num_of_targets]; //index array for positive frequency
			int max_index_negative[d_max_num_of_targets]; //index array for negative frequency
			float max_array_positive[d_max_num_of_targets]; //max array for positive frequency
			float max_array_negative[d_max_num_of_targets]; //max array for negative frequency
			int num_of_targets = 0; //Num of targets detected by the radar in the line of sight.
			int doppler_signal_bandwidth_bins = (int)((d_doppler_signal_bw*(d_vlen*2))/(d_samples_per_sec)); //Bandwidth of the doppler signal in beans of the FFT to skip to detect the next target.
			float angle_of_approach_rad = d_angle_of_approach*((float)(2.0*M_PI)/360.0); //convert from degrees to rad.
			
			for(int iterator=0;iterator<d_max_num_of_targets;iterator++) //Inizialize the array to 0.
			{
				speed_kph_radar[iterator] = 0;
				speed_mps_radar[iterator] = 0;
				speed_kph_target[iterator] = 0;
				max_index_positive[iterator] = 0;
				max_index_negative[iterator] = 0;
				max_array_positive[iterator] = 0;
				max_array_negative[iterator] = 0;
				directions[iterator] = 0; //Indicates no target detected.
			}
			
			float *optr_speed_vector = (float *)output_items[0]; //Output vector with speed of each target
            int *optr_direction_vector = (int *)output_items[1]; //Output vector with the direction of each target.
			int *optr_num_of_targets = (int *)output_items[2]; //Output with the number of targets in sight.
			
			int ninputs = input_items.size();
            
			num_of_targets = 0; //Inicialize the number of targets to 0.
			for(int i = 0; i < noutput_items; i++) 
			{
				float max_negative = ((float *)input_items[0])[i*d_vlen]; //Input negative frequency vector.
				float max_positive = ((float *)input_items[1])[i*d_vlen]; //Input positive frequency vector.
                
				for(int j = (int)((d_highpass_cut_freq*(d_vlen*2))/(d_samples_per_sec)); j < (int)((d_lowpass_cut_freq*(d_vlen*2))/(d_samples_per_sec)); j++ ) //Only look for changes in the first d_lowpass_cut_freq (Hz). This acts like a low pass filter, eliminating all the frequencies above d_lowpass_cut_freq (Hz). Also a High-Pass filter is implemented to configure the radar to ignore targets below some certain frequency depending on the application, (i.e. slow targets).
				{
					if( (((float *)input_items[0])[i*d_vlen + j] > max_negative) 
						&& (signal_dB(((float *)input_items[0])[i*d_vlen + j]) > d_signal_min_dB) ) //If the signal is above the threshold level and is bigger than the last checked value in the loop then that is the doppler frequency. This check only the negative frequency part.
					{
                    	max_negative = ((float*)input_items[0])[i*d_vlen + j];
						
						if( (((float *)input_items[0])[i*d_vlen + (j+1)] < max_negative)
						&& (((float *)input_items[0])[i*d_vlen + (j-1)] < max_negative) ) //Look for a maximum checking that both samples on the right and left are minor than the desired sample.
						{
							max_array_negative[num_of_targets] = max_negative;  //Get the value of the signal.
							max_index_negative[num_of_targets] = j; //Get the index to calculate the doppler frequency.
							num_of_targets++; //Increments the number of targets in one.
							j += doppler_signal_bandwidth_bins/2; //Skip half of the doppler signal bandwidth (in bins of the FFT) to begin to look for the next target that must be separated at least half of the bandwidth of the doppler signal, otherwise will be detected as the same target.
							max_negative = 0; //reset the maximum so it can keep looking for next maximum.
						}
                	}
					if( (((float *)input_items[1])[i*d_vlen + j] > max_positive) 
						&& (signal_dB(((float *)input_items[1])[i*d_vlen + j]) > d_signal_min_dB) ) //If the signal is above the threshold level and is bigger than the last checked value in the loop then that is the doppler frequency. This check only the positive frequency part.
					{
						max_positive = ((float*)input_items[1])[i*d_vlen + j];
						
						if( (((float *)input_items[1])[i*d_vlen + (j+1)] < max_positive)
						&& (((float *)input_items[1])[i*d_vlen + (j-1)] < max_positive) ) //Look for a maximum checking that both samples on the right and left are minor than the desired sample.
						{
							max_array_positive[num_of_targets] = max_positive;  //Get the value of the signal.
							max_index_positive[num_of_targets] = j; //Get the index to calculate the doppler frequency.
							num_of_targets++; //Increments the number of targets in one.
							j += doppler_signal_bandwidth_bins/2; //Skip half of the doppler signal bandwidth (in bins of the FFT) to begin to look for the next target that must be separated at least half of the bandwidth of the doppler signal, otherwise will be detected as the same target.
							max_positive = 0; //reset the maximum so it can keep looking for next maximum.
						}
                	}
				}
			   
				//Calculation of doppler frequency from the index and the speed in mps and kph:
				if(num_of_targets > d_max_num_of_targets)
				{
					printf("\r\nError, number of targets detected is greater than the maximum number of targets configured \r\n Please increment the parameter ""max_num_of_targets"".\r\n");
					num_of_targets = d_max_num_of_targets;
				}
				for(i=0;i<num_of_targets;i++)
				{
					/* --------------------------------No Target detected--------------------------------*/
					if(num_of_targets == 0) 
					{
						for(int iterator=0;iterator<d_max_num_of_targets;iterator++)
						{
							speed_kph_target[iterator] = 0; //As there is no target, I assign 0 to the speed array.
							directions[iterator] = 0; //As there is no target, I assign 0 to the direction array.
						}
						num_of_targets = 0; // No Target detected.
					
                        //Copy the result to the output vector:
						memcpy(optr_speed_vector, speed_kph_target, d_max_num_of_targets*sizeof(float));
						memcpy(optr_direction_vector, directions, d_max_num_of_targets*sizeof(float)); 
						*optr_num_of_targets++ = (float)num_of_targets;
	
						// Tell runtime system how many output items we produced.
						//printf("\r\nnoutput_items = %d", noutput_items);
						return noutput_items;
					}
					/* ----------------------------------------------------------------------------------*/			
	
					/* ---------------------------Going away Target detected-----------------------------*/
					if(signal_dB(max_array_negative[i]) > d_signal_min_dB) //Target going away. Negative Doppler Frequency.
					{
						doppler_freq[i] = (float)((float)max_index_negative[i]/((float)d_vlen*2))*(float)d_samples_per_sec; //Get the doppler frequency from the frequency index.
						speed_mps_radar[i] = (float)(doppler_freq[i]*lambda)/2.0; //Compute formula that extract the speed from the doppler frequency.
						speed_kph_radar[i] = (float)speed_mps_radar[i]*3.6; //Convert the speed in kilometers per hour.
						speed_kph_target[i] = (float)speed_kph_radar[i]/(float)cos(angle_of_approach_rad); //Speed of the target considering the angle between the target moving direction and the radar.
						directions[i] = 2; //Target receding. Negative Doppler Frequency.
					}
					/* ----------------------------------------------------------------------------------*/

					/* ----------------------------Aproaching Target detected----------------------------*/
					else if(signal_dB(max_array_positive[i]) > d_signal_min_dB) //Target aproaching. Positive Doppler Frequency.
					{
						doppler_freq[i] = (float)((float)max_index_positive[i]/((float)d_vlen*2))*(float)d_samples_per_sec; //Get the doppler frequency from the frequency index.
						speed_mps_radar[i] = (float)(doppler_freq[i]*lambda)/2.0; //Compute formula that extract the speed from the doppler frequency.
						speed_kph_radar[i] = (float)speed_mps_radar[i]*3.6; //Convert the speed in kilometers per hour.
						speed_kph_target[i] = (float)speed_kph_radar[i]/(float)cos(angle_of_approach_rad); //Speed of the target considering the angle between the target moving direction and the radar.
						directions[i] = 1; //Target approaching. Positive Doppler Frequency.
					}
					/* ----------------------------------------------------------------------------------*/
				}
				
				for(i=0;i<d_max_num_of_targets;i++)
				{
					if(speed_kph_target[i] != 0)
					{
						printf("\r\n\r\nTarget %d:", i+1);
						if(directions[i] == 1) //Approaching Target.
						{
							printf("\r\nMax Index = %d", max_index_positive[i]);
							printf("\r\nMax Amp = %f V", signal_volt(max_array_positive[i]));
							printf("\r\nMax Power = %f dBm", signal_dB(max_array_positive[i]));
							printf("\r\nDoppler Frequency = %f Hz", doppler_freq[i]);
							printf("\r\nSpeed = %f Kph", speed_kph_target[i]);
							printf("\r\nTarget %d Approaching", i+1);
						}
						else if(directions[i] == 2) //Receding Target.
						{
							printf("\r\nMax Index = %d", max_index_negative[i]);
							printf("\r\nMax Amp = %f V", signal_volt(max_array_negative[i]));
							printf("\r\nMax Power = %f dBm", signal_dB(max_array_negative[i]));
							printf("\r\nDoppler Frequency = -%f Hz", doppler_freq[i]);
							printf("\r\nSpeed = %f Kph", speed_kph_target[i]);
							printf("\r\nTarget %d Receding", i+1);
						}
					}
				}
				printf("\r\n\r\n###################################"
						"###############################################");
				
				memcpy(optr_speed_vector, speed_kph_target, d_max_num_of_targets*sizeof(float)); //Write on the output port the value of the velocities.
				memcpy(optr_direction_vector, directions, d_max_num_of_targets*sizeof(float)); //Write on the output port the value of the directions.
				*optr_num_of_targets++ = (float)num_of_targets; //Write on the output port the number of targets detected.
			}
			
			// Tell runtime system how many output items we produced.
			//printf("\r\nnoutput_items = %d", noutput_items);
        	return noutput_items;
		}

	} /* namespace cwradar */
} /* namespace gr */

