
Importing a HyperLynx file occurs in two steps:
 1. the HyperLynx file is translated into a matlab function (hyp2mat) 
 2. the generated matlab function is executed.

If an error occurs while scanning or parsing the HyperLynx file, an error message is printed, and scanning or parsing continues with the next HyperLynx record or subrecord, if any.

Of course, it is always prudent to check the cause of an error message. Usually, a trivial edit of the HyperLynx file will suppress the error message.

- Put all strings between double quotes
 
  error:  syntax error, unexpected STRING at 'Logo'
  line: (? REF=Altium Logo BOT1 L=Bottom_Layer)
  diagnosis: Error in program which created the .hyp file: Unquoted string contains space ' '
  solution: (? REF="Altium Logo BOT1" L=Bottom_Layer)

- Assign a value to a component if it does not have any

  error: syntax error, unexpected L, expecting FLOAT or STRING at 'L'
  line: (L REF="L1" VAL= L="Top")
  diagnosis: Coil L1 on layer "Top" has not been assigned a value.
  solution: (L REF="L1" VAL=0 L="Top")
  (or assign coil a value in schematics editor)


 
HYP2MAT(1)							    HYP2MAT(1)



NAME
       hyp2mat - convert hyperlynx files to matlab scripts

SYNOPSIS
       hyp2mat [ -o outfile ] [ -f funcname ] [ -h ] [ -d ] infile

DESCRIPTION
       hyp2mat	converts  Hyperlynx  Signal-Integrity Transfer Format files to
       Octave/Matlab scripts.

       Input files conventionally end in .hyp , while output files convention‐
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
	      Use  function name funcname for the generated code.  If no func‐
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
       ~/openEMS/hyp2mat/matlab/
	      Supporting matlab routines for OpenEMS.


SEE ALSO
       octave(1)
       OpenEMS,  a free and open-source electromagnetic field solver using the
       FDTD method.


AUTHOR
       Koen De Vleeschauwer, http://www.kdvelectronics.eu



				     local			    HYP2MAT(1)
