/**

\mainpage "Action" functions in oomph-lib's black-box Newton solver

\c oomph-lib's \c Problem class provides a  black-box Newton solver,  
\c Problem::newton_solve(), which allows the driver code to be as 
compact as this:
\code
   main()
   {
     // Create the problem object
     MyProblem problem;
     
     // Solve the problem, using oomph-lib's default Newton solver
     problem.newton_solve();
   }
\endcode

While the availability of a black-box Newton solver is helpful, 
it is sometimes necessary to interact with the 
solution process, e.g. to perform certain additional 
tasks during the Newton iteration, or to analyse the convergence
(or divergence!) of the iteration in more detail. For this 
purpose \c oomph-lib's \c Problem class has a number of empty 
virtual member functions that are executed at key stages
of the Newton iteration. Two of these functions 
(\c Problem::actions_before_newton_solve() and \c
Problem::actions_after_newton_solve() ) are pure virtual functions that must
be implemented in every specific Problem, though they may, of course,
be implemented as empty functions. Other functions may be overloaded
to provide more detailed control. 

<HR>
<HR>   
   

\section newton_solve Problem::newton_solve()
The following table lists the main steps performed by \c oomph-lib's 
Newton solver and indicates at which point the various "action" 
functions are executed.


<TABLE>
<TR>
<TD>
\f[
\left. . \hspace{6cm} \right. 
\f]
</TD>
<TD>
<B>SEQUENCE OF MATHEMATICAL STEPS</B>
</TD>
<TD>
<B>SEQUENCE OF FUNCTION CALLS / KEY CONTROL PARAMETERS</B>
</TD>
</TR>
<TR>
<TD>
<B>Step 1:</B> Perform actions before solve.
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::actions_before_newton_solve();
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 2:</B> Initialise counter for number of Newton 
iterations.
</TD>
<TD>
\f[
c=0
\f]
</TD>
<TD>
\code 
unsigned count=0;
\endcode
</TD>
</TR>
<TR>
<TD COLSPAN=3>
<CENTER><B>Start loop over Newton iterations</B></CENTER>
</TD>
</TR>
<TR>
<TD>
<B>Step 3:</B> 
Perform actions before
the next Newton step
</TD>
<TD>
\f[
\left. . \hspace{6cm}\right.
\f]
</TD>
<TD>
\code 
Problem::actions_before_newton_step();
\endcode
</TD>
</TR>
<TR>
<TD COLSPAN=3>
<CENTER>
<B>Only during the first Newton step:</B>
</CENTER>
</TR>
<TR>
<TD style="padding-left: 30px;
border-left: 3px solid grey;">
<B>Step 4a:</B>
Perform actions before Newton convergence check
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::actions_before_newton_convergence_check();
\endcode
</TD>
</TR>
<TR>
<TD style="padding-left: 30px;
border-left: 3px solid grey;">
<B>Step 4b:</B>
Compute residual vector
</TD>
<TD>
\f[
{\cal R}_i = {\cal R}_i\left(U_1,...,U_{N}\right) 
\mbox{\ \ \ \  for $i=1,...,N$}
\f]
</TD>
<TD>
\code 
Vector<double> residuals;
Problem::get_residuals(residuals); 
\endcode
</TD>
</TR>
<TR>
<TD style="padding-left: 30px;
border-left: 3px solid grey;">
<B>Step 4c:</B>
Convergence check. 
If converged go to <B>Step 13</B>
</TD>
<TD>
\f[
\mbox{If $\max_i |{\cal R}_i| < {\tt tol} \Longrightarrow $
converged.}
\f]
</TD>
<TD>
\code

...

// Check convergence
if (max_res<Problem::Newton_solver_tolerance)
{
   ...
}

... 

\endcode
</TD>
</TR>
<TR><TD colspan="3" style="height:31px;"></TD></TR>
<TR>
<TD>
<B>Step 5:</B>
Check if maximum residual or number of iterations
exceed their maxima.
</TD>
<TD>
\f[
\mbox{If \ \ }
\left\{
\begin{array}{c}
c > {\tt Max\_newton\_iterations} \\
\mbox{ or } \\ 
\max_i |{\cal R}_i| > {\tt Max\_residuals}
\end{array}
\right\} 
\Longrightarrow \mbox{diverged.}
\f]
</TD>
<TD>
\code 

...

// Check divergence/non-convergence
if ((max_res>Problem::Max_residuals) ||
    (count == Problem::Max_newton_iterations))
{
   ...

   // Throw an error
   throw NewtonSolverError(count,max_res);

}

... 


\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 6:</B>
Solve linear system for correction of unknowns
</TD>
<TD>
\f[
\begin{array}{l}
\mbox{Solve} \\
\\ 
\hspace{1cm}\sum_{j=1}^N {\cal J}_{ij} \ \delta U_j = {\cal R}_i
\mbox{\ \ \ (for $i=1,...,N$)} \\
\\
\mbox{for  $\delta U_j$ \ \ ($j=1,...,N$).} \\
\\
\mbox{Here the Jacobian matrix is given by} \\
\\
\hspace{1cm}{\cal J}_{ij} = \frac{\partial {\cal R}_i}{\partial U_j}
\mbox{\ \ \ (for $i,j=1,...,N$)} \\
\end{array}
\f]
</TD>
<TD>
\code 
// Use the linear solver specified by 
// Problem::linear_solver_pt() to assemble
// and solve the linear system.
...
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 7:</B>
Apply the corrections to the solution
</TD>
<TD>
\f[
U_i := U_i - \delta U_i
\mbox{\ \ \ (for $i=1,...,N$)} \\
\f]
</TD>
<TD>
\code 
// Update the unknowns
...
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 8:</B>
Perform actions after
Newton step
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::actions_after_newton_step();
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 9:</B> Increment counter for number of Newton 
iterations
</TD>
<TD>
\f[
c:=c+1
\f]
</TD>
<TD>
\code 
count++;
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 10:</B>
Perform actions before Newton convergence check
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::actions_before_newton_convergence_check();
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 11:</B>
Compute residual vector
</TD>
<TD>
\f[
{\cal R}_i = {\cal R}_i\left(U_1,...,U_{N}\right) 
\mbox{\ \ \ \  for $i=1,...,N$}
\f]
</TD>
<TD>
\code 
Vector<double> residuals;
Problem::get_residuals(residuals); 
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 12:</B>
Convergence check. If converged go to <B>Step 13</B>, else
go to <B>Step 3</B>.
</TD>
<TD>
\f[
\mbox{If $\max_i |{\cal R}_i| < {\tt tol} \Longrightarrow$
converged.}
\f]
</TD>
<TD>
\code

...

// Check convergence
if (max_res<Problem::Newton_solver_tolerance)
{
   ...
}

... 

\endcode
</TD>
</TR>
<TR>
<TD COLSPAN=3>
<CENTER><B>End loop over Newton iterations</B></CENTER>
</TD>
</TR>
<TR>
<TD>
<B>Step 13:</B>
Converged. Perform actions after solve.
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::actions_after_newton_solve();
\endcode
</TD>
</TR>
</TABLE>


<HR>   
<HR>   

The function \c Problem::actions_before_newton_solve() is often used to update
boundary conditions when performing parameter studies in which these
change; \c Problem::actions_after_newton_solve() may be used to
automatically perform any post-processing when a solution has
been obtained. The function 
\c Problem::actions_before_newton_convergence_check() makes it
possible to update any "dependent" problem parameters, i.e. parameters
that depend on one or more of the unknowns in the problem but are not
updated automatically when the unknown is changed. This arises most
frequently in free-boundary problems in which the position of the
nodes in the "bulk" mesh is determined by an algebraic node update
procedure. When the Newton method updates the \c Data values that 
determine the shape of the domain boundary in Step 7, the nodal positions 
themselves are not updated automatically unless the node update 
function is executed in \c Problem::actions_before_newton_convergence_check().
See the discussion of the 
<A HREF="../../interaction/macro_element_free_boundary_poisson/html/index.html">free-boundary
Poisson problem</A> for an example of its use.

<HR>
<HR>   
   

\section opt Optimisation for linear problems
Recall that, by default, \c oomph-lib regards all problems as
nonlinear. Provided a good initial guess for the unknowns
is available, the Newton solver will converge quadratically towards
the solution. Within this framework, linear problems are simply special
cases for which the Newton iteration converges in one step. However,
if the problem is known to be linear, a few of the steps in the generic 
Newton iteration are unnecessary. For instance, it is not necessary
to check the residual before or after the Newton step as we know
that the exact solution will have been obtained (modulo roundoff
errors) after step 7. The computation of the global residual 
vectors in steps 4 and 11 (which require a finite amount of cpu time)
are therefore superfluous and are omitted if the "user" declares
a problem as linear by setting the flag
\code
bool Problem::Problem_is_nonlinear
\endcode
which is initialised to \c true in the constructor of the \c Problem 
base class to \c false.

<HR>
<HR>

\section unsteady_newton_solve Problem::unsteady_newton_solve(...)
The following table lists the main steps performed by \c oomph-lib's 
unsteady Newton solver and indicates at which point the various "action" 
functions are executed. The arguments passed to the function are the
value of the timestep \c dt and a boolean flag \c shift_values which
indicates whether or not history values are to be shifted.



<TABLE>
<TR>
<TD>
\f[
\left. . \hspace{6cm} \right. 
\f]
</TD>
<TD>
<B>SEQUENCE OF MATHEMATICAL STEPS</B>
</TD>
<TD>
<B>SEQUENCE OF FUNCTION CALLS / KEY CONTROL PARAMETERS</B>
</TD>
</TR>
<TR>
<TD>
<B>Step 1:</B> Perform actions before implicit timestep.
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::actions_before_implicit_timestep();
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 2:</B> Shift time values and dts, according to
control flag.
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
if(shift_values) { shift_time_values(); }
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 3:</B> 
Advance global time and set current value of dt.
</TD>
<TD>
\f[ t:=t+dt \f]
</TD>
<TD>
\code
time_pt()->time()+=dt;
time_pt()->dt()=dt;
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 4:</B> 
Loop over all timesteppers and set weights.
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code
for(...)
{
 time_stepper_pt(i)->set_weights();
}
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 5:</B>
Solve the non-linear problem for this timestep with
Newton's method.
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::newton_solve();
\endcode
</TD>
</TR>
<TR>
<TD>
<B>Step 6:</B>
Perform actions after implicit timestep.
</TD>
<TD>
\f[
\left. . \hspace{6cm} \right.
\f]
</TD>
<TD>
\code 
Problem::actions_after_implicit_timestep();
\endcode
</TD>
</TR>
</TABLE>



<HR>
**/

