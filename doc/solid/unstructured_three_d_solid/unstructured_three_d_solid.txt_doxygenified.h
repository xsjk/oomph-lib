/**

\mainpage Demo problem: 3D Solid Mechanics on unstructured meshes

The purpose of this tutorial is to demonstrate the solution of
3D solid mechanics problems on unstructured meshes. 
The problem studied here also serves as a "warm-up problem" for the
<a href="../../../interaction/unstructured_three_d_fsi/html/index.html">
corresponding fluid-structure interaction problem </a> in which the elastic,
bifurcating vessel whose deformation is studied here conveys (and is 
loaded by) a viscous fluid.
 
 
<HR> 
<HR>

\section problem The problem (and results)
Here is an animation of the problem: An elastic, bifurcating vessel
is loaded by an internal pressure and by gravity which acts in the
negative \f$x\f$-direction. The "ends" of the vessel are held in 
a fixed position. As the magnitude of the loading is
increased the vessel inflates and sags downwards. 


\image html deform.gif "An elastic, bifurcating vessel, loaded by an internal pressure and transverse gravity. " 
\image latex deform.eps "An elastic, bifurcating vessel, loaded by an internal pressure and transverse gravity. " width=0.75\textwidth


The blue frame 
surrounding the vessel is drawn to clarify its spatial orientation. 
The yellow edges show the boundaries of the
internal faces via which the pressure loading is applied, and the
red arrows indicate the direction and magnitude of the pressure loading.

<HR>
<HR> 

\section mesh 3D unstructured mesh generation
We use 
<A HREF="http://www.wias-berlin.de/~si ">Hang Si's</A>
open-source mesh generator 
<A HREF="http://wias-berlin.de/software/tetgen//"> \c tetgen </A> to generate the 
unstructured tetrahedral mesh "offline". We then
process the output files produced by  
<A HREF="http://wias-berlin.de/software/tetgen//"> \c tetgen </A>
to generate an unstructured \c oomph-lib mesh. 

<A HREF="http://wias-berlin.de/software/tetgen//"> \c Tetgen </A>
requires the specification of the domain boundaries
via so-called facets -- planar surface patches that are bounded
by closed polygonal line segments. For simplicity, we only
consider a very simplistic bifurcation, comprising three tube segments
of approximately rectangular cross-section that meet at a common junction.
Each of the three tube segments has four internal and four external
faces. The internal and external faces are connected by three further 
faces at the "in- and outflow" cross-sections (using terminology 
that anticipates the mesh's use in the corresponding
<a href="../../../interaction/unstructured_three_d_fsi/html/index.html"> 
fluid-structure interaction problem</a>), resulting in a total of 27 
facets. 

The 27 facets are defined in a <code>*.poly</code> file that
specifies the position of the vertices, and identifies the 
facets via a "face list" that establishes their bounding vertices. 
Facets that have holes (e.g. the in- and outflow
facets) require the specification of the hole's position. Finally, 
if the mesh itself has a hole (as in the current example where the 
vessel's lumen forms a hole in the mesh) the position of the hole must be 
identified by specifying the position of a single point inside that 
hole. The well-annotated <code>*.poly</code> file is located at:

<center>
<a href="../../../../demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.poly">demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.poly</a>
</center>

We refer to the <A HREF="http://wias-berlin.de/software/tetgen//"> \c tetgen webpages </A>
and another <a href="../../../meshes/mesh_from_tetgen/html/index.html">
\c oomph-lib tutorial</a> for further details on how to 
create <code>*.poly</code> files.


Here is a plot of the domain specified by 
<a href="../../../../demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.poly">fsi_bifurcation_solid.poly</a>.
The plot was created using \c tetview which is distributed with
<A HREF="http://wias-berlin.de/software/tetgen//"> \c tetgen </A>.

\image html tetgen_boundaries.gif "The domain and its bounding facets. " 
\image latex tetgen_boundaries.eps "The domain and its bounding facets. " width=0.75\textwidth


Note that we have deliberately assigned a different boundary ID to 
each facet. This will make the assignment of the traction boundary
condition somewhat tedious as the inner surface of the vessel (where the
traction is to be applied) is represented by twelve separate
mesh boundaries. However, the assignment of distinct boundary
IDs for the different facets is essential for the automatic generation
of boundary coordinates in the
<a href="../../../interaction/unstructured_three_d_fsi/html/index.html">
corresponding fluid-structure interaction problem </a> and is
therefore <b>strongly recommended</b>.


<A HREF="http://wias-berlin.de/software/tetgen//"> \c Tetgen </A> generates
an unstructured volumetric mesh from the information contained
in the <code>*.poly</code> file and writes the mesh's nodes, 
elements and faces in the files
- <a href="../../../../demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.1.node">demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.1.node </a>
- <a href="../../../../demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.1.ele">demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.1.ele </a>
- <a href="../../../../demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.1.face">demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.1.face</a>
.
These files can be used as input to \c oomph-lib's \c TetgenMesh
class, using the procedure discussed in 
<a href="../../../meshes/mesh_from_tetgen/html/index.html">
another tutorial.</a>

  The figure below shows a \c tetview plot of the mesh, created with a volume
constraint of 0.2 (i.e. the maximum volume of each tetrahedron is
guaranteed to be less than 0.2 units), using the command
\code
tetgen -a0.2 fsi_bifurcation_solid.poly 
\endcode

\image html mesh_boundaries.gif "Plot of the mesh, generated by tetgen. " 
\image latex mesh_boundaries.eps "Plot of the mesh, generated by tetgen. " width=0.75\textwidth
 
Note how <A HREF="http://wias-berlin.de/software/tetgen//"> \c tetgen </A>
has subdivided each of the 27 original facets specified in the 
<code>*.poly</code> file into a surface triangulation. The 
nodes and tetrahedral elements that are located on (or adjacent to) the
27 original facets inherit their boundary IDs. This will be important
when we assign the boundary conditions.

<HR>
<HR>

\section mesh_code Creating the mesh

We create the solid mesh by multiple inheritance from \c oomph-lib's 
\c TetgenMesh and the \c SolidMesh base class:

\dontinclude unstructured_three_d_solid.cc
\skipline start_mesh
\until {

The constructor calls the constructor of the underlying 
\c TetgenMesh (using the <code>*.node </code>
<code>*.ele</code> and <code>*.face</code> files created
by <A HREF="http://wias-berlin.de/software/tetgen//"> \c tetgen. </A>
As usual we set the nodes' Lagrangian coordinates to their current 
Eulerian positions, making the current configuration
stress-free.

\until set_lagrangian_nodal_coordinates()

Finally, we identify the elements that are located next to the 
various mesh boundaries to facilitate the application of the
traction boundary conditions.

\until };

<HR> 
<HR>

\section namespace Problem parameters
As usual, we define the various problem parameters in a 
global namespace. We use \c oomph-lib's generalised Hookean
constitutive law as the constitutive equation, using a Poisson's
ratio of 0.3. 
(<a href="../../solid_theory/html/index.html#non-dim_solid"> Recall</a>
that omitting the specification of Young's modulus, \f$ E \f$, 
implies that the stresses are non-dimensionalised on \f$ E \f$ .)


\skipline start_namespace
\until Constitutive_law_pt

Next we define the gravitational body force which acts in
the negative \f$x\f$-direction,

\until }

and the pressure load, \f$ {\bf t} = -P {\bf n} \f$, that acts on 
internal walls of the bifurcation (note that the outer unit
normal \f$ {\bf n} \f$ on the wall is passed to the
function). 


\until end namespace

<HR>
<HR>

\section main The driver code

The driver code is straightforward. We store the command line
arguments, specify an output directory and create the problem object,
using ten-noded tetrahedral solid mechanics elements to 
discretise the principle of virtual displacements. 

\dontinclude unstructured_three_d_solid.cc
\skipline start_main
\until TPVD

We output the initial configuration and then perform a parameter
study in which we increment the gravitational body force and the pressure
loads simultaneously, causing the vessel to expand and
sag, as shown in the animation at the beginning of this tutorial.
(As usual we perform a smaller number of steps if the
code is run in self-test mode; this is indicated by a non-zero number
of command line arguments.) 

\until end main


<HR>
<HR>

\section class The Problem class
The \c Problem class has the usual member functions, and provides
storage for the various sub-meshes -- the bulk mesh of 3D solid elements
and the meshes of 2D solid traction elements that apply the
pressure load to the internal boundaries of the vessel.

\dontinclude unstructured_three_d_solid.cc
\skipline start_problem
\until Solid_traction_mesh


The two vectors \c Pinned_solid_boundary_id and
\c Solid_traction_boundary_id are used to store the IDs of mesh boundaries
that make up the in- and outflow cross-sections (where the
bifurcation is pinned), and the internal boundaries (where
the pressure load has to be applied). Recall that \c tetgen requires the domain 
boundaries to be specified as a collection of planar facets. Boundary
of interest in the computation, such as the "internal boundary of the
bifurcation", therefore tend to comprise multiple distinct mesh
boundaries.

\until };



<HR>
<HR>

\section constructor The Problem constructor

We start by building the bulk mesh, using the files created by
<A HREF="http://wias-berlin.de/software/tetgen//"> \c tetgen </A>:

\skipline start_constructor
\until face_file_name);

Next we specify the IDs of the  <A HREF="http://wias-berlin.de/software/tetgen//"> 
\c tetgen </A> boundaries that form part of specific domain
boundaries in our problem. Boundaries 0, 1 and 2 are
the in- and outflow faces along which the solid is pinned.
(See the specification of the boundaries in 
<a href="../../../../demo_drivers/solid/unstructured_three_d_solid/fsi_bifurcation_solid.poly">fsi_bifurcation_solid.poly</a>
and/or check the boundary enumeration using 
\c tetview as shown in the \c tetview plot of the domain
boundaries at the beginning of this tutorial.)


\until Pinned_solid_boundary_id[2]=2;

Similarly, boundaries 3 to 15 are the faces that define the internal
boundary of the bifurcation, i.e. the boundary along which we have
to apply the pressure load:

\until } 

We apply the boundary conditions by pinning the displacements
of all nodes that are located on the in- and outflow faces,
and document their positions.

\until bc_file.close();

We complete the build of the elements by setting the pointer to the
constitutive equation and the body force.

\until }


Next we create the traction elements, attaching them to the "bulk"
solid elements that are adjacent to the boundaries that constitute
the inside of the vessel. 

\until create_traction_elements();

Finally, we add the various meshes as sub-meshes to the \c Problem, build
the global mesh, and assign the equation numbers.

\until end constructor


<HR>
<HR>

\section traction Creating the traction elements
The helper function \c create_traction_elements() does exactly what it
says: It loops over the bulk elements that are adjacent
to the inner surface of the vessel, and attaches
\c SolidTractionElements to the appropriate faces. We store
the pointers to the newly-created traction elements in 
separate meshes and specify the function pointer to the load 
function.

\until end of create_traction_elements


<HR>
<HR>

\section doc Post-processing

The post-processing routine outputs the deformed domain shape
and the applied traction. 

\until end doc


<HR>
<HR>

\section comm_ex Comments and Exercises

\subsection bound Identification/assignment of mesh boundaries
This tutorial shows that the use of unstructured 3D meshes
for solid mechanics problems is extremely straightforward. 
The only aspect that requires some care (and not just for solid
mechanics applications) is the correct identification/assignment of
domain boundaries. The fact that we documented the position of the 
pinned nodes in the driver code suggests (correctly!) that we managed 
to  get both assignments (slightly) wrong when we first generated
the mesh and wrote the corresponding driver code. As usual, 
it pays off to <B>be  as a paranoid as possible</B>! Ignore this 
advice at your own risk...


Here is a plot of the position of the pinned solid nodes

\image html pinned_solid_nodes.gif "Plot of the pinned solid nodes. " 
\image latex pinned_solid_nodes.eps "Plot of the pinned solid nodes. " width=0.75\textwidth

and here's a plot showing the \c SolidTractionElements attached
to the inside of the vessel:

\image html traction_faces.gif "Plot of the SolidTractionElements that apply the pressure load to the inner surface of the vessel. " 
\image latex traction_faces.eps "Plot of the SolidTractionElements that apply the pressure load to the inner surface of the vessel. " width=0.75\textwidth


<HR>

\subsection your_own Exercise: Try it yourself

Experiment with the \c tetgen -based mesh generation by
modifying the <code>*.poly</code> file used in this example
to different vessel geometries.




<HR>
<HR>

\section sources Source files for this tutorial
- The source files for this tutorial are located in the directory:\n\n
<CENTER>
<A HREF="../../../../demo_drivers/solid/unstructured_three_d_solid/">
demo_drivers/solid/unstructured_three_d_solid/
</A>
</CENTER>\n
- The driver codes are: \n\n
<CENTER>
<A HREF="../../../../demo_drivers/solid/unstructured_three_d_solid/unstructured_three_d_solid.cc">
demo_drivers/solid/unstructured_three_d_solid/unstructured_three_d_solid.cc
</A>
</CENTER>
and 
<CENTER>
<A HREF="../../../../demo_drivers/solid/unstructured_three_d_solid/unstructured_three_d_solid.cc">
demo_drivers/solid/unstructured_three_d_solid/unstructured_three_d_solid.cc
</A>
</CENTER>
.











<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

