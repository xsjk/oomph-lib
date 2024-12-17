/**

\mainpage  Flow in a 2D collapsible channel revisited -- enabling adaptivity in FSI problems

 In this document we re-visit the collapsible channel problem yet
again -- this time to demonstrate the use of spatial adaptivity in 
fluid-structure interaction problems. In such problems, two
additional issues have to be addressed:
-# \ref history
-# \ref update_node_update
. 
We start with a brief reminder of \c oomph-lib's mesh adaptation 
procedures to highlight what additional complications arise
in fluid-structure interaction problems, and then explain how these 
problems are addressed.

<HR>
<HR> 

\section history The assignment of positional history values for newly-created nodes

\subsection what What is the problem?

We first discussed the use of spatial adaptivity for time-dependent 
problems in the context of 
<A HREF="../../../unsteady_heat/two_d_unsteady_heat_adapt/html/index.html#spatial_adapt">
the unsteady heat equation</A>. We showed that whenever a new node is created,
\c oomph-lib's mesh adaptation procedures automatically assign the
nodal values and "history values" (the auxiliary values that are used 
by the \c Node's \c TimeStepper to approximate
time-derivatives of the nodal values) for the newly-created \c Node 
by interpolating between the corresponding values in the new 
\c Node's father element. 

Similarly, an initial assignment for the newly-created \c Node's current
and previous positions (required for the evaluation of the mesh-velocities 
in the ALE formulation of the time-derivatives) is made by interpolating 
between the current and previous nodal positions of the new \c Node's father 
element.  This default assignment is subsequently overwritten if the 
father element is associated with a \c MacroElement, in which case the
newly-created \c Node's current position is determined by the
father element's \c MacroElement representation. This ensures
that the mesh refinement process respects curvilinear domain 
boundaries, as discussed  
<A HREF="../../../poisson/fish_poisson2/html/index.html">elsewhere.</A>
The same strategy is used to over-write the default assignment for
a new \c Node's previous positions: They are re-assigned, based on the
father element's \c MacroElement - representation, evaluated at the
relevant previous timesteps.

To illustrate the process, consider the sketch below and 
assume that the time-integration of the Navier-Stokes equations is 
performed with a \c BDF<2> timestepper -- a timestepper that requires 
two history values (the solution and the nodal positions at two
previous timesteps) to evaluate the time-derivative 
\f$ \partial u_i/\partial t \f$ in the momentum equations. The
position of the nodes in the collapsible 
part of the channel is determined by the \c MacroElement - based
representation of the \c CollapsibleChannelDomain, using the 
wall shape obtained from the compound \c MeshAsGeomObject that
we created from the discretisation of the elastic wall, 
shown in green.

Assume that the initial mesh is sufficiently 
fine to resolve the solution at \f$ t=t_0 \f$ and \f$ t=t_0 + \Delta
t \f$. At \f$ t=t_0 + 2 \Delta t \f$, the automatic mesh adaptation 
splits one of the fluid elements, creating the five new \c Nodes,
shown in red (for simplicity we only show the fluid elements' vertex
nodes). Their position is determined by the father element's
\c MacroElement mapping, using the current position of the 
\c MeshAsGeomObject, obtained from the interpolation between the 
nodal positions of its \c SolidNodes (shown as green circles). 

\image html history_values.gif "Sketch illustrating the assignment of positional history values for newly-created nodes in FSI problems. The positional history values of the newly-created Nodes (shown in red) are given by the positions they would have had if they had already existed at previous timesteps. " 
\image latex history_values.eps "Sketch illustrating the assignment of positional history values for newly-created nodes in FSI problems. The positional history values of the newly-created Nodes (shown in red) are given by the positions they would have had if they had already existed at previous timesteps. " width=0.75\textwidth


So far, so good! A subtle problem arises when we try to assign
the positional history values for the newly-created \c Nodes. The evaluation
of the \c MacroElement mapping at the previous timesteps 
(required to determine the positions the newly-created \c Nodes would have
had, if they had already existed at previous timesteps) requires
access to the previous wall shapes. In 
<A HREF="../../../navier_stokes/collapsible_channel/html/index.html">
the non-FSI problem discussed earlier</A>, the wall shape was given 
analytically and could therefore
be evaluated at arbitrary times. In the FSI problem considered here,
the previous wall shape is not available as the previous positions
of the \c SolidNodes are not required (and are therefore not stored)
for the solution of the (steady!) wall equations.


<HR>

\subsection soln The solution: The Steady<NSTEPS> timestepper

The solution to the problem is simple: Recall that the final entry in
the argument list of \c oomph-lib's mesh constructors specifies the 
\c TimeStepper to be used for the evaluation of any time-derivatives.
The \c TimeStepper's member function 
\c TimeStepper::ntstorage() specifies the total number of
values (the current value plus the number of history values) required 
to evaluate the time-derivatives; it is used by the \c
FiniteElement::construct_node(...) function to create the required
amount of storage at the elements' \c Nodes when the \c Nodes are
created in the mesh constructor. To maximise the
potential for code-reuse, we always provide a default argument for
the pointer to the \c TimeStepper -- a pointer
to a static instantiation of \c oomph-lib's dummy \c TimeStepper, \c
Steady<0>. This ensures that the "user" can use the mesh for steady problems
without having to artificially create a dummy \c TimeStepper that
would be completely irrelevant for his/her problem.

For instance, the full interface to the
constructor of the \c OneDLagrangianMesh, defined in
<A HREF="../../../../src/meshes/one_d_lagrangian_mesh.template.h">
one_d_lagrangian_mesh.template.h</A> is given by

\dontinclude one_d_lagrangian_mesh.template.h
\skipline Constructor:
\until &Mesh::Default_TimeStepper);

Thus, when we constructed the wall mesh in the non-adaptive
version of the driver code 
<A HREF="../../../../demo_drivers/interaction/fsi_collapsible_channel/fsi_collapsible_channel.cc">fsi_collapsible_channel.cc,</A> using the statements

\code
 //Create the "wall" mesh with FSI Hermite beam elements
 Wall_mesh_pt = new OneDLagrangianMesh<FSIHermiteBeamElement>
  (Ncollapsible,Lcollapsible,undeformed_wall_pt);
\endcode

the use of the dummy \c TimeStepper, \c Steady<0> was implied. 
All time-derivatives computed by this \c TimeStepper 
evaluate to zero, and the \c TimeStepper does not require (or request)
any storage for history values. This is a sensible default for problems 
that are either inherently steady (such as problems involving the 
Poisson equations, say) or for steady versions of time-dependent PDEs 
(such as the steady Navier-Stokes equations). In the present
problem, we wish to retain the dummy character of the \c TimeStepper
so that all time-derivatives in the \c FSIHermiteBeamElements
are set to zero, while retaining a 
limited history of the \c SolidNode's previous positions so that
the wall shape at previous timesteps can be evaluated. This is
possible by creating a \c Steady timestepper with 
a non-zero template argument: The class

\code
template<unsigned NSTEPS>
class Steady : public TimeStepper
\endcode

provides a dummy \c TimeStepper that requires (and maintains) \c
NSTEPS history values that represent the nodal positions 
(or nodal values) at \c NSTEPS previous timesteps. However, these
history values are not used to evaluate any time-derivatives --
all time-derivatives computed by this \c TimeStepper are zero. 

If the Navier-Stokes equations are discretised with a \c BDF<2>
timestepper, the evaluation of the mesh velocity in the 
ALE representation of the time-derivatives \f$ \partial u_i/\partial t
\f$ requires the nodal position at the two previous timesteps.
This is achieved by passing a pointer to a specifically-constructed
\c Steady<2> timestepper to the wall mesh. Here is the relevant
code fragment from the Problem constructor in the driver code
<A HREF="../../../../demo_drivers/interaction/fsi_collapsible_channel/fsi_collapsible_channel_adapt.cc">fsi_collapsible_channel_adapt.cc</A>:

\dontinclude fsi_collapsible_channel_adapt.cc
\skipline Allocate the timestepper 
\until undeformed_wall_pt,wall_time_stepper_pt);
 

<HR>
<HR>





\section update_node_update Updating the node-update data in refineable AlgebraicMeshes

When discussing 
<A HREF="../../../navier_stokes/algebraic_collapsible_channel/html/index.html">
the non-FSI version of the collapsible channel problem</A>
we explained how \c oomph-lib's mesh adaptation procedures
assign the node-update data for newly-created \c AlgebraicNodes.
Recall that the node-update data comprises:
- A pointer to the \c AlgebraicMesh that implements the 
  node-update function.  \n\n
- An ID for the node-update function. This is used in cases in which
  different regions of the \c AlgebraicMesh are updated by different
  node-update functions. The ID has a default value of 0; this is
  used if there is only a single node-update function, as in 
  the \c AlgebraicCollapsibleChannelMesh. \n\n
- A vector of (pointers to) \c GeomObjects that are involved in
  the node update. \n\n
- A vector of reference values, such as the intrinsic coordinates
  of reference points on the \c GeomObjects. \n\n
.
By default, we assume that a newly-created \c AlgebraicNode
is updated by the same node-update function as the \c AlgebraicNodes
in its father element. Therefore we pass the pointer to the \c AlgebraicMesh,
the node-update function ID, and the vector of pointers to \c
GeomObjects to the newly-created \c AlgebraicNodes, and interpolate 
the reference values between those stored at the \c AlgebraicNodes 
in the father elements. 

In most cases this provides a sensible default. For instance, it 
is hard to imagine a situation in which it would be sensible to update the
position of newly-created \c AlgebraicNodes by a procedure that
differs from that used for the surrounding 
\c AlgebraicNodes that already existed in the father element. 
Similarly, since the reference values vary from node to node
(if they were constant we would not store them in the \c AlgebraicNodes' 
node-update data but in the \c AlgebraicMesh that implements the node
update!) it makes sense to assign the values at newly-created 
\c AlgebraicNodes by interpolation. For instance, in the
\c AlgebraicCollapsibleChannelMesh one of the reference values
is the \f$ x_1 \f$ - coordinate of the reference point on the fixed
lower wall. For the \c AlgebraicNodes that already existed in the
coarse base mesh, this value is given by the the \c AlgebraicNodes' 
\f$ x_1 \f$ - coordinate in the undeformed mesh. Interpolation
of this value for the newly-created \c AlgebraicNodes results in 
an axially uniform subdivision of the refined elements.

  The same procedure may be used to assign the reference value that
represents the intrinsic coordinate of the reference point on the
upper wall -- at least as long as the upper wall is only ever
addressed as a compound \c GeomObject, as in our original (slow!) code 
that employed a \c MacroElement - based node update.  The
procedure is illustrated in the sketch below: The upper wall is 
parametrised by a compound \c GeomObject in which the beam's Lagrangian
coordinate \f$ \xi \f$ doubles as the \c GeomObject's intrinsic 
coordinate \f$ \zeta. \f$
The green arrows indicate the reference points for nodes I, II, III,
IV and V in the coarse initial mesh. The reference values
\f$ \zeta_{I}^{[ref]}, ... , \zeta_{V}^{[ref]} \f$ are stored
in the node-update data of nodes I,...,V, respectively. 
The red arrow identifies the reference point for the newly-created 
node VI whose reference coordinate \f$ \zeta_{IV}^{[ref]} \f$ is created by 
interpolation between the reference values of nodes II, III, IV and V,
i.e. the nodes in its father element.


\image html update_single.gif "Illustration of the default update procedure for the node-update data: The reference value for newly created node VI is created by interpolation from the reference values for nodes II, III, IV and V in its father element. " 
\image latex update_single.eps "Illustration of the default update procedure for the node-update data: The reference value for newly created node VI is created by interpolation from the reference values for nodes II, III, IV and V in its father element. " width=0.75\textwidth



Unfortunately, the default assignment is not appropriate for nodes
that are updated by the (much faster) algebraic node update procedure 
implemented in the \c AlgebraicCollapsibleChannelMesh. Its
\c AlgebraicNodes store pointers to the sub-\c GeomObjects (i.e. the 
\c FSIHermiteBeamElements in the wall mesh) that contain their 
reference points,  and the values of the local coordinate at which 
the reference points are located. This is illustrated in the sketch
below in which the sub- \c GeomObjects, parametrised by their local 
coordinates \f$ s = \zeta, \f$ are shown in magenta. Note that 
the reference points for nodes II and IV are located in one 
sub- \c GeomObject, those for nodes III and V are located in another. 

\image html update_compound.gif "Illustration of the revised update procedure for the node-update data: The intrinsic coordinate within the compound GeomObject can be created by interpolation. The coordinate within the sub-GeomObject has to be determined via the GeomObject::locate_zeta(...) function. " 
\image latex update_compound.eps "Illustration of the revised update procedure for the node-update data: The intrinsic coordinate within the compound GeomObject can be created by interpolation. The coordinate within the sub-GeomObject has to be determined via the GeomObject::locate_zeta(...) function. " width=0.75\textwidth

 
It obviously does not make sense to create the reference value for
the newly-created node VI by interpolation. The node update data must be
created from scratch in two steps:
-# Determine the newly-created Node's intrinsic reference coordinate within
   the compound \c MeshAsGeomObject (i.e. the continuous beam's
   Lagrangian coordinate), \f$ \xi_{VI}^{[ref]}\f$ by interpolation.
   (<A HREF="../../fsi_collapsible_channel_algebraic/html/index.html">Recall 
   </A> that the \c AlgebraicNodes in the non-refineable
   \c AlgebraicCollapsibleChannelMesh already stored this coordinate,
   even though it was not used during the node-update itself). 
   \n\n
-# Use the \c GeomObject::locate_zeta(...) function to determine
   sub-\c GeomObject and the local coordinate within it, at which
   the reference point with Lagrangian coordinate \f$
   \xi_{VI}^{[ref]}\f$  is located.
.
To ensure that this procedure is performed automatically after
every mesh adaptation, it should be implemented in the
function \c AlgebraicMesh::update_node_update(...). This function is defined
as a pure virtual function in the \c AlgebraicMesh base class in order to 
force the mesh writer to assess if the default procedure
for the assignment of the node-update data for newly-created
\c AlgebraicNodes, described above, is appropriate. If it 
is, the function may, of course, be implemented as an empty function.
We note that the function \c AlgebraicMesh::update_node_update(...)
is called \b after the default values for the node-update
data have been assigned. It is therefore only necessary to correct
those values for which the default procedure is inappropriate.

To illustrate the procedure, here is the class definition for the
\c RefineableAlgebraicCollapsibleChannelMesh. The mesh
is derived by multiple inheritance from the non-refineable
\c AlgebraicCollapsibleChannelMesh and the \c RefineableQuadMesh 
base class. The constructor calls the constructors of both underlying
meshes and then sets up the \c QuadTreeForest: 

\dontinclude collapsible_channel_mesh.template.h
\skipline start_of_refineable_algebraic
\until }

We overload the (empty) previous implementations of
\c AlgebraicMesh::update_node_update(...) with our own function:

\skipline Update the node update
\until };

Here is the actual implementation of this function. We start by 
extracting the reference values that were already assigned
by the default procedure:

\dontinclude collapsible_channel_mesh.template.cc
\skipline start_update_node_update
\until Vector<double> ref_value
 
Recall from the discussion of the algebraic node-update strategy
<A HREF="../../fsi_collapsible_channel_algebraic/html/index.html#mesh">
for the non-refineable version of the problem</A> that reference values
0 and 1 store the \f$ x_1 \f$ - coordinate along the fixed bottom
wall, and the fractional height of the node in the cross-channel
direction, respectively. These values are interpolated
correctly and do not have to be corrected. The third reference
value is the intrinsic coordinate of the reference point
in its sub-\c GeomObject. This needs to be re-computed and we 
will assign the corrected value below. The fourth reference value 
is the intrinsic coordinate of the reference point within 
the compound \c MeshAsGeomObject. We store this in a temporary 
variable:

\skipline Fourth reference value:
\until double zeta

Next, we extract vector of (pointers to the) \c GeomObjects involved 
in this node's node-update from the node.


\skipline Extract geometric objects
\until Vector<GeomObject*>

Now we use the \c GeomObject::locate_zeta(...) function to obtain
the pointer to the (sub-)\c GeomObject and the intrinsic coordinate
within it, at which the reference point (identified by its
intrinsic coordinate \c zeta in the compound \c GeomObject) is
located:

\skipline Get pointer to geometric
\until this->Wall_pt

We over-write the first (and, in fact, only) entry in the 
vector of \c GeomObjects that are involved in this node's node-update
with the pointer to the (sub-)\c GeomObject just located:

\skipline Update the pointer to the (sub-)
\until geom_object_pt[0] =

Similarly, we over-write the third reference value with the local
coordinate of the reference point within its (sub-)\c GeomObject.

\skipline Update third reference value
\until ref_value[2] =

The incorrect entries in the two vectors 
\c geom_object_pt and \c ref_value have now been corrected. We can
wipe the node's node-update data and re-assign it:

\skipline Kill the existing
\until }

That's it!



<HR>
<HR>

\section results Results

The figure below shows a snapshot of the flow field during the
early stages of the oscillation. The computation was performed with
refineable Crouzeix-Raviart elements, using the 
\c RefineableAlgebraicCollapsibleChannelMesh. Note how the automatic 
mesh adaptation has refined the mesh in the regions in which the 
Stokes layers create steep velocity gradients.
 
\image html fsi_CR_algebraic_adapt.gif "Snapshot from the animation of the flow field. " 
\image latex fsi_CR_algebraic_adapt.eps "Snapshot from the animation of the flow field. " width=0.8\textwidth



<HR>
<HR>

\section driver The driver code

The driver code 
<A HREF="../../../../demo_drivers/interaction/fsi_collapsible_channel/fsi_collapsible_channel_adapt.cc">fsi_collapsible_channel_adapt.cc</A>
for the spatially adaptive problem is a trivial extension of the 
non-adaptive code 
<A HREF="../../../../demo_drivers/interaction/fsi_collapsible_channel/fsi_collapsible_channel.cc">fsi_collapsible_channel.cc,</A>
therefore we will not provide a detailed listing here.
Comparing the two source codes (e.g. with \c sdiff) shows that 
spatial adaptivity may be enabled with a few straightforward changes:
- Change the fluid mesh from the \c
  MacroElementNodeUpdateCollapsibleChannelMesh to  the
  \c MacroElementNodeUpdateRefineableCollapsibleChannelMesh
  (or from the \c AlgebraicCollapsibleChannelMesh to the
  \c RefineableAlgebraicCollapsibleChannelMesh). \n\n
- Change the element type from \c TaylorHoodElement<2> 
  to  \c RefineableQTaylorHoodElement<2> (or 
  \c QCrouzeixRaviartElement<2> to \c
  RefineableQCrouzeixRaviartElement<2>). \n\n
- Add the functions \c Problem::actions_before_adapt() and
  \c Problem::actions_after_adapt(); see below. \n\n
- Explicitly specify the \c Steady<2> timestepper for the
  wall mesh, as discussed above. \n\n
- Create an error estimator, specify the target errors for
  the adaptation, and call the spatially adaptive Newton solver.
.

<HR>

\subsection before_and_after Actions before and after solve

As in the <A HREF="../../../navier_stokes/collapsible_channel/html/index.html">
non-FSI problem</A>, we use the function \c
Problem::actions_before_adapt() to delete the applied traction
elements before the mesh adaptation:

\dontinclude fsi_collapsible_channel_adapt.cc
\skipline start_of_actions_before_adapt
\until end of actions_before_adapt


As usual, we then employ the function \c Problem::actions_after_adapt()
to re-attach traction elements to the fluid elements that are located 
at the mesh's inflow boundary (mesh boundary 5) when the mesh 
adaptation is complete. 

\dontinclude fsi_collapsible_channel_adapt.cc
\skipline start_of_actions_after_adapt
\until rebuild_global_mesh();

Next, we pin the redundant pressure degrees of freedom
(see <A HREF="../../../navier_stokes/adaptive_driven_cavity/html/index.html">
another tutorial</A> for details) and pass the
function pointer to the function that defines the prescribed
traction to the applied traction elements:

\until }

We specify the function 
\c FSI_functions::apply_no_slip_on_moving_wall() as the
auxiliary node-update function for all fluid nodes that are located
on the FSI boundary (mesh boundary 3) -- this ensures that
the fluid velocity is updated (via the no-slip condition)
whenever the position of a fluid node on this boundary is updated.
For fluid nodes that already existed before the mesh adaptation
this statement over-writes the function pointers already stored
at those nodes. Note the use of compiler flags to distinguish between
the two node-update strategies.

\until }

Finally, we re-generate the FSI lookup scheme that establishes which
fluid elements are located next to the Gauss points in the beam
elements. This is necessary because the previous lookup scheme, set 
up in the problem constructor, becomes invalid if any of the fluid
elements next the wall are split during the refinement process.

\until end of actions_after_adapt

<HR>
<HR>

\section comments Comments and Exercises
\subsection ex Exercises
-# The post-processing function \c doc_solution(...) outputs the 
   wall shape at the present and the two previous timesteps: The
   output file \c wall\e i-\e j.\c dat is created at the \e i - th 
   timestep and contains the wall shape at the \e j - th previous
   timestep.  To check that the \c Steady<2> timestepper correctly 
   maintains the time-history of the wall displacement field, confirm that
   \c wall3-2.dat is identical to \c wall2-0.dat, say.
   \n\n
-# Explore what happens if the \c Steady<2> timestepper is not specified
   explicitly. [Hint: Unless you have compiled the library with
   \c -DRANGE_CHECKING, the code will die with a segmentation fault -- use 
   the debugger to determine where the segmentation fault occurs 
   and explain what goes wrong.] \n\n
-# Explain why refineable \c MacroElementNodeUpdateMeshes do not require
   a \c update_node_update() function.  \n\n
-# Explore what happens if the empty function \c
   AlgebraicCollapsibleChannelMesh::update_node_update(...) is not
   overloaded in the derived refineable mesh 
   \c RefineableAlgebraicCollapsibleChannelMesh.
   [Hint: Inspect the plot of the initial conditions in
   the output file \c soln0.dat to see how the 
   uniform refinement performed in the problem constructor
   fails if the function \c
   RefineableAlgebraicCollapsibleChannelMesh::update_node_update(...)
   is not executed.]
.


<HR>
<HR>


\section sources Source files for this tutorial
- The source files for this tutorial are located in the directory:\n\n
<CENTER>
<A HREF="../../../../demo_drivers/interaction/fsi_collapsible_channel/">
demo_drivers/interaction/fsi_collapsible_channel/
</A>
</CENTER>\n
- The driver code is: \n\n
<CENTER>
<A HREF="../../../../demo_drivers/interaction/fsi_collapsible_channel/fsi_collapsible_channel_adapt.cc">
demo_drivers/interaction/fsi_collapsible_channel/fsi_collapsible_channel_adapt.cc
</A>
</CENTER>
.

<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

