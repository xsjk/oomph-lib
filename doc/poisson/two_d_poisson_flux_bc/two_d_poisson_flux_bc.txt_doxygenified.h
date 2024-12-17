/**

\mainpage Demo problem: A two-dimensional Poisson problem with flux boundary conditions

In this document, we demonstrate how to solve a 2D Poisson problem
with Neumann boundary conditions, using existing objects 
from the \c oomph-lib library: 
<CENTER>
<TABLE>
<TR>
<TD>
<CENTER>
<B>Two-dimensional model Poisson problem with Neumann boundary conditions</B>
</CENTER> 
Solve
\f[
\sum_{i=1}^2 \frac{\partial^2u}{\partial x_i^2} = f(x_1,x_2),
 \ \ \ \ \ \ \ \ \ \ (1)
\f]
in the rectangular domain \f$ D = \left\{ (x_1,x_2) \in 
[0,1] \times [0,2]\right\} \f$. The domain boundary 
 \f$ \partial D = \partial D_{Neumann} \cup \partial D_{Dirichlet} \f$,
where \f$ \partial D_{Neumann} 
= \left\{ (x_1,x_2) | x_1=1, \ x_2\in [0,2] \right\} \f$.
On \f$ \partial D_{Dirichlet} \f$ we apply the Dirichlet boundary conditions
\f[
\left. u\right|_{\partial D_{Dirichlet}}=u_0,
\ \ \ \ \ \ \ \ \ \ (2)
\f]
where the function \f$ u_0 \f$ is given. On  \f$ \partial D_{Neumann} \f$
we apply the Neumann conditions 
\f[
\left. \frac{\partial u}{\partial n}\right|_{\partial D_{Neumann}} = 
\left. \frac{\partial u}{\partial x_1}\right|_{\partial D_{Neumann}}
=g_0,
\ \ \ \ \ \ \ \ \ \ (3)
\f]
where the function \f$ g_0 \f$ is given.
</TD>
</TR>
</TABLE>  
</CENTER>  


We provide
a detailed discussion of the driver code <A HREF="../../../../demo_drivers/poisson/two_d_poisson_flux_bc/two_d_poisson_flux_bc.cc">
two_d_poisson_flux_bc.cc</A> which solves the problem for
\f[
u_0(x_1,x_2) = \tanh(1-\alpha(x_1 \tan\Phi - x_2)),
\ \ \ \ \ \ \ \ \  (4)
\f]
\f[
f(x_1,x_2) = \sum_{i=1}^2 \frac{\partial^2 u_0}{\partial x_i^2},
\ \ \ \ \ \ \ \ \  (5)
\f]
and
\f[
g_0 = \left. \frac{\partial u_0}{\partial n} \right|_{x_1=1}
    = \left. \frac{\partial u_0}{\partial x_1} \right|_{x_1=1},
\ \ \ \ \ \ \ \ \  (6)
\f]
so that \f$ u_0(x_1,x_2) \f$ is the exact solution of the
problem. For large values of \f$ \alpha \f$ the solution approaches
a step function 
\f[
u_{step}(x_1,x_2) = \left\{
\begin{array}{rl}
-1 & \mbox {for $x_2 < x_1 \ \tan\Phi$} \\
1 & \mbox {for $x_2 > x_1 \ \tan\Phi$} 
\end{array}
 \right.
\f]
and presents a serious challenge for any numerical method.
The figure below compares the numerical and exact solutions for
\f$ \alpha = 1 \f$ and \f$ \Phi = 45^o \f$.

\image html soln.gif "Plot of the solution " 
\image latex soln.eps "Plot of the solution " width=0.75\textwidth

Most of the driver code is identical to the code that solves
the <A HREF="../../../poisson/two_d_poisson/html/index.html">
equivalent problem without Neumann boundary conditions</A>.
Therefore we only provide a detailed discussion of those
functions that needed to be changed to accommodate the
Neumann boundary conditions.



<HR>   
<HR>


\section global Global parameters and functions
As in the Dirichlet problem, we define the source function (5)
and the exact solution (4), together with the
problem parameters \f$ \tan \Phi \f$ and  \f$ \alpha \f$, 
in a namespace \c TanhSolnForPoisson. We add the function
 \c TanhSolnForPoisson::prescribed_flux_on_fixed_x_boundary(...)
which computes the
prescribed flux \f$ g_0 \f$ required in the Neumann boundary
condition (3). The function evaluates 
\f$ \partial u_0 /\partial n = {\bf N} \cdot \nabla u_0 \f$ 
for the normal direction specified by the vector \f$ {\bf N} =
(1,0)^T. \f$

\dontinclude two_d_poisson_flux_bc.cc
\skipline start_of_namespace
\until end of namespace


<HR>
<HR>

\section main The driver code

The driver code is very similar to that for the 
<A HREF="../../../poisson/two_d_poisson/html/index.html">pure
Dirichlet problem:</A> We set up the problem, check its integrity
and define the problem parameters.
Following this, we solve the problem for a number of \f$ \alpha \f$
values and document the solution. 

\dontinclude two_d_poisson_flux_bc.cc
\skipline start_of_main
\until end of main


<HR>
<HR>

\section problem The problem class
The problem class is virtually identical to that used
for the <A HREF="../../../poisson/two_d_poisson/html/index.html">pure
Dirichlet problem</A>: The only difference is that the 
class now contains an additional private data member,
\c  FluxPoissonProblem::Npoisson_elements, which stores the 
number of 2D "bulk" elements in the mesh, and an additional private
member function \c FluxPoissonProblem::create_flux_elements(...). pwd


\dontinclude two_d_poisson_flux_bc.cc
\skipline start_of_problem_class
\until end of problem class

[See the discussion of the 
<A HREF="../../../poisson/one_d_poisson/html/index.html">
1D Poisson problem</A> for a more detailed discussion of the
function type PoissonEquations<2>::PoissonSourceFctPt.]

<HR>
<HR>

\section constructor The Problem constructor
The first part of the \c Problem constructor is identical to that
used for the  <A HREF="../../../poisson/two_d_poisson/html/index.html">pure
Dirichlet problem</A>: We create a 2D Mesh consisting of 4x4 quadrilateral 
Poisson elements:

\dontinclude two_d_poisson_flux_bc.cc
\skipline start_of_constructor
\until SimpleRectangularQuadMesh

Before continuing, we store the number of 2D "bulk" Poisson elements in 
the variable \c FluxPoissonProblem::Npoisson_element:

\skipline Store
\until Npoisson

Now, we need to apply the prescribed-flux boundary condition
along the Neumann boundary \f$ \partial D_{Neumann} \f$. The documentation for
the \c SimpleRectangularQuadMesh shows that this boundary is mesh
boundary 1. The necessary steps are performed by the function
\c create_flux_elements(..), described in the section \ref create_flux
below.

\skipline Create prescribed
\until create_flux

The rest of the constructor is very similar to its counterpart
in the <A HREF="../../../poisson/two_d_poisson/html/index.html">pure
Dirichlet problem</A>. First we apply Dirichlet conditions on the 
remaining boundaries by pinning the nodal values. Next, we finish
the problem setup by looping over all "bulk" Poisson
elements and set the pointer to the source function.
Since we have added the \c PoissonFluxElements to the \c Mesh,
only the first \c Npoisson_element elements are "bulk" elements
and the loop is restricted to these. We then perform a second loop
over the \c PoissonFluxElements which need to be passed 
the pointer to the prescribed-flux function 
\c TanhSolnForPoisson::prescribed_flux_on_fixed_x_boundary(...). Finally,
we generate the equation numbering scheme.

\skipline Set the boundary
\until end of constructor

<HR>
<HR>
 
\section create_flux Creating the flux elements

\c oomph-lib provides
an element type \c  PoissonFluxElement, which allows the
application of Neumann (flux) boundary conditions along the "faces"
of higher-dimensional "bulk" Poisson elements.  \c  PoissonFluxElements are 
templated by the type of the corresponding higher-dimensional
"bulk" element, so that a 
\c PoissonFluxElement<QPoissonElement<2,3> >
is a one-dimensional three-node element that applies
Neumann boundary conditions along the one-dimensional edge
of a nine-node quadrilateral Poisson element. Similarly,
a \c PoissonFluxElement<QPoissonElement<3,2> >
is a two-dimensional quadrilateral four-node element that applies
Neumann boundary conditions along the two-dimensional face
of a eight-node brick-shaped Poisson element; etc.

  The constructor of the \c PoissonFluxElement takes two arguments:
- a pointer to the corresponding bulk element
- the index \c face_index of the face that is to be constructed. The
convention for two-dimensional Q-type elements is that the face_index
is \f$ -(i+1) \f$ when the coordinate \f$ s_i \f$ is fixed 
at its minimum value over the face and \f$ +(i+1) \f$ when
\f$ s_i \f$ is fixed at its maximum value over the face
.
The layout of the elements in the  \c SimpleRectangularQuadMesh
is sufficiently simple to allow the direct determination of the face
index: Elements 3, 7, 11 and 15 are located next to mesh
boundary 1 and along this boundary the element's local coordinate 
\f$ s_0 \f$ has a constant (maximum) value of +1.0. Hence we need to set 
\c face_index=1 

In more complicated meshes, the determination of the face index 
can be more difficult (or at least very tedious), especially
if a \c Mesh has been refined non-uniformly. The generic \c Mesh
class therefore provides helper functions to determine the
required face index for all elements adjacent to a specified
\c Mesh boundary. This allows the creation of the flux elements
by the following, completely generic procedure: We use
the function \c Mesh::boundary_element_pt(...) to determine 
the "bulk" elements that are adjacent to the Neumann boundary,
and obtain \c face_index from the function 
\c Mesh::face_index_at_boundary(...). We pass the parameters to the
constructor of the  \c PoissonFluxElement and add the 
(pointer to) the newly created element to the \c Problem's mesh.

\skipline  start_of_create_flux
\until end of create_flux

<HR>
<HR>
 
\section actions_before "Actions before solve" 
The function \c Problem::actions_before_newton_solve()
is identical to that in the  
<A HREF="../../../poisson/two_d_poisson/html/index.html">pure Dirichlet
problem</A> and is only listed here for the sake of completeness:

\skipline start_of_actions_before_newton_solve
\until end of actions before solve

<HR>
<HR>


\section doc Post-processing
The post-processing, implemented in \c doc_solution(...) 
is similar to that in 
<A HREF="../../../poisson/two_d_poisson/html/index.html">pure Dirichlet
problem</A>. However, since the \c PoissonFluxElements
are auxiliary elements which are only used to apply
Neumann boundary conditions on adjacent "bulk" elements,
their error checking function is not implemented. 
We cannot use the generic \c Mesh member function
\c Mesh::compute_error() to compute an overall error since this
function would try to execute the "broken virtual" function
\c GeneralisedElement::compute_error(...); see the
section \ref exercises for a more detailed discussion 
of "broken virtual" functions. Error checking would
therefore have to be implemented "by hand" (excluding the
\c PoissonFluxElements), or a suitable error measure would have 
to be defined in the  \c PoissonFluxElements.

 We do not pursue either approach here because the difficulty 
is a direct consequence of our (questionable) decision to include 
elements of different types in the same \c Mesh object. While this is 
perfectly "legal" and often convenient, the practice introduces
additional difficulties in refineable problems and we shall demonstrate 
<A HREF="../../../poisson/two_d_poisson_flux_bc2/html/index.html">
 an alternative approach in another example</A>.

\skipline start_of_doc
\until end of doc


<HR>
<HR>


\section exercises Exercises and Comments
-# What happens if you do not create the \c PoissonFluxElements
   but leave the nodes on the Neumann boundary un-pinned? 
   Compare the computational result to those obtained when you set
   the prescribed flux to zero, \f$ g_0 = 0 \f$. Does this make sense? 
   [Hint: Remember the "natural" boundary conditions for
   Poisson's equation].
-# Try to compute the error of the computed solution by
   re-instating the global error checking procedure
   \code
   // Doc error and return of the square of the L2 error
   //---------------------------------------------------
   double error,norm;
   sprintf(filename,"%s/error%i.dat",doc_info.directory().c_str(),
           doc_info.number());
   some_file.open(filename);
   mesh_pt()->compute_error(some_file,TanhSolnForPoisson::get_exact_u,
                            error,norm); 
   some_file.close();
   \endcode
   in \c doc_solution(...).
   What happens when you run the code? The code's behaviour
   illustrates a general convention in \c oomph-lib: 
   <TABLE>
   <TR>
   <TD bgcolor="cornsilk">
   <CENTER><B>\anchor broken_virtual A general convention</B></CENTER> 
   Some \c oomph-lib functions, such as 
   \c GeneralisedElement::compute_error(...),
   are defined as virtual functions in a base class to allow
   the implementation of generic procedures such as
   \c Mesh::compute_error(...), which loops over all
   of the \c Mesh's constituent elements and executes their
   specific \c compute_error(...) member functions. In some rare cases
   (such as the one encountered here), the implementation of a 
   particular virtual
   function might not be sensible for a specific element. Therefore,
   rather than forcing the "element-writer" to implement a dummy version
   of this function in his/her derived class (by declaring it as a pure
   virtual function
   in the base class), we provide a <B> "broken virtual" </b>
   implementation in \c GeneralisedElement. If the function is
   (re-)implemented in a derived element, the broken version 
   is ignored; if the function is not overloaded, the broken
   virtual function throws an error, allowing a traceback in a
   debugger to find out where the broken function was called from. 
   We note that this practice is not universally approved of in the C++
   community but we believe it to have its place in situations such as 
   the one described here.       
   \n \n
   Incidentally, the code discussed above contains another
   (possibly more convincing) example of why 
   "broken virtual" functions can be useful. Recall that the creation of 
   the \c PoissonFluxElements on the Neumann boundary was
   greatly facilitated by the availability of the helper functions
   \c Mesh::boundary_element_pt(...) and  
   \c Mesh::face_index_at_boundary(...). 
   These functions are implemented
   in the generic \c Mesh base class and determine the relevant parameters
   via lookup schemes that are stored in that class. Obviously,
   the lookup schemes need to be set up when a specific \c Mesh
   is built and this task can involve a considerable amount of
   work (see also \ref footnote). Since the lookup schemes are useful 
   but by no means essential, the three helper functions are again
   implemented as broken virtual functions. If the functions 
   are called before the required lookup schemes have been set up, 
   code execution stops with a suitable warning message. 
   </TD> 
   </TR>
   </TABLE> 
-# Implement the error computation by hand to familiarise yourself
   with the way in which the \c Mesh::compute_error(...) function works.



<HR>
\subsection footnote Setting up the boundary lookup schemes
\c oomph-lib provides a range of helper functions that
set up the boundary lookup schemes for specific \c Mesh classes. 
For instance, the \c QuadMeshBase class forms a base
class for all \c Meshes that consist of two-dimensional 
quadrilateral elements and has a member function
\c QuadMeshBase::setup_boundary_element_info()
which can be called from the constructor of any derived 
\c Mesh class to set up the lookup schemes required
by \c Mesh::boundary_element_pt(...) and
\c Mesh::face_index_at_boundary(...).



<HR>
<HR>


\section sources Source files for this tutorial
- The source files for this tutorial are located in the directory:
<CENTER>
<A HREF="../../../../demo_drivers/poisson/two_d_poisson_flux_bc/">
demo_drivers/poisson/two_d_poisson_flux_bc/
</A>
</CENTER>
- The driver code is:
<CENTER>
<A HREF="../../../../demo_drivers/poisson/two_d_poisson_flux_bc/two_d_poisson_flux_bc.cc">
demo_drivers/poisson/two_d_poisson_flux_bc/two_d_poisson_flux_bc.cc
</A>
</CENTER>
.


<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

