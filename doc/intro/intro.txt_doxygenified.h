/**

\mainpage The Finite Element Method




\section intro Introduction
This document provides a brief introduction to the finite element
method and illustrates how the method is implemented in \c oomph-lib. 
The first few sections present a brief, self-contained 
derivation of the method. The exposition is
"constructive" and avoids a number of subtleties, 
which can be found in standard  textbooks [e.g. 
E.B. Becker, G.F. Carey, and J.T. Oden. 
Finite Elements: An Introduction. Prentice-Hall, Englewood 
Cliffs, New Jersey, (1981)] . We 
generally assume that all functions are sufficiently "well behaved" 
so that all mathematical manipulations "make sense".
Function  spaces are only used where
they help to make the notation more compact. 

Readers who are 
familiar with the theory of finite elements may skip the
introductory sections, but should consult the section \ref towards_oo which 
explains the particular implementation used in \c oomph-lib.

Initially, we develop the method for scalar 
second-order (elliptic) PDEs with Dirichlet boundary 
conditions, using classical 1D and 2D Poisson problems
as model problems. Specifically, we consider the 1D problem
\f[  \frac{\mbox{d}^2 u(x)}{\mbox{d} x^2} =
f(x) \mbox{ \ \ \ \ for $x\in[0,1]$ \ \ \ subject to \ \ \ }  u(x=0)= g_0 
\mbox{\ \ \ and \ \ \ }
u(x=1)= g_1,
\mbox{\hspace{3cm}} \f]
where \f$ f(x)\f$ and the constants \f$g_0\f$ and \f$g_1\f$ are given.
The 2D equivalent is given by
\f[
\frac{\partial^2 u(x_1,x_2)}{\partial x_1^{2}}  
 + \frac{\partial^{2} u(x_1,x_2)}{\partial x_2^{2}} =
f(x_1,x_2)  \mbox{ \ \ \ \ for $(x_1,x_2)\in D$  
\ \ \ subject to \ \ \ }  u|_{\partial D} = g,
\mbox{\hspace{5cm}} \f]
where \f$ f(x_1,x_2)\f$ and \f$ g(x_1,x_2)\f$ are given.

<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> A note on notation</B> 
</CENTER>

<div class="panel-body">
Throughout this document, we use index notation and write, e.g.,
\f$ u(x_1,x_2) \f$ as \f$ u(x_i) \f$. We do not explicitly 
state the range of free indices where it is clear from the 
context -- in the above example it would be equal to the 
spatial dimension of the problem. 

All mathematical derivations are presented using
the "natural" numbering of the indices: e.g., the components
of the 3D vector \f${\bf f}\f$ are \f$ f_1, f_2 \f$ and \f$ f_3 \f$.
Unfortunately, this notation is inconsistent with the 
implementation of vectors (and most other standard "containers")
in C++ where the indices start from 0 and the components of 
\c vector<double> \c f(3) are \c f[0], \c f[1] and \c f[2].
There is no elegant way to resolve this conflict. Adopting C++-style
numbering in the theoretical development would make the mathematics
look very odd (try it!); conversely, adopting the "natural" 
numbering when discussing the C++-implementation would make this 
documentation inconsistent with the actual implementation.
We therefore use both numbering systems, each within their
appropriate context.
</div>
</TD>
</TR>
</TABLE>
</CENTER>



Physically, the Poisson equation describes steady diffusion processes.
For instance, the 2D Poisson problem describes the temperature
distribution \f$u(x_i) \f$ within a 2D body, \f$D\f$, whose boundary
\f$\partial D\f$ 
is maintained at a prescribed temperature, \f$ g(x_i)\f$. The
function \f$ f(x_i)\f$ describes the strength of (distributed) heat sources
in the body. In practical applications, the strength
of these heat sources is bounded because no physical process can
release infinite amounts of energy in a finite domain. 
Hence, we assume that 
\f[
\int_D f(x_i) \ dx_1 dx_2 < \infty.
\f]

We usually write all PDEs in "residual form", obtained
by moving all terms to the left-hand-side of the equation, so that
the general second-order scalar PDE problem is

<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> Problem P </B>
</CENTER>

<CENTER class="panel-body">
\f[ {\cal R}\left(x_i; u(x_i), \frac{\partial u}{\partial x_i},  
\frac{\partial^2 u}{\partial x_i \ \partial x_j}\right) = 0 \ 
\mbox{ \ \ \ \ in \ \ \ } D,\f]
with Dirichlet (essential) boundary conditions on 
\f$ \partial D \f$
\f[ \hfill u|_{\partial D} = g, \f]
where the function \f$g\f$ is given
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER>

To keep the notation compact, we suppress the 
explicit dependence of \f$ {\cal R} \f$
on the derivatives and write the residual as  
\f$ {\cal R}(x_i; u(x_i))\f$. For example,
the residual forms of the two Poisson problems are given by:
<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> Problem P1</B>
</CENTER>

<CENTER class="panel-body">
\f[ {\cal R}(x; u(x)) = \frac{\mbox{d}^2 u(x)}{\mbox{d} x^2} -
f(x) =0 \mbox{\ \ \ \ for $x\in[0,1]$  \ \ \ 
subject to \ \ \ }  u(x=0)= g_0 
\mbox{\ \ \ and \ \ \ }
u(x=1)= g_1,
 \f]
where \f$ f(x)\f$ and the constants \f$g_0\f$ and \f$g_1\f$ are given.
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER>

 and

<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> Problem P2</B>
</CENTER>

<CENTER class="panel-body">
\f[ {\cal R}(x_i; u(x_i)) = \sum_{j=1}^2
\frac{\partial^2 u(x_i)}{\partial x_j^2} -
f(x_i) =0 \mbox{ \ \ \ \ for $x_i\in D$ \ \ \ 
subject to \ \ \ }  u|_{\partial D} = g,
 \f]
where \f$ f(x_i)\f$ and \f$ g(x_i)\f$ are given, and \f$ i = 1,2\f$.
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER>

We stress that neither the finite element method, nor \c oomph-lib 
are restricted
to scalar second-order PDEs. Documentation for the example drivers discusses
generalisations to:
- non-Dirichlet boundary conditions
- systems of PDEs
- mixed interpolation
- discontinuous interpolation
- timestepping
- higher-order PDEs
- solid mechanics and Lagrangian coordinates.



<HR>
<HR>

\section mathFE Mathematical background
\subsection weaksoln The weak solution
A classical (or strong) solution of the problem P is any function
\f$ u(x_i) \f$ that satisfies the PDE and boundary condition
at every point in \f$ D\f$,
\f[ {\cal R}(x_i; u(x_i)) \equiv 0 \ \ \ \forall x_i \in D
\mbox{\ \ \ \ and \ \ \ \ } 
u|_{\partial D} = g. \f]
The concept of a "weak" solution is based on a slight relaxation of
this criterion. A weak solution, \f$u_w(x_i), \f$ of problem P
is any function that satisfies the essential boundary condition,
\f[ u_w|_{\partial D} = g, \f]
and for which the so-called "weighted residual"
\f[ r = \int_{D}  {\cal R}(x_i; u_w(x_i)) \ \phi^{(test)}(x_i) 
\ \mbox{d}x_1 \mbox{d}x_2 \ \ \ \ \ \ \ \ (1) \f]
vanishes for \e any  "test function" 
\f$ \phi^{(test)}(x_i) \f$ which  satisfies homogeneous
boundary conditions so that
\f[ \phi^{(test)}|_{\partial D} = 0. \f]

At this point it might appear that we have fatally weakened
the concept of a solution. If we only require the PDE to be satisfied
in an average sense, couldn't any function  
be a "solution"? In fact, this is not the case and we shall now
demonstrate that, for all practical
purposes [we refer to the standard literature for a 
rigorous derivation], the statement 
\f[ \mbox{ "weak solutions are strong solutions" } \f]
is true. The crucial observation is that the weak solution 
requires the weighted residual to vanish
for \e any test function. To show that this is equivalent to
demanding that \f$ {\cal R} \equiv 0 \ \ \forall x_i \in D  \f$
(as in the definition of the strong solution), let us try to construct
a counter-example for which \f$ {\cal R} \not\equiv 0 \f$ in 
some part of the
domain (implying that the candidate solution is not a classical
solution) while \f$r=0\f$ (so that it qualifies as a weak solution). 
For simplicity we illustrate the 
impossibility of this in a 
1D example. First consider a candidate solution \f$ u_c(x)\f$ 
which satisfies the essential boundary condition
but does not satisfy the PDE anywhere, and that
\f$ {\cal R}(x; u_c(x)) > 0 \f$  throughout the domain, as
indicated in this sketch:
\image html weaksoln1.gif "Residual (blue/solid) is nonzero (and positive) throughout the domain. A constant test function (red/dotted) is sufficient to show that the candidate solution is not a weak solution. " 
\image latex weaksoln1.eps "Residual (blue/solid) is nonzero (and positive) throughout the domain. A constant test function (red/dotted) is sufficient to show that the candidate solution is not a weak solution. " width=0.75\textwidth
Could this
candidate solution possibly qualify as a weak solution? No, using the
trivial test function 
\f$ \phi^{(test)} \equiv 1 \f$ gives a nonzero 
weighted residual, and it follows 
that  \f$ {\cal R}(x, u_c(x)) \f$ must have 
zero average if \f$ u_c(x) \f$ is to qualify as a weak solution.
   
The figure below shows the residual for a more sophisticated
candidate solution which
satisfies the PDE over most of the domain.
\image html weaksoln2.gif "Residual (blue/solid) is nonzero only in two small sub-domains. A suitably constructed test function with finite support (red/dotted) is sufficient to show that the candidate solution is not a weak solution. " 
\image latex weaksoln2.eps "Residual (blue/solid) is nonzero only in two small sub-domains. A suitably constructed test function with finite support (red/dotted) is sufficient to show that the candidate solution is not a weak solution. " width=0.75\textwidth
The residual
is nonzero only in two small subdomains, \f$ D_1\f$  and 
\f$ D_2 \f$. The candidate
solution is such that the residual has different signs in \f$ D_1\f$  and 
\f$ D_2 \f$ so that its average over the domain is zero. Could this
solution qualify as
a weak solution? Again the answer is no because
we can choose a test function  that is nonzero in only one 
of the two subdomains (e.g. the function shown by the red/dotted
line), which gives a nonzero weighted residual.

It is clear that such a procedure may be used  to obtain a nonzero
weighted residual whenever the residual \f$ {\cal R} \f$ is 
nonzero \e anywhere
in the domain. In other words, a weak solution is a strong
solution, as claimed. [To make this argument 
mathematically rigorous, 
we would have to re-assess the argument for (pathological) 
cases in which the residual is nonzero only at finite number of 
points, etc.]. 

<HR>


\subsection int_by_parts_1 A useful trick: Integration by parts
Consider now the weak form of the 2D Poisson 
problem P2,
\f[ \int_D  \left( \sum_{j=1}^2 \frac{\partial^2 u(x_i)}{\partial x_j^2} -
f(x_i) \right) \phi^{(test)}(x_i) \  \mbox{d}x_1 \mbox{d}x_2 =0
\mbox{\ \ \ \ subject to\ \ \ \ }  u|_{\partial D}=g. 
\ \ \ \ \ \ \ \  (2) \f]
After integration by parts and use of the divergence theorem, we obtain
\f[ \int_D \sum_{j=1}^2 \frac{\partial u(x_i)}{\partial x_j} \ 
 \frac{\partial \phi^{(test)}(x_i)}{\partial x_j} \  \mbox{d}x_1 \mbox{d}x_2 +
 \int_D f(x_i) \  \phi^{(test)}(x_i) \  \mbox{d}x_1 \mbox{d}x_2 =
 \oint_{\partial D}  \frac{\partial u}{\partial n} 
 \ \phi^{(test)} \ \mbox{d}s, \ \ \ \ \ (3) \f]
where \f$s\f$ is the arclength along the domain boundary \f$ \partial D\f$
and \f$ \partial/\partial n\f$ the outward normal derivative.
Since the test functions satisfy homogeneous boundary
conditions, \f$ \left. \phi^{(test)} \right|_{\partial D} = 0, \f$
the line integral on the RHS of equation (3) vanishes.
Therefore, an alternative version of the weak form of problem P2
is given by
\f[ \int_D \sum_{j=1}^2 \frac{\partial u(x_i)}{\partial x_j} \ 
 \frac{\partial \phi^{(test)}(x_i)}{\partial x_j} \  \mbox{d}x_1 \mbox{d}x_2 +
 \int_D f(x_i) \  \phi^{(test)}(x_i) \  \mbox{d}x_1 \mbox{d}x_2 = 0.
 \ \ \ \ \ \ (4) \f]
 We note that (4) involves first derivatives of the unknown function 
 \f$ u \f$
 and the test function \f$ \phi^{(test)} \f$, whereas (2) 
 involves second derivatives
 of \f$ u \f$ and the zero-th derivatives of \f$ \phi^{(test)} \f$.
 The advantages of using the "symmetric", integrated-by-parts 
 version of the weak form  will become apparent in the subsequent 
 sections. 
 
<HR>

\subsection function_spaces [Nearly an aside:] Function spaces
We stated in the introduction that all functions are assumed 
to be <EM> sufficiently  "well-behaved" so  that all 
mathematical manipulations "make sense"</EM>. It is important
to realise that we have already (tacitly) used this assumption
in the derivation of the weak form.
The weak form does not "make sense" if we allow 
candidate solutions and test functions for which the
integral in (1) does not exist. This imposes
restrictions on the types of functions that are
"admissible" in our problem. The precise form of these restrictions 
depends on the form of the residual \f${\cal R}(x_i,u(x_i))\f$, and,
typically, the restrictions are related to the functions' 
differentiability. 
It is convenient to employ the concept of a "function space"
to collectively refer to all functions that satisfy 
the required restrictions. [In this introduction, none of the 
additional (heavy) machinery from functional analysis is required].

For instance, we can ensure that the integrated-by-parts version 
of problem P2 in equation (4) 
"makes sense" if we restrict 
\f$u(x_i)\f$ and \f$\phi^{(test)}(x_i)\f$ to all functions 
whose zeroth and first derivatives are square integrable over 
the domain \f$D\f$. These functions are members of a (well-known)
function space that is usually denoted by \f$H^{1}(D)\f$. 
In fact, \f$H^{1}(D)\f$ is a particular instance of a
family of function spaces -- the Sobolev spaces \f$H^{i}(D)\f$
where \f$i=0,1,2,...\f$ which 
contain all functions whose zeroth, first, ...,
i-th derivatives are square-integrable over the domain \f$D\f$. 
The members of these function spaces have the property that
\f[ 
u(x_i) \in H^0(D) \ \ \  \iff \ \ \ \int_D u^2(x_i) \ dx_1 dx_2 < \infty,
\f]
\f[ 
u(x_i) \in H^1(D) \ \ \  \iff \ \ \ \int_D 
       \left( u^2(x_i) + 
        \sum_{j=1}^2 \left(\frac{\partial u(x_i)}{\partial x_j}\right)^2
        \right) dx_1 dx_2 < \infty,
\f]
etc. We use the subscript "0" to restrict a given function space 
to the subset of its members which vanish on the domain
 boundary \f$ \partial D,\f$
\f[ 
u(x_i) \in H^i_0(D) \ \ \  \iff \ \ \ \ u(x_i) \in H^i(D)
\mbox{\ \ \ and \ \ \ } u|{\partial D}=0. 
\f]

Using these function spaces, we can provide a concise definition of the
weak form of problem P2: 
<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> Problem P2<SUB>weak</SUB> </B>
</CENTER>

<CENTER class="panel-body">
Find the function   \f$ u(x_i) \in H^1(D) \f$ that satisfies the
essential boundary conditions
\f[ \hfill u|_{\partial D} = g, \f]
and for which
\f[ \int_D \sum_{j=1}^2 \frac{\partial u(x_i)}{\partial x_j} \ 
\frac{\partial \phi^{(test)}(x_i)}{\partial x_j} \  \mbox{d}x_1 \mbox{d}x_2 +
\int_D f(x_i) \  \phi^{(test)}(x_i) \  \mbox{d}x_1 \mbox{d}x_2 = 0
\f]
for \e all test functions  \f$ \phi^{(test)}(x_i) \in H^1_0(D). \f$
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER>

It is important to realise that the choice of suitable function spaces for
\f$u(x_i)\f$ and \f$\phi^{(test)}(x_i)\f$ is problem-dependent,  
guided by the inspection of the weak form
for a specific problem. The (pragmatic) procedure is straightforward: 
write down the weak form and 
determine the (minimal) constraints that must be 
imposed on  \f$u(x_i)\f$ and \f$\phi(x_i)\f$ for the weak form
to "make sense". All functions that satisfy these constraints,
are "admissible" and, collectively, they form a function space
\f$ H(D) \f$, say.
The weak form of the general problem P can then be written as  \n \n
<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading"><B> Problem P<SUB>weak</SUB> </B></CENTER>
<CENTER class="panel-body">
Find the function   \f$ u(x_i) \in H(D) \f$ that satisfies the
essential boundary conditions
\f[ \hfill u|_{\partial D} = g, \f]
and for which
\f[ \int_D {\cal R}(x_i,u(x_i)) \  \phi^{(test)}(x_i) \  \mbox{d}x_1 \mbox{d}x_2 = 0
\f]
for \e all test functions  \f$ \phi^{(test)}(x_i) \in H_0(D). \f$
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER>
\n

 [If you followed the above argument carefully you will have
 realised that our strategy for ensuring that the weak form
 "makes sense" uses a sufficient rather than a necessary condition.
 For instance, it is not necessary for \f$ u(x_i)\f$ and 
 \f$ \phi^{(test)}(x_i)\f$ to be members of the same function space.
 Alternative formulations are possible but we shall not pursue such ideas any 
 further in this introduction.]
 
<HR>

\subsection galerkin The Galerkin method
We now exploit the definition of the weak solution to
develop a numerical method that can be used to determine
approximate solutions to problem P<SUB>weak</SUB>. We start by 
splitting the solution into two parts, 
\f[ u(x_i) = u_{h}(x_i) + u_{p}(x_i),  \f]
where \f$ u_{p}(x_i)\f$ is an (arbitrary) function that satisfies
the Dirichlet boundary conditions,
\f[ u_{p}|_{\partial D} = g. \f]
The unknown function \f$ u_{h}(x_i) \f$ then has to satisfy the
homogeneous boundary conditions
\f[ u_{h}|_{\partial D} = 0.\f]
We expand \f$ u_{h}(x_i) \f$ in terms of a
(given) infinite set of basis functions \f$ \psi_j(x_i) \in H_0(D) \ \ \ 
(j=1,...,\infty) \f$, 
\f[ u(x_i) = u_{p}(x_i) + \sum_{j=1}^{\infty} U_j \psi_j(x_i),
\ \ \ \ \ \  (5) \f]
which discretises the problem because the solution is now
determined by the (as yet unknown) discrete coefficients 
 \f$ U_j \ \ \ (j=1,...,\infty). \f$
There are many possible sets 
of basis functions: polynomials, trigonometric functions,
systems of eigenfunctions; mathematically speaking, the only requirement
is that the basis functions are sufficiently general that
the solution can be represented by the expansion  (5). 
In other words,
the functions must be  a complete basis for \f$H_0(D)\f$.

How do we determine the discrete coefficients   \f$ U_j \f$?
Inserting the expansion for \f$u(x)\f$ into the definition
of the weighted residual yields
\f[ r = \int_{D}  {\cal R}\left(x_i; \  
 u_{p}(x_i)+\sum_{j=1}^{\infty} U_j 
\psi_j(x_i)\right) \ \phi^{(test)}(x_i) 
\ \mbox{d}x_1 \mbox{d}x_2 = 0,  \ \ \ \ \ \ (6) \f]
and we recall that this equation must be satisfied for \e any test
function \f$ \phi^{(test)} \in H_0(D) \f$. The
functions \f$\psi_j(x_i)\f$ form a complete basis for \f$ H_0(D)\f$,
and so all possible test functions \f$ \phi^{(test)}(x_i) \f$ may be
represented as
\f[  \phi^{(test)}(x_i) = \sum_{k=1}^{\infty} \Phi_k 
\psi_k(x_i). \ \ \ \ \ \ \ (7) \f]
Thus, the 
condition
\f[ \mbox{...for any basis function $ \phi^{(test)}(x_i) $...} \f]
becomes
\f[ \mbox{...for any values of the coefficients $ \Phi_k$...} \f]
Inserting the expansion  (7)  into 
the definition of the weak solution (6) yields
\f[ r =   \sum_{k=1}^{\infty} \Phi_k \  r_k(U_1, U_2,...) = 0, 
\ \ \ \ \  (8) \f] 
where 
\f[ r_k(U_1, U_2,...) = 
\int_{D}  {\cal R}\left(x_i;  \  u_{p}(x_i)+\sum_{j=1}^{\infty} U_j 
\psi_j(x_i)\right) \ \psi_k(x_i) 
\ \mbox{d}x_1 \mbox{d}x_2.  \ \ \ \ \ \ \  (9) \f]
Equation (8) must hold for \e any value of the coefficients
\f$ \Phi_k \f$, so the coefficients \f$ U_j \f$ must satisfy
the equations
\f[  r_k(U_1, U_2,...) = 0, \mbox{ \ \ \ \ for } k=1,2,... \f]


In practice, we truncate the expansions  (5) and (7) 
after a finite number of terms to obtain the approximations
(indicated by tildes)
\f[ \widetilde{u}(x_i) =  u_p(x_i) + \sum_{j=1}^{M} U_j \psi_j(x_i)
   \mbox{ \ \ and \ \ } \widetilde{\phi^{(test)}}(x_i) = 
   \sum_{k=1}^{M} \Phi_k 
   \psi_k(x_i), \ \ \ \ \ \ (10) \f]
and we determine the \f$M\f$ unknown coefficients, \f$U_1,...,U_M\f$,
from the  \f$M\f$ algebraic equations
\f[ r_k(U_1,...,U_M)=0, \mbox{\ \ where \ \ $k=1,...,M$}. 
\ \ \ \ \ \ \ \   (11)  \f]
 The number of terms in each truncated expansion must be the same, so
 that we obtain \f$M\f$ equations for \f$M\f$ unknowns.

The truncation of the expansions (5)  and (7) 
introduces two approximations:
- The approximate solution \f$ \widetilde{u}(x_i) \f$ 
  is a member of the finite-dimensional function
  space \f$\widetilde{H}(D) \subset H(D)\f$ spanned by the
  basis functions included in the expansion  (10).
- We "test" the solution with functions from 
  \f$ \widetilde{H_{0}}(D) \f$ rather than with "all" functions 
  \f$ \phi^{(test)} \in H_0(D). \f$
.
\f$\widetilde{H}(D) \to H(D)\f$ as \f$M \to \infty\f$, however, so
the approximate solution \f$\widetilde{u}(x_i)\f$ 
converges to the exact solution \f$u(x_i)\f$ as we include more and more
terms in the expansion. [The precise definition of "convergence"
requires the introduction of a norm, which allows us to measure 
the "difference" between two functions. We refer to the
standard literature for a more detailed discussion of this issue.]

In general, the equations \f$r_k(U_1,...,U_M)\f$ are nonlinear 
and must be solved by an iterative method such as Newton's
method. Consult your favourite numerical analysis textbook
(if you can't think of one, have a look through chapter 9 in Press, W. H.; 
Flannery, B. P.; 
Teukolsky, S. A.; and Vetterling, W. T. "Numerical Recipes in C++.
The Art of Scientific Computing", Cambridge University Press) for
a reminder of how (and why) Newton's method works. The following
algorithm shows the method applied to our equations:

\anchor Newton
 <CENTER> <B> Algorithm 1: Newton's method </B> </CENTER>

-# Set the iteration counter \f$i=0\f$ and provide an initial
approximation for the unknowns, \f$ U^{(0)}_j\ \ (j=1,..,M)\f$.
-# Evaluate the residuals 
   \f[  r_k^{(i)} = r_k\left( U^{(i)}_1,..., U^{(i)}_M\right)
   \mbox{\ \ \ for $k=1,...,M$}. \f]
-# Compute a suitable norm of the residual vector (e.g. the maximum norm).
   If the norm is less than some pre-assigned tolerance, stop and
   and accept \f$U_j^{(i)} (j=1,...,M) \f$ as the solution.
-# Compute the Jacobian matrix
  \f[  J_{kj} = \left. \frac{\partial r_k}{\partial U_j} 
  \right|_{\left( U^{(i)}_1,..., U^{(i)}_M\right)}
  \mbox{\ \ \ for $j,k=1,...,M$}.\f]
-# Solve the linear system
   \f[  \sum_{j=1}^{M} J_{kj} \  \delta U_j =  - r_k^{(i)}
   \mbox{\ \ \ \ \ \ where  $k=1,...,M$} \f]
   for \f$ \delta U_{j} \ \ ($j=1,...,M$).\f$
-# Compute an improved approximation via
   \f[  U_j^{(i+1)} = U_j^{(i)} +  \delta U_j  
   \mbox{\ \ \ for $j=1,...,M$}. \f]
-# Set \f$ i=i+1\f$ and go to 2. 
.

For a "good" initial approximation, 
\f$ U^{(0)}_j\f$, 
Newton's method converges quadratically towards the exact solution. 
Furthermore, for linear
problems, Newton's method provides the exact solution (modulo any
roundoff errors that might be introduced during the solution of the 
linear system) in a single iteration. Newton's method can, therefore,
be used as a robust, general-purpose solver, if (!)
a good initial guess for the solution can be provided. 
In practice, this is not a serious restriction,
because good initial guesses can often be generated by
continuation methods. In \c oomph-lib, Newton's method is
the default nonlinear solver. 


Let us, briefly, examine the cost of the non-trivial steps involved
in Newton's method: 
- Step 2 requires the evaluation of \f$M\f$ integrals 
  over the domain to determine
  the discrete residuals \f$  r_k^{(i)} \f$ from  (9).
  (We note that, in general,  the integrals must be evaluated numerically.)
- Step 3 requires the computation of \f$M^2\f$ entries
  in the Jacobian matrix, each an integral of the form 
  \f[ J_{kj} =   \int_{D} \frac{\partial }{\partial U_j}
  {\cal R}\left(x_i;  \ \sum_{j=1}^{M} U_j 
  \psi_j(x_i)\right) \ \psi_k(x_i) 
  \ \mbox{d}x_1 \mbox{d}x_2 \mbox{\ \ \ for $j,k=1,...,M$.} \f]
- Step 4 requires the solution of a \f$M \times M\f$
  linear system.
.
 
In general, steps 3 and 4 will be very costly if \f$M\f$ is 
large. However, if the domain has a simple shape and the differential 
operator has a sufficiently simple structure, it is often possible
to choose basis functions with suitable orthogonality properties
that render the Jacobian matrix  \f$J_{kj}\f$ sparse.
As an example, we consider the application of Galerkin's method
in the 1D Poisson problem P1:

<CENTER> <B> Example: Galerkin method applied to the model problem P1  
</B></CENTER>
We perform the usual integration by parts to 
derive the symmetric weak form of the problem: \n \n
<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> Problem P1<SUB>weak</SUB> </B></CENTER>
<CENTER class="panel-body">
Find the function   \f$ u(x) \in H(D) \f$ that satisfies the
essential boundary conditions
\f[ u(0) = g_0 \ \ \ \mbox{and} \ \ \   u(1) = g_1, \f]
and for which
\f[ \int_0^1 \left( \frac{\mbox{d} u(x)}{\mbox{d} x} \ 
\frac{\mbox{d} \phi^{(test)}(x)}{\mbox{d} x} \
+ f(x) \  \phi^{(test)}(x) \right)\  \mbox{d}x = 0
\f]
for \e all test functions  \f$ \phi^{(test)}(x) \in H_0(D). \f$
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER> \n\n
Inspection of the weak form shows that the choice \f$H(D) = H^1(D) \f$
is sufficient to ensure the existence of the integral. Of course,
the "higher" Sobolev spaces \f$ H^2(D), H^3(D), ... \f$ would also
ensure the existence of the integral but would impose unnecessary
additional restrictions on our functions. 

 Next, we need to construct a function
 \f$u_p(x)\f$ that satisfies the Dirichlet boundary conditions.
In 1D this is trivial, and the simplest option is the
function \f$ u_p(x)=g_0+ (g_1-g_0) x\f$, which interpolates linearly
between the two boundary values. Since \f$ \mbox{d}u_p(x)/
\mbox{d} x = g_1-g_0\f$, the discrete residuals are given by
\f[ r_k(U_1,U_2,...,U_M) = 
 \int_0^1 \left[ \left( (g_1 - g_0) + \sum_{j=1}^{M} U_j
 \frac{\mbox{d} \psi_j(x)}{\mbox{d} x} \right)
 \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} + f(x) \  \psi_k(x) \right] \ \mbox{d}x
  \mbox{\ \ for $k=1,..,M$}, \ \ \ \ \ \ \ \ (12) \f]
and the Jacobian matrix has the form
\f[ J_{kj} = 
 \int_0^1 
  \frac{\mbox{d} \psi_j(x)}{\mbox{d} x}
  \frac{\mbox{d} \psi_k(x)}{\mbox{d} x}  \ \mbox{d}x
  \mbox{\ \ for $j,k=1,..,M$}. \ \ \ \ \ \ \ \ (13) \f]
The (Fourier) basis functions
\f[ \psi_j(x) = \sin\left(\pi j x \right)\f]
are a suitable basis because 
- they satisfy the homogeneous boundary conditions,
- they and their derivatives are square
  integrable, allowing all integrals to be evaluated, and
- they are a complete basis for \f$ H^1_0(D)\f$.
.
Furthermore, the orthogonality relation
\f[ \int_0^1  \cos\left(\pi k x\right) 
              \cos\left(\pi j x\right) dx 
    = 0 \mbox{\ \ \ for \ } j\ne k \f]
 implies that the Jacobian matrix is a diagonal matrix, which is
cheap to assemble and invert. Indeed, the assembly of the Jacobian
matrix in step 4, and the solution of the linear system in step 5
have an "optimal"
computational complexity: their cost increases linearly with the
number of unknowns in the problem. 

Unfortunately, the application of the method becomes difficult, if not
impossible, in cases where the differential operators
have a more complicated structure, and/or 
the domain has a more complicated shape.
The task of finding a complete set of basis functions that vanish on
the domain boundary in an arbitrarily-shaped,
higher-dimensional domain is nontrivial. 
Furthermore, for a complicated differential operator, it
will be extremely difficult to find a system of 
basis functions for which the Jacobian matrix has a 
sparse structure. If the matrix is dense, the assembly
and solution of the linear system in steps 4 and 5 of Newton's method
can become prohibitively expensive.

<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> An aside: Integration by parts revisited </B></CENTER><div class="panel-body">

Let us briefly return to the two versions of the weak form and
examine the equations that we would have obtained had
we applied Galerkin's method to the original form of
the weak equations,
\f[ \int_0^1 \left( \frac{\mbox{d}^2 u(x)}{\mbox{d}x^2} \ 
  -
f(x) \right)  \phi^{(test)}(x) \  \mbox{d}x = 0.
\f]
The discrete residuals are then given by 
\f[ r_k = \int_0^1 \left( \sum_{j=1}^{M} U_{j}  \frac{\mbox{d}^2 
 \psi_j(x)}{\mbox{d} x^2} -
 f(x) \right)  \psi_k(x) \  \mbox{d}x = 0, \mbox{\ \ \ \ \ 
\ for $k=1,...,M.$\ \ \ \ \ \ }  (14) \f]
and the Jacobian matrix has the form
\f[ J_{kj} =  \int_0^1\frac{\mbox{d}^2 
\psi_j(x)}{\mbox{d} x^2} \ 
\psi_k(x)\  \mbox{d}x. \ \ \ \ \ \ \ (15) \f]
- The restrictions that must be imposed on the basis functions,
  \f$ \psi_j(x) \f$, if (14) is to 
  "make sense" are much more 
  awkward than  for
  the symmetric form for the problem; the product of their
  zeroth and second derivatives must be integrable over the domain.
- The Jacobian matrix (13) that arises from the symmetric 
  form of the
  weak problem is symmetric for \e any choice of basis functions
  whereas the Jacobian matrix (15) that 
  arises from the original form
  is symmetric only for certain types of basis functions.
  This is not only advantageous for the solution of the linear
  system (only a fraction of the entries in the matrix need to be computed,
  and (more efficient) linear solvers that exploit the symmetry 
  of the matrix can be used), but also properly reflects 
  the symmetry in the (self-adjoint!) ODE. 
.
</div>
</TD>
</TR>
</TABLE>
</CENTER>

<HR>
<HR>



\section fe The Finite Element Method
Galerkin's method is an efficient
method for finding the approximate solution to a given problem if 
(and only if) we can:
-# Construct a  function \f$ u_p(x_i) \in H(D)\f$ 
   that satisfies the essential boundary conditions.
-# Specify a set of basis functions that
   -# spans the function space \f$ H_0(D)\f$, 
   -# vanishes on the domain boundary, and
   -# leads to a sparse Jacobian matrix.
   .
.  
We shall now develop the finite element method: an
implementation of Galerkin's method that automatically
satisfies all the above requirements.

<HR>

\subsection fe_basis_fct Finite Element shape functions
The key feature of the finite element method is that the basis
functions have finite support, being zero over most of the domain, and
have the same functional form. We 
illustrate the idea and its implementation for the 1D Poisson
problem P1 in its symmetric (integrated-by-parts) form: 

\f[ r_k = \int_0^1 \left\{ \left( \frac{\mbox{d} 
 u_p(x)}{\mbox{d} x}  +\sum_{j=1}^{M} U_{j} \frac{\mbox{d} 
 \psi_j(x)}{\mbox{d} x} \right)
 \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} +
 f(x) \  \psi_k(x) \right\}  \mbox{d}x  =0, \mbox{\ \ \ \ \ 
\ for $k=1,...,M.$}\ \ \ \ \ \ \  (16) \f]
The integral (16) 
exists for all basis 
functions \f$ \psi_j(x)\f$ whose first derivatives
are square integrable; a class of functions that includes
piecewise linear functions.  

We shall now construct a particular set of piecewise linear
basis functions --- the (global) linear finite-element shape
functions, often known as "hat functions". 
For this purpose, we introduce \f$N\f$ equally-spaced "nodes" into the
domain \f$ x \in [0,1]\f$; node \f$j\f$ is 
located at \f$X_j = (j-1) h \f$, where \f$h=1/(N-1)\f$
is the distance between the nodes.
The (global) linear finite-element shape functions are defined by
\f[
\psi_j(x) =  \left\{ \begin{array}{ll}
          0 & \mbox{\ \ \ for $x < X_{j-1}$} \\
  \frac{x-X_{j-1}}{X_j-X_{j-1}} & \mbox{\ \ \ for $ X_{j-1} <x < X_j$} \\
  \frac{X_{j+1}-x}{X_{j+1}-X_j} & \mbox{\ \ \ for $ X_{j} <x < X_{j+1}$} \\
          0 & \mbox{\ \ \ for $x > X_{j+1}$} \\
                     \end{array}
              \right. \ \ \ \ \ \ \ \ (17)
\f]
and are illustrated below:

\image html 1Dmesh_with_linear_shape_fcts.gif "The (global) linear finite-element shape functions in 1D. " 
\image latex 1Dmesh_with_linear_shape_fcts.eps "The (global) linear finite-element shape functions in 1D. " width=0.75\textwidth
The finite-element shape functions have finite support; 
in particular, the function
\f$ \psi_j(x) \f$ is nonzero only in the vicinity of node j
and varies linearly between one 
(at node j) and zero (at nodes \f$ j-1 \f$ and \f$ j+1 \f$). 
Furthermore, the shape functions satisfy the "interpolation condition"
\f[  \psi_j(X_i) = \delta_{ij} = 
     \left\{\begin{array}{c} 1, \mbox{ if } i=j,\\
     0, \mbox{ if } i\neq j, \end{array}\right. \f]
where \f$ \delta_{ij}\f$ is the Kronecker delta.  
The coefficients \f$V_j\f$
in an expansion of the form
\f[ \tilde{v}(x) = \sum_{j=1}^{N} V_j \ \psi_j(x)\f]
have a straightforward interpretation: \f$V_j\f$ 
is the value of the function \f$ v(x) \f$ at node \f$ j \f$.
The global shape functions vary linearly between the nodes, and so
\f$ \tilde{v}(x) \f$ provides piecewise linear interpolation
between the `nodal values' \f$V_j\f$.

\image html fe_is_interpolant.gif "The superposition of the (global) linear finite-element shape functions provides a piecewise linear interpolation between the `nodal values'. " 
\image latex fe_is_interpolant.eps "The superposition of the (global) linear finite-element shape functions provides a piecewise linear interpolation between the `nodal values'. " width=0.75\textwidth

Why are these shape functions useful in the Galerkin 
method? Consider the requirements listed at the beginning
of this section:
-# It is easy to construct a function \f$ u_p(x) \f$ 
   that satisfies the essential boundary conditions by choosing
   \f[ u_p(x) = g_0 \psi_1(x) +  g_1 \psi_N(x), \f]
   where \f$ \psi_1(x) \f$ and \f$ \psi_N(x) \f$ are the global 
   finite-element shape functions associated with the 
   two boundary nodes, \f$1\f$ and \f$N\f$. 
-# Regarding the requirements on the basis functions:
   -# The global finite-element shape functions \f$ \psi_j(x) \f$ and 
      their first derivatives
      are square integrable. Hence, the 
      finite-dimensional function space \f$ H^1_{0,FEM}(D) \f$
      spanned by the basis functions  \f$ \psi_j(x) \ \ (j=2,...,N-1)\f$,
      associated with the internal nodes,  is a subset 
      of \f$ H^1_0,\f$ as required. Furthermore, it is easy to show 
      that
      \f[ \left| v(x) - \sum_{j=2}^{N-1} v(X_j) 
      \psi_j(x) \right| \to 0 
      \mbox{\ \ \ as $N\to\infty$ and $h=\frac{1}{N-1}
      \to 0,$ \ \ \ \ \ \ \ 
      (18) } \f]
      for any  \f$ v(x) \in H^1_0(D) \f$. In other words, \f$ H^1_{0,FEM}(D) 
      \f$ approaches \f$ H^1_0(D) \f$  as  \f$ N \to \infty. \f$
   -# The global finite-element shape functions 
      \f$ \psi_2(x), \psi_3(x),...,\psi_{N-1}(x) \f$
      vanish on the domain boundary.
   -# The Jacobian matrix is sparse because its entries
      \f[ J_{kj} = \frac{\partial r_k}{\partial U_j} = 
      \int_0^1 \frac{\mbox{d} 
      \psi_j(x)}{\mbox{d} x} \ 
      \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} \,  \mbox{d}x \f]
      are nonzero when the basis functions \f$ \psi_j(x) \f$
      and  \f$ \psi_k(x) \f$ are both non-zero. For these shape functions,
      \f[ J_{kj} \ne 0 \mbox{\ \ \ \ when \ \ $k=j-1,\ j, \ j+1,$} \f]
      indicating that the Jacobian matrix is tri-diagonal.
   .
.



We can now formulate the finite-element-based solution of problem P1
in the following algorithm:
<CENTER> <B> Algorithm 2: Finite Element solution of
problem P1 by Newton's method</B> </CENTER>
- Choose the number of nodal points, \f$ N \f$, and distribute
  them evenly through the domain so that \f$ X_j=(j-1)h\f$,
  where \f$h=1/(N-1)\f$. This defines the global shape 
  functions \f$ \psi_j(x)\f$.
- Set  \f[ u_p(x) = g_0 \psi_1(x) +  g_1 \psi_N(x) \f]
  and
     \f[ u_h(x) = \sum_{j=2}^{N-1} U_{j}\ \psi_j(x). \f]
- Provide an initial guess for the unknowns \f$ U_2,U_3,...,U_{N-1}\f$.
  Since P1 is a linear problem, the quality of the initial guess is 
  irrelevant 
  and we can simply set \f$ U_i^{(0)}=0 \mbox{\ \ \ for $i=2,...,N-1$}. \f$
- Determine the residuals 
  \f[ r_k^{(0)} =  \int_0^1 \left\{
  \left( g_0 \frac{\mbox{d} \psi_1(x)}{\mbox{d} x} + \ 
         g_1 \frac{\mbox{d} \psi_N(x)}{\mbox{d} x} + \
  \sum_{j=2}^{N-1} U_{j}^{(0)}\frac{\mbox{d} 
  \psi_j(x)}{\mbox{d} x} \right)
  \frac{\mbox{d} \psi_k(x)}{\mbox{d} x}\ 
  + f(x) \  \psi_k(x) \right\} \mbox{d}x \mbox{\ \ \ \ \ 
  \ for $k=2,...,N-1,$} \f]
  and the entries in the Jacobian matrix
     \f[ J_{kj} = \frac{\partial r_k}{\partial U_j} = 
     \int_0^1 \frac{\mbox{d} 
     \psi_j(x)}{\mbox{d} x} \ 
     \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} \  dx 
      \mbox{\ \  \ for $j,k=2,...,N-1.$} \f]
- Solve the linear system
  \f[ \sum_{j=2}^{N-1}  J_{kj} \ \delta U_j = - r_k^{(0)} 
  \mbox{\ \  \ for $k=2,...,N-1.$} \f]
  for \f$ \delta U_k \ (k=2,...,N-1) \f$ .
- Correct the  initial guess via 
   \f[ U_j = U_j^{(0)} + \ \delta U_j \mbox{\ \  \ for $j=2,...,N-1.$} \f] 
  P1 is a linear problem, so \f$ U_j  \ (j=2,...,N-1) \f$
  is the exact solution. For nonlinear problems, we would
  have to continue the \ref Newton iteration until the residuals
  \f$ r_k(U_2,...,U_{N-1})  \ (k=2,...,N-1) \f$ were sufficiently
  small.
- The finite-element solution is 
  \f[ u^{(FE)}(x) = g_0 \psi_1(x)+ g_1 \psi_N(x) + \sum_{j=2}^{N-1}  
  U_j \psi_j(x)  \ \ \ \ \ (19)  \f]
.

<HR>

\subsection non_uniform Improving the quality of the solution: non-uniformly spaced nodes and higher-order shape functions
 Algorithm 2 presents the simplest possible
 implementation of the finite element method for problem P1. 
 We now discuss
 two straightforward extensions that can significantly improve
 the quality of the approximate solution.

 The finite-element approximation in the previous section is
 piecewise linear between the nodal values. 
 The accuracy to which the exact solution can be
 represented by a piecewise linear interpolant is
 limited by the number of nodal points, which is
 the essence of the convergence statement (18). The 
 number of nodes required to resolve the solution to a given
 accuracy depends on the nature of the 
 solution --- more nodes
 are needed to interpolate rapidly varying functions.

 If the solution is rapidly varying in a small portion of the domain
 it would be wasteful to use the same (fine) nodal spacing throughout
 the domain. A non-uniform spacing, see below,
\image html fe_adaptive_interpolant.gif "Adaptive 1D finite element mesh: Non-uniform spacing of nodes to achieve a high resolution only where it is required. " 
\image latex fe_adaptive_interpolant.eps "Adaptive 1D finite element mesh: Non-uniform spacing of nodes to achieve a high resolution only where it is required. " width=0.75\textwidth
 improves the accuracy of the solution without greatly increasing
 the total number of unknowns. Non-uniform spacing of the nodes 
 is easy to implement and does not require any significant changes 
 in Algorithm 2 -- we simply choose 
 appropriate values for the nodal positions \f$ X_j \f$; an 
 approach known as "h-refinement" because it alters the distance,
 \f$ h \f$, between nodes.

<CENTER>
<TABLE class="oomph-table">
<TR>
<TD class="panel panel-default">
<CENTER class="panel-heading">
<B> An aside: adaptive mesh refinement </B></CENTER>
<div class="panel-body">
A non-uniform distribution of nodes requires a priori
knowledge of the regions in which we expect the solution to undergo
rapid variations. The alternative is to use adaptive mesh refinement: 
start with a 
relatively coarse, uniform mesh and compute the solution. If 
the solution on the coarse mesh displays rapid variations in certain 
parts of the domain (and is therefore likely to be poorly resolved), 
refine the mesh in these regions and re-compute. Such adaptive
mesh refinement procedures can be automated and are 
implemented in \c oomph-lib for a large number of problems; see the 
section \ref example_P2
for an example.
</div>
</TD>
</TR>
</TABLE>
</CENTER>

 The quality of the interpolation can also be improved by 
 using higher-order interpolation, but maintaining
- the compact support for the global shape functions, so that
  \f$ \psi_j(x) \f$ is nonzero only in the vicinity of node
  \f$ j \f$, and
- the interpolation condition
  \f[  \psi_j(X_i) = \delta_{ij} \f]
  so that the shape function \f$ \psi_j \f$ is equal to one 
  at node \f$ j \f$ and zero at all others.
. 
 For instance, we can use the
 (global) quadratic finite-element shape functions shown below:
\image html 1Dmesh_with_quadratic_shape_fcts.gif "(Global) quadratic finite-element basis functions in 1D. " 
\image latex 1Dmesh_with_quadratic_shape_fcts.eps "(Global) quadratic finite-element basis functions in 1D. " width=0.75\textwidth
 Note that we could also use shape functions with identical functional forms;
 instead, we have chosen two different forms for the shape functions
 so that the smooth sections of the different shape functions
 overlap within the elements. This is not necessary but facilitates
 the representation of the global shape functions in terms of
 their local counterparts within elements, see section \ref local_coords.

 The implementation of higher-order interpolation does not
 require any significant changes in Algorithm 2. We
 merely specify the functional form of the (global) quadratic
 finite-element shape functions sketched above.
 This approach is known as "p-refinement", because it increases
 order of the <B>p</B>olynomials that are used to represent the
 solution. 

<HR>
<HR>


\section implementation An element-by-element implementation for 1D problems
From a mathematical point of view, the development of the
finite element method for the 1D model problem P1 is now complete, but 
Algorithm 2 
has a number of features that would make it awkward to implement
in an actual computer program. Furthermore, having been derived
directly from Galerkin's method, the algorithm is based on 
globally-defined shape functions and does not exploit the potential subdivision
of the domain into "elements".
We shall now derive a mathematically equivalent scheme that 
can be implemented more easily.

\subsection book_keeping Improved book-keeping: distinguishing between equations and nodes.
Since only a subset of the global finite-element shape functions act 
as basis functions for the (homogeneous) functions 
\f$ \phi^{(test)}(x)\f$ and  \f$ u_h(x)\f$, algorithm 2
resulted in a slightly
awkward numbering scheme for the equations and the unknown (nodal)
values. The equation numbers range from  2 to \f$N-1\f$, 
rather than from 1 to \f$N-2\f$ because we identified the unknowns by
the node numbers. Although this is perfectly transparent in our
simple 1D example, the book-keeping quickly
becomes rather involved in more complicated problems. We therefore 
treat node and equation numbers separately.
[\b Note: We shall use
the terms "equation number" and "number of the unknown" interchangeably;
this is possible because we must always have the same number of
equations and unknowns.] We can (re-)write the finite-element 
solution (19) in more compact form as
\f[ u^{(FE)}(x) =\sum_{j=1}^{N}  U_j \ \psi_j(x). \f]
where the summation now includes \e all nodes in the finite element
mesh. To make this representation consistent with the boundary 
conditions, the nodal values, \f$U_j\f$, 
of nodes on the boundary are set to the prescribed 
boundary values
\f[ U_j = g(X_j) \mbox{\ \ \ if $X_j\in \partial D$}.\f]
In the 1D problem P1,
\f$U_1=g_0\f$ and  \f$U_N=g_1\f$.


Furthermore, we associate each unknown nodal value, \f$ U_j \f$,
with a distinct equation number, \f${\cal E}(j)\f$, in the range
from 1 to \f$N-2\f$. In the above
example, the equation numbering scheme is given by
\f[ \mbox{
    \begin{tabular}{l c c c c c c}
    \hline \\
    \mbox{Node number $j$} & 1 & 2 & 3 & ... & $N-1$ & $N$  \\
    \hline \\
    \mbox{Equation number ${\cal E}(j)$} & n/a & 1 & 2 & ...
     & $N-2$ & n/a \\
    \hline \\
    \end{tabular}
    }
 \f]
where n/a indicates a node whose value is prescribed by the
boundary conditions. To facilitate the implementation in a computer
program, we indicate the fact that
a nodal value is determined by boundary conditions (i.e. that it
is "pinned"), by setting the equation number to a negative
value (-1, say), so that the equation numbering scheme becomes 
\f[ \mbox{
    \begin{tabular}{l c c c c c c}
    \hline \\
    \mbox{Node number $j$} & 1 & 2 & 3 & ... & $N-1$ & $N$  \\
    \hline \\
    \mbox{Equation number ${\cal E}(j)$} & -1 & 1 & 2 & ...
     & $N-2$ & -1\\
    \hline \\
    \end{tabular}
    }
 \f]
We now re-formulate algorithm 2
as follows (the revised parts of the algorithm are enclosed
in boxes):

 <CENTER> <B> Algorithm 3: Finite Element solution of
 problem P1 </B> </CENTER> \n
 <B> Phase 1: Setup</B> 
 - Discretise the domain with \f$ N \f$ nodes which are located at
   \f$ X_j, \ j=1,...,N\f$, and choose the order of the global shape
 functions \f$ \psi_j(x).\f$
 <TABLE BORDER="0" >
 <TR>
 <TD bgcolor="cornsilk">
 - Initialise the total number of unknowns, \f$M=0.\f$
 - Loop over all nodes \f$ j=1,...,N \f$:
   - If node j lies on the boundary: 
      - Assign its value according to the (known) boundary condition
        \f[ U_j = g(X_j) \f]
      - Assign a negative equation number to reflect its "pinned" status:
        \f[ {\cal E}(j) = -1 \f]
      .
   - Else: 
      - Increment the number of the unknowns
        \f[ M=M+1 \f]
      - Assign the equation number
        \f[ {\cal E}(j) = M \f]
      - Provide an initial guess for the unknown nodal value, e.g.
        \f[ U_j=0. \f]
      .
   .
.
</TD>
</TR>
</TABLE>
 - Now that \f$ \psi_j(x) \f$ and  \f$ U_j \f$ have been defined and
   initialised, we can determine the current FE approximations for
   \f$ u(x)\f$ and \f$ \mbox{d} u(x)/\mbox{d} x\f$ from
   \f[ u^{(FE)}(x) = \sum_{k=1}^{N}  U_k \psi_k(x) 
   \mbox{\ \ \ \ and \ \ \ \ }
   \frac{\mbox{d} u^{(FE)}(x)}{\mbox{d} x} = \sum_{k=1}^{N}  U_k 
   \frac{\mbox{d} \psi_k(x)}{\mbox{d} x}. \f]

 <B> Phase 2: Solution     </B> 
 \n
<TABLE  BORDER="0">
<TR>
<TD bgcolor="cornsilk">
- Loop  over all  nodes \f$ k=1,...,N\f$:
  - Determine the equation number: \f$ {\cal E}(k) \f$
  - If \f$ {\cal E}(k) \ne -1 \f$:
    \f[ r_{{\cal E}(k)} =  \int_0^1 \frac{\mbox{d} 
    u^{(FE)}(x)}{\mbox{d} x} \ 
    \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} \  dx + 
    \int_0^1 f(x) \  \psi_k(x) \  dx. \f]
    -  Loop  over all  nodes \f$ j=1,...,N\f$:
    - Determine the equation number: \f$ {\cal E}(j) \f$
    - If \f$ {\cal E}(j) \ne -1 \f$:
      \f[ J_{{\cal E}(k){\cal E}(j)} = 
      \frac{\partial r_{{\cal E}(k)}}{\partial U_j} = 
      \int_0^1 \frac{\mbox{d} 
      \psi_j(x)}{\mbox{d} x} \ 
      \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} \  dx 
      \f]
    .
  .
.
</TD>
</TR>
</TABLE>
- Solve the \f$ M \times M\f$ linear system
  \f[ \sum_{j=1}^{M}  J_{kj} \ y_j = - r_k 
        \mbox{\ \  \ for $k=1,...,M.$} \f]
  for \f$ y_k \ (k=1,...,M)\f$.
.
<TABLE  BORDER="0">
<TR>
<TD bgcolor="cornsilk">
-  Loop over all  nodes \f$ j=1,...,N\f$:
  - Determine the equation number: \f$ {\cal E}(j) \f$
  - If \f$ {\cal E}(j) \ne -1 \f$: 
  - Correct the initial guess via
    \f[ U_{{\cal E}(j)} =  U_{{\cal E}(j)} +  y_{{\cal E}(j)} \f] 
  .
.  
</TD>
</TR>
</TABLE>
- P1 is a linear problem, so \f$ U_j  \ (j=1,...,N) \f$
  is the exact solution. For nonlinear problems, we would
  have to continue the \ref Newton iteration until the residuals
  \f$r_k \ (k=1,...,M) \f$ were sufficiently small.

<B> Phase 3: Postprocessing (document the solution)</B> 
- The finite-element solution is given by 
  \f[ u^{(FE)}(x) = \sum_{j=1}^{N}  U_j \psi_j(x). \f]  
.

<HR>

\subsection element_by_element Element-by-element assembly

In its current form, our algorithm assembles the equations
and the Jacobian matrix equation-by-equation and does not
exploit the finite support of the global shape functions, which
permits decomposition of the domain into elements. Each element
consists of a given number of nodes that depends upon the order of
local approximation within the element. In the case of linear
interpolation each element consists of two nodes, as seen below:
\image html 1Dmesh.gif "A 1D finite element mesh. " 
\image latex 1Dmesh.eps "A 1D finite element mesh. " width=0.75\textwidth
 
In the following three sections we shall develop an alternative, 
element-based assembly procedure, which involves the introduction of
- local and global node numbers,
- local and global equation numbers,
- local coordinates.
.

\subsubsection local_global_nodes Local and global node numbers

Consider the discretisation of the 1D problem P1 with \f$N_E\f$
two-node (linear) finite elements. The residual associated with node k
is given by
\f[ r_{{\cal E}(k)} = \int_0^1 \left( \frac{\mbox{d} 
 u^{(FE)}(x)}{\mbox{d} x} \ 
 \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} + 
 f(x) \  \psi_k(x) \right)  dx. \ \ \ \ \ \ \ \  (20) \f]
The global finite-element shape functions have finite
support, so the integrand is non-zero only in
the two elements \f$k-1\f$ and \f$k\f$, 
adjacent to node \f$k\f$. This allows us to write
\f[ r_{{\cal E}(k)} = \int_{\mbox{Element }k-1} \left( \frac{\mbox{d} 
 u^{(FE)}(x)}{\mbox{d} x} \ 
 \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} \  dx + 
 f(x) \  \psi_k(x) \right)  dx +  
 \int_{\mbox{Element }k} \left( \frac{\mbox{d} 
 u^{(FE)}(x)}{\mbox{d} x} \ 
 \frac{\mbox{d} \psi_k(x)}{\mbox{d} x} \  dx + 
 f(x) \  \psi_k(x) \right)  dx. 
 \ \ \ \ \ \ \  (21) \f]
 Typically (e.g. in the Newton method) 
 we require \e all the residuals \f$ r_k \ (k=1,...,M) \f$ --- 
 the entire residual vector. We could compute
 the entries in this vector by using the above equation to
 calculate each residual \f$ r_{{\cal E}(k)}\f$ individually
 for all (unpinned) nodes \f$k\f$.
 In the process, we would visit each element twice: once for 
 each of the two residuals that are associated with its nodes.
 We can, instead, re-arrange this procedure to consist of a single
 loop over the elements, in which the appropriate contribution is
 added to the global residuals associated with each element's nodes.
 In order to achieve this, we must introduce the concept of local 
 and global node numbers, illustrated in this sketch:
\image html 1Dmesh_local_and_global_node_numbers.gif "Local and global node numbers in a 1D mesh. " 
\image latex 1Dmesh_local_and_global_node_numbers.eps "Local and global node numbers in a 1D mesh. " width=0.75\textwidth
 We label the nodes in each element \f$ e \f$ with local node numbers
 so that for two-node elements, the left (right) node has the
 local node number 1 (2). The relation
 between local and global node numbers can be represented 
 in a simple lookup scheme 
 \f[ j_{global} = {\cal J}(j_{local},e), \f]
 which determines the global node number \f$ j_{global}\f$ of
 local node \f$ j_{local}\f$ in element \f$ e \f$.
 The lookup scheme establishes how the nodes are connected by elements
 and is one of the main steps in the
 "mesh generation" process. For the 1D example above, the lookup scheme
 is given by
\f[ \mbox{
    \begin{tabular}{l l l l l l l l}
    \hline \\
    \mbox{Element $e$} & 1 &  & 2 & & ... & $N_E$ &  \\
    \hline \\
    \mbox{Local node number $j$} & 1 & 2 & 1 & 2 & ... & 1 & 2  \\
    \hline \\
    \mbox{Global node number 
          ${\cal J}(j,e)$} & 1 & 2 & 2 & 3 &...& $N-1$ & 
      $N$ \\
    \hline \\
    \end{tabular}
    }
 \f]
where \f$N=N_E+1\f$.

If we discretise the domain with three-node (quadratic) elements, as in this
sketch, 
\image html 1Dmesh_quadr_elements_local_and_global_eqn_numbers.gif "Local and global node numbers in a 1D mesh with three-node elements. " 
\image latex 1Dmesh_quadr_elements_local_and_global_eqn_numbers.eps "Local and global node numbers in a 1D mesh with three-node elements. " width=0.75\textwidth
the lookup scheme becomes
\f[ \mbox{
    \begin{tabular}{l l l l l l  l l l l l}
    \hline \\
    \mbox{Element $e$} & 1 & &  & 2 & & & ... & $N_E$ & & \\
    \hline \\
    \mbox{Local node number $j$} & 1 & 2 & 3 & 1 & 2 & 3 & ... & 1 & 2 &3\\
    \hline \\
    \mbox{Global node number 
          ${\cal J}(j,e)$} & 1 & 2 & 3 & 3 & 4 & 5 & ...& N-2  & N-1 &N\\ 
     
    \hline \\
    \end{tabular}
    }
 \f]
where \f$N=2 N_E+1\f$.
Provided such a lookup scheme has been constructed, the global residual vector
and the global Jacobian matrix for \f$n\f$-node elements can be assembled with 
the following algorithm 

<CENTER> <B> Algorithm 4: Assembling the residual
vector and the Jacobian matrix using local and global node
numbers </B> </CENTER>
- Initialise the residual vector, \f$ r_k=0 \f$ for \f$k=1,...,M \f$
  and the Jacobian matrix \f$ J_{kj}=0 \f$ for \f$j,k=1,...,M. \f$
- Loop over the elements  \f$e = 1,...,N_E \f$
  - Loop over the local nodes \f$ k_{local}=1,...,n \f$
    - Determine the global node number 
      \f$ k_{global}={\cal J}(k_{local},e) \f$.
    - Determine the global equation number \f${\cal E}(k_{global})\f$.
      - If  \f${\cal E}(k_{global})\ne -1\f$ \n\n
        \f$ \bullet \f$ Add the element's contribution to the residual
          \f[ r_{{\cal E}(k_{global})} =  r_{{\cal E}(k_{global})} + 
          \int_{e} \left( \frac{\mbox{d} 
          u^{(FE)}(x)}{\mbox{d} x} \ 
          \frac{\mbox{d} \psi_{k_{global}}(x)}{\mbox{d} x} + 
          f(x) \  \psi_{k_{global}}(x) \right)  dx \f]
          \f$ \bullet \f$  Loop over the local nodes \f$ j_{local}=1,...,n \f$
          \n\n \f$ \bullet \f$ Determine the global node number 
          \f$ j_{global}={\cal J}(j_{local},e) \f$
          \n\n \f$ \bullet \f$ Determine the global equation number 
          \f${\cal E}(j_{global})\f$
          \n\n \f$ \bullet \f$ If  \f${\cal E}(j_{global})\ne -1\f$: 
          Add the element's contribution to the Jacobian matrix
           \f[ J_{{\cal E}(k_{global}) {\cal E}(j_{global})} =  
               J_{{\cal E}(k_{global}) {\cal E}(j_{global})} + 
            \int_{e} \left( 
            \frac{\mbox{d} \psi_{j_{global}}(x)}{\mbox{d} x} \ 
            \frac{\mbox{d} \psi_{k_{global}}(x)}{\mbox{d} x}
            \right) dx \f]

<HR>


\subsubsection local_eqn_numbers Local and global equation numbers
Each element makes a contribution to only a small number of entries in 
the global residual vector and the Jacobian matrix; namely, those
entries associated with the unknowns stored at the nodes in the
element. In general, each element is associated
with \f$ {\cal N}_{dof}(e)\f$ unknowns, say.
Element \f$e\f$ contributes to \f$ {\cal N}_{dof}(e)\f$ entries
in the \e global residual vector and \f$ {\cal N}^2_{dof}(e)\f$
entries in the \e global Jacobian matrix. In fact, 
the finite support of the shape functions leads to sparse \e global
Jacobian matrices and it would be extremely wasteful to allocate
storage for all its entries, and use Algorithm 4 to
calculate those that are non-zero. 
 Instead, we combine each element's contributions into  
an \f$ {\cal N}_{dof}(e) \times  {\cal N}_{dof}(e) \f$
"element Jacobian matrix" 
\f$ J_{ij}^{(e)}, \ i,j=1,...,{\cal N}_{dof}(e) \f$ 
and "element residual vector" 
\f$ r_{i}^{(e)}, \ i=1,..., {\cal N}_{dof}(e)
\f$. These (dense) matrices and vectors are then assembled into the
global matrix and residuals vector.

 The entries in the element's Jacobian matrix and its 
residual vector are labelled by the "local equation numbers", which range 
from 1 to \f$ {\cal N}_{dof}(e)\f$ and are illustrated in this sketch:
\image html 1Dmesh_local_and_global_node_and_equation_numbers.gif "Local and global node and equation numbers. " 
\image latex 1Dmesh_local_and_global_node_and_equation_numbers.eps "Local and global node and equation numbers. " width=0.75\textwidth

In order to add the elemental contributions to the correct \e global
entries in the residual vector and Jacobian matrix, it is necessary to
translate between the local and global equation numbers; and we introduce
another lookup scheme \f$ \widehat{\cal E}(i_{local},e)\f$ that stores
the global equation number corresponding to local equation \f$i_{local}\f$
in element \f$e\f$. The lookup scheme can be generated by the following
algorithm

 <CENTER> <B> Algorithm 5: Establishing the relation
 between local and global equation numbers </B> </CENTER>

- Construct the global equation numbering scheme, using the algorithm detailed
  in Phase 1 of Algorithm 3.
- Loop over the elements  \f$ e=1,...,N_e \f$
  - Initialise the counter for the number of degrees of 
    freedom in the element, \f$ j_{dof}=0 \f$.
  - Loop over the element's local nodes \f$ j_{local}=1,...,n\f$
    - Determine the global node number 
      \f$ j_{global} = {\cal J}(j_{local},e) \f$
    - Determine the global equation number \f${\cal E}(j_{global})\f$
    - If  \f${\cal E}(j_{global}) \ne -1\f$:
      <TABLE  BORDER="0">
      <TR>
      <TD bgcolor="cornsilk">
      - Increment the number of degrees of 
        freedom  in the element, \f$ j_{dof}=j_{dof}+1\f$
      </TD>
      </TR>
      </TABLE>
      - Add the entry to the lookup scheme that relates local and
        global equation numbers,
        \f$ \widehat{\cal E}(j_{dof},e) = {\cal E}(j_{global})\f$
      .
    .
  - Assign the number of degrees of freedom in the element,
    \f$ {\cal N}_{dof}(e)=j_{dof} \f$
  .
.

For the 1D problem P1 with two-node elements, the lookup scheme
has the following form:
\f[ \mbox{
    \begin{tabular}{l l  l  l  l  l  l  l l  l  l }
    \hline \\
    \mbox{Element $e$} & 1 & 2 & & 3 & & ... & $N_E-1$ & & $N_E$  \\
    \hline \\
    \mbox{Number of degrees of freedom ${\cal N}_{dof}(e)$} &
    1 & 2 & & 2 & & ... & 2 & & 1\\ 
    \hline \\
    \mbox{Local equation number $i_{local}$} & 1 & 1 & 2 & 1 & 2 & ...
     & 1 & 2 & 1\\
    \hline \\
    \mbox{Global equation number $\widehat{\cal E}(i_{local},e)$}   
     & 1 & 1 & 2 & 2 & 3 & ... & $N-3$ & $N-2$ & $N-2$ \\
    \hline \\
    \end{tabular}
    }
 \f]



Using this lookup scheme, we can re-arrange the computation of the 
residual vector and the Jacobian matrix as follows:

<CENTER> <B> Algorithm 6: Element-based assembly
of the residual vector and the Jacobian matrix </B> </CENTER>
- Initialise the global residual vector, \f$ r_j=0 \f$ for \f$j=1,...,M \f$
  and the global Jacobian matrix \f$ J_{jk}=0 \f$ for \f$j,k=1,...,M \f$
- Loop over the elements  \f$ e=1,...,N_E\f$
  \n\n   
  <CENTER> <B> Compute the element's residual vector and 
  Jacobian matrix </B>  </CENTER>
  \n
  \f$ \bullet \f$ Determine the number of degrees of freedom in this element,
  \f$ n_{dof}={\cal N}_{dof}(e) \f$. \n
  \f$ \bullet \f$  Initialise the counter for the local degrees of freedom
  \f$ i_{dof}=0\f$ (counting the entries in the element's residual vector
  and the rows of the element's Jacobian matrix) \n
  - Loop over the local nodes \f$ j_{local}=1,...,n \f$ \n
    \f$ \bullet \f$ Determine the global node number 
    \f$ j_{global}={\cal J}(j_{local},e)\f$ \n
    \f$ \bullet \f$ Determine the global equation number 
    \f$ {\cal E}( j_{global})
    \f$ \n 
    \f$ \bullet \f$  If  \f$ {\cal E}( j_{global}) \ne -1 \f$: 
    <TABLE BORDER="0">
    <TR>
    <TD bgcolor="cornsilk">
    \f$ \bullet \f$ Increment the counter for the local degrees of freedom
    \f$ i_{dof}= i_{dof}+1\f$
    </TD>
    </TR>
    </TABLE>
    \f$ \bullet \f$ Determine the entry in the element's residual vector
    \f[ r_{i_{dof}}^{(e)} =
    \int_{e} \left( \frac{\mbox{d} 
    u^{(FE)}(x)}{\mbox{d} x} \ 
    \frac{\mbox{d} \psi_{j_{global}}(x)}{\mbox{d} x} + 
    f(x) \  \psi_{j_{global}}(x) \right)  dx \f] 
    \f$ \bullet \f$ Initialise the second counter for the local 
    degrees of freedom \f$ j_{dof}=0\f$ (counting the columns 
    in the element's  Jacobian matrix) \n\n
    \f$ \bullet \f$ Loop over the local nodes \f$ k_{local}=1,...,n \f$
    - Determine the global node number 
      \f$ k_{global}={\cal J}(k_{local},e)\f$
    - Determine the global equation number 
      \f$ {\cal E}( k_{global}) \f$
    - If  \f$ {\cal E}( k_{global}) \ne -1 \f$:
      <TABLE  BORDER="0">
      <TR> 
      <TD bgcolor="cornsilk">
      \f$ \bullet \f$ Increment the counter for the local 
      degrees of freedom \f$ j_{dof}= j_{dof}+1\f$
      </TD>
      </TR>
      </TABLE>
      \f$ \bullet \f$ Determine the entry in the element's 
      Jacobian matrix
      \f[ J_{i_{dof}j_{dof}}^{(e)} =  
      \int_{e} \left( 
      \frac{\mbox{d} \psi_{k_{global}}(x)}{\mbox{d} x} \ 
      \frac{\mbox{d} \psi_{j_{global}}(x)}{\mbox{d} x}
      \right) dx \f]
    .
  . 
  \n  \n    
  <CENTER> <B> Add the element's contribution to the global residual
  vector and Jacobian matrix </B>  </CENTER>
  \n
  - Loop over the local degrees of freedom \f$ i_{dof}=1,...,n_{dof} \f$
    - Add the element's contribution to the global residual vector
      \f[ r_{\widehat{\cal E}(i_{dof},e)} =  
      r_{\widehat{\cal E}(i_{dof},e)} + 
      r_{i_{dof}}^{(e)} \f]
      - Loop over the local degrees of freedom \f$ j_{dof}=1,...,n_{dof}\f$
        \n\n  \f$ \bullet \f$ Add the element's contribution to the global 
        Jacobian matrix
        \f[ J_{\widehat{\cal E}(i_{dof},e)\widehat{\cal E}(j_{dof},e) } =  
        J_{\widehat{\cal E}(i_{dof},e)\widehat{\cal E}(j_{dof},e) } + 
        J_{i_{dof} j_{dof}}^{(e)} \f]
      .
    .
  .
.


Note that the order in which we loop over the local degrees of freedom
within each element \e must be the same as the order used when
constructing the local equation numbering scheme of Algorithm
5. [<B>Exercise:</B> What would happen if we
reversed the order in which we loop over the element's nodes
in Algorithm 5 while retaining Algorithm 
6 in its present form? Hint: Note that the local equation
numbers are computed "on the fly" by the highlighted sections
of the two algorithms.]
In an
actual implementation of the above procedure, Algorithms
5 and  6 are likely to be contained
in separate functions. When the functions are first implemented
(in the form described above), they will obviously be consistent
with each other. However, there is a danger that in subsequent
code revisions changes might only be introduced in one of the two 
functions. To avoid the potential for such disasters, it 
is preferable to create an explicit
storage scheme for the local equation numbers that is constructed
during the execution of Algorithm 5 and used in 
Algorithm 6. For this purpose, we introduce
yet another lookup table, \f$ {\cal L}(j_{local},e)\f$,
which stores the local equation number associated with the
nodal value stored at local node \f$ j_{local}\f$ in element
\f$e\f$. Again we
set the equation number to -1 if the nodal value is pinned. 
The revised form of Algorithm 5 is then given by
(as before, only the sections that are highlighted have been changed):

 <CENTER> <B> Algorithm 7: Establishing the relation
 between local and global equation numbers (revised) </B> </CENTER>

- Set up the global equation numbering scheme, using the algorithm detailed
  in Phase 1 of Algorithm 3.
- Loop over the elements  \f$ e=1,...,N_e \f$
  - Initialise the counter for the number of degrees of 
    freedom in the element, \f$ j_{dof}=0 \f$.
  - Loop over the element's local nodes \f$ j_{local}=1,...,n\f$
    - Determine the global node number 
      \f$ j_{global} = {\cal J}(j_{local},e) \f$
    - Determine the global equation number \f${\cal E}(j_{global})\f$
    - If  \f${\cal E}(j_{global}) \ne -1\f$:
      - Increment the number of degrees of 
        freedom  in the element, \f$ j_{dof}=j_{dof}+1\f$
      - Add the entry to the lookup scheme that relates local and
        global equation numbers,
        \f$ \widehat{\cal E}(j_{dof},e) = {\cal E}(j_{global})\f$
      - <TABLE  BORDER="0">
        <TR>
        <TD bgcolor="cornsilk">
        Store the local equation number associated with the
        current local node: \f$ {\cal L}(j_{local},e)=j_{dof}.\f$
        </TD>
        </TR>
        </TABLE>
    - Else:
      <TABLE  BORDER="0">
      <TR>
      <TD bgcolor="cornsilk">
      - Set the local equation number associated with the
        current local node to -1 to indicate that it is pinned:
        \f$ {\cal L}(j_{local},e)=-1\f$
      </TD>
      </TR>
      </TABLE>
    .
  - Assign the number of degrees of freedom in the element,
    \f$ {\cal N}_{dof}(e)=j_{dof} \f$


For the 1D problem P1 with two-node elements the elements,
the lookup table \f$ {\cal L}(j_{local},e)\f$ has the following entries:
\f[ \mbox{
    \begin{tabular}{l   l l    l l  l l   l   l l    l l }
    \hline \\
    \mbox{Element $e$} & 1 & & 2 & & 3 & & ... & $N_E-1$ & & $N_E$ & \\
    \hline \\
    \mbox{Local node number $j_{local}$} &
    1 & 2 & 1 & 2 & 1 & 2 &  ... & 1 & 2 & 1 & 2 \\ 
    \hline \\
    \mbox{Local equation number ${\cal L}(j_{local},e)$}
     & -1 & 1 & 1 & 2 & 1 & 2 & ... & 1 & 2 & 1 & -1\\
    \hline \\
    \end{tabular}
    }
 \f]
Using this lookup scheme, we revise Algorithm 6 as
follows (only the highlighted regions have changed; we have removed
the initialisation of the "counters" for the equation numbers
since they are no longer computed "on the fly"):

 <CENTER> <B> Algorithm 9: Element-based assembly
 of the residual vector and the Jacobian matrix (revised) </B> </CENTER>

- Initialise the global residual vector, \f$ r_j=0 \f$ for \f$j=1,...,M \f$
  and the global Jacobian matrix \f$ J_{jk}=0 \f$ for \f$j,k=1,...,M \f$
- Loop over the elements  \f$ e=1,...,N_E\f$
  \n\n   
  <CENTER> <B> Compute the element's residual vector and 
  Jacobian matrix </B>  </CENTER>
  \n
  \f$ \bullet \f$ Determine the number of degrees of freedom in this element,
    \f$ n_{dof}={\cal N}_{dof}(e) \f$. \n
  \f$ \bullet \f$ Loop over the local nodes \f$ j_{local}=1,...,n \f$ \n
  - Determine the global node number 
    \f$ j_{global}={\cal J}(j_{local},e)\f$
  - Determine the global equation number \f$ {\cal E}( j_{global}) \f$ \n
    \f$ \bullet \f$ If  \f$ {\cal E}( j_{global}) \ne -1 \f$: \n
    <TABLE  BORDER="0">
    <TR>
    <TD bgcolor="cornsilk">
    \f$ \bullet \f$ Determine the local equation number from the element's 
       lookup scheme \f$ i_{dof}= {\cal L}(j_{local},e)\f$.
    </TD>
    </TR>
    </TABLE>
    \f$ \bullet \f$ Determine the entry in the element's residual vector
    \f[ r_{i_{dof}}^{(e)} =
    \int_{e} \left( \frac{\mbox{d} 
    u^{(FE)}(x)}{\mbox{d} x} \ 
    \frac{\mbox{d} \psi_{j_{global}}(x)}{\mbox{d} x} + 
    f(x) \  \psi_{j_{global}}(x) \right)  dx \f]
    \f$ \bullet \f$ Loop over the local nodes \f$ k_{local}=1,...,n \f$
    - Determine the global node number 
      \f$ k_{global}={\cal J}(k_{local},e)\f$
    - Determine the global equation number 
      \f$ {\cal E}( k_{global}) \f$
    - If  \f$ {\cal E}( k_{global}) \ne -1 \f$:
      <TABLE  BORDER="0">
      <TR>
      <TD bgcolor="cornsilk">
      - Determine the local equation number from the element's 
        lookup scheme \f$ j_{dof}= {\cal L}(k_{local},e)\f$.
        </TD>
        </TR>
        </TABLE>
      - Determine the entry in the element's Jacobian matrix
        \f[ J_{i_{dof}j_{dof}}^{(e)} =  
        \int_{e} \left( 
        \frac{\mbox{d} \psi_{k_{global}}(x)}{\mbox{d} x} \ 
        \frac{\mbox{d} \psi_{j_{global}}(x)}{\mbox{d} x}
        \right) dx \f]
      .
    .
    \n  \n   
     <CENTER> <B> Add the element's contribution to the global residual
     vector and Jacobian matrix </B>  </CENTER>
    \n
  - Loop over the local degrees of freedom \f$ i_{dof}=1,...,n_{dof} \f$
    - Add the element's contribution to the global residual vector
      \f[ r_{\widehat{\cal E}(i_{dof},e)} =  r_{\widehat{\cal E}(i_{dof},e)} + 
              r_{i_{dof}}^{(e)} \f]
    - Loop over the local degrees of freedom \f$ j_{dof}=1,...,n_{dof}\f$
      - Add the element's contribution to the global Jacobian matrix
        \f[ J_{\widehat{\cal E}(i_{dof},e)\widehat{\cal E}(j_{dof},e) } =  
        J_{\widehat{\cal E}(i_{dof},e)\widehat{\cal E}(j_{dof},e) } + 
        J_{i_{dof} j_{dof}}^{(e)} \f]
      .
    .
  .
.

<HR>

\subsubsection local_coords Local coordinates
Algorithm 9 computes the residual
vector and the Jacobian matrix using an
element-by-element assembly process. The basis functions are still 
based on a global
definition (17) that involves 
unnecessary references to quantities external to the element. 
For example, in element \f$ j \f$
the tests for \f$ x < X_j\f$ and \f$ x > X_{j+1}\f$ in 
(17) are
unnecessary because these coordinate ranges are always outside the 
element. We shall now develop an alternative, local representation of
the shape functions that involves only quantities that
are intrinsic to each element.  
For this purpose, we introduce a local coordinate \f$s \in [-1,1] \f$ that
parametrises the position \f$ x(s) \f$ within an element so 
that (for two-node elements) the local
coordinates \f$s=\pm 1\f$ correspond to local nodes 1 and 2, respectively.
The local linear shape functions
\f[ \psi_1(s) = \frac{1}{2}(1-s) \mbox{\ \ \ and \ \ \ } 
    \psi_2(s) = \frac{1}{2}(1+s)\f] 
are the natural generalisations of the global shape functions: 
- \f$ \psi_j(s) \f$ is equal to 1 at local node j and zero at the 
   element's other node,
- \f$ \psi_j(s) \f$ varies linearly between the nodes. 
.
These local shape functions are easily generalised to 
elements with a larger number of nodes.
For instance, the local shape functions for a three-node element
whose nodes are distributed uniformly along the element, 
are given by
\f[ \psi_1(s) = \frac{1}{2} s (s-1), \ \ \ 
    \psi_2(s) = (s+1)(1-s) \mbox{\ \ \ and \ \ \ } 
    \psi_3(s) = \frac{1}{2} s (s+1). \f] 
We represent the solution within element \f$ e \f$ as
\f[ u^{(FE)}(s) = \sum_{j=1}^n U_{{\cal J}(j,e)} \ \psi_j(s),  \f]
where \f$n\f$ is the number of nodes in the element.
\f$ u \f$ is now represented exclusively in terms of
quantities that are intrinsic to the element: the element's nodal
values and the local coordinate. The evaluation 
of the integrals in algorithm 2 requires the evaluation of 
\f$ {\mbox{d} u}/{\mbox{d} x}\f$, rather than 
\f$ {\mbox{d} u}/{\mbox{d} s}\f$, and  \f$f(x)\f$ rather than \f$f(s).\f$
In order to evaluate these terms,  we must specify the mapping
\f$ x(s) \f$ between the local  and global coordinates. 
The mapping \f$ x(s) \f$ should be one-to-one and 
it must interpolate the nodal positions so that in a two-node 
element \f$e\f$
\f[ x(s=-1) = X_{{\cal J}(1,e)} \mbox{\ \ \ and \ \ \ }
  x(s=1) = X_{{\cal J}(2,e)}.\f]
There are many mappings that satisfy these
conditions but, within the finite-element context, the simplest
choice is to use the local shape functions themselves by writing
\f[ x(s) = \sum_{j=1}^n X_{{\cal J}(j,e)} \ \psi_j(s).  \f]
This is known as an "isoparametric mapping" because the same ("iso") 
functions are used to interpolate the unknown function \e and the
global coordinates. Derivatives with respect to the global coordinates
can now be evaluated via the chain rule
\f[ \frac{\mbox{d} u}{\mbox{d} x} = \frac{\mbox{d} u}{\mbox{d} s} 
     \left(\frac{\mbox{d} x}{\mbox{d} s}\right)^{-1}. \f]
In element \f$ e \f$,
\f[  \frac{\mbox{d} u}{\mbox{d} x} =
     \left(\sum_{j=1}^n U_{{\cal J}(j,e)} \ 
     \frac{\mbox{d} \psi_j(s)}{\mbox{d} s}
     \right)
     \left(\sum_{j=1}^n X_{{\cal J}(j,e)} \
     \frac{\mbox{d} \psi_j(s)}{\mbox{d} s}
     \right)^{-1}.
\f] 
Finally, integration over the element can be
performed in local coordinates via
\f[ \int_e (...) \ dx = 
\int_{-1}^1 (...)\ \widehat{\cal J} \ ds, \f]
where 
\f[ \widehat{\cal J} = \frac{\mbox{d} x}{\mbox{d} s} \f]
 is the Jacobian of 
the mapping between \f$x\f$ and \f$ s. \f$

Typically the integrands are too complicated to be evaluated
analytically and we use Gauss quadrature rules to evaluate
them numerically. Gauss rules (or any other quadrature rules)
are defined by
- the number of integration points \f$N_{int} \f$,
- the position of the integration points in the element 
  \f$S_{i}, \ i=1,...,N_{int} \f$,
- the weights \f$W_{i}, \ i=1,...,N_{int} \f$,

and approximate the integral over the range 
\f$ s\in [-1,1]\f$ by the sum
\f[ 
\int_{-1}^1 {\cal F}(s)\  ds \approx 
\sum_{i=1}^{N_{int}} W_i \ {\cal F}(S_i). \f]

As an example, the integration points and weights for a three-point 
Gauss rule are given by
\f[ \mbox{
    \begin{tabular}{l  c  c  c}
    \hline \\
    \mbox{Integration point $i$} & 1 & 2 & 3   \\
    \hline \\
    \mbox{Location $S_i$} & $-\sqrt{3/5}$ & $ 0$ & $\sqrt{3/5}$ \\
    \hline \\
    \mbox{Weight $W_i$} & 5/9 & 8/9 & 5/9 \\
    \hline \\
    \end{tabular}
    }
 \f]


 <HR>



\subsubsection altogether Putting it all together

We have now developed the necessary tools to formulate 
 the final version of the finite element solution of problem P1
 which we summarise in the following algorithm:


 <CENTER> <B> Algorithm 10: The final version of
 the finite-element solution of problem P1 </B> </CENTER>
 <B> Phase 1: Setup</B> 

 <B> Phase 1a: Problem specification</B> 

- Choose the number of elements, \f$ N_E \f$, and the number of 
  nodes per element, \f$ n\f$. This defines the total number of 
  nodes, \f$N\f$, and the local shape functions  
  \f$ \psi_j(s) \ \ (j=1,...,n)\f$  for all elements. 

 <B> Phase 1b: Mesh generation</B> 

- Discretise the domain by specifying the positions 
  \f$ X_j \ (j=1,..,N)\f$,
  of the \f$ N \f$ nodes. 
- Generate the lookup scheme \f$ {\cal J}(j,e)\f$ that 
   establishes the relation
   between global and local node numbers. 
- Identify which nodes are located on which domain boundaries. 

 <B> Phase 1c: "Pin" nodes with essential (Dirichlet) 
     boundary conditions </B>  
- Loop over all global nodes \f$ j \f$ that are located 
  on Dirichlet boundaries:
  - Assign a negative equation number to reflect the node's "pinned" 
    status:
    \f[ {\cal E}(j) = -1 \f]

 <B> Phase 1d: Apply boundary conditions and provide initial guesses 
     for all unknowns</B> 
- Loop over all global nodes \f$j=1,...,N\f$:
   - Provide an initial guess for the unknown nodal value (e.g. 
     \f$ U_j=0 \f$), while ensuring that the values assigned to
     nodes on the boundary are consistent with the boundary conditions
     so that  \f$ U_j = g(X_j). \f$

 <B> Phase 1e: Set up the global equation numbering scheme</B> 

- Initialise the total number of unknowns, \f$M=0.\f$
- Loop over all global nodes \f$j=1,...,N\f$:
  - If global node \f$ j \f$ is not pinned
    (i.e. if \f$ {\cal E}(j) \ne -1 \f$) 
     - Increment the number of unknowns
        \f[ M=M+1 \f]
     - Assign the global equation number
        \f[ {\cal E}(j) = M \f]

 <B> Phase 1f: Set up the local equation numbering scheme</B> 

- Loop over the elements  \f$ e=1,...,N_e \f$
  - Initialise the counter for the number of degrees of 
    freedom in this element, \f$ j_{dof}=0 \f$.
  - Loop over the element's local nodes \f$ j_{local}=1,...,n\f$
    - Determine the global node number 
      \f$ j_{global} = {\cal J}(j_{local},e) \f$
    - Determine the global equation number \f${\cal E}(j_{global})\f$
    - If  \f${\cal E}(j_{global}) \ne -1\f$:
      - Increment the number of degrees of 
        freedom in this element \f$ j_{dof}=j_{dof}+1\f$
      - Add the entry to the lookup scheme that relates global
        and local equation numbers,
        \f$ \widehat{\cal E}(j_{dof},e) = {\cal E}(j_{global})\f$
      - Store the local equation number associated with the
        current local node: \f$ {\cal L}(j_{local},e)=j_{dof}.\f$
      .
    - Else:
      - Set the local equation number associated with the
        current local node to -1 to indicate that it is pinned:
        \f$ {\cal L}(j_{local},e)=-1\f$
    .
  - Assign the number of degrees of freedom in the element
    \f$ {\cal N}_{dof}(e)=j_{dof} \f$

 <B> End of setup:</B> 

 The setup phase is now complete and we can determine the 
 current FE representation of  
 \f$ u^{(FE)}(x)\f$ and \f$ \mbox{d} u^{(FE)}(x)/\mbox{d} x\f$ 
 in any element \f$e \ (e=1,..,N_E) \f$ from
 \f[ u = \sum_{j=1}^{n}  U_{{\cal J}(j,e)} \psi_j(s) 
    \mbox{\ \ \ \ \ \ \ \  and \ \ \ \ \ \ \ \ }
    \frac{\mbox{d} u}{\mbox{d} x} =
     \left(\sum_{j=1}^n U_{{\cal J}(j,e)}
     \ \frac{\mbox{d} \psi_j(s)}{\mbox{d} s}
     \right)
     \left(\sum_{j=1}^n X_{{\cal J}(j,e)}
     \ \frac{\mbox{d} \psi_j(s)}{\mbox{d} s}
     \right)^{-1} \f]
 Similarly, the global coordinates and their derivatives 
 with respect to the local coordinates are given by
 \f[ x = \sum_{j=1}^{n}  X_{{\cal J}(j,e)} \psi_j(s) 
    \mbox{\ \ \ \ \ \ \ \  and \ \ \ \ \ \ \ \ }
    \widehat{\cal J} = \frac{\mbox{d} x}{\mbox{d} s} =
     \sum_{j=1}^n X_{{\cal J}(j,e)}
     \ \frac{\mbox{d} \psi_j(s)}{\mbox{d} s}.
 \f]

 The derivatives of the local shape functions, \f$ \psi_j \f$, with respect
 to the global coordinate \f$ x \f$ are
  \f[  \frac{\mbox{d} \psi_j}{\mbox{d} x} =
       \frac{\mbox{d} \psi_j(s)}{\mbox{d} s}  
       \left(\sum_{j=1}^n X_{{\cal J}(j,e)}
       \ \frac{\mbox{d} \psi_j(s)}{\mbox{d} s}
        \right)^{-1} \mbox{\ \ \ for $j=1,..,n$} 
  \f]

 <B> Phase 2: Solution  </B> 
 
 <B> Phase 2a: Set up the linear system </B> 

- Initialise the global residual vector, \f$ r_j=0 \f$ for \f$j=1,...,M \f$
  and the global Jacobian matrix \f$ J_{jk}=0 \f$ for \f$j,k=1,...,M \f$
- Loop over the elements  \f$ e=1,...,N_E\f$
  \n\n   
  <CENTER> <B> Compute the element's residual vector and 
  Jacobian matrix </B>  </CENTER>
  \n
  \f$ \bullet \f$ Determine the number of degrees of freedom in this element,
  \f$ n_{dof}={\cal N}_{dof}(e) \f$. \n
  \f$ \bullet \f$ Initialise the element residual vector, 
  \f$ r_{j}^{(e)}=0 \f$ for \f$j=1,...,n_{dof} \f$
  and the element Jacobian matrix \f$ J_{jk}^{(e)}=0 \f$ 
  for \f$j,k=1,...,n_{dof} \f$ \n
  \f$ \bullet \f$ Loop over the Gauss points \f$
  i_{int}=1,...,N_{int}\f$ \n
  - Determine the local coordinate of the integration point
    \f$ s = S_{i_{int}} \f$ and the associated weight \f$W_{i_{int}}.\f$
  - Compute \n
      \f$ \bullet \f$ the global coordinate  \f$ x = x(s) \f$\n
      \f$ \bullet \f$ the source function    \f$ f = f(x) =f(x(s)) \f$\n
      \f$ \bullet \f$ the derivative \f$ \mbox{d} u/\mbox{d} x \f$ \n
      \f$ \bullet \f$ the shape functions  \f$ \psi_j \f$ and their derivatives
      \f$ \mbox{d} \psi_j/\mbox{d} x \f$\n
      \f$ \bullet \f$ the Jacobian of the mapping between local and global
      coordinates, \f$ \widehat{\cal J} = \mbox{d} x/\mbox{d} s\f$\n
  - Loop over the local nodes \f$ j_{local}=1,...,n \f$
    - Determine the global node number 
      \f$ j_{global}={\cal J}(j_{local},e)\f$
    - Determine the global equation number 
      \f$ {\cal E}( j_{global}) \f$
    - If  \f$ {\cal E}( j_{global}) \ne -1 \f$ \n
      \f$ \bullet \f$ Determine the local equation number from the element's 
      lookup scheme \f$ i_{dof}= {\cal L}(j_{local},e)\f$.\n
      \f$ \bullet \f$ Add the contribution to the element's residual vector
      \f[ r_{i_{dof}}^{(e)} = r_{i_{dof}}^{(e)} +
      \left( \frac{\mbox{d} 
      u}{\mbox{d} x} \ 
      \frac{\mbox{d} \psi_{j_{local}}}{\mbox{d} x} + 
      f \  \psi_{j_{local}} \right) 
      \widehat{\cal J}\  W_{i_{int}} \f]
      \f$ \bullet \f$ Loop over the local nodes \f$ k_{local}=1,...,n\f$ 
      - Determine the global node number 
        \f$ k_{global}={\cal J}(k_{local},e)\f$
      - Determine the global equation number 
        \f$ {\cal E}( k_{global}) \f$
      - If  \f$ {\cal E}( k_{global}) \ne -1 \f$ \n
        \f$ \bullet \f$ Determine the local equation number from the element's 
        lookup scheme \f$ j_{dof}= {\cal L}(k_{local},e)\f$.\n
        \f$ \bullet \f$ Add the contribution to the element's Jacobian matrix
        \f[ J_{i_{dof}j_{dof}}^{(e)} = J_{i_{dof}j_{dof}}^{(e)} + 
        \left( 
        \frac{\mbox{d} \psi_{k_{local}}}{\mbox{d} x} \ 
        \frac{\mbox{d} \psi_{j_{local}}}{\mbox{d} x}
        \right)
        \widehat{\cal J} \ W_{i_{int}}  \f]
      .
    .
    \n  \n   
     <CENTER> <B> Add the element's contribution to the global residual
     vector and Jacobian matrix </B>  </CENTER>
    \n
  - Loop over the local degrees of freedom \f$ i_{dof}=1,...,n_{dof} \f$
    - Add the element's contribution to the global residual vector
      \f[ r_{\widehat{\cal E}(i_{dof},e)} =  r_{\widehat{\cal E}(i_{dof},e)} + 
      r_{i_{dof}}^{(e)} \f]
    - Loop over the local degrees of freedom \f$
      j_{dof}=1,...,n_{dof}\f$ \n
      \f$\bullet \f$  Add the element's contribution to the 
      global Jacobian matrix
      \f[ J_{\widehat{\cal E}(i_{dof},e)\widehat{\cal E}(j_{dof},e) } =  
      J_{\widehat{\cal E}(i_{dof},e)\widehat{\cal E}(j_{dof},e) } + 
      J_{i_{dof} j_{dof}}^{(e)} \f]
    .
  .
 <B> Phase 2b: Solve the linear system </B> 

- Solve the \f$ M \times M\f$ linear system
  \f[ \sum_{j=1}^{M}  J_{jk} \ y_k = - r_j
        \mbox{\ \  \ for $j=1,...,M.$} \f]
  for \f$ y_k \ \ (k=1,...,M).\f$ 

 <B> Phase 2c: Update the initial guess </B> 

-  Loop over all global nodes \f$ j\f$:
        - Determine the equation number: \f$ {\cal E}(j) \f$
        - If \f$ {\cal E}(j) \ne -1 \f$:
   \f[ U_{{\cal E}(j)} =   U_{{\cal E}(j)} +  y_{{\cal E}(j)} \f] 

- Since P1 is a linear problem, \f$ U_j  \ (j=1,...,N) \f$
  is the exact solution; if the problem was nonlinear, we would
  have to continue the \ref Newton iteration until the residuals
  \f$r_j \ (j=1,...,M) \f$ are sufficiently
  small.

 <B> Phase 3: Postprocessing (document the solution)</B> 
- The finite-element solution in element \f$e\f$ is 
  given by 
  \f[ u = \sum_{j=1}^{n}  U_{{\cal J}(j,e)} \psi_j(s) \f]


 <HR>

 \subsubsection example_P1 A 1D example

Here is an example of the finite element solution of the problem
P1,
\f[ \frac{\mbox{d}^2 u}{\mbox{d} x^2} =  \pm 30 \sin(5.5 x), 
  \mbox{\ \ \ subject to $u(0)=0$ and $u(1)=\mp 1$} \f]
This problem has the (fish-shaped) exact solution
\f[ u(x) =  \mp 
  \left(\sin(5.5)-1\right) x - \sin(5.5 x).
\f]

The figure below compares the exact solution against the finite-element
solution, obtained from a discretisation wth ten two-node elements. 
The finite-element solution was computed with \c oomph-lib, using
the driver codes discussed in the 
<A HREF="../../quick_guide/html/index.html">Quick Guide</A> and
the example code that illustrates the solution of a 
<A HREF="../../poisson/one_d_poisson/html/index.html">1D Poisson equation
</A> with elements from the \c oomph-lib element library.

\image html fish.gif "Exact (green) and finite-element solution (red) obtained with ten two-node elements. " 
\image latex fish.eps "Exact (green) and finite-element solution (red) obtained with ten two-node elements. " width=0.75\textwidth

<HR>
<HR>

\section higher_dims Extension to higher-dimensional problems


Thus far, we have only discussed the implementation of the finite element
method for a 1D scalar problem. 
For such (relatively trivial) problems, the sophisticated 
"machinery" of Algorithm 10 might appear to be 
overly complicated.
The advantage of the algorithm is that it can easily be
generalised to more complicated problems. We will illustrate this 
by extending the algorithm to higher-dimensional problems and 
consider the solution of the 2D Poisson problem P2 

\f[ {\cal R}(x_i; u(x_i)) = \frac{\partial^2 u(x_i)}{\partial x_i^2} -
f(x_i) =0 \mbox{ \ \ \ subject to \ \ \ }  u|_{\partial D} = g, \f]
in the non-trivial (fish-shaped) domain shown in this sketch:
\image html fish_domain.gif "Fish-shaped domain " 
\image latex fish_domain.eps "Fish-shaped domain " width=0.75\textwidth
A careful inspection of Algorithm 10 shows that only 
very few steps
need to be modified to implement this extension.
Specifically, we need to
- modify the mesh generation process so that it
   - decomposes the domain into 2D elements
   - assigns two coordinates for each nodal point.
   .
- define suitable (local) shape functions for the elements
- implement the computation of the elements' residuals and 
 Jacobian matrices which correspond to the (integrated-by-parts)
 weak form of problem P2,
\f[ r_k = \int_D \sum_{l=1}^2 \frac{\partial  u(x_i)}{\partial x_l} \ 
 \frac{\partial \psi_k(x_i)}{\partial x_l} \  dx_1 dx_2 +
 \int_D f(x_i) \  \psi_k(x_i) \  dx_1 dx_2 =0. \f]

There are many ways to decompose the 
2D domain into finite elements. The figure below 
shows a discretisation using four-node quadrilaterals: a generalisation
of the two-node 1D elements used in problem P1:
\image html fish_domain_with_mesh.gif "Fish-shaped domain discretised with four-node quadrilateral " 
\image latex fish_domain_with_mesh.eps "Fish-shaped domain discretised with four-node quadrilateral " width=0.75\textwidth
Each node in the mesh now has two coordinates and we
denote the \f$i\f$-th coordinate of nodal point \f$j\f$ by
\f$ X_{ij}\f$ where \f$ i=1,2 \f$.
We parametrise the 2D elements by two local coordinates,
\f$ s_i \in [-1,1] \ (i=1,2)\f$, where the local nodes 
1,2,3 and 4 are located 
at \f$(s_1,s_2)=(-1,-1), (1,-1), (-1,1)\f$ and \f$(1,1)\f$, respectively.
We define the local shape functions as tensor products
of the corresponding 1D linear shape functions,
\f[ \psi_1(s_1,s_2) = \frac{1}{4}(1-s_1)(1-s_2) \f]
\f[ \psi_2(s_1,s_2) = \frac{1}{4}(1+s_1)(1-s_2) \f]
\f[ \psi_3(s_1,s_2) = \frac{1}{4}(1-s_1)(1+s_2) \f]
\f[ \psi_4(s_1,s_2) = \frac{1}{4}(1+s_1)(1+s_2), \f]
and note that shape function \f$ \psi_j(s_1,s_2)\f$ is equal to 1
at local node \f$j\f$ and it vanishes at the other local nodes, as required.
The sketch below illustrates the corresponding global shape 
function associated with a node in the interior of the domain:
\image html fish_domain_with_shape_fct.gif "Discretised fish-shaped domain with a single (global) shape " 
\image latex fish_domain_with_shape_fct.eps "Discretised fish-shaped domain with a single (global) shape " width=0.75\textwidth
The solution \f$u\f$ and the coordinates
\f$x_i\f$ in an isoparametric element \f$e\f$ can be written as
\f[ u = \sum_{j=1}^n U_{{\cal J}(j,e)} \ \psi_j(s_1,s_2) \f]
and 
\f[ x_i = \sum_{j=1}^n X_{i{\cal J}(j,e)} \  \psi_j(s_1,s_2). \f]

In 1D, the Jacobian of the mapping between \f$x\f$ and \f$s\f$
is given by \f$ \widehat{\cal J} = dx/ds\f$; in 2D the derivatives 
transform according to the linear system
\f[
\left(
\begin{array}{c}
 \frac{\partial }{\partial s_1} \\ 
 \frac{\partial }{\partial s_2}
 \end{array}
\right)
= 
\left(
\begin{array}{cc}
 \frac{\partial x_1 }{\partial s_1} & \frac{\partial x_2}{\partial s_1} \\ 
 \frac{\partial x_1 }{\partial s_2} & \frac{\partial x_2}{\partial s_2}
 \end{array}
\right)
\left(
\begin{array}{c}
 \frac{\partial }{\partial x_1} \\ 
 \frac{\partial }{\partial x_2}
 \end{array}
\right),
\f]
and
\f[
\frac{\partial u}{\partial x_i} = \widehat{\cal J}_{ij}^{-1}
\frac{\partial u}{\partial s_j}
\f]
where \f$ \widehat{\cal J}_{ij}^{-1} \f$ is the inverse of the Jacobian
of the mapping (not to be confused with the Jacobian matrix in the Newton 
method!),
\f[
 \widehat{\cal J}_{ij} = \frac{\partial x_i}{\partial s_j} = 
 \sum_{k=1}^n X_{i{\cal J}(k,e)}
 \frac{\partial \psi_k(s_1,s_2)}{\partial s_j}.
\f]
If
\f[
 \widehat{\cal J} = \det \widehat{\cal J}_{ij}
\f]
then the integrals over the elements are
\f[
\int \int_e (...) \ dx_1 dx_2 =  
\int_{-1}^{1} \int_{-1}^{1} (...) \ \widehat{\cal J} \ ds_1 ds_2.
\f]

The integrals over the elements can be evaluated by 2D
Gauss rules
\f[ \int_{-1}^1  \int_{-1}^1 {\cal F}(s_1,s_2) \ ds_1 ds_2
\approx \sum_{i=1}^{N_{int}} {\cal F}(S_{i1},S_{i2}) \ W_{i},  \f]
where, for quadrilateral elements the integration points 
\f$S_{i1},S_{i2} \ (i=1,...,N_{int})\f$ and weights
\f$ W_{i} \ (i=1,...,N_{int})\f$ are given by tensor 
products of the corresponding quantities in the 1D Gauss rules.

<HR>

\subsection example_P2 A 2D example

Here is an example of the finite element solution of problem
P2,
\f[ \sum_{j=1}^2 \frac{\partial^2 u(x_i)}{\partial x_j^2} = f(x_i), 
  \mbox{\ \ \ subject to $u|_{\partial D}=0$} \f]
for
\f[  f(x_i) = -1\f]
in the fish-shaped domain.

\image html fish_poisson_soln.gif "(Adaptive) solution of Poisson's equation in a 2D fish-shaped domain. " 
\image latex fish_poisson_soln.eps "(Adaptive) solution of Poisson's equation in a 2D fish-shaped domain. " width=0.75\textwidth

The solution was computed with \c oomph-lib, using a 
<A HREF="../../poisson/fish_poisson/html/index.html">driver code</A>
discussed elsewhere. Initially, the solution is computed on a
very coarse mesh which contains only four nine-node elements. Then 
the mesh is adaptively refined, guided by an
automatic error estimator, and the problem is re-solved. This procedure
is repeated several times. Note how the adaptation
refines the mesh predominantly near the inward corners where the
solution has an unbounded gradient and is therefore (always) under-resolved.

<HR>
<HR>


\section towards_oo An object-oriented implementation

The steps described in Algorithm 10 can
easily be implemented in a procedural language such as
Fortran or C. The algorithm already contains
a number of well-defined phases, which lend themselves to
implementation as subroutines/functions. The various (global) 
parameters (the number of nodes and elements, etc) and lookup schemes 
(the relation between local and global equation numbers,
the number of unknowns per element, etc.) generated
during the setup phase would usually be passed to the subroutines
(or functions) via common blocks (or 
<A HREF="http://c2.com/cgi/wiki?GlobalVariablesAreBad">
(bad!) global data</A>). 

We shall now discuss \c oomph-lib's object-oriented implementation of the
finite-element method. We begin 
by examining the "objects" that occur
naturally in algorithm 10 and aim to re-distribute
all global data so that it becomes member data of an appropriate 
object. Three "objects" already feature prominently in many parts of 
our algorithms: 
- the Nodes,
- the Elements,
- the Mesh.

In the procedural version of the algorithm, 
data associated with Nodes is 
stored in global arrays. 
For instance, \f$ X_{ij}\f$  stores the
\f$i\f$-th coordinate of global node \f$ j \f$; similarly, 
\f$ U_{j} \f$ stores the nodal value at 
global node \f$j\f$; etc. Nodal positions and values are accessed
either directly via their global node number (which identifies
Nodes within the Mesh) or indirectly via their local node number
within an Element. The latter requires the global lookup scheme
\f$ J(j_{local},e)\f$ which determines the global node 
number of local Node \f$j_{local}\f$ in Element \f$e\f$. 

In \c oomph-lib's object-oriented implementation, all nodal data is stored
as member data of a Node object. Each Node stores
- a (vector of) coordinates, and
- a nodal value
.
as private member data. Access to these data 
is provided by member functions, e.g.
\code
Node node;
double x_coord=node.x(0);
double y_coord=node.x(1);
\endcode

[Note the C++ convention of starting indices at 0, rather than 1!]

The Node object does \b not store a node number! This is because 
node numbers are only used to identify Nodes in specific contexts. 
For instance, as discussed above, the global node number identifies 
Nodes within the context of the global Mesh. Instead, we 
provide the Mesh object with a member function \c Mesh::node_pt(...)
which provides pointer-based access to its constituent Nodes:
\code
unsigned j;
Mesh mesh;
Node* node_pt=mesh.node_pt(j);
\endcode
Thus, we obtain the \f$x\f$-coordinate of global node \f$j\f$ via
\code
unsigned j;
Mesh mesh;
double x=mesh.node_pt(j)->x(0);
\endcode
Similarly, rather than determining the coordinates of local 
node \f$j_{local}\f$ in element \f$e\f$ via the global lookup scheme
\f[ x_i = X_{iJ(j_{local},e)}, \f]
we provide each \c FiniteElement with a member function
that provides pointer-based access to its constituent Nodes
\code
unsigned j_local;
FiniteElement element;
Node* node_pt=element.node_pt(j_local);
\endcode

Similar procedures can be implemented for all other objects. We use
the general design principle that all objects store only their 
"own" intrinsic data and we replace all global lookup schemes
by pointer-based access functions. To guide the implementation, we
observe the following inter-dependence between our three
fundamental objects:

- \b The \b Nodes: Nodes have positions and values. The nodal values 
  are either "pinned" or "free", in which case they 
  represent unknowns in the problem.
  The "pinned status" of a Node is indicated by its global equation 
  number, either a unique non-negative number, for unknowns, 
  or -1, for values that are "pinned". Note that this convention 
  makes "-1" a "magic number" -- bad practice! In \c oomph-lib, we
  set the equation number of pinned Nodes to \c Node::Is_pinned --
  a static data member of the Node class that is, in fact, set to -1.
- \b The \b Elements: Elements contain Nodes and, within an Element,
  Nodes are identified by their local node numbers.
  Elements define their (local) shape functions and compute
  the elemental residual vector and Jacobian matrix, using data 
  that is intrinsic to the Element. The entries in the Element's 
  residual vector and its Jacobian matrix are labelled by local 
  equation numbers, and each element stores a lookup scheme 
  for the correspondence between local and global equation numbers.
- \b The \b Mesh: The Mesh consists of Elements and Nodes. The mesh 
  generation process defines the nodal positions and establishes 
  the connectivity between the Nodes. Meshes typically have a number 
  of boundaries and the Mesh defines which Nodes are located on which 
  boundary. Nodes are identified by a global node number within the Mesh.


An important difference between the three fundamental objects
is the degree to which their member functions can be
implemented in complete generality. For instance,
the Node object is completely generic -- there is only one type of Node,
allowing the Node class to be fully implemented (as a "concrete class",
in C++ terminology) in \c oomph-lib.
Conversely, although all Meshes have a certain amount of common functionality
(e.g. they all provide pointer-based access to their constituent
nodes), the implementation of other functions are Mesh-specific. 
For instance, the Mesh generation process itself (typically 
performed in the Mesh constructor) depends on the domain geometry, the 
topology of its elements etc. Such functions must be implemented 
differently in different Meshes. \c oomph-lib therefore provides an abstract
base class \c Mesh, which defines and implements all functions that
are common to all Meshes. The class also defines a number of 
virtual functions to establish standard interfaces 
for functions that can only be implemented
in a concrete (derived) class, such as \c FishMesh.
The same principle applies to Elements which we endow with
a three-level inheritance structure:
- The abstract base class \c GeneralisedElement defines the
  common functionality that \e all Elements must have.
  All Elements must have member functions that compute the elemental 
  Jacobian matrix and the
  elemental residual vector. The distinction 
  between \c GeneralisedElements and \c FiniteElements is
  useful because many elements in \c oomph-lib are not finite elements, 
  and, for instance, do not have nodes.
- The abstract base class \c FiniteElement is derived from 
  \c GeneralisedElement and defines the common functionality
  that is shared by all finite elements. The \c FiniteElement class 
  implements all functions that are generic for finite elements
  and specifies the interfaces for any remaining functions
  that cannot be implemented in complete generality. For instance,
  all finite elements have shape functions but their implementation
  depends on the geometry of the element.
- Finally, we have concrete Elements that are derived from 
  \c FiniteElement
  and implement any virtual functions defined at lower levels; these
  Elements provide a specific discretisation of a specific PDE. 
  The \c QPoissonElement<3,2> implements, inter alia, 
  the computation of the element residual
  and the element Jacobian matrix for the 2D Poisson equation,
  in a nine-node quadrilateral element. 
  [\b Note: In many cases, it is useful to sub-divide this
  level further by introducing an intermediate class that
  implements only the Element geometry;
  motivated by the observation that a specific (geometric) Element type
  (e.g. a 2D quadrilateral element) can form the basis for many different 
  concrete Elements, e.g. Elements that solve the Poisson, 
  Advection-Diffusion or Navier-Stokes equations. Examples 
  of "geometric Elements" in \c oomph-lib include the \c QElements,
  a family of 1D line/2D quad/3D brick elements with
  linear (or {bi/tri}linear), quadratic (or {bi/tri}quadratic),...
  shape functions.]
 
 Algorithm 10 can easily be expressed in an 
 object-oriented 
 form because most steps involve operations that act exclusively 
 on the objects' member data. Such operations are 
 implemented as member functions of the appropriate objects.
 
 The steps in  Algorithm 10 that do not 
 fall into this category (the application of 
 boundary conditions, setting of initial conditions and boundary values,
 etc) are usually problem-dependent and cannot be implemented 
 in generality. However, certain steps (e.g. the 
 solution of the nonlinear algebraic equations by Newton's method)
 are identical for all problems and should be implemented in a
 general form. For this purpose, we introduce a fourth fundamental
 object, \b The \b Problem, an abstract base class that provides
 member functions for generic procedures such as
 - setting up the equation numbering scheme
 - solving the equations by Newton's method

 and stores
 - a pointer to the Mesh object
 - a vector of pointers to the unknowns which must be updated
   during the solution of the nonlinear system by Newton's method.

 Concrete problems, such as the \c FishPoissonProblem discussed above,
 should be derived from the abstract \c Problem class. 
 Any problem-specific steps can 
 be implemented in member functions of the derived class, often the
 constructor.

Here is an overview of overall data structure, as required by the
Poisson problems described above. The complete data structure
implemented in \c oomph-lib contains numerous extensions to permit the
solution of time-dependent problems, problems with many nodal values, 
problems in deforming domains, adaptive mesh refinement and many
other features. A complete description of 
<a href="../../the_data_structure/html/index.html"> the data
structure </a> is given elsewhere.



<CENTER>
<TABLE bgcolor="pink">
<TR>
<TD>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>Data:</B>
</CENTER>
- Stores and provides access to:
  - (pointers to) to values ( = double precision numbers). 
  - (pointers to) the global equation numbers for all values.
- Defines and implements the functions that:
  - allow pinning and unpinning of the values.
  - assign global equation numbers to all unknown values.
</TD>
</TR>
</TABLE>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>Node:</B> <BR>[derived from \b Data]
</CENTER>
- Stores and provides access to:
  - the spatial dimension of the Node.
  - the Eulerian coordinates of the Node.
</TD>
</TR>
</TABLE>
</TD>
</TR>
</TABLE>
</CENTER>



<TABLE bgcolor="pink">
<TR>
<TD>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>GeneralisedElement:</B>
</CENTER>
- Stores and provides access to:
  - the lookup scheme that stores the correspondence between
    local and global equation numbers.
- Defines interfaces for the (virtual) functions that:
  - compute the element's residual vector and Jacobian matrix [\b V1].
- Defines and implements functions that:
  - return the global equation number for a given local unknown.
  - return the number of degrees of freedom (= the total number of 
    unknowns) in the element. 
</TD>
</TR>
</TABLE>
</CENTER>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>FiniteElement:</B>  <BR>[derived from \b GeneralisedElement]
</CENTER>
- Stores and provides access to:
  - (pointers to) the \c Nodes. 
  - (a pointer to) the spatial integration scheme.
  - the lookup scheme that stores the local equation number of the  
    values stored at the \c Nodes.
- Defines interfaces for the (virtual) functions that:
  - specify the dimension of the element (i.e. the number of local
    coordinates needed to parametrise its shape -- this may differ
    from the spatial dimension of its \c Nodes!) [\b V2]
  - specify the shape functions and their derivatives with respect
    to the local coordinates [\b V3]
  - specify the number of nodal values that should be stored
    at each node, etc. [\b V4]
- Defines and implements functions that:
  - construct \c Nodes of the required spatial dimension,
    with sufficient storage for the nodal values, etc. (This
    information is obtained by calls to the virtual functions [V4] 
    that are defined at this level but only implemented for specific,
    derived  elements).
  - calculate the mapping between local and 
    global (Eulerian) coordinates,
    in terms of the abstract shape functions defined in the
    virtual functions [V3]
  - provide direct access to the nodal coordinates 
    and the nodal values.
  - assign local equation numbers to the values 
    stored at the \c Nodes.
</TD>
</TR>
</TABLE>
</CENTER>
<TABLE>
<TR>
<TD bgcolor="cornsilk">
<CENTER>
<B>Specific Geometric Element:</B> [e.g. a \c QElement] <BR>
[derived from \b FiniteElement]
</CENTER>
- Implements functions that:
  - allocate the storage for the (pointers to the) element's \c Nodes.
  - construct the spatial integration scheme and set the pointer
  defined in \c FiniteElement.
  - define the dimension of the element, i.e. the number of local
    coordinates required to parametrise its shape. This implements
    the virtual function [V2] defined in \c FiniteElement.
  - compute the shape functions and their derivatives with respect
    to the local coordinates. This implements
    the virtual functions [V3], defined in \c FiniteElement.
  .
.
<B>Note:</B>
- Templating is often used to implement entire
  families of geometric elements. For instance, the elements of the 
  \c QElement family have two
  template parameters: One defines the dimension of the element, 
  the other the number of nodes along a 1D element edge, so that
  a \c QElement<1,3> is a three-node line element, while a 
  \c QElement<3,2> is an eight-node brick element, etc.
</TD>
<TD bgcolor="cornsilk">
<CENTER>
<B>Specific Equation:</B> [e.g. the \c PoissonEquations] <BR>
[derived from \b FiniteElement]
</CENTER>
- Stores and provides access to:
  - (function pointers to) any source functions, forcing terms, etc.
    that occur in the governing equations. 
  - (pointers to) any parameters in the governing equation.
- Implements functions that:
  - compute the element's residual vector and Jacobian matrix, as 
    defined by the virtual functions [V1] in \c FiniteElement. These
    functions have access to the geometric shape functions, the number of 
    \c Nodes, the mapping between local and global coordinates, 
    etc, via the (abstract and concrete) functions defined in 
    \c FiniteElement.
.
- Defines and implements functions that:
  - compute derived quantities of interest, e.g. the local flux
.
<B>Notes:</B> 
- Templating is often used to implement the equations
  in arbitrary spatial dimensions, such as
  \c PoissonEquations<DIM>.
</TD>
</TR>
</TABLE>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>Specific Element:</B> [e.g. a \c QPoissonElement] <BR>
[Derived from a <B>Specific Equation </B> and a 
<B>Specific Geometric Element</B>  by multiple inheritance]
</CENTER>
<P>
- Implements functions that:
  - specify the number of nodal values etc. to be stored
    at each \c Node; this implements the virtual functions  
    [V4] defined in \c FiniteElement.
  - provide any additional functionality that might be
    required at the \b Specific \b Element level.
</TABLE>
</TD>
</TR>
</TABLE>
</CENTER>
</TABLE>
</CENTER>

<B>Note:</B> The subdivision into classes that define the
element geometry and the specific equations to be solved
is often useful but not compulsory. It is possible
(and in some cases probably preferable) to provide the complete
implementation for a specific element in a single class.
Furthermore, not all elements in \c oomph-lib need to be
\c FiniteElements. It is possible to formulate fully functional 
GeneralisedElements. Such elements are often used to formulate
discrete constraints or boundary conditions. 




<CENTER>
<TABLE bgcolor="pink">
<TR>
<TD>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>Mesh:</B>
</CENTER>
- Stores and provides access to:
  - (pointers to) its constituent \c GeneralisedElements.
  - (pointers to) its constituent \c Nodes.
  - (pointers to) the \c Nodes that are located on the \c Mesh
    boundaries.
- Defines and implements functions that:
  - assign the global equation numbers for all nodal values.
  - execute the \c GeneralisedElement::assign_local_eqn_numbers()
    function for all constituent GeneralisedElements.
</TD>
</TR>
</TABLE>
</CENTER>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>Specific Mesh:</B> <BR>[derived from \b Mesh]
</CENTER>
- Defines and implements functions that:
  - create the Mesh. This is typically done in the Mesh constructor
    and involves the following tasks:
    - creating the \c Mesh's constituent \c Nodes and \c
      GeneralisedElements
    - setting up the lookup schemes that allow direct access to 
      \c Nodes and \c FiniteElements on the domain boundaries.
</TD>
</TR>
</TABLE>
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER>





<CENTER>
<TABLE bgcolor="pink">
<TR>
<TD>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>Problem:</B>
</CENTER>
- Stores and provides access to:
  - (pointer to) the global \c Mesh which provides ordered access
    to all \c Nodes and \c GeneralisedElements in the \c Problem. 
  - (pointers to) any global \c Data.
  - (pointer to) a \c LinearSolver that will be used in the
     Newton method.
  - a vector of (pointers to) the \c Problem's degrees of freedom 
    (= double precision numbers).
- Defines and implements functions that:
  - set up  the global and local equation numbering schemes.
  - solve the (global) nonlinear system of equations, formally
    defined by the assembly of the \c GeneralisedElement's residual
    vectors, using Newton's method. 
  - perform a self test of all fundamental objects involved in the
    problem to check if the \c Problem has been set up properly.
  - allow the entire Problem to be written to disk, in a format
    that allows restarts.
</TD>
</TR>
</TABLE>
</CENTER>
<CENTER>
<TABLE bgcolor="cornsilk">
<TR>
<TD>
<CENTER>
<B>Specific Problem:</B> <BR>[derived from \b Problem]
</CENTER>
- Implements functions that:
  - Set up the problem. This is usually done in the \c Problem
    constructor and involves the following tasks:
    - creation of the \c Mesh object
    - application of boundary conditions
    - completion of the build process for any \c GeneralisedElements
      that need to be passed some global data, such as function pointers
      to source functions, etc.
    - setting up the equation numbering scheme. 
- Defines and implements functions that:
  - perform parameter studies
  - perform the post-processing of the results.
</TD>
</TR>
</TABLE>
</CENTER>
</TD>
</TR>
</TABLE>
</CENTER>

<HR>
<HR>

 

<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

