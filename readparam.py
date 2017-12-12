import numpy as np

def readparam(path):
    f = open(path)
    f.readline()
    data = np.zeros((1000, 10))
    for i, line in enumerate(f):
        tmp_params = line.split()
        tmp_params = [float(param) for param in tmp_params]
        data[i, :] = tmp_params
    f.close()
    return data