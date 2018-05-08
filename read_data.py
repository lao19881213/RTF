
import data_set_loader

import numpy as np

ms='3c391_ctm_mosaic_spw0.ms'
data = data_set_loader.data_set_loader(ms,read_jones_terms=False) 
data.read_head() 

data.read_data()

DATA=data._arr_data

DATA_I=DATA[:,:,0]

np.savetxt('3c391_test.dat',DATA_I)


print DATA.shape
