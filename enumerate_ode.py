import networkx as nx   # graph theory, circuit enumeration
import itertools   
from prufer_to_tree import *
import matplotlib.pyplot as plt
import os
import SMC.sample_1  # model checking 
import SMC.trajectory  # model checking
import readparam  # read data 
import timeit

## convert circuits to trees and enumerate trees
n = 2     # number of modules
prufer_code_lst = []
tree_lst = []
nx_tree_lst = []

for seq in itertools.product(range(1, n+2), repeat = n-1):
    prufer_code_lst.append(seq)      # Generate Prufer code
    tree_lst.append(TreeEdges(seq))  # Generate Labeled Tree from Pruefer's code
    tmp_nx_tree = nx.Graph(TreeEdges(seq))   
    tmp_nx_tree = nx.bfs_tree(tmp_nx_tree, 1)    # Change an undirected tree to a rooted directed tree
    tmp_nx_tree = tmp_nx_tree.reverse()      # Reverse the directions
    nx_tree_lst.append(tmp_nx_tree)      
    plt.figure()
    nx.draw_networkx(tmp_nx_tree)
    
# plt.show()

## import the histograms of the bootstrapping parameters
path1 = './module1_3_bootstrap_params_err.txt'  # module 1 and inducible promoter 1
path2 = './module2_3_bootstrap_params_err.txt'  # module 2 and inducible promoter 2
data1 = readparam.readparam(path1)
data2 = readparam.readparam(path2)
 
num_of_promoters = 2

from scipy.integrate import odeint
import numpy as np
from scipy.stats import beta

# parameters related to the module 
beta_mat = np.vstack((np.transpose(data1[:, 2]), np.transpose(data2[:, 2])))   #  beta: maximum production rate
lambda2_mat = np.vstack((np.transpose(data1[:, 3]), np.transpose(data2[:, 3])))   # lambda2: degradation rate 
gamma2_mat = np.vstack((np.transpose(data1[:, 7]), np.transpose(data2[:, 7])))   # gamma2: minimum fraction active
K_2_mat = np.vstack((np.transpose(data1[:, 8]), np.transpose(data2[:, 8])))    # K_2: dissociation coefficients
h_2_mat = np.vstack((np.transpose(data1[:, 9]), np.transpose(data2[:, 9])))  # size: 2 * 1000  # h_2: Hill coefficients

# parameters related to the inducible promoter
alpha_mat = np.vstack((np.transpose(data1[:, 0]), np.transpose(data2[:, 0])))  #  alpha: maximum production rate
lambda1_mat = np.vstack((np.transpose(data1[:, 1]), np.transpose(data2[:, 1])))  # lambda1: degradation rate
gamma1_mat = np.vstack((np.transpose(data1[:, 4]), np.transpose(data2[:, 4])))  # gamma1: minimum fraction active
K_mat = np.vstack((np.transpose(data1[:, 5]), np.transpose(data2[:, 5])))  # K: dissociation coefficients 
h_mat = np.vstack((np.transpose(data1[:, 6]), np.transpose(data2[:, 6])))  # h: Hill coefficients

num_pts_mod_1 = np.nonzero(K_mat[0, :])[0][-1]+1
num_pts_mod_2 = np.nonzero(K_mat[1, :])[0][-1]+1

