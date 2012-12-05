HYP2MAT(1)							    HYP2MAT(1)



NAME
       hyp2mat - convert hyperlynx files to matlab scripts

SYNOPSIS
       hyp2mat [ -o outfile ] [ -f funcname ] [ -h ] [ -d ] infile

DESCRIPTION
       hyp2mat	converts  Hyperlynx  Signal-Integrity Transfer Format files to
       Octave/Matlab scripts.

       Input files conventionally end in .hyp , while output files convention-
       ally end in .m .

       hyp2mat	reads  input  from file infile.  If no input file is specified
       input is read from standard in.

       If  a  syntax  error  occurs  during  conversion,  error  recovery   is
       attempted.   hyp2mat exits with zero status if conversion was succesful
       and non-zero if not.

       Typical use is with simulation packages such as OpenEMS.


OPTIONS
       -o outfile
	      Write output to file outfile.  If no output  file  is  specified
	      output is to standard out.


       -f funcname
	      Use  function name funcname for the generated code.  If no func-
	      tion name is given, omit function declaration.


       -h
	      Print usage summary and exit.


       -d
	      Switch debugging on.

	      A single -d option switches scanner debugging on.

	      A double -d -d option switches parser debugging on.

	      A triple -d -d -d option switches scanner and  parser  debugging
	      on.


EXAMPLES
       Convert pcb.hyp to pcb.m:
	      hyp2mat -o pcb.m -f pcb pcb.hyp


FILES
       /usr/share/hyp2mat/matlab/
	      Supporting matlab routines for OpenEMS.


SEE ALSO
       octave(1)
       OpenEMS,  a free and open-source electromagnetic field solver using the
       FDTD method.


AUTHOR
       Koen De Vleeschauwer, http://www.kdvelectronics.eu



				     local			    HYP2MAT(1)
