/**

\mainpage Demo problem: Relaxation oscillations of an interface between two viscous fluids in an axisymmetric domain

In an <A HREF="../../../navier_stokes/two_layer_interface/html/index.html">earlier example</A>
we considered a free surface Navier--Stokes problem in which the
interface between two viscous fluids was deformed to a prescribed
shape and then allowed to relax. This example considers a similar
problem in which the two fluids occupy a cylindrical domain which is
orientated such that its axis of symmetry is normal to the equilibrium
position of the interface. The problem is assumed to be axisymmetric
and so the computational domain is rectangular as before, with the
difference here that the governing equations are the axisymmetric
Navier--Stokes equations as opposed to the two-dimensional Cartesian
Navier--Stokes equations employed by the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html">other example</A>. 
These equations are discussed in detail in
<A HREF="../../../axisym_navier_stokes/spin_up/html/index.html">another tutorial</A>.
Otherwise, the same overall strategy is employed. Since the implementation
of the Navier--Stokes elements in \c oomph-lib is based on the
Arbitrary Lagrangian Eulerian form of the equations, we can
discretise the computational domain using a boundary-fitted mesh which
deforms in 
response to the change in position of the interface. The positions of the
nodes in the `bulk' of the mesh are determined by treating the interior
of the mesh as a fictitious elastic solid and solving a solid mechanics
problem, a technique which we refer to as a `pseudo-solid node update
strategy'. The deformation of the free surface boundary itself is imposed by
solving the kinematic boundary condition for a field of Lagrange
multipliers at the interface, and this condition is discretised by
attaching \c FaceElements to those `bulk' elements in the `lower' of
the two fluids which have boundaries adjacent to the interface.

\section interface_def Choosing an appropriate interface deformation

The <A HREF="../../../navier_stokes/two_layer_interface/html/index.html">two-dimensional problem</A>
was started impulsively from a set of initial conditions such that the
fluid was at rest and the interface was prescribed to be a cosine
curve. Since this was an eigenmode of the system, the shape of the
interface remained `in mode' throughout the simulation. Choosing a
single eigenmode as an initial condition 
allowed the results of the simulation to be compared with an
analytical solution, and as we wish to do the same
thing here we choose a zeroth-order Bessel function of the
first kind, \f$ J_0(kr) \f$, where \f$ k \f$ is a wavenumber and \f$ r
\f$ is the radial spatial coordinate. This function has the property
that its derivative with respect to \f$ r \f$ evaluated at \f$ r=0 \f$
is zero, which must be the case if the interface is to be smooth at
the symmetry boundary (the central axis of the cylinder).

A further consideration is that we want the equilibrium state of the
system to be such that the interface is a flat line at \f$ z=0 \f$,
where \f$ z \f$ is the axial coordinate. This state can only be
reached if we choose an initial deformation \f$ z=h(r) \f$ which is volume
conserving, and we therefore require
\f[
\int_{r=0}^{r=a} h(r) r dr = 0
\f]
to be satisfied, where \f$ a \f$ is the radius of the cylindrical
container (or the width of the computational domain). Since in our
case \f$ h(r) = J_0(kr) \f$, and we will choose a computational domain
of width \f$ a=1 \f$, our initial condition must therefore satisfy
\f[
\int_0^1 r \, J_0(kr) \, dr = 0.
\f]
Using the properties of Bessel functions we find that this condition
is met if \f$ J_1(k) = 0 \f$, and hence the values of \f$ k \f$ are
constrained to be zeroes of \f$ J_1(k) \f$.

Furthermore, the properties of Bessel functions are such that the
derivative of \f$ J_0(kr) \f$ with respect to \f$ r \f$, evaluated at
any point along the \f$ r \f$-axis which corresponds to a zero of \f$
J_1(kr) \f$, is itself zero. The velocity boundary condition in the
axial direction is therefore the traction-free condition \f$ \partial
u_z / \partial r = 0 \f$. This could be physically realised by having
a `slippery' outer wall where the contact line can move but the
contact angle is fixed at \f$ 90^o \f$, and is in fact an identical
condition to the symmetry boundary condition prescribed at the axis of
symmetry (\f$ r=0 \f$).

<HR>
<HR>

\section example_problem The example problem

We will illustrate the solution of the unsteady axisymmetric
Navier--Stokes equations using the example of a distorted interface
between two viscous fluids which is allowed to relax. The domain is 
symmetric about the line \f$ r=0 \f$, which corresponds to the axis of
the cylindrical container.

<CENTER>
<TABLE>
<TR> 
<TD>
<CENTER>
<B>
The unsteady axisymmetric Navier--Stokes equations either side of a distorted interface.</B>
</CENTER> 
Solve
\f[
Re \left[
 St \frac{\partial u_r}{\partial t}
 + u_r \frac{\partial u_r}{\partial r}
 - \frac{u_\theta^2}{r}
 + u_z \frac{\partial u_r}{\partial z}
\right] = -\frac{\partial p}{\partial r}
+ \frac{Re}{Fr} G_r
+ \left[
  \frac{\partial^2 u_r}{\partial r^2}
  + \frac{1}{r}\frac{\partial u_r}{\partial r}
  - \frac{u_r}{r^2}
  + \frac{\partial^2 u_r}{\partial z^2}
\right],
\f]
\f[
Re \left[
  St \frac{\partial u_\theta}{\partial t}
  + u_r \frac{\partial u_\theta}{\partial r}
  + \frac{u_r u_\theta}{r}
  + u_z \frac{\partial u_\theta}{\partial z}
\right]
= \frac{Re}{Fr} G_\theta
+ \left[
  \frac{\partial^2 u_\theta}{\partial r^2}
  + \frac{1}{r}\frac{\partial u_\theta}{\partial r}
  - \frac{u_\theta}{r^2}
  + \frac{\partial^2 u_\theta}{\partial z^2}
\right],
\f]
\f[
Re \left[
  St \frac{\partial u_z}{\partial t}
  + u_r \frac{\partial u_z}{\partial r}
  + u_z \frac{\partial u_z}{\partial z}
\right] = -\frac{\partial p}{\partial z}
+ \frac{Re}{Fr} G_z
+ \left[
  \frac{\partial^2 u_z}{\partial r^2}
  + \frac{1}{r}\frac{\partial u_z}{\partial r}
  + \frac{\partial^2 u_z}{\partial z^2}
\right],
\f]
and
\f[
\frac{\partial u_r}{\partial r}
+ \frac{u_r}{r}
+ \frac{\partial u_z}{\partial z} = 0
\ \ \ \ \ \ \ \ \ \ (1)
\f]
in the `lower' fluid, and
\f[
R_\rho Re \left[
 St \frac{\partial u_r}{\partial t}
 + u_r \frac{\partial u_r}{\partial r}
 - \frac{u_\theta^2}{r}
 + u_z \frac{\partial u_r}{\partial z}
\right] = -\frac{\partial p}{\partial r}
+ R_\rho \frac{Re}{Fr} G_r
+ R_\mu \left[
  \frac{\partial^2 u_r}{\partial r^2}
  + \frac{1}{r}\frac{\partial u_r}{\partial r}
  - \frac{u_r}{r^2}
  + \frac{\partial^2 u_r}{\partial z^2}
\right],
\f]
\f[
R_\rho Re \left[
  St \frac{\partial u_\theta}{\partial t}
  + u_r \frac{\partial u_\theta}{\partial r}
  + \frac{u_r u_\theta}{r}
  + u_z \frac{\partial u_\theta}{\partial z}
\right]
= R_\rho \frac{Re}{Fr} G_\theta
+ R_\mu \left[
  \frac{\partial^2 u_\theta}{\partial r^2}
  + \frac{1}{r}\frac{\partial u_\theta}{\partial r}
  - \frac{u_\theta}{r^2}
  + \frac{\partial^2 u_\theta}{\partial z^2}
\right],
\f]
\f[
R_\rho Re \left[
  St \frac{\partial u_z}{\partial t}
  + u_r \frac{\partial u_z}{\partial r}
  + u_z \frac{\partial u_z}{\partial z}
\right] = -\frac{\partial p}{\partial z}
+ R_\rho \frac{Re}{Fr} G_z
+ R_\mu \left[
  \frac{\partial^2 u_z}{\partial r^2}
  + \frac{1}{r}\frac{\partial u_z}{\partial r}
  + \frac{\partial^2 u_z}{\partial z^2}
\right],
\f]
and
\f[
\frac{\partial u_r}{\partial r}
+ \frac{u_r}{r}
+ \frac{\partial u_z}{\partial z} = 0
\ \ \ \ \ \ \ \ \ \ (2)
\f]
in the `upper' fluid. Gravity acts in the negative \f$ z \f$
direction and so \f$ G_r = G_\theta = 0 \f$ and \f$ G_z = 1 \f$. The
governing equations are subject to the no slip boundary conditions
\f[
u_r = u_\theta = u_z = 0 \ \ \ \ \ \ \ \ \ \ (3)
\f]
on the top (\f$ z = 2.0 \f$) and bottom (\f$ z = 0.0 \f$) solid
boundaries and the symmetry boundary conditions
\f[
u_r = u_\theta = 0 \ \ \ \ \ \ \ \ \ \ (4)
\f]
on the left (\f$ r = 0.0 \f$) and right (\f$ r = 1.0 \f$)
boundaries.

We denote the position vector to the interface between the two fluids
by \f$ \mathbf{R} \f$, which is subject to the kinematic condition
\f[
\left(u_i - St\, \frac{\partial R_i}{\partial t}\right) n_i = 0,
\ \ \ \ \ \ \ \ \ \ (5)
\f]
and the dynamic condition
\f[
\tau_{ij}^{[2]}n_j = \tau_{ij}^{[1]}n_j+\frac{1}{Ca}\kappa n_i.
\ \ \ \ \ \ \ \ \ \ (6)
\f]
</TD>
</TR>
</TABLE>  
</CENTER>

<CENTER>
<TABLE>
<TR> 
<TD>
The non-dimensional, symmetric stress tensor in the `lower' fluid is
defined as
\f[
\tau_{rr} = -p
+ 2\frac{\partial u_r}{\partial r},
\qquad \tau_{\theta\theta} = -p
+ 2\frac{u_r}{r},
\ \ \ \ \ \ \ \ \ \ 
\f]
\f[
\tau_{zz} = -p
+ 2\frac{\partial u_z}{\partial z},
\qquad \tau_{rz} = 
\frac{\partial u_r}{\partial z} +
  \frac{\partial u_z}{\partial r},
\ \ \ \ \ \ \ \ \ \ (7)
\f]
\f[
\tau_{\theta r} = r \frac{\partial}{\partial
    r}\left(\frac{u_{\theta}}{r}\right),
  \qquad \tau_{\theta z} =
  \frac{\partial u_{\theta}}{\partial z},
\ \ \ \ \ \ \ \ \ \ 
\f]
and that in the `upper' fluid is defined as
\f[
\tau_{rr} = -p
+ 2R_\mu \frac{\partial u_r}{\partial r},
\qquad \tau_{\theta\theta} = -p
+ 2R_\mu \frac{u_r}{r},
\ \ \ \ \ \ \ \ \ \ 
\f]
\f[
\tau_{zz} = -p
+ 2R_\mu \frac{\partial u_z}{\partial z},
\qquad \tau_{rz} = R_\mu
\left(\frac{\partial u_r}{\partial z} +
  \frac{\partial u_z}{\partial r}\right),
\ \ \ \ \ \ \ \ \ \ (8)
\f]
\f[
\tau_{\theta r} = R_\mu r \frac{\partial}{\partial
    r}\left(\frac{u_{\theta}}{r}\right),
  \qquad \tau_{\theta z} =
  R_\mu \frac{\partial u_{\theta}}{\partial z}.
\ \ \ \ \ \ \ \ \ \ 
\f]

The initial shape of the interface is defined by
\f[
\mathbf{R} = r \, \mathbf{e}_r + \left[ 1.0 + \epsilon J_0(kr) \right]
\, \mathbf{e}_z,
\ \ \ \ \ \ \ \ \ \ (9)
\f]
where \f$ \epsilon \f$ is the amplitude of the initial deflection, \f$
k \f$ is a wavenumber and \f$ J_0(kr) \f$ is a zeroth-order Bessel
function of the first kind.
</TD>
</TR>
</TABLE>  
</CENTER>

<HR>
<HR>

\section results Results

The figure below shows a contour plot of the pressure distribution
taken from <A HREF="../figures/two_layer_interface_axisym.avi">an animation of the flow field</A>, 
for the parameters \f$ Re = Re \, St = Re/Fr = 5.0 \f$, \f$ R_\rho =
0.5 \f$, \f$ R_\mu = 0.1 \f$ and \f$ Ca = 0.01 \f$.

\image html two_layer_interface_axisym.gif "Pressure contour plot for the axisymmetric relaxing interface problem. " 
\image latex two_layer_interface_axisym.eps "Pressure contour plot for the axisymmetric relaxing interface problem. " width=0.6\textwidth

The restoring forces of surface tension
and gravitational acceleration act to revert the interface to its
undeformed flat state. The interface oscillates up and down, but the 
motion is damped as the energy in the system is dissipated through
viscous forces. Eventually the interface settles down to its
equilibrium position, as can be seen in the
following time-trace of the height of the interface at the left-hand
edge of the domain (\f$ r=0 \f$).

\image html two_layer_interface_axisym_trace.gif "Time-trace of the height of the interface at the symmetry axis (r=0). " 
\image latex two_layer_interface_axisym_trace.eps "Time-trace of the height of the interface at the symmetry axis (r=0). " width=0.95\textwidth

<HR>
<HR>

\section validation Validation

The free surface boundary conditions for the axisymmetric
Navier--Stokes equations have been validated against an analytical
test case, and we present the results in the figure
below. For sufficiently small amplitudes, \f$ \epsilon \ll 1 \f$, we
can linearise the governing equations and obtain a dispersion relation
\f$ \lambda(k) \f$. In the <A HREF="../../../navier_stokes/single_layer_free_surface/html/index.html#validation">two-dimensional single-layer example</A>
we discussed the derivation of such a dispersion relation, and the
technique used for this problem is very similar. The major difference
is that the proposed separable solution for the linearised equations
is of the form
\f[
\hat{h}(r,t) = H e^{\lambda t} J_0(kr),
\f]
\f[
\hat{u}_r(r,z,t) = U_r(z) e^{\lambda t} J_1(kr),
\f]
\f[
\hat{u}_z(r,z,t) = U_z(z) e^{\lambda t} J_0(kr),
\f]
and
\f[
\hat{p}(r,z,t) = P(z) e^{\lambda t} J_0(kr).
\f]
As in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#validation">two-dimensional two-layer example</A>, 
we will have a linear system containing nine unknowns, rather than the
five unknowns that arise from a single-fluid problem.
The real and imaginary parts of \f$ \lambda \f$
correspond to the growth rate and the frequency of the oscillating
interface respectively, and can be compared to numerical results
computed for given values of the wavenumber \f$ k \f$. We choose an
initial deflection amplitude of \f$ \epsilon = 0.01 \f$ and determine
the growth rate and frequency of the oscillation from a time-trace of
the left-hand edge of the interface.

\image html two_layer_interface_axisym_code_validation.gif "Validation of the code (points) by comparison with an analytical dispersion relation (lines). " 
\image latex two_layer_interface_axisym_code_validation.eps "Validation of the code (points) by comparison with an analytical dispersion relation (lines). " width=0.95\textwidth

<HR>
<HR>

\section namespace Global parameters and functions

As in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#namespace">earlier example</A>, we use a namespace to define the dimensionless parameters
\f$ Re \f$, \f$ St \f$, \f$ Re/Fr \f$ and \f$ Ca \f$. Since we are
solving for the unknown bulk nodal positions by treating the interior
of
the mesh as a fictitious elastic solid, we also need to define the
Poisson ratio for this `pseudo-solid's' generalised Hookean
constitutive law, \f$ \nu \f$. As before, we define the density and
viscosity ratios of the top fluid to the bottom fluid and a unit
vector \f$ G \f$ which points in the direction of gravity. The only
difference from <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#namespace">before</A> 
is that in this case \f$ G \f$ must have three dimensions since this
is an axisymmetric problem.

\dontinclude elastic_two_layer_interface_axisym.cc
\skipline Direction of gravity
\until Vector<double> G

<HR>
<HR>

\section main The driver code

The first section of the driver code is identical to that of the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#main">earlier example</A>. We define a command line flag which allows us to run a
`validation' version of the code (for \c oomph-lib's self-testing 
routines) and check that the non-dimensional quantities provided in
the \c Global_Physical_Variables namespace are self-consistent.
Next we specify the duration of the simulation and the
size of the timestep. If we are running the code as a validation, we
set the length of the simulation such that only two timesteps are
taken. The direction in which gravity acts is defined to be vertically
downwards.

\skipline Set direction of gravity
\until G[2]

Finally, we build the problem using the `pseudo-solid' version of \c
RefineableAxisymmetricQCrouzeixRaviartElements and the \c BDF<2> timestepper, before calling \c
unsteady_run(...). This function solves the system at each timestep
using the \c Problem::unsteady_newton_solve(...) function before
documenting the result.

\skipline Set up the elastic test problem
\until End of main

<HR>
<HR>

\section mesh The mesh class

The mesh class is almost identical to that in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#mesh">two-dimensional problem</A>.
The only difference is that, given that this (specific) mesh is to be
used in an axisymmetric problem, it does not make sense to allow the
mesh to be periodic in the horizontal direction.

\section problem_class The problem class

The problem class is almost identical to the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#problem_class">two-dimensional problem</A>.
The only modification arises in the arguments of \c
deform_free_surface(...), as the initial shape of the interface is a
Bessel function \f$ J_0(kr) \f$ as opposed to a cosine curve.

\dontinclude elastic_two_layer_interface_axisym.cc
\skipline Deform the mesh
\until deform_free_surface

\section constructor The problem constructor

The constructor starts by building the timestepper and setting the
dimensions of the mesh. The number of elements in the
\f$ r \f$ and \f$ z \f$ directions in both fluid layers are
specified. The next section of the constructor is exactly as <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#constructor">before</A>.
We build the bulk mesh, create an error estimator and set
the maximum refinement level. An empty surface mesh is created
and populated with a call to \c create_interface_elements, before the
two meshes are combined to form the global mesh. Next we define the
boundary conditions.
On the top and bottom boundaries (\f$ z = 2.0 \f$ and \f$ z = 0.0 \f$)
we apply the no-slip condition by pinning all three velocity
components. On the outer (solid) wall (\f$ r = 1.0 \f$) we pin the
radial (no penetration) and azimuthal (no slip) components but not the
axial component. This simulates the "slippery" outer
wall. These conditions are also applied at the symmetry boundary (\f$
r = 0.0 \f$). The vertical position of the nodes on the top and
bottom boundaries are also pinned.

\skipline Set the boundary conditions for this problem
\until End of loop over mesh boundaries

We pin the horizontal position of all nodes in the mesh as well as the
azimuthal velocity components throughout the bulk of the domain (since
in an axisymmetric problem these should remain zero always).

\skipline Loop over all nodes in mesh
\until }

The remainder of the problem constructor is identical to that of the
<A HREF="../../../navier_stokes/two_layer_interface/html/index.html#constructor">two-dimensional problem</A>. 
We create a generalised Hookean constitutive equation for the
pseudo-solid mesh before looping over all elements in the bulk mesh
and assigning pointers to the Reynolds and Womersley numbers, \f$ Re \f$ and \f$
Re\, St \f$, the product of the Reynolds number and the inverse of the
Froude number, \f$ Re/Fr \f$, the direction of gravity, \f$ G \f$, the
constitutive law and the global time object. For those elements which
correspond to the upper fluid layer, we also assign pointers for the viscosity and density ratios, \f$ R_\mu \f$ and \f$
R_\rho \f$. We pin one degree of freedom and call the function which
assigns the boundary conditions, before finally setting up the
equation numbering scheme.

<HR>
<HR>

\section set_initial_condition Initial conditions

The \c set_initial_conditions() function is very similar to that in
the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#set_initial_condition">two-dimensional problem</A>. 
We loop over all nodes in the mesh and set all three
velocity components to zero, before calling \c
Problem::assign_initial_values_impulsive(). This function copies the
current nodal values and positions into the required number of history
values for the timestepper in question, simulating an
impulsive start.

\skipline start_of_set_initial_condition
\until End of set_initial_condition

<HR>
<HR>

\section set_boundary_conditions Boundary conditions

The \c set_boundary_conditions() function is very similar to that in
the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#set_boundary_conditions">two-dimensional problem</A>. 

\skipline start_of_set_boundary_conditions
\until End of set_boundary_conditions

\section before_after_adapt Actions before and after adaptation

These functions follow exactly the same structure as those in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#before_adapt">two-dimensional problem</A>.

<HR>
<HR>

\section create_interface_elements Create interface elements

This function is identical to that in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#create_interface_elements">two-dimensional problem</A>, 
with the exception that instead of creating interface elements of the
type

\code
ElasticLineFluidInterfaceElement<ELEMENT>,
\endcode

we are creating interface elements of the type

\code
ElasticAxisymFluidInterfaceElement<ELEMENT>,
\endcode

where \c ELEMENT is the templated bulk element type.

<HR>
<HR>

\section delete_interface_elements Delete interface elements

This function is identical to that in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#delete_interface_elements">two-dimensional problem</A>.


<HR>
<HR>

\section deform_free_surface Prescribing the initial free surface position

At the beginning of the simulation the interface is deformed by a
prescribed function (9), implemented in
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

This function follows an identical structure to that in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#doc">two-dimensional problem</A>.

<HR>
<HR>

\section unsteady_run The timestepping loop

The function \c unsteady_run(...) is used to perform the timestepping
procedure, and is very similar to that in the <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#unsteady_run">two-dimensional problem</A>. 
The only difference arises from the fact that the initial
interface deformation is a Bessel function rather than a cosine
curve. 

\skipline Set value of epsilon
\until deform_free_surface

The rest of the function is identical to <A HREF="../../../navier_stokes/two_layer_interface/html/index.html#unsteady_run">before</A>.

<HR>
<HR>

\section sources Source files for this tutorial
- The source files for this tutorial are located in the directory:
<CENTER>
<A HREF="../../../../demo_drivers/axisym_navier_stokes/two_layer_interface_axisym/">
demo_drivers/axisym_navier_stokes/two_layer_interface_axisym/
</A>
</CENTER>
- The driver code is:
<CENTER>
<A HREF="../../../../demo_drivers/axisym_navier_stokes/two_layer_interface_axisym/elastic_two_layer_interface_axisym.cc">
demo_drivers/axisym_navier_stokes/two_layer_interface_axisym/elastic_two_layer_interface_axisym.cc
</A>
</CENTER>
.


<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

