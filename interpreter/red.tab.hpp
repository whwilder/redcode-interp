/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INTERPRETER_RED_TAB_HPP_INCLUDED
# define YY_YY_INTERPRETER_RED_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OPDAT = 258,
    OPMOV = 259,
    OPADD = 260,
    OPSUB = 261,
    OPMUL = 262,
    OPDIV = 263,
    OPMOD = 264,
    OPJMP = 265,
    OPJMZ = 266,
    OPDJN = 267,
    OPSPL = 268,
    OPCMP = 269,
    OPSEQ = 270,
    OPSNE = 271,
    OPSLT = 272,
    OPLDP = 273,
    OPSTP = 274,
    OPNOP = 275,
    MOD_A = 276,
    MOD_B = 277,
    MOD_AB = 278,
    MOD_BA = 279,
    MOD_F = 280,
    MOD_X = 281,
    MOD_I = 282,
    STRING = 283,
    UNKNOWN = 284,
    INTCON = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 22 "interpreter/red.ypp" /* yacc.c:1909  */

   char *charptr;
   char ch;
   int intcon;

   int opcode;
   int addr_mode;
   int modifier;

#line 95 "interpreter/red.tab.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INTERPRETER_RED_TAB_HPP_INCLUDED  */
