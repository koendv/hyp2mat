
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

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "hypfile.h"

  /* 
   * hooks in bison (parser) and flex (scanner) code 
   */

  extern FILE *yyin;          /* file the parser reads from */
  extern int yy_flex_debug ;  /* debug scanner if == 1 */
  extern int yydebug ;        /* debug parser if == 1 */
  extern int yylineno;
  extern char *yytext;
  extern int yyparse(HypFile::Hyp *);
  extern int yylex(void);

#endif 
