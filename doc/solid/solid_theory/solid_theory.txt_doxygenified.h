/**

\mainpage Solid mechanics: Theory and implementation

This document provides the theoretical background to \c oomph-lib's 
solid mechanics capabilities. We start with a review of the relevant
theory to establish the overall framework and notation, and then
discuss the implementation of the methodology in \c oomph-lib. 

Here is an overview of the structure of this document:
- \ref theory
  - \ref solids_lagrange
  - \ref geometry
  - \ref equilibrium
  - \ref elastic_constitutive
  - \ref non-dim_solid
  - \ref two_d_solid
  - \ref isotropic_growth
  - \ref CartesianLagrangian
  .
- \ref solid_implementation
  - \ref solid_node
  - \ref solid_fe
  - \ref solid_mesh
  - \ref solid_traction
  .
- \ref Solid_IC
.
If you're not keen on theory, you may prefer to start by exploring the
solid mechanics tutorials in \c oomph-lib's 
<A HREF="../../../example_code_list/html/index.html#solid">list of example 
driver codes.</A>


<HR>
<HR>

\section theory Theory

<HR>

\subsection solids_lagrange Solid mechanics problems -- Lagrangian coordinates
All problems discussed so far were formulated in an Eulerian frame,
i.e. we assumed all unknowns to be functions of the spatially-fixed,
Eulerian coordinates, \f$x_i \ (i=1,...,3)\f$, and of time, 
\f$ t \f$. Many problems 
in solid mechanics are formulated more conveniently in terms of 
body-attached, Lagrangian coordinates. In this section we will
briefly review the essential concepts of nonlinear continuum mechanics
and present the principle of virtual displacements which forms the
basis for all large-displacement solid mechanics computations in \c oomph-lib.

Throughout this section we will use the summation convention 
that repeated indices are to be summed over the range
of the three spatial coordinates and all free indices range 
from 1 to 3. We will retain the summation signs
for all other sums, such as sums over the nodes etc.

<HR>


\subsection geometry The geometry
The figure below introduces the essential geometrical concepts: 
We employ a set of Lagrangian coordinates, \f$ \xi^i\f$, to
parametrise the (Eulerian) position vector to material points in the
 body's undeformed position:
\f[
{\bf r} = {\bf r}(\xi^i).
\f]
The specific choice of the Lagrangian coordinates is 
irrelevant for the subsequent development. For analytical studies, it is
advantageous to choose a body-fitted Lagrangian coordinate
system (as shown in the sketch) because this allows boundary
conditions to be applied on iso-levels of the coordinates;
in computational studies, it is usually preferable to use a 
coordinate system in which the governing equations are as compact
as possible. A Cartesian coordinate system is best suited for this 
purpose.

We denote the tangent vectors to the coordinate lines \f$ \xi^i = const.\f$
in the undeformed configuration by
\f[
{\bf g}_i = \frac{\partial {\bf r}}{\partial \xi^i},
\f]
and define the components of the covariant metric tensor via 
the inner products
\f[
g_{ij} = {\bf g}_i \cdot {\bf g}_j.
\f]
This tensor defines the "metric" because the (square of the) infinitesimal 
length, \f$ ds \f$, of a line element with coordinate increments 
\f$ d\xi^i\f$ is given by
\f[
(ds)^2 = g_{ij} \  d\xi^i d\xi^j.
\f]
The volume of the infinitesimal parallelepiped formed by
the coordinate increments  \f$ d\xi^i\f$ is given by
\f[
dv = \sqrt{g} \ d\xi^1 d\xi^2 d\xi^3,
\f]
where 
\f[
g = \det g_{ij}. 
\f]
\image html Lagrangian_coord.gif "2D sketch of the Lagrangian coordinates. The Lagrangian coordinates parametrise the position vector to material points in the body. As the body deforms, the Lagrangian coordinates remain attached to the same material points and the coordinate lines become distorted. The change in the length of infinitesimal material line elements provides an objective measure of the body's deformation. " 
\image latex Lagrangian_coord.eps "2D sketch of the Lagrangian coordinates. The Lagrangian coordinates parametrise the position vector to material points in the body. As the body deforms, the Lagrangian coordinates remain attached to the same material points and the coordinate lines become distorted. The change in the length of infinitesimal material line elements provides an objective measure of the body's deformation. " width=0.75\textwidth

As the body deforms, the Lagrangian coordinates remain "attached" to the
same material points. The body's deformation can therefore be
described by the vector field that specifies the position vectors 
to material particles in the deformed configuration,
\f[
{\bf R} = {\bf R}(\xi^i).
\f]
As in the undeformed coordinate system, we  form the 
tangent vectors to the deformed coordinate lines \f$ \xi^i = const.\f$
and denote them by
\f[
{\bf G}_i = \frac{\partial {\bf R}}{\partial \xi^i}.
\f]
The inner product of these vectors defines the metric tensor in 
the deformed configuration
\f[
G_{ij} = {\bf G}_i \cdot {\bf G}_j
        = \frac{\partial {\bf R}}{\partial \xi^i} \cdot  
          \frac{\partial {\bf R}}{\partial \xi^j},
\f]
and we have equivalent relations for the lengths of line elements,
\f[
(dS)^2 = G_{ij} \  d\xi^i d\xi^j,
\f]
and the volume of infinitesimal parallelepipeds,
\f[
dV = \sqrt{G} \ d\xi^1 d\xi^2 d\xi^3,
\f]
where
\f[
G = \det G_{ij}. 
\f] 
Since the metric tensors \f$ G_{ij }\f$ and \f$ g_{ij }\f$ provide 
a measure of the length of 
material line elements in the deformed and undeformed configurations,
respectively, their difference
\f[
\gamma_{ij} =  \frac{1}{2} (G_{ij} - g_{ij}),  
\f]
provides an objective measure of the strain and is known as 
the Green strain tensor. 

<HR>

\subsection equilibrium Equilibrium and the Principle of Virtual Displacements

Let us now assume that the body is subjected to
- an applied surface traction  \f$ {\bf T}   \f$ --
  a force per unit deformed surface area, applied on (part of) the
  body's deformed surface area \f$ A_{tract} \subset \partial V\f$,
- a body force \f$ {\bf f} \f$ --
  a force per unit volume of the undeformed (!) body [This can
  easily be expressed in terms of a force per unit deformed volume, 
 \f$ {\bf F }\f$, by invoking conservation of mass, which shows that
 \f$ {\bf f } = \sqrt{G/g} \ {\bf F }\f$.],
. 
and its displacements are prescribed on the remaining part of
the boundary, \f$ A_{displ}  \subset \partial V \f$ (where 
\f$ A_{tract} \cap A_{displ}=0 \f$ and  \f$A_{tract} \cup A_{displ}
= \partial V\f$ ), <EM> i.e. </EM>
\f[
{\bf R}(\xi^k) = {\bf R}^{(BC)}(\xi^k) 
\mbox{ \ \ \ \ on \ \ \ $ A_{displ},$ }
  \ \ \ \ \ \ \ \ (1)
\f]
for given \f$ {\bf R}^{(BC)}(\xi^k) \f$.

\image html Solid_boundary_conds.gif "Sketch of the boundary conditions: The surface of the body is either subject to a prescribed traction or its displacement is prescribed. " 
\image latex Solid_boundary_conds.eps "Sketch of the boundary conditions: The surface of the body is either subject to a prescribed traction or its displacement is prescribed. " width=0.75\textwidth

The deformation is governed by the principle of virtual displacements
\f[
\int \left\{ \sigma^{ij}  \ \delta \gamma_{ij} -  \left( {\bf f} - 
 \rho \frac{\partial^2 {\bf R}}{\partial t^2} \right) \cdot 
 \delta {\bf R} \right\} \ dv -
  \oint_{A_{tract}} {\bf T} \cdot \delta {\bf R} \ dA =0,
  \ \ \ \ \ \ \ \ (2)
 \f]
 where \f$ \sigma^{ij} = \sigma^{ji}\f$ is the (symmetric) 
 second Piola-Kirchhoff stress tensor, \f$ \rho \f$ is the density of
 the undeformed body, and
 \f$ \delta (.) \f$ represents the variation of \f$ (.)\f$.
 See, e.g., Green, A.E. \& Zerna, W. "Theoretical Elasticity", 
 Dover (1992); or Wempner, G. "Mechanics of Solids with Applications 
 to Thin Bodies", Kluwer (1982) for more details.

 Upon choosing the particles' position vector in the deformed 
 configuration, \f${\bf R}(\xi^j)\f$, as the unknown, the variation 
 of the strain tensor becomes
 \f[
 \delta \gamma_{ij} = \frac{1}{2} \left( 
         \frac{\partial {\bf R}}{\partial \xi^i} \cdot  
         \delta \ \frac{\partial {\bf R}}{\partial \xi^j} +
         \frac{\partial {\bf R}}{\partial \xi^j} \cdot  
         \delta \ \frac{\partial {\bf R}}{\partial \xi^i}
       \right),
 \f]
 and we have
 \f[
  \delta {\bf R}  = {\bf 0} \mbox{ \ \ \ \ on \ \ \ $ A_{displ}.$ }
  \ \ \ \ \ \ \ \ (3)
 \f]
 The 2nd Piola Kirchhoff stress tensor is symmetric, therefore
 we can write the variation of the strain energy in
 (2) as
 \f[
 \int  \sigma^{ij}  \ \delta \gamma_{ij} \ dv = 
 \int \sigma^{ij}  \ \frac{\partial {\bf R}}{\partial \xi^i} \cdot  
          \delta \ \frac{\partial {\bf R}}{\partial \xi^j} \ dv
  \f]
 and obtain the displacement form of the principle of 
 virtual displacements:
 \f[
 \int \left\{ \sigma^{ij} \ \frac{\partial {\bf R}}{\partial \xi^i} \cdot  
          \delta \ \frac{\partial {\bf R}}{\partial \xi^j} 
      - \left( {\bf f} - 
  \rho \frac{\partial^2 {\bf R}}{\partial t^2} \right) \cdot 
 \delta {\bf R} \right\} \ dv 
 - \oint_{A_{tract}} {\bf T} \cdot \delta {\bf R} \ dA =0.
 \f]
This must be augmented by a constitutive equation which
determines the stress as a function of the body's deformation,
(and possibly the history of its deformation). Here we will only
consider elastic behaviour, where the stress is only a function of the
strain.

<HR>

\subsection elastic_constitutive Constitutive Equations for Purely Elastic Behaviour
For purely
elastic behaviour, the stress is only a function of the
instantaneous, local strain and the constitutive equation
has the form
\f[
\sigma^{ij} =  \sigma^{ij} (\gamma_{kl}).
\f]

The functional form of the constitutive equation is different
for compressible/incompressible/near-incompressible behaviour:
-# \b Compressible \b Behaviour: \n If the material is compressible,
   the stress can be computed directly from the deformed and undeformed
   metric tensors,
   \f[
  \sigma^{ij} = \sigma^{ij}(\gamma_{kl}) =  
  \sigma^{ij}\bigg( \frac{1}{2} (G_{kl} - g_{kl})\bigg).
  \f]
-# \b Incompressible \b Behaviour: \n If the material is incompressible,
   its deformation is constrained by the condition that
   \f[
   \det G_{ij} - \det g_{ij}= 0, 
   \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ (4) 
   \f]
   which ensures that the volume of infinitesimal material
   elements remains constant during the deformation. This
   condition is typically enforced by a Lagrange multiplier which
   plays the role of a pressure. In such cases, the 
   stress tensor has the form
   \f[
   \sigma^{ij} = -p G^{ij} + 
   \overline{\sigma}^{ij}\big(\gamma_{kl}\big),
   \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ (5)  
   \f]
   where only the deviatoric part of the stress tensor, 
   \f$ \overline{\sigma}^{ij}, \f$ depends directly on the
   strain. The pressure \f$ p \f$ must be determined
   independently by enforcing the incompressibility constraint
   (4).
   Given the deformed and undeformed metric tensors,
   the computation of the stress tensor \f$ \sigma^{ij} \f$ 
   for an incompressible
   material therefore requires the computation of the following quantities:
   - The deviatoric stress \f$ \overline{\sigma}^{ij} \f$
   - The contravariant deformed metric tensor  \f$ G^{ij} \f$
   - The determinant of the deformed 
     metric tensor \f$ \det G_{ij} \f$ which 
     is required in equation (4)
     whose solution determines the pressure.
   .
-# \b Nearly \b Incompressible \b Behaviour: \n If the material is nearly
   incompressible, it is advantageous to split the stress into
   its deviatoric and hydrostatic parts by writing the
   constitutive law in the form
   \f[
   \sigma^{ij} = -p G^{ij} + 
   \overline{\sigma}^{ij}\big(\gamma_{kl}\big),
   \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ (6)
   \f]
   where the deviatoric part of the stress tensor, 
   \f$ \overline{\sigma}^{ij}, \f$ depends on the
   strain. This form of the constitutive
   law is identical to that of the incompressible
   case and it involves a pressure \f$ p \f$ which must be
   determined from an additional equation. In the 
   incompressible case, this equation was given by the incompressibility
   constraint (4). Here, we must augment 
   the constitutive law for the deviatoric stress by
   an additional equation for the pressure. Generally this takes the
   form
   \f[
   p = - \kappa \ d,
   \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ (7)
   \f]
   where \f$ \kappa \f$ is the "bulk modulus", a material property
   that must be specified by the constitutive law.   
   \f$ d \f$ is the (generalised) dilatation, i.e. the relative change
   in the volume of an infinitesimal material element (or some
   suitable generalised quantity that is related to it). As the 
   material approaches incompressibility, \f$ \kappa \to \infty\f$, so that
   infinitely large pressures would be required to achieve
   any change in volume. To facilitate the implementation of
   (7) as the equation for the pressure, we re-write 
   it in the form
   \f[
   p \ \frac{1}{\kappa} + d\big(g_{ij},G_{ij}\big) = 0,
   \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ (8) 
   \f]
   which only involves quantities that remain finite 
   as we approach true incompressibility.
   \n 
   Given the deformed and undeformed metric tensors,
   the computation of the stress tensor \f$ \sigma^{ij} \f$ 
   for a nearly incompressible
   material therefore requires the computation of the following quantities:
   - The deviatoric stress \f$ \overline{\sigma}^{ij} \f$
   - The contravariant deformed metric tensor  \f$ G^{ij} \f$
   - The generalised dilatation \f$ d \f$
   - The inverse of the bulk modulus \f$ \kappa \f$
   .

The abstract base class \c ConstitutiveLaw provides
interfaces for the computation of the stress in all three forms.

\subsubsection strain-energy Strain-energy functions

A hyperelastic material is one for which the stress can be derived
from a potential function \f$ W(\gamma_{ij})\f$,
known as the strain-energy function, and
\f[
 \sigma^{ij} = \frac{\partial W}{\partial
 \gamma_{ij}}.
\f]
 A strain-energy function exists if the  elastic deformations are 
reversible and isothermal, or reversible and isentropic. If the
material is homogeneous and isotropic then the strain-energy function
depends only on the three strain invariants:
\f[
 I_{1} = g^{ij} G_{ij},\quad I_{2} = G^{ij} g_{ij} I_{3},\quad
 I_{3} = G/g
\f]
and can be written \f$W(I_{1},I_{2},I_{3})\f$. It may be shown, see
Green \& Zerna,  that
\f[ \sigma^{ij} = \Phi g^{ij} + \Psi B^{ij} + p G^{ij},\f]
where 
\f[ \Phi = 2\, \frac{\partial W}{\partial
I_{1}},\quad
 \Psi = 2\, \frac{\partial W}{\partial I_{2}},\quad
 p = 2 I_{3} \frac{\partial W}{\partial I_{3}} \quad\mbox{and}\quad
B^{ij} = I_{1} g^{ij} - g^{ir}g^{js}G_{rs}
\f]

 The abstract base class \c StrainEnergyFunction provides the interfaces \f$
 W(\gamma_{ij}) \f$ and \f$ W(I_{1},I_{2},I_{3}) \f$ and should be
 used as the base class for all strain-energy functions.
 A class \c
 StrainEnergyFunctionConstitutiveLaw that inherits from \c
 ConstitutiveLaw uses a specified  strain-energy function 
 to compute the appropriate stresses. 

<HR>

\subsection non-dim_solid Non-dimensionalisation
The principle of virtual displacements (2)
is written in dimensional form.
We generally prefer to work with non-dimensional quantities and
will now discuss the non-dimensionalisation used in the
actual implementation of the equations in \c oomph-lib. For this purpose we 
first re-write equation (2) as
\f[
\int_{v} \left\{ \sigma^{*ij} \ \delta \gamma_{ij} -  \left( {\bf f}^* - 
 \rho \frac{\partial^2 {\bf R}^*}{\partial t^{*2}} \right) \cdot 
 \delta {\bf R}^* \right\} \ dv^* 
 - \oint_{A_{tract}} {\bf T}^* \cdot \delta {\bf R}^* \ dA^* =0,
  \ \ \ \ \ \ \ \ (9)
 \f]
where we have used asterisks to label those
dimensional quantities that will have non-dimensional equivalents.
(Some quantities, such as the strain, are already
dimensionless, while others, such as the density will not have
any non-dimensional counterparts. We do not introduce modifiers
for these).

We now non-dimensionalise all lengths with a problem-specific
length-scale, \f$ {\cal L}\f$, given e.g. the length of the solid
body, so that
\f[
\xi^{*i} = {\cal L} \ \xi^{i}, \ \ \  {\bf R}^* = {\cal L} \ {\bf R}, 
\ \ \ dA^* = {\cal L}^2 \ dA, \mbox{ \ \ and \ \ } 
dv^* = {\cal L}^3 \ dv.\ \ \ 
\f]
We use a characteristic stiffness, \f$ {\cal S}\f$, (e.g. the
material's Young's modulus \f$ E \f$) to non-dimensionalise
the stress and the loads as
\f[
\sigma^{*ij} = {\cal S} \ \sigma^{ij}, \ \ \  {\bf T}^* = {\cal S}\
{\bf T}, \mbox{ \ \ and \ \ } {\bf f}^* = {\cal S} / {\cal L} \
{\bf f},
\f]
and we non-dimensionalise time with a problem-specific
timescale \f$ {\cal T}\f$ (e.g. the period of some external forcing),
so that
\f[
t^* = {\cal T} t.
\f]
This transforms (9) into
\f[
\int_{v} \left\{ \sigma^{ij} \ \delta \gamma_{ij} - \left( {\bf f} - 
 \Lambda^2 \frac{\partial^2 {\bf R}}{\partial t^{2}} \right) \cdot 
 \delta {\bf R} \right\} \ dv 
 -\oint_{A_{tract}} {\bf T} \cdot \delta {\bf R} \ dA =0,
  \ \ \ \ \ \ \ \ (10)
 \f]
 where 
\f[
\Lambda = \frac{{\cal L}}{\cal T} \sqrt{\frac{\rho}{\cal S}}
\f]
is the ratio of system's "intrinsic" timescale, 
\f$T_{intrinsic} = {\cal L} \sqrt{\rho /{\cal S}},\f$ to the
time, \f${\cal T}\f$, used in the non-dimensionalisation of the
equations. If a given problem has no externally imposed timescale
(e.g. in the free vibrations of an elastic body) \f$T_{intrinsic}\f$
(or some suitable problem-dependent multiple thereof) provides a natural 
timescale for the non-dimensionalisation. Therefore we use 
\f$ \Lambda=1 \f$ as the default value in all solid mechanics
equations. If preferred, computations can, of course, be
performed with dimensional units, provided all quantities
are expressed in consistent units (e.g. from the SI system).
In this case \f$ \Lambda^2 \f$ represents the dimensional
density of the material.

We adopt a similar approach for non-dimensionalisation of the 
constitutive equations. Typically, the constitutive parameters
(e.g. Young's modulus and Poisson's ratio for a Hookean
constitutive equation) are passed to the \c ConstitutiveLaw
as arguments to its constructor. Where possible, we select one of these
parameters as the reference stress \f$ {\cal S} \f$ and give it
a default value of 1.0. Hence, if a Hookean constitutive
law is instantiated with just one argument (the Poisson ratio \f$
\nu \f$), the stress is assumed to have been scaled on Young's
modulus. If two arguments are provided, the second argument
should be interpreted as the ratio of
the material's Young's modulus to the reference stress 
\f$ {\cal S} \f$ used in the non-dimensionalisation of the equations.

<HR>

\subsection two_d_solid 2D problems: Plane strain.
Many solid mechanics problems can be regarded as two-dimensional
in the sense that the quantities of interest only depend on two
spatial coordinates. In such problems it is important to consider what
constraints the system is subjected to in the third direction --- clearly
all real solid bodies are three-dimensional!
In plane strain problems, the displacements of material points are assumed
to be parallel to the 2D plane spanned by the coordinates \f$x_1
\f$ and \f$ x_2\f$, so that any displacements  
normal to this plane are suppressed. In this case, significant 
transverse stresses can develop.
Conversely, in plane stress problems, it is assumed that no stresses
develop in the transverse direction; in this case we must 
allow material particles to be displaced transversely.
Since we have formulated our problem in terms of positions 
(i.e. in terms of the displacement of material points), our formulation
naturally produces a <B>plane strain</B> problem if we reduce
the equations to two dimensions:
We assume that the transverse displacement vanishes, 
while the remaining (in-plane) displacements are only 
functions of the in-plane coordinates, \f$ x_1\f$ and \f$ x_2\f$.

It is important to remember that the 2D version of all equations must 
produce plane-strain behaviour when new, strain-energy-based 
constitutive equation classes are formulated;
when implementing such strain-energy functions, recall that any
invariants of metric tensors etc. are the invariants of the full
3D quantities which are not necessarily the same as those
of the corresponding 2D quantities.



<HR>

\subsection isotropic_growth Isotropic growth.
Many biological tissues undergo growth processes. For instance, the cells 
that make up a solid tumour divide regularly and as a result the
total mass of the tumour increases. If the growth 
occurs non-uniformly so that certain parts of the tumour grow faster
than others, regions that grow more slowly restrain the expansion
of their neighbours. This process can induce significant growth stresses.
The scenario is similar (but not identical) to that of thermal 
growth in which a non-uniform temperature distribution in 
a body creates thermal stresses. (The important difference between
these two cases is that in the latter process mass is conserved --
thermal expansion only leads to an increase in the volume occupied
by the material, whereas biological growth via cell division 
increases the mass of the tumour). 

It is easy to incorporate such growth processes into our 
theoretical framework. The general idea is sketched in the 
following figures:


<CENTER>
<center><b>State 0:</b></center>
<CENTER>
\image html Isotropic_growth_0.gif " " 
\image latex Isotropic_growth_0.eps " " width=0.35\textwidth
Undeformed, ungrown and stress-free reference configuration.
</CENTER>
</CENTER>



<TABLE>
<TR>
<TD>
<center><b>(Hypothetical) state U1:</b></center>

\image html Isotropic_growth_U1.gif " " 
\image latex Isotropic_growth_U1.eps " " width=0.35\textwidth

The individual infinitesimal material elements
have expanded (or contracted) isotropically and the elements are in 
a stress-free state. The isotropic growth
is spatially uniform -- all elements have expanded (or contracted) 
by the same amount.
</TD>
<TD>
<center><b>(Hypothetical) state N1:</b></center>

\image html Isotropic_growth_N1.gif " " 
\image latex Isotropic_growth_N1.eps " " width=0.35\textwidth

All infinitesimal material elements
have expanded (or contracted) isotropically and the elements are in 
a stress-free state. The isotropic growth
is spatially non-uniform, so individual elements
have grown (or contracted) by different amounts.
</TD>
</TR>
</TABLE>


<TABLE>
<TR>
<TD>
<center><b>State U2:</b></center>

\image html Isotropic_growth_U2.gif " " 
\image latex Isotropic_growth_U2.eps " " width=0.35\textwidth
Since the isotropically-grown infinitesimal
 material elements have grown (or contracted) by the same amount, they can
 be (re-)assembled to form a continuous, stress-free body. 
</TD>
<TD>
<center><b>State N2:</b></center>

\image html Isotropic_growth_N2.gif " " 
\image latex Isotropic_growth_N2.eps " " width=0.35\textwidth
Since the individual material elements have
grown (or contracted) by different amounts they can (in general) 
not be (re-)assembled to form a continuous body without undergoing 
some deformation. The deformation of the material elements (relative to their
stress-free shape in the hypothetical, stress-free state N1) 
induces internal stresses -- the so-called growth-stresses.
</TD>
</TR>
</TABLE>



<TABLE>
<TR>
<TD>
<center><b>State UE:</b></center>

\image html Isotropic_growth_UE.gif " " 
\image latex Isotropic_growth_UE.eps " " width=0.35\textwidth
When subjected to external tractions 
and to body forces, the uniformly-grown material elements deform. Their
deformation (relative to their stress-free shape in state
U1 (or, equivalently, U2), generates internal stresses which
- balance the applied loads,
- keep all infinitesimal material elements in local equilibrium.
</TD>
<TD>
<center><b>State NE:</b></center>

\image html Isotropic_growth_NE.gif " " 
\image latex Isotropic_growth_NE.eps " " width=0.35\textwidth
When subjected to external tractions 
and to body forces, the material elements deform further. Their
deformation (relative to their stress-free shape in state
N1), generates internal stresses which
- balance the applied loads,
- keep the material elements in local equilibrium.
</TD>
</TR>
</TABLE>


 
We start our analysis with the stress-free (and 
"ungrown") reference configuration "0" at the top of the diagram,
and initially follow the deformation shown in the left half
of the sketch. In a first step, each infinitesimal material
element in  the body is subjected to the same isotropic growth
which changes its mass from  \f$\mbox{d}M\f$ to
\f$ \Gamma \ \mbox{d}M \f$. Assuming that the growth process does not
change the density of the material, \f$ \Gamma\f$ also specifies
the volumetric growth of the material. All material
elements grow by the same amount, therefore the individual elements
can be (re-)assembled to form a continuous body (state
U2). In this state, the body is 
stress-free and, compared to the reference configuration "0",
all lengths have increased by a factor of \f$\Gamma^{1/d}\f$ 
where \f$ d \f$ is the body's spatial dimension (i.e. \f$d=2\f$ in 
the sketch above). The covariant basis vectors in this 
uniformly-grown, stress-free configuration are therefore given by
\f[
\widetilde{\bf g}_i = \Gamma^{1/d} \ {\bf g}_i,
\f] 
the metric tensor is given by
\f[
\widetilde{g_{ij}} = \Gamma^{2/d} \ g_{ij},
\f] 
and the volume of an infinitesimal material element
has increased from 
\f[
dv = \sqrt{g}  \ d\xi^1 ... \ d\xi^D
\f]
to 
\f[
\widetilde{dv} = \Gamma \sqrt{g} \ d\xi^1 ... \ d\xi^D 
\ \ \ \ \ \ \ \ \ \ \ \ (11)
\f]
We now subject the stress-free, uniformly-grown body to
external loads and determine its deformation, using the principle
of virtual displacements. Since the uniformly grown state "U2"
is stress-free, we may regard it as the reference state for
the subsequent deformation. The strain tensor that describes
the deformation from the stress-free (and uniformly-grown) state "U2"
to the final equilibrium configuration "UE" must therefore
be defined as
\f[
\gamma_{ij} = \frac{1}{2} \left(G_{ij} - \widetilde{g_{ij}}\right)
 = \frac{1}{2} \left(G_{ij} - \Gamma^{2/D} \ g_{ij} \right)
\ \ \ \ \ \ \ \ \ \ \ \ (12)
\f]
Equations (11) and (12) allow
us to express the principle of virtual displacements
in terms of
- the metric of the undeformed (and un-grown) reference
  state "0", 
- the volumetric growth \f$ \Gamma \f$, and
- quantities associated with the final deformed configuration
  "UE":
.
\f[
\int_{v} \left\{ \sigma^{ij} \ \delta \gamma_{ij} - \left( {\bf f} - 
 \Lambda^2 \frac{\partial^2 {\bf R}}{\partial t^{2}} \right) \cdot 
 \delta {\bf R} \right\} \ \Gamma \ dv 
 -\oint_{A_{tract}} {\bf T} \cdot \delta {\bf R} \ dA =0,
  \ \ \ \ \ \ \ \ (13)
\f]
Note that this equation does not contain any references to quantities
in the intermediate states "U1" and "U2". 

We will now consider the case of spatially non-uniform growth,
illustrated in the right half of the sketch. If the isotropic
growth is spatially non-uniform, \f$ \Gamma = \Gamma(\xi^i)\f$,
the growth will try to expand all infinitesimal material 
elements isotropically -- but each one by a different amount as
illustrated by the hypothetical state N1 in which each material
element has expanded to its stress-free state in which its
metric tensor is given by
\f[
\widetilde{g_{ij}} = \Gamma(\xi^k) \ g_{ij}.
\f] 
Material elements will only be stress-free if the strain 
\f[
\gamma_{ij} = \frac{1}{2} \left(G_{ij} - \widetilde{g_{ij}}\right)
 = \frac{1}{2} \left(G_{ij} - \Gamma^{2/D}(\xi^k) \ g_{ij} \right)
\ \ \ \ \ \ \ \ \ \ \ \ (14)
\f]
relative to their isotropically grown shape in state N1 is zero.
In general, the displacements induced by such an isotropic
expansion will be
incompatible and it would be impossible to (re-)assemble the 
individually grown material elements to a continuous body
unless the material elements undergo some deformation. 
The elements' deformation relative to their
stress-free shape in N1 will generate internal "growth-stresses"
(stage N2). When subjected to external loads and body forces
the body will undergo further deformations until the
the stress (generated by the particles' total deformation relative to
their stress-free state N1) balances the applied loads.

It is important to realise that, as in the case of spatially
uniform growth, the strain defined by (12) is 
an intrinsic quantity that provides a measure of each particles' 
\e local deformation relative to its stress-free shape in N1. 
The intermediate (and in the current case hypothetical), isotropically 
grown state N1 does not appear in the analysis -- it only serves
to define the stress-free shape for each infinitesimal
material element. Equation (13) therefore
remains valid.


<HR>

\subsection CartesianLagrangian Specialisation to a Cartesian basis and finite element discretisation
If the problem does not have any
symmetries (e.g. axisymmetry) whose exploitation would reduce
the spatial dimension of the problem, the most compact form
of the equations is obtained by resolving all vectors into
a fixed Cartesian basis so that the undeformed position vector
is given by
\f[
{\bf r}(\xi^j) = r_i(\xi^j) \ {\bf e}_i,
 \ \ \ \ \ \ \ \ (15)
\f]
where the  \f$ {\bf e}_i \f$ are the basis vectors in the direction of
the Cartesian Eulerian coordinate axes. 

Similarly, we write 
\f[
{\bf f} = f_i \ {\bf e}_i,
\f]
\f[
{\bf T} = T_i \ {\bf e}_i,
\f]
and 
\f[
{\bf R}^{(BC)} = R^{(BC)}_i \ {\bf e}_i.
\f]

We use the Eulerian coordinates in the undeformed 
configuration as the Lagrangian coordinates so that
\f[
r_i(\xi^j) = \xi^i. \ \ \ \ \ \ \ \ (16)
\f]
With this choice, the tangent vectors to the undeformed coordinate lines
are the Cartesian basis vectors
\f[
{\bf g}_i =  {\bf e}_i, 
\f]
and the undeformed metric tensor is the Kronecker delta
(the unit matrix)
\f[
g_{ij} = \delta_{ij}.
\f]
We now expand the (unknown) deformed position vector in the same basis,
\f[
{\bf R}(\xi^j) = R_i(\xi^j) \ {\bf e}_i,
\f]
and derive a finite element approximation for this vector field
from the principle of virtual displacements. For this purpose we decompose
the undeformed body into a number of finite elements, using the
standard mesh generation process described previously.
This establishes the Eulerian position \f$ X_{ij}^{(0)} \
(j=1,...,N) \f$ of the \f$ N \f$
nodes in the body's undeformed configuration.
Since the Eulerian coordinates of material points in the undeformed 
configuration coincide with their Lagrangian coordinates (see 
(16)), a finite-element representation of the 
Lagrangian coordinates is established by writing
\f[
\xi^i = \sum_{j=1}^{N} \Xi_{ij} \ \psi_j,
\f]
where \f$ \Xi_{ij} = X_{ij}^{(0)} \f$ is the \f$i\f$-th 
Lagrangian coordinate of global node \f$j\f$, and the \f$\psi_j\f$
are the global finite-element shape functions. In practice, the
\f$\psi_j\f$ are, of course, represented by local shape functions, 
\f$\psi_j(s_k)\f$, so that the Lagrangian coordinate at local 
coordinate \f$ s_k \f$ in element \f$ e \f$ is given by
\f[
\xi^i(s_k) = \sum_{j=1}^{n} \Xi_{i{\cal J}(j,e)} \ \psi_j(s_k)
\f]
where we use the same notation as in the 
<A HREF="../../../intro/html/index.html">Introduction to the
Finite Element Method</A> document.

We employ the same basis functions to represent the components of 
the unknown vector field \f$ {\bf R}(\xi^j)\f$, by writing
\f[
R_i(\xi^k) = \sum_{j=1}^{n} X_{ij} \ \psi_j(\xi^k),
\ \ \ \ \ \ \ \   (17)
\f]
and treat the (Eulerian) nodal positions \f$ X_{ij} \f$ as 
the unknowns. With this discretisation, the 
variations in  \f$ {\bf R}(\xi^j) \f$ correspond to variations
in the nodal positions \f$ X_{ij} \f$ so that
\f[
\delta {\bf R} =   \sum_{j=1}^{N} \delta X_{ij} \ \psi_j \ {\bf e}_i
\f]
\f[
\delta \frac{\partial {\bf R}}{\partial \xi^k} = 
\sum_{j=1}^{N} \delta X_{ij} \ 
\frac{\partial \psi_j}{\partial \xi^k} \ {\bf e}_i,
\f]
\f[
\mbox{ etc. }
\f]
The principle of virtual displacement  (13) 
therefore becomes
 \f[\sum_{m=1}^{N}
 \left\{
  \int \left[ \sigma^{ij} \ 
  \left(\sum_{l=1}^N X_{kl} \frac{\partial \psi_l}{\partial \xi^i} \right)
       \frac{\partial \psi_m}{\partial \xi^j} \
      - \left( f_k - 
  \Lambda^2 \left(\sum_{l=1}^N
    \frac{\partial^2 X_{kl}}{\partial t^2} \psi_l \right) \right) 
 \psi_m \right] \ \Gamma \ dv 
 - \oint_{A_{tract}} T_k \ \psi_m \ dA \right\} 
 \delta X_{km}  =0. \ \ \ \  (18)
 \f]
[Note that summation convention enforces the summation over the index 
\f$ k \f$.]
The displacement boundary condition (1)
determines the positions of all nodes that are located on the
boundary \f$A_{displ}\f$, 
\f[
 X_{ij} = R^{(BC)}_i(\Xi_{lj}) \mbox{ \ \ \ if node $j$ is 
 located on $A_{displ},$}
\f]
and equation (3) 
requires their variations to vanish,
\f[
 \delta X_{ij} = 0 \mbox{ \ \ \ if node $j$ is located on $A_{displ}.$}
\f]
The variations of all other nodal positions are arbitrary (and independent
of each other), therefore the terms in the curly brackets in 
(18) must vanish individually. This provides one 
(discrete) equation for each unknown \f$ X_{km} \f$ ,
 \f[ f_{km} = 
  \int \left[ \sigma^{ij} \  
  \left(\sum_{k=1}^N X_{kl} \frac{\partial \psi_l}{\partial \xi^i} \right)
       \frac{\partial \psi_m}{\partial \xi^j} \
      - \left( f_k - 
  \Lambda^2 \left(\sum_{k=1}^N
    \frac{\partial^2 X_{kl}}{\partial t^2} \psi_l \right) \right) 
 \psi_m \right] \ \Gamma \ dv \
 - \oint_{A_{tract}} T_k \ \psi_m \ dA 
  =0. \ \ \ \ \  \ \ \ \ (19)
 \f]
These equations can again be assembled in an element-by-element
fashion.



<HR>
<HR>

\section solid_implementation Implementation
We will now discuss how the discrete equations (19) are
implemented in \c oomph-lib. To facilitate the analysis
of multi-physics problems, we introduce generalisations of the \c Node, 
\c FiniteElement and \c Mesh classes which provide
separate storage (and access functions) for all solid mechanics
data. The resulting \c SolidFiniteElements can 
be used as stand-alone elements for the simulation of 
pure solid mechanics problems. More importantly, however, the design
makes it easy to employ multiple inheritance to create 
more complex elements that solve the equations of solid mechanics
together with any other field equations. For instance, if we combine
a \c FiniteElement that solves the unsteady heat equation
with a \c SolidFiniteElement that describes the elastic 
deformations, we obtain an element that can be used to simulate 
unsteady heat conduction in an elastic body that is subject to
large-amplitude deformations, say. This is illustrated in one of 
\c oomph-lib's
<A HREF="../../../multi_physics/thermo/html/index.html">multi-physics
example codes.</A>

<HR>

\subsection solid_node The SolidNode class

The \c SolidNode class is derived from the \c Node class and 
implements the additional functionality required for solid
mechanics problems. The key new feature is that each \c Node
must store its (fixed) Lagrangian coordinate \f$ \Xi_{ij}\f$,
while its Eulerian position  \f$ X_{ij}\f$ must be regarded
as an unknown. This requires the following changes to
member functions of the \c Node class:

- The function \c Node::x(...) returns the
  Eulerian coordinates of the \c Node. Internally, this function
  accesses the nodal coordinates via pointers to double
  precision numbers. In solid mechanics problems we must be able to 
  regard the nodal positions as unknowns. In \c SolidNodes
  the nodal positions are therefore stored as values of a 
  (member) \c Data object created during construction of the \c
  SolidNode. (As usual, the values can be either unknown or pinned, and 
  can have time histories). The function
  \code
  SolidNode::position_eqn_number(...)
  \endcode
  gives access to the global equation number for each
  nodal coordinate and, following our usual convention, the function
  returns the static integer \c Data::Is_pinned (which is set to -1) if a 
  coordinate is pinned.
- We introduce a new member function 
  \code
  SolidNode::xi(...)
  \endcode
  which returns the (fixed) Lagrangian coordinates of the node.
- The function has the usual extensions to generalised
  coordinates
  \code
  SolidNode::xi_gen(...)
  \endcode
  which is required for Hermite elements and any other elements
  that use generalised nodal coordinates to interpolate the
  element's geometry.
- Similar to the \c Data member function \c Data::pin(...) which can
  be used to pin specific nodal values, we provide the function
  \code
  SolidNode:pin_position(...)
  \endcode
  which allows pinning of selected nodal coordinates.
- Dynamic problems require the evaluation of time-derivatives
  of the nodal positions, such as \f$ \partial^2 X_{ij}/\partial t^2\f$,
  see (19). These time-derivatives are evaluated by the
  \c TimeStepper of the positional \c Data.  By default,
  we use the same \c TimeStepper for the nodal \c Data and for the nodal 
  positions. In multi-physics problems this may not be 
  appropriate, however. Consider, for instance, solving an unsteady 
  heat equation in a dynamically deforming, elastic body. In this problem
  the 2nd time-derivatives of the nodal position might be evaluated
  by a \c Newmark scheme, acting on the history values of the nodal
  positions, whereas the time-derivatives of the temperature
  might be determined by a \c BDF scheme, operating on the 
  history values of the nodal \c Data.
  In such cases, the default assignment for the two timesteppers 
  can be overwritten with the access functions
  \code
  Node::position_time_stepper_pt()
  \endcode
  and 
  \code
  Node::time_stepper_pt()
  \endcode
  where the latter is inherited from \c Data::time_stepper_pt().
- Our implementation is based on the displacement form of the
  principle of virtual displacements in which the position vector 
  \f$ {\bf R}(\xi^i) \f$ in the deformed configuration is regarded
  as the unknown vector field. Equation  (17)
  defines the representation of this vector
  field within each finite element in terms of the nodal coordinates.
  Some constitutive equations require the representation
  of additional (non-positional) variables.
  For instance, for incompressible (or nearly incompressible) materials,
  the stress \f$ \sigma^{ij} \f$ contains a contribution from the
  (scalar) pressure field \f$ p \f$; see the discussion of the
  constitutive equations above. If we choose a continuous
  representation for the pressure in which its value is
  interpolated between nodal values (as in Taylor-Hood-type
  elements), the nodal pressure values are stored
  in the "normal" nodal \c Data. Similarly, the elements'
  internal \c Data is used to store any discontinuous solid
  pressures. 
- Finally, \c SolidNodes overload the function 
  \c Data::assign_eqn_numbers()
  with 
  \code
  SolidNode::assign_eqn_numbers()
  \endcode
  which creates global equation numbers for all (non-pinned) 
  positional values, and then deals with the "normal" nodal \c Data by calling
  \c Data::assign_eqn_numbers().
.

<HR>

\subsection solid_fe The SolidFiniteElement class

The class \c SolidFiniteElement is derived from \c FiniteElement and 
implements the additional functionality required for solid
mechanics problems. Again, most of the additional (or revised)
functionality is related to the presence of the two coordinate
systems which requires the following changes to \c FiniteElement
member functions:
- The nodes of \c SolidFiniteElements are \c SolidNodes, therefore
  we overload the function \c FiniteElements::construct_node(...)
  to ensure that a \c SolidNode with the appropriate 
  amount of storage is built. As in the case of \c FiniteElements, 
  the required spatial dimension of the elements' constituent 
  nodes, their number of nodal values etc. are specified via
  the \c FiniteElement's (pure) virtual member functions 
  \c FiniteElement::required_ndim(...),
  \c FiniteElement::required_nvalue(...), etc, which must be
  implemented for all specific elements that are derived from
  the \c SolidFiniteElement base class.  As discussed above, the 
  constructor of the \c SolidNodes requires additional parameters, such
  as the number of Lagrangian coordinates. These must be specified 
  by implementing \c SolidFiniteElement::required_nlagrangian(...) 
  and similar other functions. As in the 
  case of \c FiniteElements, many of these functions are already
  implemented as virtual (rather than pure virtual) functions which provide
  sensible default values. Such functions must be overloaded in 
  specific derived elements if the default assignments are not 
  appropriate.
- The interpolation of the Eulerian coordinates, implemented
  in \c FiniteElement::interpolated_x(...), can remain unchanged
  because \c Node::x(...) always returns the Eulerian nodal positions.
  \c SolidFiniteElements provide additional functions, such as 
  \code
  SolidFiniteElement::interpolated_xi(...)
  \endcode
  which determines the interpolated Lagrangian coordinates at
  a specified local coordinate within the element, or
  \code
  SolidFiniteElement::raw_lagrangian_position(...)
  \endcode
  and 
  \code
  SolidFiniteElement::lagrangian_position(...)
  \endcode
  which provides a wrapper for the nodal values of the
  Lagrangian coordinates. (The "raw" version of the function returns 
  the Lagrangian coordinates stored at the \c SolidNode; the second
  version automatically computes the suitably constrained Lagrangian 
  coordinates if the \c SolidNode is hanging.)
- The displacement form of the principle of virtual displacements 
  (19) contains derivatives of the shape functions
  with respect to the Lagrangian (rather than the Eulerian)
  coordinates. Their computation is implemented in
  \code
  SolidFiniteElement::dshape_lagrangian(...)
  \endcode
- We have now created storage and access functions for the 
  \c Data that represents the nodal positions. We must ensure that
  these \c Data items are included in the various equation numbering
  schemes. For this purpose we provide the function
  \c SolidFiniteElement::assign_solid_local_eqn_numbers() 
  which sets up the local equation numbering scheme for all
  solid \c Data associated with an element. This function is
  called when the SolidFiniteElement's local equation numbers
  are generated.
- We're done! \c SolidFiniteElements now form a suitable basis for
  all elements whose deformation is determined
  by the equations of solid mechanics (or some variant thereof). 
  To implement a specific
  solid mechanics element, we must represent its geometry,
  its state of stress, etc., in terms of the \c
  SolidFiniteElement's positional and non-positional
  \c Data. This requires the specification of the shape functions
  and the functions that compute the element's Jacobian matrix
  and its residual vector -- the latter implementing the 
  element's contribution to the global residual vector defined by the 
  discretised principle of virtual displacements, (19).
  As for "normal" \c 
  FiniteElements it is sensible to construct specific \c
  SolidFiniteElements in a hierarchy which separates between
  the implementation of the governing equations and the 
  representation of the element geometry. For instance,
  the \c SolidQElement family represents the generalisation of the 
  \c QElement family to \c SolidFiniteElements, while \c
  PVDEquations implement the principle of virtual
  displacements (19). The two are combined
  by multiple inheritance to form the \c QPVDElement class.
- The computation of the
  element's Jacobian matrix requires the evaluation of
  the derivatives of the discrete residuals (19) with respect
  to the unknown nodal positions, and with respect to any
  additional unknown solid mechanics variables (e.g. pressures). The
  derivatives with respect to the nodal positions result in 
  fairly complex algebraic expressions and it is sometimes
  more efficient to evaluate these entries by finite
  differencing. By default, the derivatives are evaluated analytically
  using carefully optimised assembly loops, but
  a finite-difference-base evaluation can be activated instead;
  see the detailed description of the \c PVDEquations and the 
  \c PVDEquationsWithPressure classes.


<HR>

\subsection solid_mesh The SolidMesh class

The \c SolidMesh class is a generalisation of the \c Mesh class 
whose key additional features are:
- It overloads the \c Mesh::node_pt(...) function with
  \code
  SolidMesh::node_pt(...)
  \endcode
  which returns a pointer to an \c SolidNode, rather than a
  "normal" \c Node. Equivalent access functions are implemented for
  all other \c Mesh member functions that return pointers to
  \c Nodes.
- We provide the function
  \code
  SolidMesh::set_lagrangian_nodal_coordinates()
  \endcode
  which assigns the current Eulerian coordinates of all \c Nodes
  to their Lagrangian coordinates, thus turning the current
  configuration into the stress-free reference configuration. This function
  greatly facilitates the construction of \c SolidMeshes via
  inheritance from existing \c Meshes. If, for instance, \c SomeMesh
  is an existing, fully functional \c Mesh, the corresponding
  \c SolidMesh can be constructed with a few lines
  of code, as in this example:
  \code
  //=================================================================
  // SolidMesh version of SomeMesh
  //=================================================================
  template<class ELEMENT>
  class SomeSolidMesh : public virtual SomeMesh<ELEMENT>,
                          public virtual SolidMesh
   {
     public:

     // Constructor: Call the constructor to the underlying Mesh
     // then assign the Lagrangian coordinates -- for the
     // PARANOID user, check that the element specified
     // in the template argument is derived from an 
     // SolidFiniteElement
     SomeSolidMesh() : SomeMesh()
      {
  #ifdef PARANOID
     // Check that the element type is derived from
     // the SolidFiniteElement class
     SolidFiniteElement* el_pt=dynamic_cast<SolidFiniteElement*>
      (finite_element_pt(0));
     if (el_pt==0)
      {
       cout << "Element must be derived from SolidFiniteElement " << endl;
       abort();
      }
  #endif
  
     // Make the current configuration the undeformed one by
     // setting the nodal Lagrangian coordinates to their current
     // Eulerian ones
     set_lagrangian_nodal_coordinates();
    }
 };
\endcode

<HR>


\subsection solid_traction The SolidTractionElement class

To evaluate the load terms 
\f[
\oint_{A_{tract}} T_k \ \psi_m \ dA 
\f]
in the discretised form of the variational principle (19)
we employ the same strategy as for most other Neumann-type boundary 
conditions and attach so-called \c SolidTractionElements to the 
appropriate faces of higher-dimensional "bulk" solid mechanics elements.
Our default implementation allows the load (specified by the "user"
via a function pointer that is passed to the \c SolidTractionElements) 
to depend on the Eulerian and Lagrangian coordinates, and on the outer
unit normal to the solid. This interface should be sufficiently
general for most cases of interest. If additional dependencies
are required, it is easy to create new \c SolidTractionElements. 
The use of the  \c SolidTractionElements is demonstrated in several 
<A HREF="../../../example_code_list/html/index.html#solid">solid mechanics 
tutorials.</A>



<HR>
<HR>

\section Solid_IC Timestepping and the generation of initial conditions for solid mechanics problems
In time-dependent problems, the boundary value problem defined by
the variational principle (10) must be
augmented by suitable initial conditions which specify the
state of the system at time \f$ t=t_0. \f$ The initial
conditions specify the initial shape of the solid body,
\f[
{\bf R}(\xi^i,t=t_0) =  {\bf R}^{(IC)}(\xi^i),
\ \ \ \ \ \ \ \ \ \ \ \ \ (20)
\f]
and its initial velocity,
\f[
\left. \frac{\partial {\bf R}(\xi^i,t)}{\partial t} \right|_{t=t_0} = 
{\bf V}^{(IC)}(\xi^i),
\ \ \ \ \ \ \ \ \ \ \ \ (21)
\f]
where \f$ {\bf R}^{(IC)}(\xi^i) \f$ and \f$ {\bf V}^{(IC)}(\xi^i) \f$
are given. The accelerations \f$ \partial^2 {\bf R}/\partial t^2
\f$ at \f$t=t_0\f$ follow from the solution of
(10) and can therefore not be enforced,
unless we wish to initialise the time-stepping procedure with a known
exact solution \f$ {\bf R}^{(exact)}(\xi^i,t) \f$. (Only!) in this case
are we allowed to assign an initial value for the acceleration via
\f[
\left. \frac{\partial^2 {\bf R}(\xi^i,t)}{\partial t^2} \right|_{t=t_0} = 
{\bf A}^{(IC)}(\xi^i) \equiv
\left. \frac{\partial^2 {\bf R}^{(exact)}(\xi^i,t)}{\partial t^2} 
\right|_{t=t_0}.
\ \ \ \ \ \ \ \ \ \ \ \ (22)
\f]
We will assume that time-stepping is performed with the \c Newmark 
method which is our default timestepper for hyperbolic
problems. In this case the time-derivatives of the nodal positions in
(10) are replaced by an approximation
which involves the current and three "history values" of the nodal 
positions. To start the time-integration, we must assign 
suitable values to these quantities to ensure that the initial 
state of the system is represented correctly.

To assign the initial values for the nodal positions, we
(temporarily) remove all boundary conditions for the nodal 
positions and determine their initial values by solving equation
(20) in its weak form,
\f[
f_{il}^{(0)} = \int \left( \sum_{j=1}^N X_{ij}^{(0)} \psi_j(\xi^k) 
- R^{(IC)}_i(\xi^k)
\right) \psi_l(\xi^k) \ dv = 0
\mbox{ \ \ \ \ for \ \ \ $l=1,...,N; \ \ i=1,..,3$}
\ \ \ \ \ \ \ \ \ \ \ \ \ (23)
\f]
where  \f$ R^{(IC)}_i \f$ is the \f$i\f$-th component of 
\f${\bf R}^{(IC)}.\f$ Equation (23) provides
\f$ 3 \times N\f$ equations for the \f$ 3 \times N \f$ components
of the initial nodal positions, \f$ X_{ij}^{(0)} \f$ (where
\f$i=1,...3; \ j=1,...,N)\f$. To determine the initial nodal
velocities, we repeat the same procedure with the prescribed
velocities and solve
\f[
f_{il}^{(1)} = \int \left( \sum_{j=1}^N X_{ij}^{(1)} \psi_j(\xi^k) 
- V^{(IC)}_i(\xi^k)
\right) \psi_l(\xi^k) \ dv = 0
\mbox{ \ \ \ \ for \ \ \ $l=1,...,N;  \ \ i=1,..,3$}
\ \ \ \ \ \ \ \ \ \ \ \ \ (24)
\f]
for the initial nodal velocities, \f$ X_{ij}^{(1)} \f$ (where
\f$i=1,...3; \ j=1,...,N)\f$. Finally, assuming that we have 
an exact solution for the accelerations, we solve
\f[
f_{il}^{(2)} = \int \left( \sum_{j=1}^N X_{ij}^{(2)} \psi_j(\xi^k) 
- A^{(IC)}_i(\xi^k)
\right) \psi_l(\xi^k) \ dv = 0
\mbox{ \ \ \ \ for \ \ \ $l=1,...,N; \ \ i=1,..,3$}
\ \ \ \ \ \ \ \ \ \ \ \ \ (25)
\f]
for the initial nodal accelerations, \f$ X_{ij}^{(2)} \f$ (where
\f$i=1,...3; \ j=1,...,N)\f$. Having determined the 
nodal positions and their first and second time-derivatives
at \f$ t=t_0 \f$, we can use the functions
\c Newmark::assign_initial_data_values_stage1(...) and
\c Newmark::assign_initial_data_values_stage2(...) to compute the 
positional history values which ensure that the Newmark 
approximations for the
initial velocity and acceleration are correct.
This procedure is fully implemented in the function
\code
SolidMesh::Solid_IC_problem.set_newmark_initial_condition_directly(...)
\endcode
whose arguments are:
- The pointer to the problem being solved. This is needed because
  the solution of equations (23) -
  (25) requires a temporary change to the
  boundary conditions and to the equation numbering scheme. Once
  the history values have been assigned, the original
  boundary conditions are restored and the equation numbers 
  are re-generated by executing \c Problem::assign_eqn_numbers().
- A pointer to the \c SolidMesh on which the initial conditions
  are assigned.
- A pointer to the \c TimeStepper (which has to be a member of the \c Newmark
  family).
- A pointer to the "Elastic initial condition" object (discussed below).
- The initial timestep. 

Here is a brief outline of the implementation:
All \c SolidFiniteElements store a pointer to a
\c SolidInitialCondition object. By default this pointer is set to NULL,
indicating that \c FiniteElement::get_residual(...)
should compute the residuals of the "normal" governing equations.
\c SolidFiniteElements whose initial conditions are to be set with 
the above function, must re-direct the computation of the residual to
\code 
SolidFiniteElement:get_residuals_for_solid_ic(...)
\endcode
whenever the pointer to the \c SolidInitialCondition is non-NULL, as 
illustrated in this code fragment:

\code
//=======================================================================
// Compute the residuals for the elasticity equations.
//=======================================================================
template <unsigned DIM>
 void SolidEquations<DIM>::get_residuals(Vector<double> &residuals)
 {
 
  // Simply set up initial condition?
  if (Solid_ic_pt!=0)
   {
    get_residuals_for_solid_ic(residuals);
    return;
   }

  // Set up residuals for principle of virtual displacements

  [...]


  }
 \endcode

The \c SolidInitialCondition object stores a (pointer to a) \c GeomObject
and a flag that indicates which time-derivative (0th, 1st
or 2nd) of the \c GeomObject's position vector is to be assigned to
the nodal coordinates. Based on the value of this flag,
the function \c SolidFiniteElement::get_residuals_for_solid_ic(...),
is able to compute the residuals corresponding to equations
(23), (24) or 
(25). 

This all sounds very complicated (and it is!) but luckily
all the hard work has already been done and the relevant
procedures are fully implemented. Hence,
the actual assignment of the initial conditions is as simple as
this:

\code

// Created a Problem
Problem* problem_pt = new SomeProblem;

// Created an SolidMesh 
SolidMesh* solid_mesh_pt = new SomeSolidMesh;

// Created a Newmark timestepper
TimeStepper* time_stepper_pt=new Newmark<1>; 

[...]

// Create the GeomObject whose time-dependent deformation 
// specifies the initial conditions for our solid body:
// The position vector and its time-derivatives
// are accessible via the GeomObject's member function
// GeomObject::dposition_dt(...).
GeomObject* geom_obj_pt=new SomeGeomObject;

//Setup object that specifies the initial conditions:
SolidInitialCondition* ic_pt = new SolidInitialCondition(geom_obj_pt);

// Choose the initial timestep
double dt=0.01; 

// Assign the initial conditions:
SolidMesh::Solid_IC_problem.set_newmark_initial_condition_directly(
             problem_pt,solid_mesh_pt,time_stepper_pt,IC_pt,dt);

// Done!

[...]

 \endcode

If we do not know an exact solution to our problem (and in most
cases we obviously won't...), we can only use the procedure described above
to determine the initial nodal positions and velocities. 
In that case we solve the equations (19) for the
remaining "history value". Since the equations (19)
are linear in the accelerations, this is a linear problem
whose Jacobian matrix is proportional to the mass matrix
\f[
M_{ij} = \int {\cal M} \ \psi_i \ \psi_j \ dv. 
\ \ \ \ \ \ \ (26)
\f]
The procedure which determines the initial "history values" 
from the given initial positions and velocities while ensuring
consistency with the governing equation at \f$ t = t_0\f$
is implemented in 
\c SolidMesh::Solid_IC_problem.set_newmark_initial_condition_consistently(...)
which takes the same arguments as the function that assigns 
the acceleration directly, but also requires a function pointer
to a "multiplier" \f$  {\cal M} \f$.  If there
is no growth, i.e. if \f$\Lambda=1\f$ in  (19), 
the multiplier is given by the timescale ratio
\f$\Lambda^2\f$; if the body is subjected to uniform isotropic
growth, \f$\Gamma \ne 1\f$, the multiplier is equal to 
\f$\Gamma \Lambda^2\f$. 
If the wrong multiplier is specified (or if it is omitted, in
which case the default value of 1.0 is used) the residuals (19)
will be nonzero (or at least larger than the tolerance specified in \c
SolidICProblem). In this case a warning is issued and the
code execution terminates. This behaviour can be suppressed
by increasing the tolerance suitably, but you do this at your own risk!


<B>Important:</B> 
The above procedures can only handle the assignment of 
initial conditions in problems that are formulated in
terms of displacements and do \b not involve any additional variables
such as solid pressures. We do not believe that
it is possible to implement the assignment of initial conditions
for such problems without additional knowledge about the precise 
form of the constitutive equations. Therefore we provide 
a virtual function \c SolidFiniteElement::has_internal_solid_data()
whose role it is to return a bool that indicates if a specific 
\c SolidFiniteElement stores such data. By default, the function
returns \c false and should be overloaded in derived elements
which involve unknowns that do not represent nodal positions. If the function
returns \c true for any element that is used during the automatic
assignment of initial conditions the code execution stops with an 
appropriate warning message.




<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

