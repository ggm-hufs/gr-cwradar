#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: CW Radar Single Target
# Author: Gabriel Gomez Molina
# Description: Continuous Wave Doppler Radar
# Generated: Mon Dec  1 12:41:13 2014
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import filter
from gnuradio import gr
from gnuradio import uhd
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from gnuradio.wxgui import waterfallsink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import cwradar
import threading
import time
import wx

class CW_Radar_Single_Target(grc_wxgui.top_block_gui):

    def __init__(self, lo_offset_freq=1e6, threshold_dB=-70, RF=2.49e9, DC_filter_num_elements=4, speed_samp_rate=1, samp_rate=1e6, highpass_cutoff_freq=0, lowpass_cutoff_freq=1700, fft_len=pow(2,20), samp_rate_sink=8000, angle=0, tx_amp=1, rx_gain=30.5):
        grc_wxgui.top_block_gui.__init__(self, title="CW Radar Single Target")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Parameters
        ##################################################
        self.lo_offset_freq = lo_offset_freq
        self.threshold_dB = threshold_dB
        self.RF = RF
        self.DC_filter_num_elements = DC_filter_num_elements
        self.speed_samp_rate = speed_samp_rate
        self.samp_rate = samp_rate
        self.highpass_cutoff_freq = highpass_cutoff_freq
        self.lowpass_cutoff_freq = lowpass_cutoff_freq
        self.fft_len = fft_len
        self.samp_rate_sink = samp_rate_sink
        self.angle = angle
        self.tx_amp = tx_amp
        self.rx_gain = rx_gain

        ##################################################
        # Variables
        ##################################################
        self.target_speed = target_speed = 0
        self.target_direction = target_direction = 0
        self.tx_amp_tuner = tx_amp_tuner = tx_amp
        self.threshold_dB_tuner = threshold_dB_tuner = threshold_dB
        self.speed_textbox = speed_textbox = target_speed
        self.rx_gain_tuner = rx_gain_tuner = rx_gain
        self.lowpass_cutoff_freq_tuner = lowpass_cutoff_freq_tuner = lowpass_cutoff_freq
        self.highpass_cutoff_freq_tuner = highpass_cutoff_freq_tuner = highpass_cutoff_freq
        self.direction_textbox = direction_textbox = target_direction
        self.angle_tuner = angle_tuner = angle
        self.RF_tuner = RF_tuner = RF

        ##################################################
        # Blocks
        ##################################################
        _tx_amp_tuner_sizer = wx.BoxSizer(wx.VERTICAL)
        self._tx_amp_tuner_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_tx_amp_tuner_sizer,
        	value=self.tx_amp_tuner,
        	callback=self.set_tx_amp_tuner,
        	label="TX Signal Amp",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._tx_amp_tuner_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_tx_amp_tuner_sizer,
        	value=self.tx_amp_tuner,
        	callback=self.set_tx_amp_tuner,
        	minimum=0,
        	maximum=1,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_tx_amp_tuner_sizer, 0, 17, 1, 26)
        _threshold_dB_tuner_sizer = wx.BoxSizer(wx.VERTICAL)
        self._threshold_dB_tuner_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_threshold_dB_tuner_sizer,
        	value=self.threshold_dB_tuner,
        	callback=self.set_threshold_dB_tuner,
        	label="Detected Target Threshold (dB)",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._threshold_dB_tuner_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_threshold_dB_tuner_sizer,
        	value=self.threshold_dB_tuner,
        	callback=self.set_threshold_dB_tuner,
        	minimum=-90,
        	maximum=-30,
        	num_steps=60,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_threshold_dB_tuner_sizer, 2, 0, 1, 17)
        self.speed_probe = blocks.probe_signal_f()
        _rx_gain_tuner_sizer = wx.BoxSizer(wx.VERTICAL)
        self._rx_gain_tuner_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_rx_gain_tuner_sizer,
        	value=self.rx_gain_tuner,
        	callback=self.set_rx_gain_tuner,
        	label="USRP RX Gain (dB)",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._rx_gain_tuner_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_rx_gain_tuner_sizer,
        	value=self.rx_gain_tuner,
        	callback=self.set_rx_gain_tuner,
        	minimum=0,
        	maximum=30.5 + 62,
        	num_steps=185,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_rx_gain_tuner_sizer, 0, 0, 1, 17)
        self.notebook = self.notebook = wx.Notebook(self.GetWin(), style=wx.NB_TOP)
        self.notebook.AddPage(grc_wxgui.Panel(self.notebook), "FFT CW Doppler Radar Receiver")
        self.notebook.AddPage(grc_wxgui.Panel(self.notebook), "Frequency/Time CW Doppler Radar Receiver")
        self.notebook.AddPage(grc_wxgui.Panel(self.notebook), "FFT CW Doppler Radar Receiver Full Spectrum")
        self.GridAdd(self.notebook, 5, 0, 13, 75)
        _lowpass_cutoff_freq_tuner_sizer = wx.BoxSizer(wx.VERTICAL)
        self._lowpass_cutoff_freq_tuner_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_lowpass_cutoff_freq_tuner_sizer,
        	value=self.lowpass_cutoff_freq_tuner,
        	callback=self.set_lowpass_cutoff_freq_tuner,
        	label="Lowpass Cutoff Frequency (Hz)",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._lowpass_cutoff_freq_tuner_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_lowpass_cutoff_freq_tuner_sizer,
        	value=self.lowpass_cutoff_freq_tuner,
        	callback=self.set_lowpass_cutoff_freq_tuner,
        	minimum=0,
        	maximum=3000,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_lowpass_cutoff_freq_tuner_sizer, 1, 43, 1, 32)
        _highpass_cutoff_freq_tuner_sizer = wx.BoxSizer(wx.VERTICAL)
        self._highpass_cutoff_freq_tuner_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_highpass_cutoff_freq_tuner_sizer,
        	value=self.highpass_cutoff_freq_tuner,
        	callback=self.set_highpass_cutoff_freq_tuner,
        	label="High-Pass Cutoff Frequency (Hz)",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._highpass_cutoff_freq_tuner_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_highpass_cutoff_freq_tuner_sizer,
        	value=self.highpass_cutoff_freq_tuner,
        	callback=self.set_highpass_cutoff_freq_tuner,
        	minimum=0,
        	maximum=1600,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_highpass_cutoff_freq_tuner_sizer, 0, 43, 1, 32)
        self.direction_probe = blocks.probe_signal_i()
        _angle_tuner_sizer = wx.BoxSizer(wx.VERTICAL)
        self._angle_tuner_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_angle_tuner_sizer,
        	value=self.angle_tuner,
        	callback=self.set_angle_tuner,
        	label="Angle of Approach of the Target (Deg)",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._angle_tuner_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_angle_tuner_sizer,
        	value=self.angle_tuner,
        	callback=self.set_angle_tuner,
        	minimum=0,
        	maximum=89,
        	num_steps=890,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_angle_tuner_sizer, 1, 17, 1, 26)
        _RF_tuner_sizer = wx.BoxSizer(wx.VERTICAL)
        self._RF_tuner_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_RF_tuner_sizer,
        	value=self.RF_tuner,
        	callback=self.set_RF_tuner,
        	label="Radar Frequency (Hz)",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._RF_tuner_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_RF_tuner_sizer,
        	value=self.RF_tuner,
        	callback=self.set_RF_tuner,
        	minimum=2.4e9,
        	maximum=2.5e9,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_RF_tuner_sizer, 1, 0, 1, 17)
        self.wxgui_waterfallsink = waterfallsink2.waterfall_sink_c(
        	self.notebook.GetPage(1).GetWin(),
        	baseband_freq=0,
        	dynamic_range=100,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate_sink,
        	fft_size=1024,
        	fft_rate=100,
        	average=False,
        	avg_alpha=None,
        	title="Time/Frequency CW Doppler Radar",
        	win=window.blackmanharris,
        )
        self.notebook.GetPage(1).Add(self.wxgui_waterfallsink.win)
        self.wxgui_fftsink2_full_spectrum = fftsink2.fft_sink_c(
        	self.notebook.GetPage(2).GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=4096,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT CW Doppler Radar Receiver Full Spectrum",
        	peak_hold=False,
        	win=window.blackmanharris,
        )
        self.notebook.GetPage(2).Add(self.wxgui_fftsink2_full_spectrum.win)
        self.wxgui_fftsink = fftsink2.fft_sink_c(
        	self.notebook.GetPage(0).GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate_sink,
        	fft_size=1024,
        	fft_rate=100,
        	average=False,
        	avg_alpha=None,
        	title="FFT CW Doppler Radar Receiver ",
        	peak_hold=False,
        	win=window.blackmanharris,
        )
        self.notebook.GetPage(0).Add(self.wxgui_fftsink.win)
        self.usrp_transmitter = uhd.usrp_sink(
        	",".join(('addr=192.168.10.2', "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.usrp_transmitter.set_clock_source("internal", 0)
        self.usrp_transmitter.set_samp_rate(samp_rate)
        self.usrp_transmitter.set_center_freq(RF_tuner, 0)
        self.usrp_transmitter.set_gain(0, 0)
        self.usrp_transmitter.set_antenna('TX/RX', 0)
        self.usrp_receiver = uhd.usrp_source(
        	",".join(('addr=192.168.10.3', "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.usrp_receiver.set_clock_source("mimo", 0)
        self.usrp_receiver.set_time_source("mimo", 0)
        self.usrp_receiver.set_samp_rate(samp_rate)
        self.usrp_receiver.set_center_freq(uhd.tune_request(RF_tuner, lo_offset_freq), 0)
        self.usrp_receiver.set_gain(rx_gain_tuner, 0)
        self.usrp_receiver.set_antenna("J1", 0)
        self.usrp_receiver.set_bandwidth(15e6, 0)
        def _target_speed_probe():
            while True:
                val = self.speed_probe.level()
                try:
                    self.set_target_speed(val)
                except AttributeError:
                    pass
                time.sleep(1.0 / (2))
        _target_speed_thread = threading.Thread(target=_target_speed_probe)
        _target_speed_thread.daemon = True
        _target_speed_thread.start()
        def _target_direction_probe():
            while True:
                val = self.direction_probe.level()
                try:
                    self.set_target_direction(val)
                except AttributeError:
                    pass
                time.sleep(1.0 / (2))
        _target_direction_thread = threading.Thread(target=_target_direction_probe)
        _target_direction_thread.daemon = True
        _target_direction_thread.start()
        self._speed_textbox_text_box = forms.text_box(
        	parent=self.GetWin(),
        	value=self.speed_textbox,
        	callback=self.set_speed_textbox,
        	label="Target Speed (Kph)",
        	converter=forms.float_converter(),
        )
        self.GridAdd(self._speed_textbox_text_box, 3, 0, 1, 17)
        self.rational_resampler = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=int(samp_rate/samp_rate_sink),
                taps=None,
                fractional_bw=None,
        )
        self.fft_vxx_0 = fft.fft_vcc(fft_len, True, (window.blackmanharris(fft_len)), True, 1)
        self._direction_textbox_text_box = forms.text_box(
        	parent=self.GetWin(),
        	value=self.direction_textbox,
        	callback=self.set_direction_textbox,
        	label="Target Direction",
        	converter=forms.str_converter(),
        )
        self.GridAdd(self._direction_textbox_text_box, 4, 0, 1, 17)
        self.cwradar_vector_flip_ff = cwradar.vector_flip_ff(fft_len/2)
        self.cwradar_doppler_velocity_single_target_ff_0 = cwradar.doppler_velocity_single_target_ff(fft_len/2, samp_rate, RF_tuner, threshold_dB_tuner, angle_tuner, lowpass_cutoff_freq_tuner, highpass_cutoff_freq_tuner)
        self.complex_to_mag = blocks.complex_to_mag(fft_len)
        self.blocks_vector_to_stream_0_0 = blocks.vector_to_stream(gr.sizeof_float*1, fft_len)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_float*1, fft_len)
        self.blocks_stream_to_vector_1_0 = blocks.stream_to_vector(gr.sizeof_float*1, fft_len/2)
        self.blocks_stream_to_vector_1 = blocks.stream_to_vector(gr.sizeof_float*1, fft_len/2)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, fft_len)
        self.blocks_keep_m_in_n_0_0 = blocks.keep_m_in_n(gr.sizeof_float, fft_len/2, fft_len, fft_len/2)
        self.blocks_keep_m_in_n_0 = blocks.keep_m_in_n(gr.sizeof_float, fft_len/2, fft_len, 0)
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 0, tx_amp_tuner, 0)
        self.DC_filter_0 = blocks.multiply_const_vff(([0]*DC_filter_num_elements+[1]*((fft_len/2)-DC_filter_num_elements)))
        self.DC_filter = blocks.multiply_const_vff(([0]*DC_filter_num_elements+[1]*((fft_len/2)-DC_filter_num_elements)))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_vector_to_stream_0_0, 0), (self.blocks_keep_m_in_n_0_0, 0))
        self.connect((self.blocks_keep_m_in_n_0, 0), (self.blocks_stream_to_vector_1, 0))
        self.connect((self.blocks_keep_m_in_n_0_0, 0), (self.blocks_stream_to_vector_1_0, 0))
        self.connect((self.blocks_vector_to_stream_0, 0), (self.blocks_keep_m_in_n_0, 0))
        self.connect((self.blocks_stream_to_vector_1_0, 0), (self.DC_filter_0, 0))
        self.connect((self.blocks_stream_to_vector_1, 0), (self.cwradar_vector_flip_ff, 0))
        self.connect((self.cwradar_vector_flip_ff, 0), (self.DC_filter, 0))
        self.connect((self.rational_resampler, 0), (self.wxgui_waterfallsink, 0))
        self.connect((self.rational_resampler, 0), (self.wxgui_fftsink, 0))
        self.connect((self.fft_vxx_0, 0), (self.complex_to_mag, 0))
        self.connect((self.blocks_stream_to_vector_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.complex_to_mag, 0), (self.blocks_vector_to_stream_0, 0))
        self.connect((self.complex_to_mag, 0), (self.blocks_vector_to_stream_0_0, 0))
        self.connect((self.DC_filter_0, 0), (self.cwradar_doppler_velocity_single_target_ff_0, 1))
        self.connect((self.DC_filter, 0), (self.cwradar_doppler_velocity_single_target_ff_0, 0))
        self.connect((self.cwradar_doppler_velocity_single_target_ff_0, 0), (self.speed_probe, 0))
        self.connect((self.cwradar_doppler_velocity_single_target_ff_0, 1), (self.direction_probe, 0))
        self.connect((self.usrp_receiver, 0), (self.wxgui_fftsink2_full_spectrum, 0))
        self.connect((self.usrp_receiver, 0), (self.rational_resampler, 0))
        self.connect((self.usrp_receiver, 0), (self.blocks_stream_to_vector_0, 0))
        self.connect((self.analog_sig_source_x_0, 0), (self.usrp_transmitter, 0))



    def get_lo_offset_freq(self):
        return self.lo_offset_freq

    def set_lo_offset_freq(self, lo_offset_freq):
        self.lo_offset_freq = lo_offset_freq
        self.usrp_receiver.set_center_freq(uhd.tune_request(self.RF_tuner, self.lo_offset_freq), 0)

    def get_threshold_dB(self):
        return self.threshold_dB

    def set_threshold_dB(self, threshold_dB):
        self.threshold_dB = threshold_dB
        self.set_threshold_dB_tuner(self.threshold_dB)

    def get_RF(self):
        return self.RF

    def set_RF(self, RF):
        self.RF = RF
        self.set_RF_tuner(self.RF)

    def get_DC_filter_num_elements(self):
        return self.DC_filter_num_elements

    def set_DC_filter_num_elements(self, DC_filter_num_elements):
        self.DC_filter_num_elements = DC_filter_num_elements
        self.DC_filter.set_k(([0]*self.DC_filter_num_elements+[1]*((self.fft_len/2)-self.DC_filter_num_elements)))
        self.DC_filter_0.set_k(([0]*self.DC_filter_num_elements+[1]*((self.fft_len/2)-self.DC_filter_num_elements)))

    def get_speed_samp_rate(self):
        return self.speed_samp_rate

    def set_speed_samp_rate(self, speed_samp_rate):
        self.speed_samp_rate = speed_samp_rate

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.cwradar_doppler_velocity_single_target_ff_0.set_samples_per_sec(self.samp_rate)
        self.wxgui_fftsink2_full_spectrum.set_sample_rate(self.samp_rate)
        self.usrp_receiver.set_samp_rate(self.samp_rate)
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)
        self.usrp_transmitter.set_samp_rate(self.samp_rate)

    def get_highpass_cutoff_freq(self):
        return self.highpass_cutoff_freq

    def set_highpass_cutoff_freq(self, highpass_cutoff_freq):
        self.highpass_cutoff_freq = highpass_cutoff_freq
        self.set_highpass_cutoff_freq_tuner(self.highpass_cutoff_freq)

    def get_lowpass_cutoff_freq(self):
        return self.lowpass_cutoff_freq

    def set_lowpass_cutoff_freq(self, lowpass_cutoff_freq):
        self.lowpass_cutoff_freq = lowpass_cutoff_freq
        self.set_lowpass_cutoff_freq_tuner(self.lowpass_cutoff_freq)

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len
        self.blocks_keep_m_in_n_0_0.set_offset(self.fft_len/2)
        self.blocks_keep_m_in_n_0_0.set_m(self.fft_len/2)
        self.blocks_keep_m_in_n_0_0.set_n(self.fft_len)
        self.blocks_keep_m_in_n_0.set_m(self.fft_len/2)
        self.blocks_keep_m_in_n_0.set_n(self.fft_len)
        self.DC_filter.set_k(([0]*self.DC_filter_num_elements+[1]*((self.fft_len/2)-self.DC_filter_num_elements)))
        self.DC_filter_0.set_k(([0]*self.DC_filter_num_elements+[1]*((self.fft_len/2)-self.DC_filter_num_elements)))

    def get_samp_rate_sink(self):
        return self.samp_rate_sink

    def set_samp_rate_sink(self, samp_rate_sink):
        self.samp_rate_sink = samp_rate_sink
        self.wxgui_fftsink.set_sample_rate(self.samp_rate_sink)
        self.wxgui_waterfallsink.set_sample_rate(self.samp_rate_sink)

    def get_angle(self):
        return self.angle

    def set_angle(self, angle):
        self.angle = angle
        self.set_angle_tuner(self.angle)

    def get_tx_amp(self):
        return self.tx_amp

    def set_tx_amp(self, tx_amp):
        self.tx_amp = tx_amp
        self.set_tx_amp_tuner(self.tx_amp)

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain
        self.set_rx_gain_tuner(self.rx_gain)

    def get_target_speed(self):
        return self.target_speed

    def set_target_speed(self, target_speed):
        self.target_speed = target_speed
        self.set_speed_textbox(self.target_speed)

    def get_target_direction(self):
        return self.target_direction

    def set_target_direction(self, target_direction):
        self.target_direction = target_direction
        self.set_direction_textbox(self.target_direction)

    def get_tx_amp_tuner(self):
        return self.tx_amp_tuner

    def set_tx_amp_tuner(self, tx_amp_tuner):
        self.tx_amp_tuner = tx_amp_tuner
        self._tx_amp_tuner_slider.set_value(self.tx_amp_tuner)
        self._tx_amp_tuner_text_box.set_value(self.tx_amp_tuner)
        self.analog_sig_source_x_0.set_amplitude(self.tx_amp_tuner)

    def get_threshold_dB_tuner(self):
        return self.threshold_dB_tuner

    def set_threshold_dB_tuner(self, threshold_dB_tuner):
        self.threshold_dB_tuner = threshold_dB_tuner
        self.cwradar_doppler_velocity_single_target_ff_0.set_signal_min_dB(self.threshold_dB_tuner)
        self._threshold_dB_tuner_slider.set_value(self.threshold_dB_tuner)
        self._threshold_dB_tuner_text_box.set_value(self.threshold_dB_tuner)

    def get_speed_textbox(self):
        return self.speed_textbox

    def set_speed_textbox(self, speed_textbox):
        self.speed_textbox = speed_textbox
        self._speed_textbox_text_box.set_value(self.speed_textbox)

    def get_rx_gain_tuner(self):
        return self.rx_gain_tuner

    def set_rx_gain_tuner(self, rx_gain_tuner):
        self.rx_gain_tuner = rx_gain_tuner
        self._rx_gain_tuner_slider.set_value(self.rx_gain_tuner)
        self._rx_gain_tuner_text_box.set_value(self.rx_gain_tuner)
        self.usrp_receiver.set_gain(self.rx_gain_tuner, 0)

    def get_lowpass_cutoff_freq_tuner(self):
        return self.lowpass_cutoff_freq_tuner

    def set_lowpass_cutoff_freq_tuner(self, lowpass_cutoff_freq_tuner):
        self.lowpass_cutoff_freq_tuner = lowpass_cutoff_freq_tuner
        self.cwradar_doppler_velocity_single_target_ff_0.set_lowpass_cut_freq(self.lowpass_cutoff_freq_tuner)
        self._lowpass_cutoff_freq_tuner_slider.set_value(self.lowpass_cutoff_freq_tuner)
        self._lowpass_cutoff_freq_tuner_text_box.set_value(self.lowpass_cutoff_freq_tuner)

    def get_highpass_cutoff_freq_tuner(self):
        return self.highpass_cutoff_freq_tuner

    def set_highpass_cutoff_freq_tuner(self, highpass_cutoff_freq_tuner):
        self.highpass_cutoff_freq_tuner = highpass_cutoff_freq_tuner
        self.cwradar_doppler_velocity_single_target_ff_0.set_highpass_cut_freq(self.highpass_cutoff_freq_tuner)
        self._highpass_cutoff_freq_tuner_slider.set_value(self.highpass_cutoff_freq_tuner)
        self._highpass_cutoff_freq_tuner_text_box.set_value(self.highpass_cutoff_freq_tuner)

    def get_direction_textbox(self):
        return self.direction_textbox

    def set_direction_textbox(self, direction_textbox):
        self.direction_textbox = direction_textbox
        self._direction_textbox_text_box.set_value(self.direction_textbox)

    def get_angle_tuner(self):
        return self.angle_tuner

    def set_angle_tuner(self, angle_tuner):
        self.angle_tuner = angle_tuner
        self.cwradar_doppler_velocity_single_target_ff_0.set_angle_of_approach(self.angle_tuner)
        self._angle_tuner_slider.set_value(self.angle_tuner)
        self._angle_tuner_text_box.set_value(self.angle_tuner)

    def get_RF_tuner(self):
        return self.RF_tuner

    def set_RF_tuner(self, RF_tuner):
        self.RF_tuner = RF_tuner
        self.cwradar_doppler_velocity_single_target_ff_0.set_radar_freq(self.RF_tuner)
        self._RF_tuner_slider.set_value(self.RF_tuner)
        self._RF_tuner_text_box.set_value(self.RF_tuner)
        self.usrp_receiver.set_center_freq(uhd.tune_request(self.RF_tuner, self.lo_offset_freq), 0)
        self.usrp_transmitter.set_center_freq(self.RF_tuner, 0)

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    parser.add_option("", "--lo-offset-freq", dest="lo_offset_freq", type="eng_float", default=eng_notation.num_to_str(1e6),
        help="Set Local Oscilator Offset Frequency [default=%default]")
    parser.add_option("", "--threshold-dB", dest="threshold_dB", type="eng_float", default=eng_notation.num_to_str(-70),
        help="Set Threshold (dB) [default=%default]")
    parser.add_option("", "--RF", dest="RF", type="eng_float", default=eng_notation.num_to_str(2.49e9),
        help="Set Radio Frequency (Hz) [default=%default]")
    parser.add_option("", "--DC-filter-num-elements", dest="DC_filter_num_elements", type="intx", default=4,
        help="Set DC Filter Num Elements [default=%default]")
    parser.add_option("", "--speed-samp-rate", dest="speed_samp_rate", type="eng_float", default=eng_notation.num_to_str(1),
        help="Set Speed Sample Rate (Hz) [default=%default]")
    parser.add_option("", "--samp-rate", dest="samp_rate", type="eng_float", default=eng_notation.num_to_str(1e6),
        help="Set Sample Rate (Hz) [default=%default]")
    parser.add_option("", "--highpass-cutoff-freq", dest="highpass_cutoff_freq", type="eng_float", default=eng_notation.num_to_str(0),
        help="Set High-Pass Cutoff Frequency (Hz) [default=%default]")
    parser.add_option("", "--lowpass-cutoff-freq", dest="lowpass_cutoff_freq", type="eng_float", default=eng_notation.num_to_str(1700),
        help="Set Low-Pass Cutoff Frequency (Hz) [default=%default]")
    parser.add_option("", "--fft-len", dest="fft_len", type="intx", default=pow(2,20),
        help="Set FFT Length [default=%default]")
    parser.add_option("", "--samp-rate-sink", dest="samp_rate_sink", type="intx", default=8000,
        help="Set Sample Rate FFT Sink (Hz) [default=%default]")
    parser.add_option("", "--angle", dest="angle", type="eng_float", default=eng_notation.num_to_str(0),
        help="Set Angle (Deg) [default=%default]")
    parser.add_option("", "--tx-amp", dest="tx_amp", type="eng_float", default=eng_notation.num_to_str(1),
        help="Set TX Amplitude [default=%default]")
    parser.add_option("", "--rx-gain", dest="rx_gain", type="eng_float", default=eng_notation.num_to_str(30.5),
        help="Set RX Gain (dB) [default=%default]")
    (options, args) = parser.parse_args()
    if gr.enable_realtime_scheduling() != gr.RT_OK:
        print "Error: failed to enable realtime scheduling."
    tb = CW_Radar_Single_Target(lo_offset_freq=options.lo_offset_freq, threshold_dB=options.threshold_dB, RF=options.RF, DC_filter_num_elements=options.DC_filter_num_elements, speed_samp_rate=options.speed_samp_rate, samp_rate=options.samp_rate, highpass_cutoff_freq=options.highpass_cutoff_freq, lowpass_cutoff_freq=options.lowpass_cutoff_freq, fft_len=options.fft_len, samp_rate_sink=options.samp_rate_sink, angle=options.angle, tx_amp=options.tx_amp, rx_gain=options.rx_gain)
    tb.Start(True)
    tb.Wait()
