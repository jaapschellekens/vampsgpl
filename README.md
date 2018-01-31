           vamps - a Vegetation-AtMosPhere-Soil water model

                         Version 0.99i (beta)
			
			    J. Schellekens




WHAT IS VAMPS 
~~~~~~~~~~~~~ 

Vamps is a one dimensional model for describing the flow of water
through a forested ecosystem.  The main part of vamps is the module that
calculates the unsaturated flow of water through the soil by solving
Richard's equation.  This part is derived from the swap94 model. vamps
can handle time-steps of arbitrary size and they can change in a run. You
could run one year using daily time-steps but have one day of that year
(in which you might have detailed measurements) using smaller steps, 15
minutes steps for example. Because vamps includes the S-Lang stack-based
interpreted language as a language for extension it can be customized to
most applications. Users can change functions for ones that better suite
a specific situation without recompiling the program. Also vamps can be
run in an interactive mode in which variables can be studied at almost
each step in the modeling.  A running program can be stopped and forced
into interactive mode and then started again.  A start has been made with
the addition of an optimizing routine to the model. This routine uses
marquardt non-linear regression.  Parts of Vamps are derived from slash
(S-Lang Application Shell). Slash was written by Raymond Venneker with
some parts written by Jaap Schellekens.


DISTRIBUTION
~~~~~~~~~~~~

Vamps is distributed with full source code it has been tested  (and
should compile) on systems running SunOS 5.5.*, FreeBSD 2.2.5 and
RedHat-Linux 4.0--5.1. Other unix systems are untested but should work
as well.

Because of the limited testing that has been done with vamps vamps will
remain in beta for a while. It is fully functional but probably not
completely bug free and some features still lack. However, the
biggest problem is the documentation ;-)



SYSTEM REQUIREMENTS
~~~~~~~~~~~~~~~~~~~

If you are not in a hurry a 386sx MS-DOS based computer is the minimum you
need for running vamps. As most calculations are done with floating point
numbers, a 387 co-processors will speed up thing a lot. djgpp compiled
programs (a MS-DOS port of gcc) should run with 512 Kb of memory, although
1Mb should be considered a minimum. You need some disk space as well.  The
vamps binary does not use a lot of it, but your input (and certainly output)
data will probably by the biggest users of disk space. You also need
the S-lang library installed if you want to recompile the program.  (The
S-Lang library can be found at ftp://space.mit.edu/pub/davis/slang).

Vamps has some graphics abilities if you have one or several of the following
programs/libraries installed. Low level S-Lang functions are library dependent
but some library independent functions can be found in plot.sl (type '?plot'
from the vamps prompt. The show_plotsys commands show the graphics
possibilities of your version of vamps). The following list give the options in
order of preference:

	1 gnuplot (preferably 3.7 or  3.6 beta). 
	  a special character in interactive
	  mode will allow you to use gnuplot commands from within vamps. Type
	  '?plt' or '?#' from the vamps prompt to get extra information)
	  This is somewhere between octave's and fudgit's gnuplot interface
	  (if you happen to know these programs).

	2 AGL - a free cross-platform graphics library available via ftp at:
	         sisifo.arcetri.astro.it:pub/agl
	  If it is found by configure a S-Lang interface to the library will
	  be compiled and linked.

	3 GNU plotutils library - S-Lang intrinsics

	4 GNU plotutils library - external plot-* programs


J. Schellekens


