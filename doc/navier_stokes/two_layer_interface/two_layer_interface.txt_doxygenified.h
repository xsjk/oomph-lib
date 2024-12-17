/**

\mainpage Demo problem: Relaxation oscillations of an interface between two viscous fluids

In the <A HREF="../../single_layer_free_surface/html/index.html">previous example</A>
we considered a two-dimensional free surface Navier--Stokes
problem. The free surface was deformed to a
prescribed shape and then allowed to relax. In this example we
consider the relaxation of an interface between two viscous fluids.

<HR>
<HR>

\section implementation Implementation

The extension from a single fluid problem to one involving two fluids
separated by an interface is very straightforward. As \c oomph-lib's
Navier--Stokes elements are based on the Arbitrary Lagrangian Eulerian
form of the equations, and can therefore be used to solve problems in
moving domains, we can discretise the domain using a rectangular mesh
containing an additional boundary corresponding to the position of the
interface. The construction of such a mesh is described <A HREF="#mesh">below</A>. 
We also require a method of distinguishing between
(and assigning) the physical properties of the two fluid layers.

\subsection interface_eqns Boundary conditions at the interface

A discussion of the theory and implementation of the boundary
conditions at a free surface is given in the <A HREF="../../single_layer_free_surface/html/index.html#free_surface_eqns">previous example</A>,
where we showed that the dynamic boundary condition is given by
\f[
\tau_{ij}^{[2]} \, n_j^{[1]} =
\tau_{ij}^{[1]} \, n_j^{[1]} + \frac{1}{Ca} \, \kappa \, n_i^{[1]}.
\f]
Here \f$ \tau_{ij}^{[1]} \f$ and \f$ \tau_{ij}^{[2]} \f$ are the
stress tensors in the `lower' and `upper' fluids respectively, \f$
n_j^{[1]} \f$ is the unit normal pointing out of the `lower' fluid and
\f$ Ca \f$ is the Capillary number.

The interface conditions in this two-fluid example are implemented in
an almost identical way to the <A HREF="../../single_layer_free_surface/html/index.html#implementation">single layer case</A>. 
We employ the same pseudo-solid node-update strategy, where the
interior mesh is treated as a fictitious elastic solid and thus the
(unknown) nodal positions can be determined by solving a solid
mechanics problem. The deformation of the free surface boundary itself
is imposed by introducing a a field of Lagrange multipliers at the
interface, and the equation associated with these additional unknowns
is the kinematic condition. In the <A HREF="../../single_layer_free_surface/html/index.html#implementation">single layer case</A> 
this equation is discretised by attaching \c FaceElements to the
boundaries of the `bulk' elements that are adjacent to the free
surface. In this two-layer example we attach the same \c FaceElements
to only those `bulk' elements which are in the `lower' fluid (and have
boundaries adjacent to the interface). These same \c FaceElements are
also responsible for adding the surface tension contributions to the
momentum equations that arise through the application of the dynamic
boundary condition. We note that since we are solving the
Navier--Stokes equations on either side of the interface we do not
specify an external pressure, a step that was necessary in the <A HREF="../../single_layer_free_surface/html/index.html#implementation">single layer case</A>. 

The other difference between this problem and the <A HREF="../../single_layer_free_surface/html/index.html">previous example</A>
is that we shall be solving this one using spatial adaptivity. We
refer to <A HREF="../../../poisson/two_d_poisson_flux_bc_adapt/html/index.html">another tutorial</A> 
for a discussion of how to apply boundary conditions in such problems.

\subsection dens_visc_ratios Distinguishing between the two fluids

In a problem containing a single fluid, the definitions of the
Reynolds number, Strouhal number, Capillary number and so on are based
on the physical properties of that fluid, as well as the geometry of
the problem and typical timescales. In our <A HREF="../../driven_cavity/html/index.html">discussion of the non-dimensionalisation of the Navier--Stokes equations</A>
we describe how the various dimensionless parameters are defined in
terms of a reference density, \f$ \rho_{ext} \f$, and a reference
viscosity, \f$ \mu_{ext} \f$. We can then define two dimensionless
ratios, \f$ R_\rho \f$ and \f$ R_\mu \f$, which describe a particular
fluid's density \f$ \rho \f$ and kinematic viscosity \f$ \mu \f$
relative to these reference quantities:
\f[
R_\rho = \frac{\rho}{\rho_{ref}} 
\qquad ; \qquad 
R_\mu = \frac{\mu}{\mu_{ref}}.
\f]
\c oomph-lib's implementation of the Navier--Stokes equations contains
these ratios in the appropriate terms. They default to one but can be
set to other values via the member functions \c density_ratio_pt() and
\c viscosity_ratio_pt() in each element.

It is convenient to choose one of the fluids in this problem to be
the `reference fluid' on which the dimensionless parameters are
based. We choose the lower fluid (fluid 1), and hence the
density \f$ \rho^{[1]} \f$ and viscosity \f$ \mu^{[1]} \f$ of this
fluid are identically equal to \f$ \rho_{ref} \f$ and \f$ \mu_{ref}
\f$ (and thus \f$ R_\rho^{[1]} = R_\mu^{[1]} = 1 \f$). We can now
control the relative density and viscosity of the upper fluid to the
lower fluid using the ratios \f$ R_\rho^{[2]} = \rho^{[2]}/\rho_{ref}
\f$ and \f$ R_\mu^{[2]} = \mu^{[2]}/\mu_{ref} \f$ respectively. For
simplicity we will from now on refer to \f$ R_\rho^{[2]} \f$ simply as
\f$ R_\rho \f$ and to \f$ R_\mu^{[2]} \f$ as \f$ R_\mu \f$.

<HR>
<HR>

\section example_problem The example problem

We will illustrate the solution of the unsteady two-dimensional
Navier--Stokes equations using the example of a distorted interface
between two viscous fluids which is allowed to relax. The domain is
periodic in the \f$ x_1 \f$ direction.

<CENTER>
<TABLE>
<TR> 
<TD>
<CENTER>
<B>
The 2D unsteady Navier--Stokes equations either side of a distorted interface.</B>
</CENTER> 
Solve
\f[
Re\left(St\frac{\partial u_i}{\partial t} +
\ u_j\frac{\partial u_i}{\partial x_j}\right) =
- \frac{\partial p}{\partial x_i} + \frac{Re}{Fr}G_i +
\frac{\partial }{\partial x_j} \left[
\frac{\partial u_i}{\partial x_j} +  
\frac{\partial u_j}{\partial x_i} \right]
\ \ \ \ \ \ \ \ \ \ (1)
\f]
and
\f[
\frac{\partial u_i}{\partial x_i} = 0
\ \ \ \ \ \ \ \ \ \ (2)
\f]
in the `lower' fluid, and
\f[
R_\rho Re\left(St\frac{\partial u_i}{\partial t} +
\ u_j\frac{\partial u_i}{\partial x_j}\right) =
- \frac{\partial p}{\partial x_i} + R_\rho \frac{Re}{Fr}G_i +
\frac{\partial }{\partial x_j} \left[R_\mu \left(
\frac{\partial u_i}{\partial x_j} +  
\frac{\partial u_j}{\partial x_i} \right) \right]
\ \ \ \ \ \ \ \ \ \ (3)
\f]
and
\f[
\frac{\partial u_i}{\partial x_i} = 0
\ \ \ \ \ \ \ \ \ \ (4)
\f]
in the `upper' fluid. Gravity acts in the negative \f$ x_2 \f$
direction and so \f$ G_1 = 0 \f$ and \f$ G_2 = 1 \f$. The governing
equations are subject to the no slip boundary conditions
\f[
u_1 = u_2 = 0 \ \ \ \ \ \ \ \ \ \ (5)
\f]
on the top (\f$ x_2 = 2.0 \f$) and bottom (\f$ x_2 = 0.0 \f$) solid
boundaries and the symmetry boundary conditions
\f[
u_1 = 0 \ \ \ \ \ \ \ \ \ \ (6)
\f]
on the left (\f$ x_1 = 0.0 \f$) and right (\f$ x_1 = 1.0 \f$)
boundaries.

We denote the position vector to the interface between the two fluids
by \f$ \mathbf{R} \f$, which is subject to the kinematic condition
\f[
\left(u_i - St\, \frac{\partial R_i}{\partial t}\right) n_i = 0,
\ \ \ \ \ \ \ \ \ \ (7)
\f]
and the dynamic condition
\f[
\tau_{ij}^{[2]} \, n_j^{[1]} =
\tau_{ij}^{[1]} \, n_j^{[1]} + \frac{1}{Ca} \, \kappa \, n_i^{[1]}.
\ \ \ \ \ \ \ \ \ \ (8)
\f]
The stress tensor in the `lower' fluid is defined as
\f[
\tau_{ij}^{[1]} = -p^{[1]} \, \delta_{ij} + \left(\frac{\partial u^{[1]}_i}{\partial x_j}
+ \frac{\partial u^{[1]}_j}{\partial x_i}\right),
\ \ \ \ \ \ \ \ \ \ (9)
\f]
and that in the `upper' fluid is defined as
\f[
\tau_{ij}^{[2]} = -p^{[2]} \, \delta_{ij} + R_\mu \left(\frac{\partial u^{[2]}_i}{\partial x_j}
+ \frac{\partial u^{[2]}_j}{\partial x_i}\right).
\ \ \ \ \ \ \ \ \ \ (10)
\f]

The initial shape of the interface is defined by
\f[
 \mathbf{R} = x_1 \, \mathbf{i} + \left[ 1.0 + 
 \epsilon\cos\left( 2 n \pi x_1 \right)\right] \, \mathbf{j},
\ \ \ \ \ \ \ \ \ \ (11)
\f]
where \f$ \epsilon \f$ is the amplitude of the initial deflection and
\f$ n \f$ is an integer.
</TD>
</TR>
</TABLE>  
</CENTER>

<HR>
<HR>

\section results Results

The figure below shows a contour plot of the pressure distribution
taken from <A HREF="../figures/two_layer_interface.avi">an animation of the flow field</A>,
for the parameters \f$ Re = Re \, St = Re/Fr = 5.0 \f$, \f$ R_\rho =
0.5 \f$, \f$ R_\mu = 0.1 \f$ and \f$ Ca = 0.01 \f$.

\image html two_layer_interface.gif "Pressure contour plot for the relaxing interface problem. " 
\image latex two_layer_interface.eps "Pressure contour plot for the relaxing interface problem. " width=0.6\textwidth

The restoring forces of surface tension
and gravitational acceleration act to revert the interface to its
undeformed flat state. The interface oscillates up and down, but the 
motion is damped as the energy in the system is dissipated through
viscous forces. Eventually the interface settles down to its
equilibrium position, as can be seen in the
following time-trace of the height of the interface at the left-hand
edge of the domain (\f$ x_1 = 0 \f$).

\image html two_layer_interface_trace.gif "Time-trace of the height of the interface at the point x_1 = 0. " 
\image latex two_layer_interface_trace.eps "Time-trace of the height of the interface at the point x_1 = 0. " width=0.95\textwidth

<HR>
<HR>

\section validation Validation

The free surface boundary conditions for the Cartesian Navier--Stokes
equations have been validated against an analytical test
case, and we present the results in the figure
below. For sufficiently small amplitudes, \f$ \epsilon \ll 1 \f$, we
can linearise the governing equations and obtain a dispersion relation
\f$ \lambda(k) \f$, the derivation of which is discussed in the <A HREF="../../single_layer_free_surface/html/index.html#validation">previous tutorial</A>. 
The only difference in this two layer case is that the linear system
which needs to be solved contains nine unknowns rather than five,
since the two fluids have different properties.
The real and imaginary parts of \f$ \lambda \f$
correspond to the growth rate and the frequency of the oscillating
interface respectively, and can be compared to numerical results
computed for given values of the wavenumber \f$ k \f$. We choose an
initial deflection amplitude of \f$ \epsilon = 0.01 \f$ and determine
the growth rate and frequency of the oscillation from a time-trace of
the left-hand edge of the interface.

\image html two_layer_interface_code_validation.gif "Validation of the code (points) by comparison with an analytical dispersion relation (lines). " 
\image latex two_layer_interface_code_validation.eps "Validation of the code (points) by comparison with an analytical dispersion relation (lines). " width=0.95\textwidth

<HR>
<HR>

\section namespace Global parameters and functions

As in the <A HREF="../../single_layer_free_surface/html/index.html#namespace">previous example</A>, we use a namespace to define the dimensionless parameters
\f$ Re \f$, \f$ St \f$, \f$ Re/Fr \f$ and \f$ Ca \f$. The pseudo-solid
mesh is governed by a generalised Hookean constitutive law which
requires the definition of the Poisson ratio, \f$ \nu \f$, and we
create a vector \f$ G \f$ which will define the direction in which
gravity acts. Because this is a two-fluid problem, we also need to
define the density ratio \f$ R_{\rho} = \rho^{[2]}/\rho^{[1]} \f$ and
viscosity ratio \f$ R_{\mu} = \mu^{[2]}/\mu^{[1]} \f$ of the two
fluids.

\dontinclude elastic_two_layer_interface.cc
\skipline start_of_namespace
\until End of namespace

<HR>
<HR>

\section main The driver code

The driver code is very similar to the <A HREF="../../single_layer_free_surface/html/index.html#main">previous example</A>. 
We define a command line flag which allows us to run a
`validation' version of the code (for \c oomph-lib's self-testing
routines) and check that the non-dimensional quantities provided in the
\c Global_Physical_Variables namespace are self-consistent.

\skipline start_of_main
\until }

Next we specify the duration of the simulation and the
size of the timestep. If we are running the code as a self-test, we
set the length of the simulation such that only two timesteps are
taken. The direction in which gravity acts is defined to be vertically
downwards.

\skipline Maximum time
\until G[1]

Finally, we build the problem using the `pseudo-solid' version of \c
RefineableQCrouzeixRaviartElements and the \c BDF<2> timestepper, before calling \c
unsteady_run(...). This function solves the system at each timestep
using the \c Problem::unsteady_newton_solve(...) function before
documenting the result.

\skipline Set up the elastic test problem
\until End of main

<HR>
<HR>

\section mesh The mesh class

In the <A HREF="../../single_layer_free_surface/html/index.html#main">previous example</A> we employed \c oomph-lib's \c
ElasticRectangularQuadMesh. If we wanted to solve the same
single-layer problem but with spatial adaptivity we would simply use
that mesh's refineable counterpart, the \c
ElasticRefineableRectangularQuadMesh. For this two-layer problem,
however, it is convenient to define a new mesh class which contains an
additional `boundary' which corresponds to the interface between the
two fluids. We start by
defining the new class (which is templated by the element type) and
inheriting from \c ElasticRefineableRectangularQuadMesh.

\dontinclude elastic_two_layer_interface.cc
\skipline start_of_specific_mesh_class
\until {

Next we define the mesh's constructor. We pass it
the usual parameters, including a boolean flag which indicates whether
or not the mesh is periodic in the \f$ x \f$ direction.

\skipline public:
\until {

We have so far created an \c ElasticRefineableRectangularQuadMesh,
which has four boundaries corresponding to the edges of our
problem's domain. We wish to define a fifth boundary which corresponds
to the interface position. First we set the number of boundaries to
five and then convert all the \c Nodes which lie on the interface into
\c BoundaryNodes. These are then added to the fifth boundary.

\skipline Set the number of boundaries
\until End of loop over horizontal elements

All that remains is to set up the boundary element information. This
is required so that we can easily have access to the bulk elements
either side of this new boundary.

\skipline Set up the boundary element information
\until End of specific mesh class

<HR>
<HR>

\section problem_class The problem class 

This is very similar to the problem class in the <A HREF="../../single_layer_free_surface/html/index.html#problem_class">previous example</A>, 
with a few modifications. Because we shall use spatial
adaptivity when solving this problem, we 
define the following two functions, which will be called before and after
mesh adaptation.

\skipline Strip off the interface elements
\until actions_after_adapt

These two functions will employ two helper functions which are
responsible for the creation and deletion of the interface elements.

\skipline Create the 1d interface elements
\until delete_interface_elements

The final modification to the problem class is the addition of a
helper function which pins a pressure value in a specified element and
assigns a specific value.

\skipline Fix pressure in element e
\until }

<HR>
<HR>

\section constructor The problem constructor

The constructor starts by building the timestepper and setting the
dimensions of the mesh. The number of elements in the
\f$ x_1 \f$ and \f$ x_2 \f$ directions in both fluid layers are
specified.

\skipline start_of_constructor
\until double h2

Next we build the bulk mesh, create an error estimator for the problem
and set the maximum refinement level.

\skipline Build and assign the "bulk" mesh
\until max_refinement_level

An empty surface mesh is created and then populated with a call to \c
create_interface_elements(). The bulk and surface meshes are then
combined to form the global mesh.

\skipline Create the "surface" mesh
\until build_global_mesh

On the top and bottom boundaries (\f$ y = 2.0 \f$ and \f$ y = 0.0 \f$)
we apply the no-slip condition by pinning both velocity components. On
the left and right symmetry boundaries (\f$ x = 0.0 \f$ and \f$ x =
1.0 \f$) we pin the \f$ x \f$
component of the velocity but leave the \f$ y \f$ component
unconstrained. We pin the vertical displacement of the nodes on the
solid boundaries (since these must remain stationary) and pin the
horizontal displacement of all nodes in the mesh.

\skipline Set the boundary conditions for this problem
\until Bulk_mesh_pt->node_pt(n)->pin_position(0)

Next we create a generalised Hookean constitutive equation for the
pseudo-solid mesh. This constitutive equation is discussed in <A HREF="../../../solid/disk_compression/html/index.html#hooke">another tutorial</A>.

\skipline Define a constitutive law
\until GeneralisedHookean

We loop over the bulk elements in the lower fluid and pass them
pointers to the Reynolds and Womersley numbers, \f$ Re \f$ and \f$
Re\, St \f$, the product of the Reynolds number and the inverse of the
Froude number, \f$ Re/Fr \f$, the direction of gravity, \f$ G \f$, the
constitutive law and the global time object, created when we called \c
Problem::add_time_stepper_pt(...) above. Note that we do not assign
pointers for the viscosity and density ratios, \f$ R_\mu \f$ and \f$
R_\rho \f$, since these take the default value in the lower fluid.

\skipline Complete the problem setup to make the elements fully functional
\until End of loop over bulk elements in lower fluid

We then do the same for the bulk elements in the upper fluid, and this
time we do assign pointers for the viscosity and density ratios.

\skipline Loop over bulk elements in upper fluid
\until End of loop over bulk elements in upper fluid

We then pin one pressure degree of freedom.

\skipline Set the pressure
\until fix_pressure

At this point we set up the boundary conditions.

\skipline Apply the boundary conditions
\until set_boundary_conditions

Finally, we set up the equation numbering scheme.

\skipline Set up equation numbering scheme
\until End of constructor

<HR>
<HR>

\section set_initial_condition Initial conditions

The \c set_initial_conditions() function is identical to that in the <A HREF="../../single_layer_free_surface/html/index.html#set_initial_condition">previous example</A>.

<HR>
<HR>

\section set_boundary_conditions Boundary conditions

The \c set_boundary_conditions() function is very similar to that in
the <A HREF="../../single_layer_free_surface/html/index.html#set_boundary_conditions">previous example</A>.

<HR>
<HR>

\section before_adapt Actions before adaptation

The mesh adaptation is driven by the error estimates in the bulk
elements and only performed for that mesh. The interface elements must 
therefore be removed before adaptation. We do this by calling 
the function \c delete_interface_elements(), and
then rebuilding the \c Problem's global mesh.

\skipline start_of_actions_before_adapt
\until End of actions_before_adapt

<HR>
<HR>

\section after_adapt Actions after adapt

After the bulk mesh has been adapted we must create new interface
elements and rebuild the problem's global mesh. Any newly-created
boundary nodes will automatically have the appropriate boundary
conditions applied; however, we must remember to pin the horizontal
displacement of all nodes throughout the bulk of the domain.

\skipline start_of_actions_after_adapt
\until Bulk_mesh_pt->node_pt(n)->pin_position(0)

To ensure that precisely one fluid pressure degree of freedom is
pinned, we unpin all pressure degrees of freedom, pin any redundant
nodal pressures which have arisen following mesh adaptation and call
\c fix_pressure(...). We also pin any redundant solid pressures before
re-setting the boundary conditions.

\skipline Unpin all fluid pressure dofs
\until End of actions_after_adapt

<HR>
<HR>

\section create_interface_elements Create interface elements

This function is used to `attach'
interface elements to the upper face of those bulk elements in the
lower fluid which are adjacent to the interface. Firstly, we loop over
all bulk elements (in either fluid) which are adjacent to the
interface.

\skipline start_of_create_interface_elements
\until Bulk_mesh_pt->boundary_element_pt

In order to determine whether the bulk element we are currently
looking at is in the upper or lower fluid, we check to see whether
that element's \c viscosity_ratio_pt is pointing to the address of \c
Global_Physical_Variables::Viscosity_Ratio. If it is, then that bulk
element is in the upper fluid, and we ignore it. Otherwise, we
determine the face of the bulk element that corresponds to the
interface and create an interface element on that face. The
newly-created element is then added to the surface mesh.

\skipline We only want to attach
\until }

Finally, we pass the Strouhal and Capillary numbers to the interface
elements.

\skipline Complete the setup to make the elements fully functional
\until End of create_interface_elements

<HR>
<HR>

\section delete_interface_elements Delete interface elements

This function loops over all the interface elements (i.e. those in the
surface mesh) and deletes them and their storage.

\skipline start_of_delete_interface_elements
\until End of delete_interface_elements

<HR>
<HR>

\section deform_free_surface Prescribing the initial free surface position

At the beginning of the simulation the interface is deformed by a
prescribed function (11), implemented in
the function \c deform_free_surface(...), which cycles through
the bulk mesh's \c Nodes and modifies their positions such that the
nodes on the free surface follow the prescribed interface shape and
the bulk nodes retain their fractional position between the solid
boundaries and the (now deformed) interface.

\skipline start_of_deform_free_surface
\until End of deform_free_surface

<HR>
<HR>

\section doc Post-processing

This function is identical to that in the <A HREF="../../single_layer_free_surface/html/index.html#doc">previous example</A>.

<HR>
<HR>

\section unsteady_run The timestepping loop

The function \c unsteady_run(...) is used to perform the timestepping
procedure, and is very similar to that in the <A HREF="../../single_layer_free_surface/html/index.html#unsteady_run">previous example</A>. The only changes arise due to this problem being solved
with spatial adaptivity. We start by deforming the interface in the
manner specified by equation (11).

\skipline start_of_unsteady_run
\until deform_free_surface

We then create a \c DocInfo object to store the output directory and
the label for the output files.

\skipline Initialise DocInfo object
\until doc_info.number()=0

Next we open and initialise the trace file.

\skipline Open trace file
\until std::endl

Before using any of \c oomph-lib's timestepping functions, the timestep
\f$ dt \f$ must be passed to the problem's timestepping routines by calling
the function \c Problem::initialise_dt(...) which sets the weights for
all timesteppers in the problem. Next we assign the initial conditions
by calling \c Problem::set_initial_condition(), which was discussed
<A HREF="#set_initial_condition">above</A>.

\skipline Initialise timestep
\until set_initial_condition

We limit number of spatial adaptations per timestep to two and refine
the problem uniformly twice, before documenting the initial conditions.

\skipline Maximum number of spatial adaptations per timestep
\until doc_info.number()++

Finally, we determine the number of timesteps to be performed and
perform the actual timestepping loop. For each timestep the function
\c unsteady_newton_solve(dt,max_adapt,first_timestep) is called and 
the solution documented. The boolean flag \c first_timestep is used to
instruct the code to re-assign the initial conditions after every mesh
adaptation. After the first timestep, the maximum number of
adaptations for all timesteps is reset to one. We refer to <A HREF="../../../unsteady_heat/two_d_unsteady_heat_adapt/html/index.html#spatial_adapt">another example</A> for a discussion of why this is recommended.

\skipline Determine number of timesteps
\until End of unsteady_run

<HR>
<HR>

\section comments Comments

- Since the problem discussed in this example has the fluid velocity
  prescribed along the entire domain boundary, the fluid pressure is
  only determined up to an arbitrary constant and hence we fix the
  pressure at a single point in the domain. However, it is worth
  noting that in the case of Crouzeix--Raviart elements being used in
  a problem with spatial adaptivity the notion of a fixed pressure is
  slightly subtle. In this example we fix the first pressure value of
  the first element in the mesh to zero, which in the case of
  Crouzeix--Raviart elements means that we fix the average value of
  the pressure to zero in this element. Were this element to be
  refined, however, we would then be imposing a zero pressure
  elsewhere in the domain, causing the entire pressure field to `jump'
  from one timestep to the next. Since the problem is incompressible
  this would not affect the computed velocity fields, but could lead
  to some confusion when studying the time evolution of the pressure
  field.

<HR>
<HR>

\section exercises Exercises

- In the <A HREF="../../single_layer_free_surface/html/index.html">single-layer example</A> we chose to discretise the problem using
  Crouzeix--Raviart elements, but we could also have chosen to use
  Taylor--Hood elements. Why can we not use Taylor--Hood elements to
  solve this two-layer problem?

- What happens if we do not call \c setup_boundary_element_info() in
  the <A HREF="#mesh">mesh constructor</A>? Why is this?

<HR>
<HR>

\section sources Source files for this tutorial
- The source files for this tutorial are located in the directory:
<CENTER>
<A HREF="../../../../demo_drivers/navier_stokes/two_layer_interface/">
demo_drivers/navier_stokes/two_layer_interface/
</A>
</CENTER>
- The driver code is:
<CENTER>
<A HREF="../../../../demo_drivers/navier_stokes/two_layer_interface/elastic_two_layer_interface.cc">
demo_drivers/navier_stokes/two_layer_interface/elastic_two_layer_interface.cc
</A>
</CENTER>
.


<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

