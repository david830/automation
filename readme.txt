This repository shows how to generate circuit designs from experimental data.  "module1_ind" and
"module2_ind" contain the experimental data.  In each folder, there is a "run_analysis.m" file. 
This script uses a bootstrapping method to calculate the parameter distributions.  Afterwards, Run
"enumerate_ode.py".  This script enumerates possible circuit structures, builds models and verifies
the models against PBLTL specs. 
