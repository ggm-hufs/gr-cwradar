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
#include "doppler_velocity_single_target_ff_impl.h"
#include <math.h>
#include <stdio.h>

#define C 299792458 //Definition of the speed of light.

namespace gr 
{
	namespace cwradar
	{
		doppler_velocity_single_target_ff::sptr
		doppler_velocity_single_target_ff::make(size_t vlen, double samples_per_sec, double radar_freq, double signal_min_dB, double angle_of_approach, double lowpass_cut_freq, double highpass_cut_freq)
		{
      		return gnuradio::get_initial_sptr
        	(new doppler_velocity_single_target_ff_impl(vlen, samples_per_sec, radar_freq, signal_min_dB, angle_of_approach, lowpass_cut_freq, highpass_cut_freq));
    	}

    	/*
     	* The private constructor
     	*/
    	doppler_velocity_single_target_ff_impl::doppler_velocity_single_target_ff_impl(size_t vlen, double samples_per_sec, double radar_freq, double signal_min_dB, double angle_of_approach, double lowpass_cut_freq, double highpass_cut_freq)
      	: gr::sync_block("doppler_velocity_single_target_ff",
			         gr::io_signature::make(2, 2, vlen*sizeof(float)),
                     gr::io_signature::make2(2, 2, sizeof(float), sizeof(int))),
		d_vlen(vlen), //Half of the FFT lengt (because it's divided in posivite and negative frequencies).
        d_samples_per_sec(samples_per_sec), //sample rate.
        d_radar_freq(radar_freq), //radar frequency.
        d_signal_min_dB(signal_min_dB), //Power Threshold in dBm.
        d_angle_of_approach(angle_of_approach), //Relative angle between the target movement direction and the line of sight of the radar.
        d_highpass_cut_freq(highpass_cut_freq), //High-Pass Cutoff Frequency for the fft.
        d_lowpass_cut_freq(lowpass_cut_freq) //Low-Pass Cutoff Frequency for the fft.
    	{
            //Set the local variables of the parameters with its values from the constructor parameters.
    		set_samples_per_sec(samples_per_sec);
			set_radar_freq(radar_freq);
			set_signal_min_dB(signal_min_dB);
			set_angle_of_approach(angle_of_approach);
			set_lowpass_cut_freq(lowpass_cut_freq);
			set_highpass_cut_freq(highpass_cut_freq);

    		//std::vector<int> output_sizes;
			//output_sizes.push_back(sizeof(float));
			//output_sizes.push_back(sizeof(int));
    		//gr::sync_block("doppler_velocity_single_target_ff",
			           //gr::io_signature::make(2, 2, vlen*sizeof(float)),
                       //gr::io_signature::makev(2, 2, output_sizes));
    	}

    	/*
     	* Our virtual destructor.
     	*/
    	doppler_velocity_single_target_ff_impl::~doppler_velocity_single_target_ff_impl()
    	{
    	}
    	
    	double 
    	doppler_velocity_single_target_ff_impl::signal_dB(double signal_fft) //Function to calculate the signal power in dBm.
    	{
    		//Blackmanharris factor: Calculated from obeservation in GNU Radio FFT2SINK.
    		double BH_factor = 1.9733; //This is the stimated attenuation of the Blackman-Harris window function.
    		return 20*log10((signal_fft*BH_factor)/(d_vlen*2));
    	}
    	double
        doppler_velocity_single_target_ff_impl::signal_volt(double signal_fft) //Function to calculate the signal power in natural units.
    	{
    		//Blackmanharris factor: Calculated from obeservation in GNU Radio FFT2SINK.
    		double BH_factor = 1.9733; //This is the stimated attenuation of the Blackman-Harris window function.
    		return (signal_fft*BH_factor)/(d_vlen*2);
    	}

