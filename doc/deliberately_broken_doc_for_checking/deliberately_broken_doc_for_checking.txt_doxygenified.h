/**

\mainpage IGNORE THIS! Deliberately broken doc page for self-test


Here's a code inclusion that works:

\dontinclude scattering.cc
\skipline start_of_namespace
\until std::complex<double> I(0.0,1.0);  

...and here's one that doesn't because I've deliberately broken the hook:


\dontinclude scattering.cc
\skipline start_of_bla_shite
\until std::complex<double> I(0.0,1.0);  


Here's a link that works

<a href="http://en.wikipedia.org/wiki/Sommerfeld_radiation_condition">
Sommerfeld radiation condition</a>

and here's one that possibly doesn't because of an isolated "<a":

<a 
href="http://en.wikipedia.org/wiki/Sommerfeld_radiation_condition">
Sommerfeld radiation condition</a>

Bla bla <a href="http://www.maths.man.ac.uk/~mheil/page_doesnt_exist">click on
this and you won't get anywhere because the link is broken!</a> bla bla.

<hr>
<hr>
\section pdf PDF file
A <a href="../latex/refman.pdf">pdf version</a> of this document is available.
**/

