import sample_1
import re
import numpy as np

def build(path, time_matrix): # path where to store the temporary trace file
    num_parts = np.shape(time_matrix)[1]
    num_time_pts = np.shape(time_matrix)[0]
    headings = ['prot'+str(i) for i in range(num_parts)]
    e  = open(path, 'w')
    e.write('# time ' + ' '.join(headings) +'\n')
    for i in range(num_time_pts):
        e.write(str(i) + ' ' + np.array_str(time_matrix[i, :])[1:-1]+ '\n' )
    e.close
    
def check2(path_property, path_trace): # path_property: store the temporary property file, 
    result = False
    try:
        if sample_1.check(path_trace, path_property) == 'satisfiable!\n':
            result = True
    except:
        pass
    return result

    