# -*- coding: utf-8 -*-
"""
@author: JS 2016
@brief: Multisignal human-computer interface
        Script for data processing and visualization
"""
import numpy as np
from matplotlib import pyplot
from scipy.fftpack import fft

MAX_ADC_LVL = 4095
RANGE_BEGINING = 0
RANGE_ENDING = 20000

with open('/testy/ecg_noise.log', 'r') as content_file:
    raw_file = content_file.read()
   
splitted = raw_file.split('\n')
splitted = splitted[:-1]
processed = [((int(a)/4095.0) * 3.3) for a in splitted ]

y = [x for x in processed if x < MAX_ADC_LVL]

pyplot.plot(y[RANGE_BEGINING:RANGE_ENDING])
pyplot.title("EMG waveform")
pyplot.ylabel("Amplitude [V]")
pyplot.xlabel("Time [ms]")

#FOURIER
y_fft = y[RANGE_BEGINING:RANGE_ENDING]
N = RANGE_ENDING - RANGE_BEGINING
## sample spacing
T = 1.0 / 1000.0 #1kHz
xf = np.linspace(0.0, 1.0/(2.0*T), N/2)
yf = fft(y_fft)
pyplot.figure()
axes = pyplot.gca()
pyplot.plot(xf, 2.0/N * np.abs(yf[0:N/2]))
pyplot.title("Power spectral density")
pyplot.ylabel("Power")
pyplot.xlabel("Frequency [Hz]")

pyplot.grid()
axes.set_ylim([0,0.12])
axes.set_xlim([0,120])
pyplot.show()
