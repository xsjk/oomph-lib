/**

\mainpage Frequently Asked Questions


Here are some frequently asked questions (with assorted frequently
given answers...). Please check these before contacting us
with any problems.

- \ref installation \n\n
  - \ref comp_opt \n\n
  - \ref cpp11 \n\n
  - \ref other_compilers \n\n
  - \ref cygwin \n\n
  - \ref apple \n\n
  - \ref apple_link \n\n
  - \ref self_tests \n\n
  - \ref run1 \n\n
  - \ref run2 \n\n
  - \ref only_compile_dont_run \n\n
  - \ref grep_run \n\n
  - \ref link \n\n
  - \ref missing_this \n\n 
- \ref compilation_and_seg \n\n
  - \ref rodata \n\n
  - \ref seg_fault \n\n
  - \ref wrong_results \n\n
  - \ref newton_divergence \n\n
- \ref cust_opt \n\n
  - \ref tecplot \n\n
  - \ref element_too_general \n\n
  - \ref build_not_run_demos \n\n
- \ref nav \n\n
  - \ref too_much \n\n
  - \ref includes  \n\n
- \ref all_else_fails \n\n
  - \ref reporting \n\n
.



\n\n
<HR>
<HR>


\section installation Installation


\subsection comp_opt How do I specify different levels of optimisation and/or non-standard compiler options?


-# <B>Compiler flags/options for the compilation of the library</B> \n\n
   By default, \c oomph-lib's installation is performed with the 
   <A HREF="http://gcc.gnu.org/">gcc compiler suite</A>, using full 
   optimisation and warning (\c -O3 \c -Wall ). 
   \n\n
   If you install the library with the \c autogen.sh script you are 
   given the option to overwrite the default compilation flags,
   either by specifying alternative flags on the command line,
   or by recycling any of the previously used combinations of flags, stored
   in the directory <A HREF="../../../config/configure_options/">
   config/configure_options</A>. The files in this directory 
   also contain more details on available flags and options.
   \n\n
   If you prefer the standard linux \c configure/make/make \c install 
   installation procedure, you should already know how to specify
   flags at the configure stage...
   \n\n
-# <B>Compiler flags/options for the compilation of individual
   (driver) codes</B> \n\n
   Within the \c autotools framework,
   additional flags for the compilation of individual (driver) codes 
   may be specified by setting the appropriate automake variables
   in the \c Makefile.\c am.  For instance, to use the compiler
   flag \c -DUSE_TAYLOR_HOOD during the compilation of the 
   executable \c test_code, add
   \code
   test_code_CXXFLAGS = -DUSE_TAYLOR_HOOD
   \endcode
   to the appropriate \c Makefile.\c am. The 
   C++ section of the
   <A HREF="http://www.gnu.org/software/automake/manual/automake.html">
   automake manual</A> provides more detail on other \c automake variables.
   \n\n
   If you link against \c oomph-lib from outside the \c autotools 
   framework, you can, of course, use whatever technique you
   prefer to customise the compilation of your driver code.
.



<HR>

\subsection cpp11 My compiler doesn't support C++11 by default

\c oomph-lib uses the C++11 standard and assumes that your compiler supports
this. If you have an older version of the gcc compiler you can add
the flag \c -std=c++11 to the \c g++ configure options by adding it to
the \c CXXFLAGS in your configure options file. The one that's used
to build \c oomph-lib is
<center>
\c config/configure_options/current.
</center>
<br>
Note that the directory \c config/configure_options also contains many
other, generally sensibly named files that specify configure options
for other configurations and compilers. You can choose from these when running
\c autogen.sh or you can explore them yourself for further options.

If you are using a different compiler check its man pages to find out
what flag is needed to enable C++11.



<HR>

\subsection other_compilers The compilation fails when I use my xxx [non gcc] compiler

\c oomph-lib is developed in a GNU Linux environment, using the
<A HREF="http://gcc.gnu.org/">gcc compiler suite</A>. We believe (!)
the source code complies with the  
<A HREF="http://www.open-std.org/jtc1/sc22/wg21/"> C++ standard</A>
and it compiles cleanly under the <A HREF="http://gcc.gnu.org/">gcc
compilers</A> that we have access to (version 3.2.3 and later). 
Some warning messages tend to be issued during the compilation of
the third-party libraries distributed with \c oomph-lib, but
these are not our responsibility! 

We occasionally compile (or get other people to compile) the library 
under other compilers and we generally try to rectify any problems that
are flagged up in the process.  If you encounter any problems
(errors or warning messages) while compiling the library with your 
own (non-gcc) compiler we would <A HREF="../../contact/html/index.html">
like to hear from you</A>, especially if you suggest concrete bug
fixes.

 There are problems with version 7 of the intel compiler suite but the
library compiles cleanly under version 9. We suggest you upgrade
to that if you wish to use the intel compilers.

<HR>



\subsection cygwin The build process fails under cygwin

Here are a few things to check:
- Make sure the gcc compiler suite is sufficiently up-to-date
  (we recommend version 3.2.3 or later). \n\n
- The build process will fail if you have (foolishly!) chosen
  a windows username that contains spaces. The build script 
  (executed under linux) makes frequent references to the (absolute!) 
  path to the \c oomph-lib installation directory. Linux does
  not allow spaces in directory names! 
.


<HR>


\subsection apple The build process fails under Apple's Darwin (OSX) or other BSD Linux distributions

Here are some general guidelines for installing under OSX, contributed
by <A HREF="http://hewitt.ddns.net/">Rich Hewitt</A>:
\n\n
-# Make sure you have installed the development tools from the
OSX install DVDs. If your install media is fairly old, you may
wish to look for an updated online version at:
\n\n
<CENTER>
<A HREF="http://developer.apple.com/tools/xcode/">
http://developer.apple.com/tools/xcode/</A>
</CENTER>
\n
You do not need to use the Xcode IDE, but we do need the associated
development kit tools (in particular GCC!)
\n\n
-# Install a FORTRAN compiler. Apple does not ship a FORTRAN version,
so the easiest way to do this is to install the relevant pre-packaged GNU
FORTRAN from here:
\n\n
<CENTER>
<A HREF="http://hpc.sourceforge.net/">
http://hpc.sourceforge.net/
</A>
</CENTER>
\n
(I would recommend sticking with the version of GCC4 provided by Apple.)
\n\n
-# Make sure the FORTRAN compiler ('gfortran' in the packages
pointed to above) is in the installing user's path.
\n\n
-# Follow the  <A HREF="../../the_distribution/html/index.html">
oomph-lib install procedure</A> as normal.
\n\n
-# For associated Unix-goodness (doxygen, gnuplot, git, etc),
I would recommend using a ports mechanism to install any additional
unix tools painlessly:
\n\n
<CENTER>
<A HREF="https://www.macports.org/">
https://www.macports.org/</A>
</CENTER>
\n
For example: "sudo port install gnuplot" etc.
.

<HR>


\subsection apple_link The linking self-test fails under Apple's OSX

As discussed in the 
<a href="../../the_distribution/html/index.html#apple_linking">
installation instructions,</a> Darwin (the BSD-derived UNIX core of 
Apple's OSX operating system) requires a slight change to the 
default procedure for linking against \c oomph-lib from
outside its autotools framework. As a result, the linking self-test
tends to fail on machines with this operating system. Instructions
on how to fix this problem are provided in 
<a href="../../the_distribution/html/index.html#apple_linking">
installation instructions</a>.

<HR>

\subsection self_tests Some of the self tests fail

<B>Tolerance of comparison between floating point numbers</B> \n\n

The full \c oomph-lib distribution contains a large number of demo
codes in the sub-directory \c demo_drivers. Primarily, these
codes serve as demo codes for the \c oomph-lib documentation (contained in the 
sub-directory \c doc) but they are also used during the library's self-test
procedure which checks that \c oomph-lib was installed correctly. The 
self-tests can either be initiated at the end of the 
<CODE>autogen.sh</CODE> - based installation or by typing
\code
make check -k
\endcode
in \c oomph-lib's top-level directory. The self-test builds
and runs all demo codes and compares their results against the reference
data stored in the \c validata sub-directories. The comparison
is performed with the python script <CODE> bin/fpdiff.py</CODE> which tolerates
small (machine- and compiler-dependent) roundoff errors
and suppresses the comparison of numbers whose absolute value falls below
some threshold (to avoid the comparison of numerical zeroes).

If any of the self-test fail, you should first check the output in the file \c
validation.log to assess if the differences between the computed
data and the reference data are significant. If the
discrepancy appears to be due to larger-than-anticipated round-off
errors (you'll have to judge this yourself!), modify the 
\c validate.sh script to specify a larger
relative tolerance and/or a larger value for the threshold
below which \c fpdiff.py regards numbers as numerical zeroes and
excludes them from the comparison. [Type
\code
bin/fpdiff.py 
\endcode
in \c oomph-lib's top-level directory for instructions on how to use 
the script].

<B>The self-test fails even though the output files produced by 
the code are correct</B> \n\n

This is an odd error that is usually caused by the use of wildcards
in the validations scripts,
or the comparison of data that is stored in certain STL containers.
Typically, the self-test is performed by
the \c validate.sh shell script, which runs the executable and 
concatenates selected output files to a single file whose contents 
are compared against the reference file in the \c validata directory. 

While it is tempting to write
\code
cat RESLT* /soln0.dat > results_file.dat
\endcode
it is important to realise that the order in which the files are 
concatenated is machine- and/or operating-system dependent. If 
the above command is run in a directory with the following structure
\code
|-- RESLT
|   |-- soln0.dat
|   `-- trace.dat
|-- RESLT_elastic
|   |-- soln0.dat
|   `-- trace.dat
\endcode
some operating systems will expand the command to 
\code
cat RESLT/soln0.dat RESLT_elastic/soln0.dat > results_file.dat
\endcode
while others will execute
\code
cat RESLT_elastic/soln0.dat RESLT/soln0.dat > results_file.dat
\endcode
In this case the self-test will report a failure, even though the 
solution files are correct. The <CODE>validate.sh</CODE> scripts
should therefore not contain any wildcards.

Similar problems can arise if the validation data includes
data that is stored in certain STL containers such as sets.
The order in which items are stored in such containers may
vary from machine to machine and from compiler to compiler.
If such data is to be included in a self-test the data should be
sorted first, based on a user-controllable sorting criterion.


<HR>

\subsection run1 How do I compile/run the demo driver codes?

Assume you have followed our advice to explore \c oomph-lib by
playing with representative demo driver codes, and have ended up
in a directory that contains the following files:
\code
mheil@biolaptop:~/version339/demo_drivers/poisson/one_d_poisson$ ls -l
total 68
-rw-rw-r-- 1 mheil mheil 21604 2012-12-31 11:10 Makefile
-rw-rw-r-- 1 mheil mheil   409 2012-12-08 13:34 Makefile.am
-rw-rw-r-- 1 mheil mheil 19335 2012-12-31 11:09 Makefile.in
-rw-rw-r-- 1 mheil mheil  9638 2012-12-08 13:34 one_d_poisson.cc
drwxrwxr-x 3 mheil mheil  4096 2012-12-08 13:34 validata
-rwxrwxr-x 1 mheil mheil  2402 2012-12-08 13:34 validate.sh
\endcode
The temptation is to compile the driver code \c one_d_poisson.cc
with your favourite C++ compiler, e.g. \c g++:
\code
g++  one_d_poisson.cc
\endcode
but this does \b not work:
\code
mheil@biolaptop:~/version339/demo_drivers/poisson/one_d_poisson$ g++ one_d_poisson.cc 
one_d_poisson.cc:31:21: fatal error: generic.h: No such file or directory
compilation terminated.
\endcode
Why? If you look into the code, you notice that it \#includes 
\c oomph-lib's \c generic.h header file, and \c g++ obviously doesn't
know where to find this. You can, in principle, find
out where it (and the associated library files themselves!) live
but this is rather against the spirit of the 
<A HREF="http://www.gnu.org/software/automake/">automake</A> 
machinery. What you should do instead is to use \c make:
\code
make one_d_poisson
\endcode 
Note the omitted postfix! The argument to \c make is the name of the
target, i.e. the executable that you wish to build -- which source
code to compile, where to locate the required headers and which 
libraries to link against is all encoded in the \c Makefile (which has
itself been created by <A HREF="http://www.gnu.org/software/automake/">
automake</A> (during the installation of the \c oomph-lib)
from the \c Makefile.am file in the directory). As you see
from the on-screen output, there is a lot of information that needs
to be passed to the compiler/linker:
\code
mheil@biolaptop:~/version339/demo_drivers/poisson/one_d_poisson$ make one_d_poisson
mpic++ -DHAVE_CONFIG_H -I. -I../../..  -I/home/mheil/local/hypre_default_installation_mpi/include -DOOMPH_HAS_HYPRE -I/home/mheil/local/trilinos_default_installation_mpi/include -DOOMPH_HAS_TRILINOS -I/home/mheil/local/mumps_and_scalapack_default_installation/include -DOOMPH_HAS_MUMPS -DOOMPH_HAS_STACKTRACE -DOOMPH_HAS_MPI -DOOMPH_HAS_TRIANGLE_LIB -DOOMPH_HAS_TETGEN_LIB -DUSING_OOMPH_SUPERLU -DUSING_OOMPH_SUPERLU_DIST -I/home/mheil/version339/build/include  -DgFortran -O3 -Wall -MT one_d_poisson.o -MD -MP -MF .deps/one_d_poisson.Tpo -c -o one_d_poisson.o one_d_poisson.cc
mv -f .deps/one_d_poisson.Tpo .deps/one_d_poisson.Po
/bin/bash ../../../libtool --tag=CXX   --mode=link mpic++ -DgFortran -O3 -Wall -L/home/mheil/local/hypre_default_installation_mpi/lib -L/home/mheil/local/trilinos_default_installation_mpi/lib -L/home/mheil/local/mumps_and_scalapack_default_installation/lib  -o one_d_poisson one_d_poisson.o -L/home/mheil/version339/build/lib -lpoisson -lgeneric -lHYPRE -lml -lifpack -lamesos -lanasazi -laztecoo -lepetraext -ltriutils -lepetra -lteuchos -ldmumps -lmumps_common /home/mheil/local/mumps_and_scalapack_default_installation/lib/libscalapack.a /home/mheil/local/mumps_and_scalapack_default_installation/lib/blacs.a /home/mheil/local/mumps_and_scalapack_default_installation/lib/blacsF77.a /home/mheil/local/mumps_and_scalapack_default_installation/lib/blacs_copy.a /home/mheil/local/mumps_and_scalapack_default_installation/lib/libpord.a -loomph_hsl -loomph_triangle -loomph_tetgen -loomph_superlu_4.3 -loomph_parmetis_3.1.1 -loomph_superlu_dist_3.0 -loomph_metis_from_parmetis_3.1.1 /home/mheil/local/lib/lapack/lapack.a /home/mheil/local/lib/blas/blas.a -L/usr/lib/openmpi/lib -L/usr/lib/gcc/x86_64-linux-gnu/4.6.1 -L/usr/lib/gcc/x86_64-linux-gnu/4.6.1/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/4.6.1/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/4.6.1/../../.. -lmpi_f77 -lmpi -lopen-rte -lopen-pal -ldl -lnsl -lutil -lgfortran -lm -lquadmath -lpthread 
libtool: link: mpic++ -DgFortran -O3 -Wall -o one_d_poisson
one_d_poisson.o
-L/home/mheil/local/hypre_default_installation_mpi/lib
-L/home/mheil/local/trilinos_default_installation_mpi/lib
-L/home/mheil/local/mumps_and_scalapack_default_installation/lib
-L/home/mheil/version339/build/lib
/home/mheil/version339/build/lib/libpoisson.a
/home/mheil/version339/build/lib/libgeneric.a -lHYPRE -lml -lifpack
-lamesos -lanasazi -laztecoo -lepetraext -ltriutils -lepetra -lteuchos
-ldmumps -lmumps_common
/home/mheil/local/mumps_and_scalapack_default_installation/lib/libscalapack.a
/home/mheil/local/mumps_and_scalapack_default_installation/lib/blacs.a
/home/mheil/local/mumps_and_scalapack_default_installation/lib/blacsF77.a
/home/mheil/local/mumps_and_scalapack_default_installation/lib/blacs_copy.a
/home/mheil/local/mumps_and_scalapack_default_installation/lib/libpord.a
/home/mheil/version339/build/lib/liboomph_hsl.a
/home/mheil/version339/build/lib/liboomph_triangle.a
/home/mheil/version339/build/lib/liboomph_tetgen.a
/usr/lib/openmpi/lib/libmpi_cxx.so
/home/mheil/version339/build/lib/liboomph_superlu_4.3.a
/home/mheil/version339/build/lib/liboomph_parmetis_3.1.1.a
/home/mheil/version339/build/lib/liboomph_superlu_dist_3.0.a
/home/mheil/version339/build/lib/liboomph_metis_from_parmetis_3.1.1.a
/home/mheil/local/lib/lapack/lapack.a
/home/mheil/local/lib/blas/blas.a -L/usr/lib/openmpi/lib
-L/usr/lib/gcc/x86_64-linux-gnu/4.6.1
-L/usr/lib/gcc/x86_64-linux-gnu/4.6.1/../../../x86_64-linux-gnu
-L/usr/lib/gcc/x86_64-linux-gnu/4.6.1/../../../../lib
-L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu
-L/usr/lib/../lib
-L/usr/lib/gcc/x86_64-linux-gnu/4.6.1/../../.. /usr/lib/openmpi/lib/libmpi_f77.so
/usr/lib/openmpi/lib/libmpi.so /usr/lib/openmpi/lib/libopen-rte.so
/usr/lib/openmpi/lib/libopen-pal.so -ldl -lnsl -lutil -lgfortran -lm
-lquadmath -lpthread -pthread
\endcode
Looking at the content of the directory again shows that the
required executable has now been generated:
\code
mheil@biolaptop:~/version339/demo_drivers/poisson/one_d_poisson$ ls -l
total 9988
-rw-rw-r-- 1 mheil mheil   21604 2012-12-31 11:10 Makefile
-rw-rw-r-- 1 mheil mheil     409 2012-12-08 13:34 Makefile.am
-rw-rw-r-- 1 mheil mheil   19335 2012-12-31 11:09 Makefile.in
-rwxrwxr-x 1 mheil mheil 9608851 2012-12-31 15:14 one_d_poisson
-rw-rw-r-- 1 mheil mheil    9638 2012-12-08 13:34 one_d_poisson.cc
-rw-rw-r-- 1 mheil mheil  548256 2012-12-31 15:14 one_d_poisson.o
drwxrwxr-x 3 mheil mheil    4096 2012-12-08 13:34 validata
-rwxrwxr-x 1 mheil mheil    2402 2012-12-08 13:34 validate.s
\endcode

\n\n

<HR>

\subsection run2 Why doesn't make make anything? What are the targets called? 
If you are familiar with makefiles, you are unlikely to have
fallen into the trap discussed above. You will almost certainly 
have spotted the \c
Makefile and tried to build the executable(s) with \c make, but
oddly this doesn't work as you (probably) expected:
\code
mheil@biolaptop:~/version339/demo_drivers/poisson/one_d_poisson$ make
make: Nothing to be done for `all'.
\endcode
The reason for this is that 
-# <A HREF="http://www.gnu.org/software/automake/">automake</A>
   distinguishes between several types of targets,
-# the demo driver codes in the subdirectories of \c demo_drivers
   double up as self-tests to assert the correct installation of 
   \c oomph-lib when \c make \c check is run from the top-level
   directory.
.
The latter requires them to be declared (in the \c Makefile.am)
in the \c check_PROGRAMS variable. You  can either build (and
automatically run) all executables in a \c demo_driver directory
by issuing the command
\code
make check -k
\endcode
or you can inspect the  \c check_PROGRAMS variable in the
\c Makefile.am to find out which targets have been declared
(and can therefore be built by \c make).
For instance, in the \c one_d_poisson directory the first
few lines of the \c Makefile.am are
\code
mheil@biolaptop:~/version339/demo_drivers/poisson/one_d_poisson$ more Makefile.am 

[...]

check_PROGRAMS=one_d_poisson

one_d_poisson_SOURCES = one_d_poisson.cc 

[...]
\endcode
This  declares the target (the executable called \c one_d_poisson) 
and specifies the source code to be compiled (\c one_d_poisson.cc).
The build target needs to specified explicitly:
\code
make one_d_poisson
\endcode 
We note that the names of the executable and the source code do not
have to be as closely related as in this example. While we usually
create the name of the executable by dropping the
postfix of the source file, this is not guaranteed! Inspect 
the \c Makefile.am if \c make doesn't give you what you want.


<HR>

\subsection only_compile_dont_run Running the self-tests takes a long time. Is it possible to suppress the execution of the built driver codes and only check if they compile? (Mainly useful for developers).


Issuing \c make \c check from the top 
level \c oomph-lib directory descends into all oomph-lib directories
that contain demo driver codes, builds these and then runs them
using the script \c validate.sh which typically also compares
the output against some reference data 
that is stored in the \c validata sub-directories. This process 
(which allows for some tolerances
in floating point data) assesses that
the installation is working properly, but the entire procedure obviously
takes a long time. As a developer, you may want to check (more) quickly
if the changes you've made to any source code (in \c src) still compile
without running any of the self-tests. It is easy to suppress 
the execution of the self tests because the \c validate.sh script is, in fact,
run through the wrapper script
\code
bin/wrapped_validate.sh
\endcode
which, by default, runs \c validate.sh through \c time, allowing us to
report the run times. However, you can easily edit this file and
make it do other things. For instance, if you put \c exit onto the 
first line it won't do anything at all. Success! If you then issue
\code
make check
\endcode
(without the "-k"!) from the top level, the build machinery will 
descend through all the demo driver directories, build the
executables and stop when it encounters the first error. 
If you want to continue with the compilation after encountering
failures do 
\code
make -k check
\endcode
You can assess all the failures at the same time by searching
backwards through the on-screen output.

\n\n


<HR>

\subsection grep_run I only want to run the self-tests for demo driver codes that contain a certain string (typically a class name; again mainly useful for developers)

The script
\code
bin/run_selected_self_tests_based_on_string.bash
\endcode
will search through all demo driver directories and run the self tests
in all directories where the source code of a demo driver contains a 
certain string. You can specify the string and the search directory in the
script or via the command line. Run
\code
bin/run_selected_self_tests_based_on_string.bash --help
\endcode
for details.

<HR>

\subsection link The oomph-lib distribution includes some third-party libraries. How do I get the code to link against optimised local versions of these libraries that are already installed on my machine?

To facilitate the installation, the oomph-lib distribution includes 
the relevant parts of certain third-party libraries, such as 
<A HREF="http://crd.lbl.gov/~xiaoye/SuperLU">SuperLU</A> and
<A HREF="http://www.netlib.org/blas/">BLAS</A>. These "external libraries"
are built and installed along with \c oomph-lib's own sub-libraries.
To distinguish them from any already existing local versions of
these libraries, their names are pre-fixed with the string \c oomph_.

All demo codes are automatically linked against these libraries.
This is achieved by defining the variable
\c EXTERNAL_LIBS in  the \c configure script:

\code
 # Define "external" libraries
 #----------------------------
 # These are the third-party libraries distributed with oomph-lib.
 # Note: If you want to link against another, already existing local
 # installation of one of these libraries, edit the EXTERNAL_LIBS
 # assignment below. As an example, if you want to use your own BLAS
 # library, and you'd usually link against this with
 #
 #    g++ -o my_code my_code.cc -lblas
 #
 # then replace "-loomph_blas" by "-lblas" below:
 #
 EXTERNAL_LIBS='-loomph_hsl -loomph_superlu_3.0 -loomph_metis_4.0 -loomph_lapack -loomph_flapack -loomph_blas'
\endcode

If you wish to link against some other version of the library, edit
the \c EXTERNAL_LIBS variable accordingly. 



\b WARNING: If you work within the autotools framework, remember
that the \c configure script is generated by \c autoconf,
using the file <CODE> configure.ac</CODE>, which is itself
(re-)generated whenever you run
<CODE>autogen.sh</CODE>. 
If you use the autotools, you should edit the \c EXTERNAL_LIBS 
variable in the file
<CODE>configure/configure.ac_scripts/start</CODE> which forms one of the
building blocks from which <CODE> autogen.sh </CODE>assembles the
<CODE>configure.ac </CODE> file. Once you have changed the
\c EXTERNAL_LIBS variable in
<CODE>configure/configure.ac_scripts/start</CODE>
you should (re-)run <CODE>autogen.sh</CODE>
in \c oomph-lib's top-level directory.

<CENTER>
<table border=1>
<TR>
<TD bgcolor="cornsilk">
<CENTER>
<B>NEW  (version 0.85)</B>
</CENTER>
You can now use the
\c --with-blas and \c --with-lapack configure flags to 
specify the location of your own blas and lapack libraries;
see <a href="../../the_distribution/html/index.html#external_src">
the installation tutorial</a> for details. Equivalent flags
will soon be added for the other libraries.
</TD>
</TR>
</TABLE>
</CENTER>

<HR> 

\subsection missing_this Missing 'this->' 

Recent versions of the <A HREF="http://gcc.gnu.org/">gcc compilers</A>
enforce the <A HREF="http://www.open-std.org/jtc1/sc22/wg21/">C++
standard</A> much more rigorously than earlier versions. 
Unfortunately, the standard includes some rules that are so counter-intuitive
that it is hard get into the habit of using them, especially if code
is developed on a compiler that does not enforce the standard
as rigorously. 

The most frequent problem arises in classes that
are derived from a templated base class.
<A HREF="http://www.open-std.org/jtc1/sc22/wg21/">The C++ standard</A>
insists that all references to member functions (or member data) that
is defined in the templated base class must be preceded by  "this->"
when the reference is made in the derived class.  Allegedly, this
is necessary to avoid ambiguities, though it is not entirely clear what this
ambiguity is supposed to be... Here is a driver code that illustrates 
the problem.  

\include broken_this_demo.cc

If you compile this with sufficiently recent versions of the
<A HREF="http://gcc.gnu.org/">gcc compilers</A>, using the
flag \c -DUSE_BROKEN_VERSION, the compilation will fail with the
following error:
\code
broken_this_demo.cc: In member function 'void SomeDerivedClass<TEMPLATE_PARAMETER>::output_template_parameter()':
broken_this_demo.cc:55: error: there are no arguments to 'say_hello_world' that depend on a template parameter, so a declaration of 'say_hello_world' must be available
broken_this_demo.cc:55: error: (if you use '-fpermissive', G++ will accept your code, but allowing the use of an undeclared name is deprecated)
\endcode

You may not only stumble across this problem in one of your own codes but
it is also possible that some code in the library itself still violates
this rule. This is because templated classes are only built when
needed and it is conceivable that \c oomph-lib's suite of 
self-tests do not instantiate all templated classes that exist in the
library. If you encounter any such problems, check if 
putting a "this->" in front of the function call fixes the problem.
If it does, <A HREF="../../contact/html/index.html">let us know!</A>



\n\n
<HR>
<HR>


\section compilation_and_seg Compilation problems and run-time errors

\subsection rodata Warning about "discarded sections" during linking

When linking, some versions of the <a href="http://gcc.gnu.org/">
gcc compiler</a> produce warnings
about references to "discarded sections" being referenced. Here's an
example:

\code
/usr/bin/ld: `.gnu.linkonce.t._ZNK5oomph8QElementILj3ELj3EE14vertex_node_ptERKj' referenced in section `.rodata' of /home/mheil/version185/oomph/build/lib/libgeneric.a(Qelements.o): defined in discarded section `.gnu.linkonce.t._ZNK5oomph8QElementILj3ELj3EE14vertex_node_ptERKj' of /home/mheil/version185/oomph/build/lib/libgeneric.a(Qelements.o)
\endcode

We admit to being slightly baffled by this. Other libraries seem to suffer
from the same problem (google for .\c rodata \c discarded, say),
but as far as we can tell no solution has ever been suggested, nor does one
seem to be required. The executable works fine. Upgrade to a newer version of
<a href="http://gcc.gnu.org/">gcc?</a>


<HR>

\subsection seg_fault My driver code compiles but dies with a segmentation fault

Suggestions:
- Run \c Problem::self_test() before solving the problem. This function
  performs a large number of sanity checks and reports
  any inconsistencies in the data structure. \n\n
- Recompile the relevant libraries
  with the PARANOID and RANGE_CHECKING flags set.
  Segmentation faults are often caused by out-of-bounds access
  to STL containers. Since > 95% (?) of the containers used in \c oomph-lib
  are \c Vectors (\c oomph-lib's wrapper to the STL vector class,
  with optional range checking) they can easily be detected.
  Make sure to re-compile again with RANGE_CHECKING switched
  off before you start any production runs -- the run-time overheads 
  incurred by range-checking are significant!  \n\n
- Recompile the relevant libraries and your driver code with
  the debugging flag ("-g" for the gnu compiler suite) switched on
  and all optimisation disabled. Re-run the code in a debugger
  (\c gdb or its GUI-based equivalent 
  <A HREF="http://www.gnu.org/software/ddd/">ddd</a>) to (try to!) find out 
  where the segmentation fault occurred.
  [\b Careful: If the segmentation fault is caused by a pointer problem,
  this naive inspection can be quite misleading -- tell-tale signs are that
  the traceback displays a non-sensical call stack, e.g. a function being
  called "out of nowhere"; variables that have just been given
  values not existing; etc.] \n\n

<HR>

\subsection wrong_results My driver code runs but it produces incorrect/non-sensical results

Suggestions:
- Have you applied the boundary conditions correctly? Check this by looping
  over all nodes in your mesh and documenting the pinned-status
  of their nodal values, using the function \c Node::is_pinned(...). \n\n
- Have you passed the required (function) pointers to the elements?
  Most element constructors assign default values for any physical parameters,
  e.g. the Reynolds number in the Navier-Stokes
  elements. Similarly, most source functions etc. default to zero. For
  instance \c oomph-lib's Poisson elements solve the Laplace equation unless a
  function pointer to the source function is specified. \n\n
- In time-dependent problems, have you assigned suitable initial
  conditions? Note that, if you use elements that are based on 
  the ALE formulation (and all time-dependent elements should be!) 
  you must initialise the history values for the nodal positions,
  even if the mesh is stationary!
  See the discussion of \c oomph-lib's timestepping procedures in the
  context of <A HREF="../../unsteady_heat/two_d_unsteady_heat/html/index.html">
  the unsteady heat equation</A> for details. \n\n
- Have you implemented the relevant "action" functions, such as
  \c Problem::actions_before_implicit_timestep()
  to update any time-dependent boundary conditions? 
.

<HR>

\subsection newton_divergence The Newton solver diverges

Suggestions:
- \c oomph-lib's default solver \c Problem::newton_solve(...)
  will converge quadratically, provided  \n\n
  - a "good" initial guess for the solution has been assigned, 
  .
  \n
  or
  \n 
  - the problem is linear.
  .
  \n
  If the Newton solver fails to converge for a nonlinear problem,
  try to identify a related linear problem and use continuation
  to generate a sequence of good initial guesses. For instance,
  to solve the Navier-Stokes equations at a Reynolds number
  of 500, say, start by solving the problem for zero Reynolds number 
  (in which case the problem becomes linear so that the Newton
  method converges in one iteration); increase the Reynolds number
  by 50, say, and re-solve. Repeat this procedure until 
  the desired value of the Reynolds number is reached. 
  \n\n
  \b Note: \c oomph-lib also provides automatic continuation methods,
  based on Keller's arclength continuation, but at the moment,
  no tutorials exist for these. 
  \n\n
- If you have tried the above and the Newton method fails to converge   
  even for a linear version of your problem, the most likely reasons are
  that 
  \n\n
  -# You have developed a new element and made a mistake in the
     implementation of the element's Jacobian matrix.
     To check if this is the case, comment out the
     function that computes the element's Jacobian matrix,
     i.e. the element member functions
     \c get_jacobian(...) or \c fill_in_contribution_to_jacobian(...).
     \c oomph-lib will then use the default implementation
     of these functions in the \c GeneralisedElement base class
     to compute the Jacobian matrices by finite-differencing.
     The executable is likely to run more slowly since the
     finite-difference-based computation 
     is unlikely to be as efficient as the customised
     implementation for your specific element, but if the 
     Newton method then converges, you know where to look for your
     bug! You may also want to check for any un-initialised variables.
     They are the most likely culprits if your code behaves
     differently at different levels of optimisation as 
     more aggressive optimisation may suppress any default
     initialisations of data -- in fact, you should never rely on that
     anyway!
     \n\n
  -# Your problem contains "dependent" variables, such as
     the nodal positions in a free-boundary problem. If the
     node update in response to changes in the shape of the 
     domain boundary is performed by an algebraic node update
     procedure (using \c AlgebraicNodes, \c SpineNodes or nodes whose
     position is updated by a \c MacroElement/Domain - based
     procedure), the position of the nodes in the "bulk" mesh
     must be updated whenever the Newton method updates the unknowns.
     This is most easily done by calling \c Mesh::node_update() in
     \c Problem::actions_before_newton_convergence_check().
  .
. 
  


\n\n
<HR>
<HR>

\section cust_opt Customisation and optimisation 

\subsection tecplot I don't have tecplot. How do I change oomph-lib's output so it can be displayed by my own plotting package?

\c oomph-lib's high-level post-processing routines output the results
of the computations in a form that is suitable for display with
<A HREF="http://www.tecplot.com">tecplot</A>, a powerful commercial 
plotting package. Purists may find it odd that an open-source library
should choose an output format that is customised for a commercial
software package. We tend to agree... Our only excuse is that
<A HREF="http://www.tecplot.com">tecplot</A> is very very good,
and without it we would have found it extremely difficult to create 
many of the plots shown in the 
<A HREF="../../example_code_list/html/index.html">tutorials.</A>
[If you know of any open-source plotting package whose capabilities
are comparable to those of <A HREF="http://www.tecplot.com">tecplot</A>,
<A HREF="../../contact/html/index.html">let us know!]</A>


Angelo Simone has written a python script that converts \c oomph-lib's
output to the vtu format that can be read by 
<a href="http://www.paraview.org">paraview</a>,
an open-source 3D plotting package. The conversion script
can currently deal with output from meshes that are composed
of 2D quad elements -- the extension to 3D is work in progress. 
Use of the conversion script is documented 
<A HREF="../../paraview/html/index.html">in another tutorial.</A>


It is possible to display \c oomph-lib's default output (in more
elementary  form, obviously) with <A HREF="http://www.gnuplot.info">
gnuplot</A>. The trick is to specify the \c using option in
<A HREF="http://www.gnuplot.info">gnuplot</A>'s
plot commands -- in this mode <A HREF="http://www.gnuplot.info">gnuplot</A> 
ignores tecplot's "ZONE" commands. For instance, trying to 
plot the x-y data created by 
<A HREF="../../poisson/one_d_poisson/html/index.html">the demo code 
for the solution of the 1D Poisson equation</A> with
\code
plot "RESLT/soln0.dat"
\endcode
will fail because 
<A HREF="http://www.gnuplot.info">gnuplot</A> gets confused by
the \c ZONE specifications required by 
<A HREF="http://www.tecplot.com">tecplot.</A> However,
\code
plot "RESLT/soln0.dat" using 1:2
\endcode
works.


If the data is too complex to be displayed by
<A HREF="http://www.gnuplot.info">gnuplot</A>, you may wish to customise the 
output for your preferred plotting package. This is easily done as 
\c oomph-lib creates its output element-by-element. The elements' 
various \c output(...) functions are virtual 
functions that can easily be overloaded in a user-defined
wrapper class.

Here is an example driver code that illustrates how to change the 
output from \c oomph-lib's \c QPoissonElement family of 
1D-line/2D-quad/3D-brick Poisson elements so that they 
output the string "Hello world". 


We include \c oomph-lib's \c generic and \c poisson library
headers:
\dontinclude customising_output.cc
\until poisson.h

and then create a customised version of the Poisson elements 
in which we overload the tecplot-based 
\c QPoissonElement<DIM,NNODE_1D>::output(...) function, defined in 
the \c poisson library:

\dontinclude customising_output.cc
\skipline The wrapper class for the element
\until end extension of 
  
If we now call the output function, the version defined
in the customised element is used. The remaining
implementation of the Poisson element remains
unchanged.

\until end of main

\n\n

<HR>

\n\n

\subsection element_too_general oomph-lib's implementation of the Navier-Stokes equations (say) is too general (and therefore too expensive) for my application. How do I change this?

Many of \c oomph-lib's equations classes (or elements) are implemented
in great generality. For instance, our discretisation of the 
Navier-Stokes equations includes a source term in the continuity equation, 
and body force terms in the momentum equations; it allows switching between
the stress-divergence and simplified forms of the viscous terms;
it includes the mesh velocity into the ALE formulation of the
time-derivatives; etc. This makes the elements very versatile
and robust. However, the generality/robustness comes at a price:
Even though we provide default values for most functions
(e.g. the body force terms default to zero), their
evaluation requires a finite amount of CPU time. If you wish
to use the elements in a simple application in which the 
Navier-Stokes equations are solved in a fixed domain, without
any body forces or other source terms, say, you may wish to disable
the additional functionality.

This is easily done: After all, \c oomph-lib is open-source software and
you can therefore change anything you want! In principle, you 
could edit the source code in the \c src/navier_stokes directory and 
delete (or at least comment out) all the functionality that you do not require.
However, this is probably a risky step as it will break all 
demo codes (used during \c oomph-lib's self-test procedure) that use
some of the features that you are not interested in. We therefore
recommend copying the content of the directory \c src/navier_stokes 
into a new directory, e.g. \c user_src/my_navier_stokes
and to edit the copied sources. Follow the instructions on the
\c oomph-lib <A HREF="../../the_distribution/html/index.html#add_src">
installation page</A> to turn these sources into a separate
library against which you can link.

\n\n

<HR>

\n\n

\subsection build_not_run_demos How do I build the self tests without running them?

The default behaviour when using \c make \c check is to compile and run the
self tests one test at a time. This means that compilation failures may
take a long time to appear which can be frustrating when making changes to
the main library. The alternative is to first compile all self tests using

\code
make check -k TESTS_ENVIRONMENT=true
\endcode

and then once you are sure everything compiles run the self tests as
normal.

Note: this "trick" relies on undocumented behaviour in automake and so it
is possible (but unlikely) that it will not work in new versions (tested
and working with GNU automake 1.11.3).

 
\n\n
<HR> 
<HR> 



\section nav Finding your way around the distribution


<HR> 

\subsection too_much There is so much information -- how do I get started?

Yes, \c oomph-lib does contain a lot of code and a lot of
documentation. How to get started obviously depends on your background:
Are you familiar with the finite element method? How good is your
knowledge of C++? Etc. \n\n
Here are some possible "routemaps" around the library: 

- <B>You are familiar with the finite element method and have a fairly
  good knowledge of C++</B>\n\n
  - Have a look through the <A HREF="../../example_code_list/html/index.html">
    list of example codes</A> to get a feeling for \c oomph-lib's
    capabilities. Pick a problem that interests you and 
    study the associated tutorial. Copy the driver code into your
    own directory and play with it. \n\n
  - Once you have played with a few example codes, you may wish to 
    to learn more about \c oomph-lib's 
    <A HREF="../../the_data_structure/html/index.html">overall data 
    structure,</A> or find out <A HREF="../../optimisation/html/index.html">
    how to optimise the library</A> for your particular application. \n\n
- <B>You have never used finite element methods but have a fairly
   good knowledge of C++</B>\n\n
   - Study the <A HREF="../../intro/html/index.html">
     "Top Down" introduction</A>. This document includes a "low tech" overview
     of the mathematical/theoretical background and contrasts
     procedural implementations of the finite element method with
     the object-oriented approach adopted in \c oomph-lib. \n\n
   - Consult the <A HREF="../../quick_guide/html/index.html">
     (Not-So-)Quick-Guide</A> to learn how to construct
     basic \c oomph-lib objects for your problem: \c Problems,
     \c Meshes, \c FiniteElements, etc. \n\n
   - Continue with the steps suggested above. 
     \n\n
- <B>You have never used finite element methods and are a newcomer to
  C++</B>\n\n
   - Buy <A HREF="http://www.math.wayne.edu/~yang/">Daoqi Yang</A> 
     brilliant book 
    <A HREF="http://www.springeronline.com/sgw/cda/frontpage/0,11855,4-40007-22-2105335-0,00.html?changeHeader=true">
     C++ and Object-Oriented Numeric Computing for Scientists and
     Engineers.</A> Read it! Pretty much everything in this book is
     relevant for some parts of \c oomph-lib. You should at least
     understand: \n\n
     - The procedural aspects of C++ (basic types, functions and control
       structures).
     - Namespaces.
     - Classes (private, protected and public members; inheritance
       and multiple inheritance;
       virtual and pure virtual functions; base classes and derived
       classes; static and dynamic casts).
     - Templates and template instantiations.
     - The standard template library (STL).
     .
     \n\n
   - Continue with the steps suggested above. 
     \n\n
   .
.
     



\n\n

<HR>

\n\n

\subsection includes Where is this class/function/... defined?

Assume you have studied one of the 
<A HREF="../../example_code_list/html/index.html">example codes</A> 
and wish to find out more about the implementation of a particular
class or function that is used there. How do you find
its source code and/or its full documentation?

Generally, a class/function that is used in a demo code 
can only be defined in one of two places:
-# In the demo driver code itself.
-# In an included file and/or an associated library.
.
\c oomph-lib's <A HREF="../../example_code_list/html/index.html">
tutorials</A> tend to provide a fairly complete annotated
listing of the relevant driver codes; if the function you are 
interested in is not mentioned explicitly in the tutorial, it is most 
likely to be defined in an include file. You can inspect the driver code 
in its entirety by following the link at the end of the
tutorials. If you cannot find the class/function there,
it must be defined in one of the include files listed
at the beginning of the source code.

  The included files themselves can either be located in 
the same directory as the demo driver (the directory also tends
to be mentioned at the end of the tutorial) or in one of
\c oomph-lib's sub-libraries. The source code for these
is located in the sub-directories of the \c src directory. Often 
the class/function is defined in a source file
with an "obvious" name; if not, use \c grep to find it. 
This can, of course, be done recursively. For instance,
the command

\code
find . \( -name '*.h' -o -name '*.cc' \) -exec grep -H FiniteElement {} \;  
\endcode

issued in \c oomph-lib's top-level directory 
will search through the entire distribution to 
locate files that contain the string "FiniteElement".


You can also use the html-based representation of \c oomph-lib's
data structure, created by <A HREF="http://www.doxygen.org">doxygen,</A>
in the <A HREF="../../the_data_structure/html/index.html">"bottom-up"
discussion of the data structure.</A> (Note that the search
menu may not work on your browser.) 

\n\n
<HR> 
<HR>


\section all_else_fails If all else fails...


\subsection reporting How to report problems/bugs

If all else fails and you think you have found a bug in the library,
make sure you follow these steps:
-# Isolate the problem: Try to identify the shortest driver code 
   that still produces the problem.\n\n
-# Double-check the <A HREF="../../the_data_structure/html/index.html">
   relevant documentation</A>, the 
   <A HREF="../../the_distribution/html/index.html">installation 
   instructions</A> and the other FAQs listed here.\n\n
-# Does the problem persist when you compile the library and
   your test code without optimisation,
   and when the \c RANGE_CHECKING and
   \c PARANOID flags are set? \n\n
-# Does the problem occur with a sufficiently recent version of
   the <A HREF="http://gcc.gnu.org/">gcc compiler suite</A>
   (version 3.2.3 and later)?\n\n
-# If the above steps identify the problem, 
   <A HREF="../../contact/html/index.html">let us know</A>, ideally
   with a bug fix! \n\n
-# If you can't fix the problem yourself, get in touch
   <A HREF="../../contact/html/index.html">either directly</A>,
   or via our GitHub-based
   bug tracking system, accessible online at
   \n\n
   <CENTER> 
   <a href="https://github.com/oomph-lib/oomph-lib/issues">
   https://github.com/oomph-lib/oomph-lib/issues</a>
   </CENTER> 
   \n
   and provide as much information as possible (clear description
   of the problem; the source code; the
   Makefile; details of the compiler and compilation flags used; 
   any warning/error messages that are displayed during the
   compilation of the library or the driver code itself; etc.) \n\n
.





<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/
