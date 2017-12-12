/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID_TOK = 258,
     T_DOUBLE_TOK = 259,
     T_INT_TOK = 260,
     TRUE_TOK = 261,
     FALSE_TOK = 262,
     L_PAREN_TOK = 263,
     R_PAREN_TOK = 264,
     L_SQUARE_TOK = 265,
     R_SQUARE_TOK = 266,
     COMMA_TOK = 267,
     EQ_TOK = 268,
     NEQ_TOK = 269,
     LTE_TOK = 270,
     LT_TOK = 271,
     GTE_TOK = 272,
     GT_TOK = 273,
     MUL_TOK = 274,
     ADD_TOK = 275,
     SUB_TOK = 276,
     DIV_TOK = 277,
     NEG_TOK = 278,
     AND_TOK = 279,
     OR_TOK = 280,
     NEXT_TOK = 281,
     FUTURE_TOK = 282,
     GLOBAL_TOK = 283,
     UNTIL_TOK = 284,
     UNKN_TOK = 285,
     ENDOFFILE = 286
   };
#endif
/* Tokens.  */
#define ID_TOK 258
#define T_DOUBLE_TOK 259
#define T_INT_TOK 260
#define TRUE_TOK 261
#define FALSE_TOK 262
#define L_PAREN_TOK 263
#define R_PAREN_TOK 264
#define L_SQUARE_TOK 265
#define R_SQUARE_TOK 266
#define COMMA_TOK 267
#define EQ_TOK 268
#define NEQ_TOK 269
#define LTE_TOK 270
#define LT_TOK 271
#define GTE_TOK 272
#define GT_TOK 273
#define MUL_TOK 274
#define ADD_TOK 275
#define SUB_TOK 276
#define DIV_TOK 277
#define NEG_TOK 278
#define AND_TOK 279
#define OR_TOK 280
#define NEXT_TOK 281
#define FUTURE_TOK 282
#define GLOBAL_TOK 283
#define UNTIL_TOK 284
#define UNKN_TOK 285
#define ENDOFFILE 286




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 126 "parser.ypp"
{
  double constant;
  Block *block_node;
  Arithmetic *arith_node;
}
/* Line 1529 of yacc.c.  */
#line 117 "parser.tab.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

