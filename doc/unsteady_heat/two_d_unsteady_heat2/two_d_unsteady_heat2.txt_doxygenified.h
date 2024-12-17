/**

\mainpage Example problem: Solution of the 2D unsteady heat equation with restarts
 
Simulations of time-dependent problem can be very time consuming
and it is important to be able to restart simulations, e.g. to 
continue a run after a system crash, etc. 
We shall illustrate \c oomph-lib's dump/restart capabilities by
re-visiting the 2D unsteady heat equation discussed
in a  <A HREF="../../two_d_unsteady_heat/html/index.html">previous example.</A>

<CENTER>
<TABLE>
<TR> 
<TD>
<CENTER>
<B>The two-dimensional unsteady heat equation in a square domain.</B>
</CENTER> 
Solve
\f[
\sum_{i=1}^2\frac{\partial^2 u}{\partial x_i^2} 
= \frac{\partial u}{\partial t} + f\left(x_1,x_2,t\right),
 \ \ \ \ \ \ \ \ \ \ (1)
\f]
in the square domain \f$ D = \{x_i \in [0,1]; i=1,2 \} \f$ ,
subject to the Dirichlet boundary conditions
\f[
\left. u\right|_{\partial D}=g_0
\ \ \ \ \ \ \ \ \ \ (2)
\f]
and initial conditions
\f[
u(x_1,x_2,t=0)=h_0(x_1,x_2),
\ \ \ \ \ \ \ \ \ \ (3)
\f]
where the functions \f$ g_0 \f$ and  \f$ h_0 \f$ are given.
</TD>
</TR>
</TABLE>  
</CENTER>

As before, we consider the unforced case, \f$ f=0 \f$, and choose boundary
and initial conditions that are consistent with the exact solution
\f[
u_0(x_1,x_2,t) = e^{-Kt}\sin\left( \sqrt{K} \left( x_1 \cos \Phi + x_2 \sin
\Phi\right)\right),
 \ \ \ \ \ \ \ \ \ \ (4)
\f]
where \f$ K \f$ and \f$ \Phi \f$ are constants, controlling the
decay rate of the solution and its spatial orientation, respectively.

The figure below shows a plot of computed and exact solution at 
a control node as a function of time. The solid lines represent
quantities computed during the original simulation; the dashed line
shows the corresponding data from a second simulation that was 
restarted with the restart file generated at timestep 23 of
the original simulation.


\image html trace.gif "Time evolution of the computed and exact solutions at a control node, the global error norm and the norm of the solution. Solid lines: original simulation; dashed lines: restarted simulation. " 
\image latex trace.eps "Time evolution of the computed and exact solutions at a control node, the global error norm and the norm of the solution. Solid lines: original simulation; dashed lines: restarted simulation. " width=0.75\textwidth
 


Most of the driver code for this example is identical to that 
discussed in the 
<A HREF="../../two_d_unsteady_heat/html/index.html">previous example,</A>
therefore we only discuss the modifications required to enable the
dump and restart operations:
- We use optional command line arguments to specify the name of the restart
  file. If the code is run without any command line arguments, we
  start the simulation at time \f$ t=0 \f$ and generate the
  initial conditions as in the 
  <A HREF="../../two_d_unsteady_heat/html/index.html">previous example</A>
- We add dump and restart functions to the Problem class and
  call the dump function when post-processing the solution 
  in \c doc_solution(...).
- We modify the Problem member function \c set_initial_condition()        
  so that the initial conditions are read from the restart file
  if a restart file was specified on the command line.
.
<HR>
<HR> 

\section namespace Global parameters and functions

The namespace \c ExactSolnForUnsteadyHeat 
that stores the problem parameters is identical
to that in the <A HREF="../../two_d_unsteady_heat/html/index.html">previous example.</A>

<HR>
<HR>

\section main The driver code

The only change to the main function is that we record the command line
arguments and store them in the namespace \c CommandLineArgs

\dontinclude two_d_unsteady_heat_restarted.cc
\skipline start_of_main
\until CommandLine

The rest of the main function is identical
to that in the <A HREF="../../two_d_unsteady_heat/html/index.html">previous example.</A>

<HR>
<HR>

\section problem The problem class 
 
The problem class contains the two additional member functions

\dontinclude two_d_unsteady_heat_restarted.cc
\skipline Dump
\until restart_file

<HR>
<HR>

\section constructor The problem constructor

The  problem constructor is identical to that in the 
<A HREF="../../two_d_unsteady_heat/html/index.html">previous example.</A>


<HR>
<HR>

\section destructor The problem destructor

The  problem destructor is identical to that in the 
<A HREF="../../two_d_unsteady_heat/html/index.html">previous example.</A>


<HR>
<HR>

\section before_timestep Actions before timestep
This function is identical to that in the 
<A HREF="../../two_d_unsteady_heat/html/index.html">previous example.</A>


<HR>
<HR>

\section IC Set initial condition
We start by checking the validity of the command line arguments,
accessible via the namespace \c CommandLineArgs, as
only a single command line argument is allowed. If a command line 
argument is provided, it is interpreted as the name of the restart file. 
We try to open the file and, if successful, pass the input stream
to the \c restart(...) function, discussed below. If no command line
arguments are specified, we generate the initial conditions, 
essentially as in the  
<A HREF="../../two_d_unsteady_heat/html/index.html">previous example.</A>
The only difference is that in the current version of the code, we 
moved the specification and initialisation of the timestep from the
\c main function into 
\c set_initial_condition(). This is because in a restarted
simulation, the value of \c dt must be consistent with that
used in the original simulation. If the simulation is restarted,
the generic \c Problem::read(...) function, called by \c restart(...), 
automatically initialises the previous timestep; otherwise
we have to perform the initialisation ourselves.

\skipline start_of_set_initial_condition
\until end of set_initial_condition

<HR>
<HR>


\section doc Post-processing
The Problem member function \c doc_solution(...) is identical to that
in the  <A HREF="../../two_d_unsteady_heat/html/index.html">previous 
example,</A> apart from the addition of a call to the new \c
dump_it(...) function, discussed below. 



\skipline Write restart file
\until  some_file.close();

<HR>
<HR>

\section dump Dumping the solution

The \c Problem::dump(...) function writes the generic \c Problem data
in ASCII format to the specified output file. The content of the
file can therefore be inspected and, if necessary, manipulated
before a restart. However, the specific content of the file is
generally of little interest -- it is written in a format that
can be read by the corresponding function  \c Problem::read(...). 

 Briefly, the dump file contains:
- A flag that indicates if the data was produced by a time-dependent
  simulation.
- The current value of the "continuous" time, i.e. the value returned
  by \c Problem::time_pt()->time();
- The number of previous timesteps, \c dt, stored in the \c Problem's \c Time
  object, and their values. 
- The values and history values for all \c Data objects in
  the \c Problem, as well as the present and previous coordinates
  of all \c Nodes.
.
The "raw data" is augmented by brief comments that facilitate 
the identification of individual entries. 

In the present problem, the generic \c Problem::dump(...) function
is sufficient to record the current state of the simulation, therefore
no additional information needs to be added to the dump file.
The section \ref comments below contains an exercise that 
illustrates how to customise the dump function to record additional 
parameters; the 
<A HREF="../../two_d_unsteady_heat_ALE/html/index.html">demo code
with spatial adaptivity</A> provides another example of a customised
dump/restart function.

\skipline start_of_dump_it
\until end of dump_it

 
<HR>
<HR>

\section read Reading a solution from disk

Since the restart file was written by the generic \c
Problem::dump(...) function, it can be read back with the
generic  \c Problem::read(...) function. If any additional data
had been recorded in the restart file, additional read statements
could be added here; see \ref comments.

\skipline start_of_restart
\until end of restart

<HR>
<HR>

\section comments Comments and Exercises

The \c Problem::dump(...) and \c Problem::read(...)
functions write/read the generic data that is common to 
all \c oomph-lib problems. Occasionally, it is necessary 
to record additional data to re-create the system's
state when the simulations is restarted. We will explore this
in more detail in 
<A HREF="../../two_d_unsteady_heat_2adapt/html/index.html">another
example</A>. Here we provide a brief exercise that illustrates 
the general idea and addresses a shortcoming of the driver code:
Currently the program computes the same number of timesteps,
regardless of whether or not the simulation was
restarted. If a simulation is restarted, the computation therefore
continues past \f$ t=t_{max}. \f$
  
\subsection exercises Exercises
- Change the \c for loop over the fixed number of timesteps
  in the \c main function to a \c while loop that checks if
  the continuous time, accessible via \c Problem::time_pt()->time(), 
  has reached or exceeded \f$ t_{max} \f$. This works because
  the \c Problem::dump(...) and \c Problem::read(...) functions
  dump and restore the value of the continuous time. 
- Following this trivial change, the restarted simulation 
  is terminated at the appropriate point but the numbering of the
  output files begins at 0, making it difficult to merge the 
  results from the original and the restarted simulations.
  Modify the functions \c dump_it(...) and \c restart(...) so that
  they write/read the current label for the output files
  to/from the restart file.
  \n\n \b Hints: <B>(i)</B> You can write to/read from the restart file
  before calling \c Problem::dump(...) and \c Problem::read(...); just
  make sure you do it in the same order in both functions. <B>(ii)</B> 
  The easiest way to make the label, currently stored in the \c
  DocInfo object in the \c main function,
  accessible to all member functions in the \c Problem is to 
  make the \c DocInfo object a private data member of the \c Problem 
  class.




<HR>
<HR>


\section sources Source files for this tutorial
- The source files for this tutorial are located in the directory:
<CENTER>
<A HREF="../../../../demo_drivers/unsteady_heat/two_d_unsteady_heat/">
demo_drivers/unsteady_heat/two_d_unsteady_heat/
</A>
</CENTER>
- The driver code is: 
<CENTER>
<A HREF="../../../../demo_drivers/unsteady_heat/two_d_unsteady_heat/two_d_unsteady_heat_restarted.cc">
demo_drivers/unsteady_heat/two_d_unsteady_heat/two_d_unsteady_heat_restarted.cc
</A>
</CENTER>
.















<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