    	int
    	doppler_velocity_single_target_ff_impl::work(int noutput_items,
			  	gr_vector_const_void_star &input_items,
			  	gr_vector_void_star &output_items)
    	{
    		// Do <+signal processing+>
        	float speed_mps_radar = 0.0;  //The speed measured by the radar in meters per second.
        	float speed_kph_radar = 0.0;  //The speed measured by the radar in kilometer per hour.
        	float speed_kph_target = 0.0; //The speed of the target in kilometer per hour.
        	float doppler_freq = 0.0;     //Variable to store the doppler frequency.
        	const float c = C; //Light speed. Difined with the constant C: #define C 299792458
        	float lambda = c/(float)d_radar_freq; //Wavelength of the radar RF signal.
        	int max_index_negative = 0;
			int max_index_positive = 0;
			int direction = 0; //if direction = 1 Target is Approaching, if direction = 2 Target is Receding.
        	float angle_of_approach_rad = d_angle_of_approach*((float)(2.0*M_PI)/360.0); //convert from degrees to rad.
        	          
        	float *optr = (float *)output_items[0]; //Definition of the speed output port.
       		int *optr_direction = (int *)output_items[1]; //Definition of the direction outpot port.
            
        	int ninputs = input_items.size();
            
        	for(int i = 0; i < noutput_items; i++) 
			{
            	float max_negative = ((float *)input_items[0])[i*d_vlen]; //Input negative frequency vector.
				float max_positive = ((float *)input_items[1])[i*d_vlen]; //Input positive frequency vector.
                
            	for(int j = (int)((d_highpass_cut_freq*(d_vlen*2))/(d_samples_per_sec)); j < (int)((d_lowpass_cut_freq*(d_vlen*2))/(d_samples_per_sec)); j++ ) //Only look for changes in the first d_lowpass_cut_freq (Hz). This acts like a low pass filter, eliminating all the frequencies above d_lowpass_cut_freq (Hz). Also a High-Pass filter is implemented to configure the radar to ignore targets below some certain frequency depending on the application, (i.e. slow targets).
				{
                	if( (((float *)input_items[0])[i*d_vlen + j] > max_negative) 
						&& (signal_dB(((float *)input_items[0])[i*d_vlen + j]) > d_signal_min_dB) ) //If the signal is above the threshold level and is bigger than the last checked value in the loop then that is the doppler frequency. This check only the negative frequency part.
					{
                    	max_negative = ((float*)input_items[0])[i*d_vlen + j]; //Get the value of the signal.
                    	max_index_negative = j; //Get the index to calculate the doppler frequency.
                	}
					if( (((float *)input_items[1])[i*d_vlen + j] > max_positive) 
						&& (signal_dB(((float *)input_items[1])[i*d_vlen + j]) > d_signal_min_dB) ) //If the signal is above the threshold level and is bigger than the last checked value in the loop then that is the doppler frequency. This check only the positive frequency part.
					{
                   		max_positive = ((float*)input_items[1])[i*d_vlen + j]; //Get the value of the signal.
                    	max_index_positive = j; //Get the index to calculate the doppler frequency.
                	}
            	}
                
            	//Calculation of doppler frequency from the index, and the speed from doppler frequency:
            	
				/* --------------------------------No Target detected--------------------------------*/
            	if( (signal_dB(max_negative) < d_signal_min_dB) && (signal_dB(max_positive) < d_signal_min_dB) ) 
				{
                	speed_kph_target = 0; // -1 means that there is no moving target on sight.
					direction = 0; // No Target detected.
				
					*optr++ = (float)speed_kph_target;  //Set output speed to 0.
            		*optr_direction++ = (int)direction; //Set output direction to 0.

					// Tell runtime system how many output items we produced.
					//printf("\r\nnoutput_items = %d", noutput_items);
            		return noutput_items;
            	}
				/* ----------------------------------------------------------------------------------*/			

				/* ---------------------------Receding Target detected-----------------------------*/
            	if(signal_dB(max_negative) > d_signal_min_dB) //Target Receding. Negative Doppler Frequency.
				{
               		doppler_freq = (float)((float)max_index_negative/((float)d_vlen*2))*(float)d_samples_per_sec; //Get the doppler frequency from the frequency index.
                	speed_mps_radar = (float)(doppler_freq*lambda)/2.0; //Compute formula that extract the speed from the doppler frequency.
                	speed_kph_radar = (float)speed_mps_radar*3.6; //Convert the speed in kilometers per hour.
                	speed_kph_target = (float)speed_kph_radar/(float)cos(angle_of_approach_rad); //Speed of the target considering the angle between the target moving direction and the radar.
						direction = 2; //Target Receding. Negative Doppler Frequency.
            	}
				/* ----------------------------------------------------------------------------------*/

				/* ----------------------------Approaching Target detected----------------------------*/
				else if(signal_dB(max_positive) > d_signal_min_dB) //Target Approaching. Positive Doppler Frequency.
				{
                	doppler_freq = (float)((float)max_index_positive/((float)d_vlen*2))*(float)d_samples_per_sec; //Get the doppler frequency from the frequency index.
                	speed_mps_radar = (float)(doppler_freq*lambda)/2.0; //Compute formula that extract the speed from the doppler frequency.
                	speed_kph_radar = (float)speed_mps_radar*3.6; //Convert the speed in kilometers per hour.
                	speed_kph_target = (float)speed_kph_radar/(float)cos(angle_of_approach_rad); //Speed of the target considering the angle between the target moving direction and the radar.
					direction = 1; //Target Approaching. Positive Doppler Frequency.
            	}
				/* ----------------------------------------------------------------------------------*/
			
				if(direction == 1) //Approaching Target.
				{
					printf("\r\n\r\nMax Index = %d", max_index_positive);
					printf("\r\nMax Amp = %f V", signal_volt(max_positive));
					printf("\r\nMax Power = %f dBm", signal_dB(max_positive));
					printf("\r\nDoppler Frequency = %f Hz", doppler_freq);
					printf("\r\nSpeed = %f Kph", speed_kph_target);
					printf("\r\nTarget Approaching");
				}
				else if(direction == 2) //Receding Target.
				{
					printf("\r\n\r\nMax Index = %d", max_index_negative);
					printf("\r\nMax Amp = %f V", signal_volt(max_negative));
					printf("\r\nMax Power = %f dBm", signal_dB(max_negative));
					printf("\r\nDoppler Frequency = -%f Hz", doppler_freq);
					printf("\r\nSpeed = %f Kph", speed_kph_target);
					printf("\r\nTarget Receding");
				}

            	*optr++ = (float)speed_kph_target;  //Write on the output port the value of the speed.
            	*optr_direction++ = (int)direction; //Write on the output port the value of the direction.
        	}
            
        	// Tell runtime system how many output items we produced.
			//printf("\r\nnoutput_items = %d", noutput_items);
        	return noutput_items;
    	}

	} /* namespace cwradar */
} /* namespace gr */

