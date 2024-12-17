/**

\mainpage Linking VMTK with oomph-lib 

The <a href="http://www.vmtk.org">Vascular 
Modeling Toolkit</a> is a collection of libraries 
and tools for the 3D reconstruction, geometric analysis, mesh 
generation and surface data analysis for image-based modelling 
of blood vessels.

This tutorial demonstrates how to open your dataset in VMTK; navigate
around a 3D volume; reconstruct the 3D surface of a vascular segment from
CT or MR images; process a surface model to generate a mesh; and
how to extract the surface mesh. Finally, we demonstrate
how to use \c oomph-lib's conversion code 
\c create_fluid_and_solid_surface_mesh_from_fluid_xda_mesh
to generate fluid and solid meshes that allow the simulation of
physiological fluid-structure interaction problems in which the
(assumed constant thickness) vessel wall deforms in response to the fluid
traction.
 
Before starting, make sure that you have VMTK installed on your machine 
and that you have your DICOM (Digital Imaging and Communications in Medicine) 
images. Note that this tutorial describes the use of VMTK version 0.8.

For further information on VMTK, please refer to VMTK's 
<a href="http://www.vmtk.org">home page</a>.




<CENTER>
<TABLE BORDER=1, WIDTH=500px>
<TR>
<TD bgcolor="cornsilk">
<CENTER>
<B>Acknowledgement:</B>
</CENTER>
This tutorial and the associated driver codes were developed jointly
with Amine Massit (ENSTA, Paris).
</TD>
</TR>
</TABLE>
</CENTER>


<CENTER>
<TABLE BORDER=1, WIDTH=500px>
<TR>
<TD bgcolor="cornsilk">
<CENTER>
<B>Disclaimer:</B>
</CENTER>
The generation of high-quality computational meshes from medical
images is a non-trivial task. The
<a href="http://www.vmtk.org"> Vascular Modeling
Toolkit</a>  greatly simplifies this task but
it is important to remember that (intelligent) user input is required
at various stages of the process. 

 This tutorial does \b not provide
any guidance on this aspect. We simply demonstrate \b how to generate
meshes that can be used in \c oomph-lib - based  
physiological flow (or fluid-structure interaction) simulations.
If you smooth the vessels walls too much, you may lose essential
features; if you smooth too little, the shape of the vessel wall
may be polluted by imaging artifacts; etc. 
</TD>
</TR>
</TABLE>
</CENTER>

<hr>
<hr>

\section vmtk Using VMTK: From the MR/CT images to the fluid mesh

\subsection voi Volume of interest

VMTK is capable of reading DICOM directories (which contain the files 
with .dcm extension). You can open your dataset in VMTK and navigate around
the 3D volume with:
\code
 vmtkimagereader -f dicom -d dicom_directory --pipe vmtkimageviewer
\endcode

where \c dicom_directory is the path to the directory containing the
.dcm files.

 
\image html scan.gif " " 
\image latex scan.eps " " width=0.75\textwidth


Once the viewer pops up, you can:

 - Rotate the volume by left-clicking anywhere on the render window 
(outside the image).
 - Translate the volume by middle-clicking anywhere on the render
 window (outside the image).
 - Zoom the volume by right-clicking anywhere on the render window
(outside the image).
 - Probe the image planes (coordinates and graylevel) by left-clicking
   on them.
 - Move through the image planes by middle-clicking on them.
 - Change the window-level by right-clicking on image planes.
 - Quit the viewer by pressing \c q or \c e.


You can extract a volume of interest (VOI) from a dataset with :

\code
 vmtkimagereader -f dicom -d dicom_directory --pipe \
vmtkimagevoiselector -ofile image_volume_voi.vti
\endcode

where the argument to the \c -ofile option specifies the output file name.

When the render window pops up, pressing \c i will activate the
"interactor": a yellowish cube will appear that is used to
select the VOI.

\image html voi_cube.gif " " 
\image latex voi_cube.eps " " width=0.75\textwidth

    - Translate the cube by middle-clicking on it.
    - Resize the cube by left-clicking and dragging the little spheres 
(handles) on the faces of the cube.
    - Normal interaction with the image is still active, so you 
can still navigate in the image as explained before. 

\image html voi_cube2.gif " " 
\image latex voi_cube2.eps " " width=0.75\textwidth


When satisfied with the VOI, press \c q or \c e. The new volume is
displayed in the render window. 

\image html voi.gif " " 
\image latex voi.eps " " width=0.75\textwidth

You can quit by pressing \c q or \c e, or you can define another VOI 
by pressing \c i once again.


<hr>
<hr>



\subsection surface The 3D surface reconstruction of a vascular segment

VMTK is capable of generating an image based surface model from a \c vti
file (obtained using the procedure explained in the
section \ref voi above), using the Level Set method and can then
extract the surface model using a Marching Cubes algorithm.

First, enter:
\code
vmtklevelsetsegmentation -ifile image_volume_voi.vti -ofile level_sets.vti
\endcode

\image html voi_display.gif " " 
\image latex voi_display.eps " " width=0.75\textwidth

When the VOI is displayed in a render window, press \c q or \c e to proceed.

A message will appear on your terminal:

\code
 Please choose initialization type: (0: colliding fronts; 1: fast
 marching; 2: threshold; 3: isosurface)
\endcode

This lets you choose the initialization method used to create the
model of the blood vessel. In this tutorial, we will only demonstrate
the \c colliding \c fronts method. In this method two seeds are placed on
the image. Two fronts are then propagated from the seeds (one front
from each) with their speeds proportional to the image intensity. 
The region where the two fronts cross (or collide), defines the 
"deformable model" -- the initial representation of the vessel 
volume. For further information on this and the other
initialization methods, please refer to VMTK's 
<a href="http://www.vmtk.org">tutorial page</a>.

Now, enter \c 0 to initialise with \c colliding \c fronts. 

A message will then appear on the terminal:
\code
 Please input lower threshold ('i' to activate image, 'n' for none):
\endcode

Wave propagation, used in the \c colliding \c fronts method, can be 
restricted to a set of intensity levels between two thresholds.
Enter the lower threshold, if you want to use
one, otherwise enter \c n. Note that if you don't know the appropriate
threshold value you can press \c i to activate the image 
and probe it, then quit 
with \c q or \c e when probing is done.

For our example there is no need for thresholds. Therefore enter \c n.

The next message is:
\code
 Please input upper threshold ('i' to activate image, 'n' for none):
\endcode

We repeat the same steps as for the lower threshold.

Next, this message appears:
\code
 Please place two seeds (click on the image while pressing Ctrl).
\endcode

The render window is now activated. Place the two seeds, one 
at each of the two ends of a branch. (The geometry of the entire bifurcation
is created by "merging" the surfaces of individual branches.)
Interact with the image planes to find where you want to place your 
first seed. When
satisfied, left-click on the image while pressing \c Ctrl. A red sphere
will then appear. 

\image html seed1.gif " " 
\image latex seed1.eps " " width=0.75\textwidth

Repeat this procedure for the second seed.
Press \c q or \c e when done.

\image html seed2.gif " " 
\image latex seed2.eps " " width=0.75\textwidth

Provided everything went OK, a translucent surface will appear in the
render window between the two seeds. This is your initial deformable
model. 

\image html deformable_model1.gif " " 
\image latex deformable_model1.eps " " width=0.75\textwidth

Press \c q or \c e to proceed. 

Next, you will be prompted with:
\code
 Accept initialization? (y/n):
\endcode

If you are not satisfied with your initialisation, enter \c n to perform
it once again, otherwise enter \c y.

Now press \c q or \c e again (in fact, you should do this whenever
the system appears to be frozen and the message "Displaying" is 
shown on the terminal.). 
 
You are now prompted if you want to initialise another branch:

\code
Initialize another branch? (y/n): 
\endcode

Press \c n -- we will add the other branches later.

The following message will now appear: 

\code
Please input parameters (type return to accept current values, 'e' to
end, 'q' to quit):
 NumberOfIterations(0) [PropagationScaling(1.0) CurvatureScaling(0.0) 
AdvectionScaling(0.0)]:
\endcode

These parameters control the deformation of your Level Set:

- Number of iterations is the number of deformation steps 
  the model will perform.

- Propagation scaling is the weight you assign to model inflation.

- Curvature scaling is the weight you assign to model surface
  regularisation (this will eventually make the model collapse and
  vanish if it is too large).

- Advection scaling regulates the attraction of the surface of the 
  image gradient modulus ridges.

Based on our limited experience, we recommended that propagation and curvature
should be set to \c 0.0, and advection to \c 1.0. The number of iterations 
should be set large enough for the level set not to move anymore 
(if the region is not too big, try with \c 300). We encourage you to
experiment with these parameters yourself.

Therefore, enter:
\code
 300 0 0 1
\endcode

The level set will then evolve until the maximum number of iterations is
reached. Then the render window will activate, displaying the final
model.

\image html curvature0x0.gif " " 
\image latex curvature0x0.eps " " width=0.75\textwidth

To quit the render window press \c q or \c e as usual.

You will then be asked:
\code
 Accept result? (y/n)
\endcode

If you are not satisfied with your final model, enter \c n to go back to
the level set parameter question. Otherwise, enter \c y.

Let us assume we are not satisfied, therefore enter \c n. You will
then go back to:

\code
Please input parameters (type return to accept current values, 'e' to
end, 'q' to quit):
 NumberOfIterations(0) [PropagationScaling(1.0) CurvatureScaling(0.0) 
AdvectionScaling(0.0)]:
\endcode

Try again with a bigger curvature scaling:
\code
 300 0 0.5 1
\endcode

This yields the following surface:

\image html curvature0x05.gif " " 
\image latex curvature0x05.eps " " width=0.75\textwidth

To quit the render window press \c q or \c e as usual.

You will then be asked once again:
\code
 Accept result? (y/n)
\endcode

Assuming we are now satisfied, we enter \c y .

Next, we receive the prompt:
\code
Merge branch? (y/n)
\endcode

If you are (still) satisfied with this final model, enter \c y and this branch 
will be merged with any branches you segmented before. Otherwise,
enter \c n and this branch will be discarded. (\b Important: Since
this is our first branch we enter \c y even though no previous 
branches exist yet!)

The render window will activate, showing you the merged result. To quit 
the render window press \c q or \c e as usual.


Then you will be prompted with:
\code
Segment another branch? (y/n)
\endcode

If you want to segment another branch enter \c y, otherwise 
enter \c n.

Let's segment another branch and enter \c y. You will go back to :
\code
 Please choose initialization type: (0: colliding fronts; 1: fast
 marching; 2: threshold; 3: isosurface)
\endcode

Exactly the same procedure as described above can be used again.
Start by placing your two seeds:

\image html seed4.gif " " 
\image latex seed4.eps " " width=0.75\textwidth

This yields the following deformable model:

\image html deformable_model2.gif " " 
\image latex deformable_model2.eps " " width=0.75\textwidth

Deform it as before, until you reach the final model:

\image html curvature1x05.gif " " 
\image latex curvature1x05.eps " " width=0.75\textwidth

Then when you merge the branches, we obtain this:

\image html merge_branches.gif " " 
\image latex merge_branches.eps " " width=0.75\textwidth

Continue until you have segmented all branches. In our case, we have three
branches (we are extracting the surface model of an iliac bifurcation). The
final result is:

\image html merge_branches2.gif " " 
\image latex merge_branches2.eps " " width=0.75\textwidth


Now you have a file named \c level_sets.vti which contains an image. 
The zero level of this image is the surface you generated. Now we  
extract a polygonal surface from it with :
\code
 vmtkmarchingcubes -ifile level_sets.vti -ofile model.vtp
\endcode

The final surface model is \c model.vtp which you can display with :
\code
vmtksurfacereader -ifile model.vtp --pipe vmtksurfaceviewer
\endcode

\image html surfaceviewer1.gif " " 
\image latex surfaceviewer1.eps " " width=0.75\textwidth

\image html surfaceviewer2.gif " " 
\image latex surfaceviewer2.eps " " width=0.75\textwidth


We note that you can add further branches to the
\c level_sets.vti file by using the command

\code
 vmtklevelsetsegmentation -ifile image_volume_voi.vti -levelsetsfile \
level_sets.vti -ofile level_sets2.vti
\endcode

<hr>
<hr>

\subsection smooth Smoothing the surface


At this point, we have our surface model and we want to generate a 
computational mesh. In most cases, the surface model has bumpy
surfaces, especially if the image quality is not high and if we didn't
use the curvature term in the level set evolution. Artificial
bumps in the surface can result in spurious flow features and will affect
the wall shear stress distribution, so one may want to increase surface
smoothness before building the mesh.

You can do this in VMTK with :
\code
 vmtksurfacesmoothing -ifile model.vtp -passband 0.1 -iterations 30 \
-ofile model_sm.vtp
\endcode

There are two parameters controlling the amount of smoothing:
passband, which is the cut-off spatial frequency of the low pass
filter, and iterations, which is the number of smoothing passes. 

For typical vessels, a passband of 0.1 and a number of iteration of 30
should be OK. For example, for the surface model obtained in the
section \ref surface above, the resulting surface is:

\image html smooth0130.gif " " 
\image latex smooth0130.eps " " width=0.75\textwidth

Remember that we can display the surfaces with :
\code
vmtksurfacereader -ifile model_sm.vtp --pipe vmtksurfaceviewer
\endcode

Smoothing with a passband of 0.01 turns this into:

\image html smooth00130.gif " " 
\image latex smooth00130.eps " " width=0.75\textwidth

If you want more smoothing, you can increase the passband and/or the
number of iterations, but be careful not to remove all surface features 
by smoothing too much. Also, watch the apex of bifurcations since
its curvature may decrease resulting in a shallower apex, affecting 
the simulated haemodynamics. For example, with a passband of \c 0.001 and
a number of iterations of \c 100, the result is:

\image html smooth0001100.gif " " 
\image latex smooth0001100.eps " " width=0.75\textwidth

<hr>
<hr>

\subsection clip Clipping the endcaps

Once we have created our smooth model, the next step is to open the
inlets and outlets, as they are normally closed with a blobby
appearance. We proceed by clipping the blobby endcaps with:

\code
vmtksurfaceclipper -ifile model_sm.vtp -ofile model_cl.vtp
\endcode

When the render window pops up, pressing \c i will activate the
"interactor". A cube will appear (as in \c vmtkimagevoiselector in the
section \ref voi). Position the cube in such a way that the portion 
of the surface you want to clip lies inside the cube.

\image html clip0.gif " " 
\image latex clip0.eps " " width=0.75\textwidth

Press the space bar to proceed with clipping.

\image html clip1.gif " " 
\image latex clip1.eps " " width=0.75\textwidth

Press i again if you want to clip another piece, or q if you want to
quit.

The final result should look like this:
 

\image html clip2.gif " " 
\image latex clip2.eps " " width=0.75\textwidth

Note that it is possible to clip the endcaps automatically with
VMTK. For more information, please refer to VMTK's own
<a href="http://www.vmtk.org">tutorial pages</a>.


<hr>
<hr>

\subsection add_extensions Adding flow extensions

VMTK provides the option to generate cylindrical extensions to the
inlet and outlet cross-sections. These are typically used to
ensure that in the numerical simulation, the boundary conditions 
are applied at a reasonable distance from the region of interest
and do not have a strong effect on the flow field. Exactly what sort
of boundary conditions to apply and where to apply them is, of course,
a non-trivial problem. Nonetheless, the appropriate
VMTK command to add the cylindrical extensions is:

\code
vmtksurfacereader -ifile model_cl.vtp --pipe vmtkcenterlines \
-seedselector openprofiles --pipe vmtkflowextensions -adaptivelength 1 \
-extensionratio 8 -normalestimationratio 1 -interactive 0 --pipe \
vmtksurfacewriter -ofile model_ex.vtp
\endcode

where the control parameters are:

- \c adaptivelength (here set to 1) is a boolean flag. If set
  to 1, the length of each flow extension is made proportional to the mean 
  radius of the vessel

- \c extensionratio (here set to 8) is the proportionality factor of
the length of each flow extension. 

- \c normalestimationratio: We suggest setting this parameter to 1. For further
  information, please refer to VMTK's 
  <a href="http://www.vmtk.org">tutorial page</a>.

- The flag \c -interactive was set to \c 0: This means that
  \c vmtkflowextensions will not prompt the user about what inlet or outlet 
  to extend, but it will perform the task on all the available open
  boundaries. If you use  \c -interactive \c 1, VMTK prompts the user 
  to specify which boundaries to extend through a graphical window.

When the render window pops up, before pressing \c q, make sure you note
the different ids of the inlets and outlets. For our example, we see
in the figure below that we have one inlet with an id 0 and two
outlets with the ids 1 and 2.

\image html extension1.gif " " 
\image latex extension1.eps " " width=0.75\textwidth


A message will appear on your terminal:
\code
Please input list of inlet profile ids:
\endcode

Enter the list of ids with a space between them. In our example enter
\c 0 . Next you will receive:
\code
Please input list of outlet profile ids:
\endcode

Enter the list of ids with a space separating them. In our
example enter:
\code
1 2
\endcode

You can display the resulting surface using:
\code
vmtksurfacereader -ifile model_ex.vtp --pipe vmtksurfaceviewer
\endcode

The result is:

\image html extenstion_display.gif " " 
\image latex extenstion_display.eps " " width=0.75\textwidth


<hr>
<hr>

\subsection vmtk_mesh The mesh generation

Finally, we can generate our mesh using the command line:
\code
vmtkmeshgenerator -ifile model_ex.vtp -ofile mesh.vtu -edgelength 0.5
\endcode

where the edgelength parameter expresses the nominal edge length of a 
surface triangle, in physical units. Note that the surface remeshing
may fail if you specify a large edge length.


<hr>
<hr>
\section  oomph_mesh Creating an oomph-lib mesh based on output files generated by VMTK

VMTK exports the mesh for different solvers. The one we chose is the
".xda" format (of <a href="http://libmesh.sourceforge.net/">LibMesh</a>). 
To export the mesh in this format enter:

\code
 vmtkmeshwriter -ifile mesh.vtu -entityidsarray CellEntityIds -ofile \
iliac.xda
\endcode

\c oomph-lib provides a conversion code 
\code 
bin/create_fluid_and_solid_surface_mesh_from_fluid_xda_mesh
\endcode

which uses the VMTK output mesh in "*.xda" format to generate a file 
"fluid_*.poly" that can be used to generate the fluid mesh 
using the open-source mesh generator 
<a href="http://wias-berlin.de/software/tetgen//">TetGen</a>.

When executing the conversion code you will be prompted with:
\code 
Please enter the file name without the file extension '.xda':
\endcode

In our example, enter \c iliac .

To facilitate the simulation of physiological fluid-structure 
interaction problems in which the vessel walls deform elastically 
in response to the fluid traction, the conversion 
code also generates a file "solid_*.poly" that can be used to 
to generate a mesh for the 3D vessel wall which is assumed to be of
constant thickness.

The conversion code therefore requests the wall thickness:

\code 
Enter the (uniform) wall thickness
\endcode


As explained in 
<a href="../../../interaction/unstructured_three_d_fsi/html/index.html">another
tutorial</a>, the consistent generation of surface coordinates in FSI problems
requires that all faces with the same boundary ID should be co-planar. 
For this reason the conversion code provides the option to assign a distinct
boundary ID to each surface triangle on the fluid-structure
interaction boundary and ensures the IDs are consistent for the fluid and the
solid meshes. (The "*.poly" files generated by the conversion code list
the relation between the old and boundary IDs at the end.)

\code 
Do you want to create a separate ID for each planar facet
on the fluid-structure interaction boundary?
Enter y or n: 
\endcode

For single-physics problem, however, it is much easier to assign a
single boundary ID to each "physical" surface (<em> i.e. </em> 
inlet, outlet, vessel boundary). Assuming that we are solving such a
single-physics problem we enter \c n. 
Two files are now generated : \c fluid_iliac.poly 
which contains the fluid domain and \c solid_iliac.poly containing the 
solid one.

<a href="http://wias-berlin.de/software/tetgen//tetview.html">Tetgen's</a> 
mesh viewer \c tetview can now be used to 
display the surface meshes: 
\code 
tetview fluid_iliac.poly
\endcode  
displays the fluid surface mesh with its four boundaries 

\image html fluid1.gif " " 
\image latex fluid1.eps " " width=0.75\textwidth
\image html fluid2.gif " " 
\image latex fluid2.eps " " width=0.75\textwidth

While
\code 
tetview fluid_iliac.poly
\endcode  
displays the solid surface mesh (for a wall thickness of 2mm):

\image html solid1.gif " " 
\image latex solid1.eps " " width=0.75\textwidth
\image html solid2.gif " " 
\image latex solid2.eps " " width=0.75\textwidth

<hr>

If multiple boundary IDs are requested, the surface meshes
are:

\image html fluid_mult_ids.gif "The fluid surface with multiple boundary IDs " 
\image latex fluid_mult_ids.eps "The fluid surface with multiple boundary IDs " width=0.75\textwidth

\image html solid_mult_ids.gif "The solid surface with multiple boundary IDs for a wall thickness of 2mm " 
\image latex solid_mult_ids.eps "The solid surface with multiple boundary IDs for a wall thickness of 2mm " width=0.75\textwidth


<HR>
<HR>


\section comm_and_ex Comments and Exercises

- In its current form, the conversion code 
   <CENTER>
  <a href="../../../../demo_drivers/meshing/mesh_from_vmtk/create_fluid_and_solid_surface_mesh_from_fluid_xda_mesh.cc">demo_drivers/interaction/create_fluid_and_solid_surface_mesh_from_fluid_xda_mesh.cc</a>
  </CENTER>
  is restricted to bifurcation-like geometries, and makes certain 
  assumptions about the enumeration of the boundaries. 
  Furthermore, it can only deal with constant thickness vessel
  walls: the outer surface of the blood vessel is assumed to be
  parallel to the FSI interface, with the wall thickness being
  measured in the direction normal to that interface. This simplistic
  construction can easily create overlapping elements if the 
  apex of the bifurcation is highly curved and/or if a large
  wall thickness is requested. If this happens tetgen will be
  unable to generate the solid mesh. It should be easy to modify the
  code to deal with such cases, however. 
  <a href="../../../contact/html/index.html">Get in touch</a> if you 
  need help with this (and feel free to share any improvements
  with us).
- We provide three separate tutorials to demonstrate the use of
  VMTK-based meshes in \c oomph-lib driver codes:
  - <a href="../../../solid/vmtk_solid/html/index.html">The inflation
    of the arterial bifurcation by a constant pressure -- a pure solid
    mechanics problem.</a>
  - <a href="../../../navier_stokes/vmtk_fluid/html/index.html">Steady
    finite-Reynolds number flow through the (rigid) arterial bifurcation --
    a pure fluid-mechanics mechanics problem.</a>
  - <a href="../../../interaction/vmtk_fsi/html/index.html">Steady 
    finite-Reynolds-number flow through an elastic
    arterial bifurcation -- a fluid-structure interaction
    problem.</a>
  .
- To facilitate experimentation with the methodology we distribute the  
  \c xda file that was used to create the meshes in the above
  examples. It is available at:
  <center>
  <a href="../../../../demo_drivers/interaction/vmtk_fsi/iliac.xda">
  demo_drivers/interaction/vmtk_fsi/iliac.xda</a>
  </center>
.

<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

