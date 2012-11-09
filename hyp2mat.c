
/*
 * hyp2mat - convert hyperlynx files to matlab scripts
 * Copyright 2012 Koen De Vleeschauwer.
 *
 * This file is part of hyp2mat.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

/* external variables */

extern int yy_flex_debug ;  /* debug scanner if == 1 */
extern int yydebug ;        /* debug parser if == 1 */

extern int yyparse();       /* parser */
extern FILE *yyin;          /* file the parser reads from */
FILE *out;                  /* file the parser writes to */

/* usage info string array */
static char *usage = "Usage: %s [-o outfile] [-d] [-d] [-d] [-f funcname] [infile]\n";


main(argc, argv)
int argc;
char **argv;
{
  char *progname;         /* name of this program */
  int c;                  /* for getopt() */
  extern char *optarg;    /* for getopt() */
  extern int optind;      /* for getopt() */
  int error_flag;         /* for command line parsing */

  char *outfile;          /* filename from -o */
  char *infile;           /* supplied filename */
  char *funcnam;          /* matlab function name */

  progname = argv[0];     /* name of this program */
  outfile = (char*) NULL; /* no filename supplied, write to stdout */
  infile = (char*) NULL;  /* no filename given, read from stdin */
  funcnam = (char*) NULL; /* no function name given, omit function declaration */

  yy_flex_debug = 0;      /* scanner debugging off */
  yydebug = 0;            /* parser debugging off */

  yyin = stdin;
  out = stdout;
  error_flag = 0;

  while ((c=getopt(argc,argv,"o:f:hd")) != -1) {
    switch(c) {
    case 'o':
      outfile = optarg ;
      break ;
    case 'f':
      funcnam = optarg ;
      break ;
    case 'd':
      if (yy_flex_debug) yydebug = !yydebug;
      yy_flex_debug = !yy_flex_debug;
      break;
    case 'h':
      error_flag++;
      break;
    case '?':
    default:
      error_flag++;
    }
  }

  if (error_flag) {
    fprintf(stderr, usage, progname);
    return 1;
  }

  /* optional input file argument */

  if (optind < argc - 1) {
    fprintf(stderr, "at most one input file\n");
    fprintf(stderr, usage, progname);
    return 1;
  }

  if (optind < argc) {
    infile = argv[optind]; /* input file name */
  }

  /* open input and output files */

  if (infile) {
    yyin = fopen (infile,"r");
    if (!yyin) {
      fprintf(stderr, "can't open input file %s\n",infile);
      return 1;
    }
  }

  if (outfile) {
    out = fopen (outfile,"w");
    if (!out) {
      fprintf(stderr, "can't open output file %s\n",outfile);
      return 1;
    }
  }

  /* print the command line which generated this file */

  fprintf(out, "%%");
  int i;
  for(i = 0; i < argc; i++) fprintf(out, " %s", argv[i]);
  fprintf(out, "\n");

  /* print function header */

  if (funcnam) fprintf(out, "\nfunction CSX = %s(CSX)\n\n", funcnam);

  /* process input file */

  int code = yyparse();

  /* print function end */

  if (funcnam) fprintf(out, "\nend\n");
  fprintf(out, "\n%% not truncated\n");

  /* close input and output files */

  fclose(yyin);
  fclose(out);

  return code;
}

/* not truncated */
