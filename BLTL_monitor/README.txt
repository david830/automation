checker is a trace checker for BLTL formulae. The usage is
checker <property-file> <trace-file>

<property-file> is a text file containing the BLTL property to check
<trace-file> is a text file containing the trace data

you can plot the trace file with gnuplot:

plot 'trace1.txt' using 1:2, 'trace1.txt' using 1:3
plot 'trace1.txt' using 1:2, 'trace1.txt' using 1:3, 'trace1.txt' using 1:4

====================
BLTL property format
====================
The semantics of BLTL is presented in the HSCC 2010 paper on Bayesian 
Statistical Model Checking. The examples folder has a few examples of
the syntax accepted by the checker. 

From http://en.wikipedia.org/wiki/Linear_temporal_logic

\rho U \phi :	Until: \rho has to hold at least until \phi, which holds at the current or a future state.
G \phi : 	Globally: \phi has to hold on the entire subsequent path.
F \phi :	Finally: \phi eventually has to hold (somewhere on the subsequent path).

For the BLTL time bounded versions, more help is at
http://www.prismmodelchecker.org/manual/PropertySpecification/ThePOperator

NOTE that, as interpreted by the monitor code, the first column is sim time as real value, and 
these time bounds in the BLTL expressions are on that absolute timescale, not an "index" into the
simulation results by line...  So the times t, below, can be floating-point values in the property file.


G[t] \phi : 	 Globally: \phi has to hold for at least t time units of the immediately subsequent path.
F[t] \phi :	 Finally: \phi eventually has to hold within t time units, somewhere on the subsequent path.


F[x] G[y] \phi : sometime within the next x time units, \phi must become true and hold true for at least y time units.

\phi U[t] \rho : within time t \rho must become true, and \phi must hold until then.  It's just like the "regular" 
until, except that \rho now must be true within time t.

	
=================
Trace data format
=================
Suppose we only have three variables v1, v2, v3, and the simulation time.
The format is a simple text file in which the first line contains the 
text '# time' followed by the names of the variables, space-separated 
(ie, 'v1 v2 v3' in our case). Each following line reports the value 
of time and of all the variables at that time point, space-separated. 
For example:
# time v1	v2 	v3
0.0	2.3 	4.5 	0
0.1	1.2	3.4	55.3
0.2	2.1	3.0	0

The trace above reports a simulation trace in which, for example, 
at time 0.1 the values of the variables v1, v2, and v3 were 1.2, 3.4, 
and 55.3, respectively.

