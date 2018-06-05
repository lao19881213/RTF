
import data_set_loader

import numpy as np

ms='3c391_ctm_mosaic_spw0.ms'
data = data_set_loader.data_set_loader(ms,read_jones_terms=False) 
data.read_head() 

data.read_data()
#visibility data
vis = data._arr_data
#antenna ID
ant1 = data._arr_antenna_1 
ant2 = data._arr_antenna_2
#Observation time
time_stamp = data._time_wc
#channel frequency
chan_freq = data._chan_freqs

baseline0 = []
baseline_t = []
#read one baseline data and save
for r in range(len(ant1)):
    if ant1[r] == 0 and ant2[r] == 2 :
        baseline0.append(vis[r,:,0])
        baseline_t.append(time_stamp[r])

baseline0 = np.array(baseline0)
baseline_t = np.array(baseline_t)
np.savetxt('3c391_test.dat',baseline0)