## automate SMC for circuits using histograms of parameter fits
sol_lst = []
inducer_conc = 10
path_property = './property1.txt'
path_trace = './tmp_trace.txt'
for i, tmp_nx_tree in enumerate(nx_tree_lst):  # check every circuit design
    tmp_nx_edges = tmp_nx_tree.edges()  
    numbers_1 = range(1, n+2)  # contains all the nodes that have no incoming edges, inducible promoters
    for edge in tmp_nx_edges:  
        if edge[1] in numbers_1:  
            numbers_1.remove(edge[1])   # numbers_1 will contain all the nodes that are inputs to the circuit.
    for alpha_index in itertools.permutations(range(num_of_promoters), r = len(numbers_1)):  # itertools.permutations(alpha_lst, repeat = len(numbers_1)) is all permutations of promoters from the library in spots that are open.  
        def pend(y, t, K_lst, gamma_lst, alpha_lst, lambda1_lst, h_lst, alpha_index, gamma_2_lst, beta_lst, K_2_lst, lambda2_lst, h_2_lst):  # odes to integrate
            dydt = [0] * len(y)
            for k, j in enumerate(numbers_1):
                dydt[j-1] += alpha_lst[alpha_index[k]] * ((1 - 1/(1 + (inducer_conc/K_lst[alpha_index[k]]) ** h_lst[alpha_index[k]])) * (1 - gamma_lst[alpha_index[k]])  + gamma_lst[alpha_index[k]]) - lambda1_lst[j-2] * y[j-1]  
            for edge in tmp_nx_edges:   #  all the nodes that have ingoing edges.  
                dydt[edge[1]-1] += ((1 - gamma_2_lst[edge[0]-2])/(1+(y[edge[0]-1]/K_2_lst[edge[0]-2]) + gamma_2_lst[edge[0]-2])** h_2_lst[edge[0]-2]) * beta_lst[edge[0]-2] - lambda2_lst[edge[1]-2] * y[edge[1]-1]        
            return dydt      
        
        # now start model checking  
        t = range(600)  # simulation time     
        theta = 0.95  # confidence level in the PBLTL formula
        T = 100.0  # probability 0.99 ( 1- 1/T).  
        n_total = 0  # total number
        x = 0  # number of successes
        B = 0  # Bayes factor
        completed = False   # flag
        decision = '' 
        while not completed:
            y0 = [0] * (n+1)
            tmp_rand = np.array([np.random.randint(num_pts_mod_1)-1, np.random.randint(num_pts_mod_2)-1])   # randomly select a bootstrap parameter set 
            K_lst_1 = [10 ** K_vec[tmp_rand[j]] for j, K_vec in enumerate(K_mat)]    # the stored params are log10, now calculate the exponentials 
            gamma_lst_1 = [10 ** gamma_vec[tmp_rand[j]] for j, gamma_vec in enumerate(gamma1_mat)]   
            alpha_lst_1 = [10 ** alpha_vec[tmp_rand[j]] for j, alpha_vec in enumerate(alpha_mat)]
            h_lst_1 = [10 ** h_vec[tmp_rand[j]] for j, h_vec in enumerate(h_mat)]
            lambda1_lst_1 = [10 ** lambda1_vec[tmp_rand[j]] for j, lambda1_vec in enumerate(lambda1_mat)]
            tmp_rand_2 = np.array([np.random.randint(num_pts_mod_1)-1, np.random.randint(num_pts_mod_2)-1])
            gamma_2_lst_1 = [10 ** gamma_vec[tmp_rand_2[j]] for j, gamma_vec in enumerate(gamma2_mat)]
            beta_lst_1 = [10 ** beta_vec[tmp_rand_2[j]] for j, beta_vec in enumerate(beta_mat)]
            K_2_lst_1 = [10 ** K_2_vec[tmp_rand_2[j]] for j, K_2_vec in enumerate(K_2_mat)]
            lambda2_lst_1 = [10 ** lambda2_vec[tmp_rand_2[j]] for j, lambda2_vec in enumerate(lambda2_mat)]
            h_2_lst_1 = [10 ** h_2_vec[tmp_rand_2[j]] for j, h_2_vec in enumerate(h_2_mat)]
            sol = odeint(pend, y0, t, args = (K_lst_1, gamma_lst_1, alpha_lst_1, lambda1_lst_1, h_lst_1, alpha_index, gamma_2_lst_1, beta_lst_1, K_2_lst_1, lambda2_lst_1, h_2_lst_1))  # plug in the parameters and integrate ODEs           
            SMC.trajectory.build(path_trace, sol)  
            if SMC.trajectory.check2(path_property, path_trace):
                x += 1   # update x, the number of successes
            n_total += 1  # update n, the number of events
            B = theta/(1 - theta) * (1/ beta.cdf(theta, x+1, n_total-x+1) - 1)
            if B > T:
                completed = True
                decision = 'H_0 accepted'
            elif B < 1 / T:
                completed = True
                decision = 'H_1 accepted' 
            print n_total, x, B  
        print n_total, x, decision        

        sol_lst.append(sol)

