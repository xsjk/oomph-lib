/**

\mainpage Demo problem: The two-dimensional Poisson problem with flux boundary conditions revisited -- multiple meshes

In this document, we discuss an alternative approach for solving the
2D Poisson problem:
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
in the rectangular domain \f$D = \left\{ (x_1,x_2) \in 
[0,1] \times [0,2]\right\} \f$. The domain boundary
\f$ \partial D = \partial D_{Neumann} \cup \partial D_{Dirichlet} \f$,
where \f$ \partial D_{Neumann} 
= \left\{ (x_1,x_2) | x_1=1, \ x_2\in [0,2] \right\} \f$.
On \f$ \partial D_{Dirichlet}\f$ we apply the Dirichlet boundary conditions
\f[
\left. u\right|_{\partial D_{Dirichlet}}=u_0,
\ \ \ \ \ \ \ \ \ \ (2)
\f]
where the function \f$ u_0 \f$ is given. On  \f$ \partial D_{Neumann}\f$
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


In a <A HREF="../../two_d_poisson_flux_bc/html/index.html">
previous example,</A> we applied the Neumann boundary conditions
by adding \c PoissonFluxElements 
(elements that apply the Neumann (flux) 
boundary conditions on surfaces of higher-dimensional "bulk" Poisson 
elements) to the \c Problem's \c Mesh object.
The ability to combine elements of different types in a single
\c Mesh object is convenient, and in certain circumstances
absolutely essential, but it can cause problems;
see the discussion of the \c doc_solution(...) function in the
<A HREF="../../two_d_poisson_flux_bc/html/index.html">
previous example</A>. Furthermore, it seems strange (if not wrong!) that
the \c SimpleRectangularQuadMesh -- an object that is templated by
a particular (single!) element type -- also contains elements of a
different type.

We shall now demonstrate an alternative approach,
based on the use of multiple meshes, each containing only one type of
element. The ability to use multiple
\c Meshes in a single \c Problem is an essential feature of
\c oomph-lib and is vital in
fluid-structure interaction problems, where the fluid and solid domains 
are distinct and each domain is discretised by a different 
element type. 


 We consider the same problem as in the 
<A HREF="../../two_d_poisson_flux_bc/html/index.html">
previous example</A> and choose a source function and
boundary conditions for which the function 
\f[
u_0(x_1,x_2) = \tanh(1-\alpha(x_1 \tan\Phi - x_2)),
\ \ \ \ \ \ \ \ \  (4)
\f]
is the exact solution of the problem.

\image html soln.gif "Plot of the solution " 
\image latex soln.eps "Plot of the solution " width=0.75\textwidth

<HR>   
<HR>


\section global Global parameters and functions
The specification of the source function and the exact solution
in the namespace \c TanhSolnForPoisson is identical to that
in the single-mesh version discussed in the
<A HREF="../../two_d_poisson_flux_bc/html/index.html">
previous example</A>. 
 
<HR>
<HR>

\section main The driver code

The driver code is identical to that in the single-mesh version 
discussed in the
<A HREF="../../two_d_poisson_flux_bc/html/index.html#main">
previous example</A>. 

<HR>
<HR>

\section problem The problem class
The problem class is virtually identical to that in the 
<A HREF="../../two_d_poisson_flux_bc/html/index.html#problem">
single-mesh implementation</A>: The only difference is that 
we store pointers to the two separate \c Mesh objects as private
member data, and provide a slightly different implementation of the
function \c create_flux_elements(...).


\dontinclude two_d_poisson_flux_bc2.cc
\skipline start_of_problem_class
\until end of problem class

[See the discussion of the 
<A HREF="../../one_d_poisson/html/index.html">
1D Poisson problem</A> for a more detailed discussion of the
function type PoissonEquations<2>::PoissonSourceFctPt.]
 
<HR>
<HR>

\section constructor The Problem constructor
As before we start by creating the "bulk" mesh and store
a pointer to this mesh in the private data member \c TwoMeshFluxPoissonProblem::Bulk_mesh_pt:

\dontinclude two_d_poisson_flux_bc2.cc
\skipline start_of_constructor
\until SimpleRectangularQuadMesh

Next, we construct an (empty) \c Mesh and store a pointer
to it in the private data member \c
TwoMeshFluxPoissonProblem::Surface_mesh_pt.

\skipline Create
\until Surface_mesh

We use the function \c create_flux_elements(...), 
to create the prescribed-flux elements for the
elements on boundary 1 of the bulk mesh and
add them to the surface mesh.

\skipline Create
\until create_flux

We have now created all the required elements and can access them
directly via the two data members \c TwoMeshFluxPoissonProblem::Bulk_mesh_pt
and \c TwoMeshFluxPoissonProblem::Surface_mesh_pt. However, many 
of \c oomph-lib's generic procedures require ordered access to \e 
all of the \c Problem's elements, nodes, etc. For instance,
\c Problem::newton_solve(...) computes the entries in the global Jacobian
matrix by adding the contributions from all elements in
all (sub-)meshes. Ordered access to the \c Problem's elements, nodes, etc
is generally obtained via the \c Problem's (single!) global \c Mesh object,
which is accessible via \c Problem::mesh_pt(). The \c Problem base 
class also provides a private data member \c Problem::Sub_mesh_pt 
(a vector of type \c Vector<Mesh*>) which stores the (pointers to the) 
\c Problem's  sub-meshes. We must add the pointers to our two sub-meshes to
the problem,

\skipline Add
\until Surface_mesh

and use the function \c Problem::build_global_mesh() to 
combine the \c Problem's sub-meshes into a single, global \c Mesh
that is accessible via \c Problem::mesh_pt():

\skipline Combine
\until build_

The rest of the constructor is identical to that in the 
<A HREF="../../two_d_poisson_flux_bc/html/index.html">
single-mesh implementation</A>. We pin the nodal values on the
Dirichlet boundaries, pass the function pointers to the elements,
and set up the equation numbering scheme:

\skipline Set the boundary
\until end of constructor

<HR>
<HR>

\section actions_before "Actions before solve" 
The only (minor) change to \c Problem::actions_before_newton_solve() 
is that the nodes on the boundaries of the bulk (!) mesh are 
now obtained via the \c Bulk_mesh_pt pointer, rather than from the
combined \c Mesh, pointed to by \c Problem::mesh_pt(). 
While this may appear to be a trivial change, it is a potentially 
important one. Recall that the surface mesh is an instantiation of
the \c Mesh base class. We created the (empty) mesh in the \c 
Problem constructor (by calling the default \c Mesh constructor), and
used the function \c create_flux_elements(...) to add 
the (pointers to the) prescribed-flux
elements to it. The surface mesh therefore does not have any nodes
of its own, and its lookup schemes for the boundary nodes have 
not been set up. The combined mesh,
pointed to by \c Problem::mesh_pt(), therefore only contains the 
boundary lookup scheme for the bulk mesh. Hence, the combined mesh
has four boundaries and their numbers correspond to those
in the bulk mesh. 

If we had set up the boundary lookup scheme in the 
surface mesh, the constructor of the combined \c Mesh, 
would have concatenated the boundary lookup schemes of the
two sub-meshes so that the four boundaries in sub-mesh 0
would have become boundaries 0 to 3 in the combined mesh, while
the two boundaries in the surface mesh would have become boundaries
4 and 5 in the combined \c Mesh. While the conversion is
straightforward, it is obvious that \c Mesh boundaries are 
best identified via the sub-meshes.

\skipline start_of_actions_before_newton_solve
\until end of actions before solve

<HR>
<HR>


\section doc Post-processing
The post-processing, implemented in \c doc_solution(...) 
is now completely straightforward. Since the 
\c PoissonFluxElements only apply boundary conditions,
they do not have to be included in the plotting or
error checking routines, so we perform these only
for the elements in the bulk mesh.

\skipline start_of_doc
\until end of doc


<HR>
<HR>

\section comments  Further comments
We mentioned that the \c Mesh constructor that builds a combined
\c Mesh from a vector of sub-meshes, concatenates the sub-meshes'
element, node and boundary lookup schemes. There are a few
additional features that the "user" should be aware of:
- The sub-meshes should not contain any duplicate nodes or elements.
  If they do, the function \c Problem::build_global_mesh() 
  will issue a warning and
  ignore any duplicates. This is because the \c Problem's global \c Mesh object
  is used by many functions in which operations must be performed exactly
  once for each node or element. For instance, in time-dependent
  problems, the function \c Problem::shift_time_values(),
  which is called automatically by \c Problem::unsteady_newton_solve(...),
  advances all "history values" by one time-level to prepare for the
  next timestep. If this was done repeatedly for nodes that are common
  to multiple sub-meshes, the results would be incorrect. 
  If your problem requires a combined mesh in which duplicates
  are allowed, you must construct this mesh yourself. 
- Recall that the function \c Mesh::add_boundary_node()
  "tells" the mesh's constituent nodes which boundaries they are
  located on. What happens if a (sub-)mesh for which this lookup scheme has
  been set up becomes part of a global \c Mesh? For various (good!) reasons,
  the \c Mesh constructor \b does \b not \b update this information. 
  The boundary number stored by the nodes therefore always refers to 
  the boundary in the \c Mesh that created them. If this is not
  appropriate for your problem, you must construct the
  combined mesh yourself.



<HR>
<HR>


\section sources Source files for this tutorial
- The source files for this tutorial are located in the directory:
<CENTER>
<A HREF="../../../../demo_drivers/poisson/two_d_poisson_flux_bc2/">
demo_drivers/poisson/two_d_poisson_flux_bc2/
</A>
</CENTER>
- The driver code is:
<CENTER>
<A HREF="../../../../demo_drivers/poisson/two_d_poisson_flux_bc2/two_d_poisson_flux_bc2.cc">
demo_drivers/poisson/two_d_poisson_flux_bc2/two_d_poisson_flux_bc2.cc
</A>
</CENTER>
.

<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

