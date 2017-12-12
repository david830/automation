/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 51 "parser.ypp"

  #include "defs.hpp"
  #include "lex.yy.c" /* can't think of a better way to avoid clashes between C
                         and C++ */

  extern FILE *yyin;
  extern char *yytext;
  Block *parse_tree; // root of the parse-dag
  int blockCount = 0; // number of blocks

  vector<Block *> eq, neq, lt, lte, gt, gte, dlr, clr, frf,
                  grf, ulrf;
                  // store the blocks created so far, for the respective operators to enable
                  // sharing
  Block *fal = NULL; // block representing FALSE
  Block *tru = NULL; // block representing TRUE
  vector<Arithmetic *> var; // stores the arithmetics denoting "variables" to
                            //   add the indices (into the observation array)
                            //   after seeing the trace file


  #ifdef VERBOSE
    class Q_el {
    public:
      Node *n;
      Q_el *next;
    };
  #endif

  void copySubtreeParse(Block *);
  void copySubtreeParse(Arithmetic *);
  void recursiveDelete(Block *);
  void recursiveDelete(Arithmetic *);
  Block *find(vector<Block *>, const double &, Block *, Block *);
  Block *find(vector<Block *>, Arithmetic *, Arithmetic *);
  Block *pushNeg(Block *);
  void assignIds(Block *);
  #ifdef VERBOSE
    void traverse(Node *);
  #endif
  int yyerror(const char *);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 126 "parser.ypp"
{
  double constant;
  Block *block_node;
  Arithmetic *arith_node;
}
/* Line 193 of yacc.c.  */
#line 207 "parser.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 220 "parser.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  29
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   82

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  35
/* YYNRULES -- Number of states.  */
#define YYNSTATES  70

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,    11,    13,    17,    19,    26,
      29,    35,    41,    43,    45,    47,    49,    51,    55,    59,
      63,    67,    71,    75,    79,    81,    84,    87,    91,    95,
      97,   101,   105,   107,   109,   111
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      33,     0,    -1,    34,    -1,    35,    -1,    34,    25,    35,
      -1,    36,    -1,    35,    24,    36,    -1,    37,    -1,    36,
      29,    10,    38,    11,    37,    -1,    23,    37,    -1,    27,
      10,    38,    11,    37,    -1,    28,    10,    38,    11,    37,
      -1,    39,    -1,     5,    -1,     4,    -1,     6,    -1,     7,
      -1,     8,    34,     9,    -1,    40,    15,    40,    -1,    40,
      16,    40,    -1,    40,    13,    40,    -1,    40,    14,    40,
      -1,    40,    17,    40,    -1,    40,    18,    40,    -1,    41,
      -1,    21,    41,    -1,    20,    41,    -1,    40,    20,    41,
      -1,    40,    21,    41,    -1,    42,    -1,    41,    19,    42,
      -1,    41,    22,    42,    -1,     5,    -1,     4,    -1,     3,
      -1,     8,    40,     9,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   137,   137,   152,   159,   176,   183,   200,   207,   224,
     239,   250,   261,   270,   277,   286,   298,   310,   324,   338,
     352,   366,   380,   394,   410,   417,   424,   431,   438,   447,
     454,   461,   470,   477,   484,   492
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID_TOK", "T_DOUBLE_TOK", "T_INT_TOK",
  "TRUE_TOK", "FALSE_TOK", "L_PAREN_TOK", "R_PAREN_TOK", "L_SQUARE_TOK",
  "R_SQUARE_TOK", "COMMA_TOK", "EQ_TOK", "NEQ_TOK", "LTE_TOK", "LT_TOK",
  "GTE_TOK", "GT_TOK", "MUL_TOK", "ADD_TOK", "SUB_TOK", "DIV_TOK",
  "NEG_TOK", "AND_TOK", "OR_TOK", "NEXT_TOK", "FUTURE_TOK", "GLOBAL_TOK",
  "UNTIL_TOK", "UNKN_TOK", "ENDOFFILE", "$accept", "formula", "disj",
  "conj", "until", "neg", "bound", "relation", "sum", "term", "factor", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    36,    36,    37,
      37,    37,    37,    38,    38,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    40,    40,    40,    40,    40,    41,
      41,    41,    42,    42,    42,    42
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     3,     1,     3,     1,     6,     2,
       5,     5,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     3,     3,     1,
       3,     3,     1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    34,    33,    32,    15,    16,     0,     0,     0,     0,
       0,     0,     0,     2,     3,     5,     7,    12,     0,    24,
      29,     0,     0,     0,    26,    25,     9,     0,     0,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,    35,     0,    14,    13,     0,     0,
       4,     6,     0,    20,    21,    18,    19,    22,    23,    27,
      28,    30,    31,     0,     0,     0,    10,    11,     0,     8
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    48,    17,    18,    19,
      20
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -12
static const yytype_int8 yypact[] =
{
      23,   -12,   -12,   -12,   -12,   -12,    23,     7,     7,    23,
      -7,     8,    20,    -1,    18,     3,   -12,   -12,    58,    -3,
     -12,    -2,    47,     1,    -3,    -3,   -12,     9,     9,   -12,
      23,    23,    35,     1,     1,     1,     1,     1,     1,     7,
       7,     7,     7,   -12,   -12,    49,   -12,   -12,    38,    46,
      18,     3,     9,    27,    27,    27,    27,    27,    27,    -3,
      -3,   -12,   -12,    23,    23,    55,   -12,   -12,    23,   -12
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -12,   -12,    71,    50,    51,    -9,   -11,   -12,     2,    -6,
      11
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      26,    24,    25,    27,     1,     2,     3,    43,    22,    23,
       1,     2,     3,    46,    47,    23,    41,    49,    28,    42,
      29,     7,     8,    30,    30,    45,     1,     2,     3,     4,
       5,     6,    32,    59,    60,    53,    54,    55,    56,    57,
      58,    65,    31,     7,     8,    52,     9,    39,    40,    63,
      10,    11,    61,    62,    66,    67,    44,    64,    44,    69,
      33,    34,    35,    36,    37,    38,    68,    39,    40,    39,
      40,    33,    34,    35,    36,    37,    38,    21,    39,    40,
      50,     0,    51
};

static const yytype_int8 yycheck[] =
{
       9,     7,     8,    10,     3,     4,     5,     9,     6,     8,
       3,     4,     5,     4,     5,     8,    19,    28,    10,    22,
       0,    20,    21,    25,    25,    23,     3,     4,     5,     6,
       7,     8,    29,    39,    40,    33,    34,    35,    36,    37,
      38,    52,    24,    20,    21,    10,    23,    20,    21,    11,
      27,    28,    41,    42,    63,    64,     9,    11,     9,    68,
      13,    14,    15,    16,    17,    18,    11,    20,    21,    20,
      21,    13,    14,    15,    16,    17,    18,     6,    20,    21,
      30,    -1,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    20,    21,    23,
      27,    28,    33,    34,    35,    36,    37,    39,    40,    41,
      42,    34,    40,     8,    41,    41,    37,    10,    10,     0,
      25,    24,    29,    13,    14,    15,    16,    17,    18,    20,
      21,    19,    22,     9,     9,    40,     4,     5,    38,    38,
      35,    36,    10,    40,    40,    40,    40,    40,    40,    41,
      41,    42,    42,    11,    11,    38,    37,    37,    11,    37
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 138 "parser.ypp"
    {
             #ifdef VERBOSE
               printf("\tformula: disj\n");
             #endif
             (yyval.block_node) = (yyvsp[(1) - (1)].block_node);
             #ifdef VERBOSE
               traverse((yyval.block_node));
             #endif
             /* copy $$ to global memory */
             parse_tree = (yyval.block_node);
             assignIds(parse_tree);
           ;}
    break;

  case 3:
#line 153 "parser.ypp"
    {
          #ifdef VERBOSE
            printf("\tdisj: conj\n");
          #endif
          (yyval.block_node) = (yyvsp[(1) - (1)].block_node);
        ;}
    break;

  case 4:
#line 160 "parser.ypp"
    {
          #ifdef VERBOSE
            printf("\tdisj: disj OR conj\n");
          #endif
          if ((yyvsp[(1) - (3)].block_node) == (yyvsp[(3) - (3)].block_node)) {
            (yyval.block_node) = (yyvsp[(1) - (3)].block_node);
          } else {
            (yyval.block_node) = find(dlr, -0.1, (yyvsp[(1) - (3)].block_node), (yyvsp[(3) - (3)].block_node));
            if ((yyval.block_node) == NULL) {
              (yyval.block_node) = new Block(NOOP, D_LR, -0.1, (yyvsp[(1) - (3)].block_node), (yyvsp[(3) - (3)].block_node));
              dlr.push_back((yyval.block_node));
            }
          }
        ;}
    break;

  case 5:
#line 177 "parser.ypp"
    {
          #ifdef VERBOSE
            printf("\tconj: until\n");
          #endif
          (yyval.block_node) = (yyvsp[(1) - (1)].block_node);
        ;}
    break;

  case 6:
#line 184 "parser.ypp"
    {
          #ifdef VERBOSE
            printf("\tconj: conj AND until\n");
          #endif
          if ((yyvsp[(1) - (3)].block_node) == (yyvsp[(3) - (3)].block_node)) {
            (yyval.block_node) = (yyvsp[(1) - (3)].block_node);
          } else {
            (yyval.block_node) = find(clr, -0.1, (yyvsp[(1) - (3)].block_node), (yyvsp[(3) - (3)].block_node));
            if ((yyval.block_node) == NULL) {
              (yyval.block_node) = new Block(NOOP, C_LR, -0.1, (yyvsp[(1) - (3)].block_node), (yyvsp[(3) - (3)].block_node));
              clr.push_back((yyval.block_node));
            }
          }
        ;}
    break;

  case 7:
#line 201 "parser.ypp"
    {
           #ifdef VERBOSE
             printf("\tuntil: neg\n");
           #endif
           (yyval.block_node) = (yyvsp[(1) - (1)].block_node);
         ;}
    break;

  case 8:
#line 208 "parser.ypp"
    {
           #ifdef VERBOSE
             printf("\tuntil: until UNTIL L_SQUARE bound R_SQUARE neg\n");
           #endif
           if ((yyvsp[(1) - (6)].block_node) == (yyvsp[(6) - (6)].block_node)) {
             (yyval.block_node) = (yyvsp[(1) - (6)].block_node);
           } else {
             (yyval.block_node) = find(ulrf, (yyvsp[(4) - (6)].constant), (yyvsp[(1) - (6)].block_node), (yyvsp[(6) - (6)].block_node));
             if ((yyval.block_node) == NULL) {
               (yyval.block_node) = new Block(NOOP, U_LRF, (yyvsp[(4) - (6)].constant), (yyvsp[(1) - (6)].block_node), (yyvsp[(6) - (6)].block_node));
               ulrf.push_back((yyval.block_node));
             }
           }
         ;}
    break;

  case 9:
#line 225 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tneg: NEG neg\n");
         #endif
         //$$ = pushNeg((LogicalRelationalNode *)$2);
         (yyval.block_node) = pushNeg((yyvsp[(2) - (2)].block_node));
       ;}
    break;

  case 10:
#line 240 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tneg: FUTURE L_SQUARE bound R_SQUARE neg\n");
         #endif
         (yyval.block_node) = find(frf, (yyvsp[(3) - (5)].constant), NULL, (yyvsp[(5) - (5)].block_node));
         if ((yyval.block_node) == NULL) {
           (yyval.block_node) = new Block(NOOP, F_RF, (yyvsp[(3) - (5)].constant), NULL, (yyvsp[(5) - (5)].block_node));
           frf.push_back((yyval.block_node));
         }
       ;}
    break;

  case 11:
#line 251 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tneg: GLOBAL L_SQUARE bound R_SQUARE neg\n");
         #endif
         (yyval.block_node) = find(grf, (yyvsp[(3) - (5)].constant), NULL, (yyvsp[(5) - (5)].block_node));
         if ((yyval.block_node) == NULL) {
           (yyval.block_node) = new Block(NOOP, G_RF, (yyvsp[(3) - (5)].constant), NULL, (yyvsp[(5) - (5)].block_node));
           grf.push_back((yyval.block_node));
         }
       ;}
    break;

  case 12:
#line 262 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tneg: relation\n");
         #endif
         (yyval.block_node) = (yyvsp[(1) - (1)].block_node);
       ;}
    break;

  case 13:
#line 271 "parser.ypp"
    {
           #ifdef VERBOSE
             printf("\tbound: T_INT\n");
           #endif
           (yyval.constant) = atof(yytext);
         ;}
    break;

  case 14:
#line 278 "parser.ypp"
    {
           #ifdef VERBOSE
             printf("\tbound: T_DOUBLE\n");
           #endif
           (yyval.constant) = atof(yytext);
         ;}
    break;

  case 15:
#line 287 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: TRUE\n");
              #endif
              if (tru == NULL) {
                (yyval.block_node) = new Block(NOOP, T, -0.1, (Block *)NULL, (Block *)NULL);
                tru = (yyval.block_node);
              } else {
                (yyval.block_node) = tru;
              }
            ;}
    break;

  case 16:
#line 299 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: FALSE\n");
              #endif
              if (fal == NULL) {
                (yyval.block_node) = new Block(NOOP, F, -0.1, (Block *)NULL, (Block *)NULL);
                fal = (yyval.block_node);
              } else {
                (yyval.block_node) = fal;
              }
            ;}
    break;

  case 17:
#line 311 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: L_PAREN disj R_PAREN\n");
              #endif
              (yyval.block_node) = (yyvsp[(2) - (3)].block_node);
            ;}
    break;

  case 18:
#line 325 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: sum LTE sum\n");
              #endif
              (yyval.block_node) = find(lte, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
              if ((yyval.block_node) == NULL) {
                (yyval.block_node) = new Block(LTE, B, -0.1, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
                lte.push_back((yyval.block_node));
              } else {
                recursiveDelete((yyvsp[(1) - (3)].arith_node));
                recursiveDelete((yyvsp[(3) - (3)].arith_node));
              }
            ;}
    break;

  case 19:
#line 339 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: sum LT sum\n");
              #endif
              (yyval.block_node) = find(lt, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
              if ((yyval.block_node) == NULL) {
                (yyval.block_node) = new Block(LT, B, -0.1, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
                lt.push_back((yyval.block_node));
              } else {
                recursiveDelete((yyvsp[(1) - (3)].arith_node));
                recursiveDelete((yyvsp[(3) - (3)].arith_node));
              }
            ;}
    break;

  case 20:
#line 353 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: sum EQ sum\n");
              #endif
              (yyval.block_node) = find(eq, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
              if ((yyval.block_node) == NULL) {
                (yyval.block_node) = new Block(EQ, B, -0.1, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
                eq.push_back((yyval.block_node));
              } else {
                recursiveDelete((yyvsp[(1) - (3)].arith_node));
                recursiveDelete((yyvsp[(3) - (3)].arith_node));
              }
            ;}
    break;

  case 21:
#line 367 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: sum NEQ sum\n");
              #endif
              (yyval.block_node) = find(neq, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
              if ((yyval.block_node) == NULL) {
                (yyval.block_node) = new Block(NEQ, B, -0.1, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
                neq.push_back((yyval.block_node));
              } else {
                recursiveDelete((yyvsp[(1) - (3)].arith_node));
                recursiveDelete((yyvsp[(3) - (3)].arith_node));
              }
            ;}
    break;

  case 22:
#line 381 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: sum GTE sum\n");
              #endif
              (yyval.block_node) = find(gte, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
              if ((yyval.block_node) == NULL) {
                (yyval.block_node) = new Block(GTE, B, -0.1, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
                gte.push_back((yyval.block_node));
              } else {
                recursiveDelete((yyvsp[(1) - (3)].arith_node));
                recursiveDelete((yyvsp[(3) - (3)].arith_node));
              }
            ;}
    break;

  case 23:
#line 395 "parser.ypp"
    {
              #ifdef VERBOSE
                printf("\trelation: sum GT sum\n");
              #endif
              (yyval.block_node) = find(gt, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
              if ((yyval.block_node) == NULL) {
                (yyval.block_node) = new Block(GT, B, -0.1, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
                gt.push_back((yyval.block_node));
              } else {
                recursiveDelete((yyvsp[(1) - (3)].arith_node));
                recursiveDelete((yyvsp[(3) - (3)].arith_node));
              }
            ;}
    break;

  case 24:
#line 411 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tsum: term\n");
         #endif
         (yyval.arith_node) = (yyvsp[(1) - (1)].arith_node);
       ;}
    break;

  case 25:
#line 418 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tsum: SUB term\n");
         #endif
         (yyval.arith_node) = new Arithmetic(SUB, "", 0.0, NULL, (yyvsp[(2) - (2)].arith_node));
       ;}
    break;

  case 26:
#line 425 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tsum: ADD term\n");
         #endif
         (yyval.arith_node) = new Arithmetic(ADD, "", 0.0, NULL, (yyvsp[(2) - (2)].arith_node));
       ;}
    break;

  case 27:
#line 432 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tsum: sum ADD term\n");
         #endif
         (yyval.arith_node) = new Arithmetic(ADD, "", 0.0, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
       ;}
    break;

  case 28:
#line 439 "parser.ypp"
    {
         #ifdef VERBOSE
           printf("\tsum: sum SUB term\n");
         #endif
         (yyval.arith_node) = new Arithmetic(SUB, "", 0.0, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
       ;}
    break;

  case 29:
#line 448 "parser.ypp"
    {
          #ifdef VERBOSE
            printf("\tterm: factor\n");
          #endif
          (yyval.arith_node) = (yyvsp[(1) - (1)].arith_node);
        ;}
    break;

  case 30:
#line 455 "parser.ypp"
    {
          #ifdef VERBOSE
            printf("\tterm: term MUL factor\n");
          #endif
          (yyval.arith_node) = new Arithmetic(MUL, "", 0.0, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
        ;}
    break;

  case 31:
#line 462 "parser.ypp"
    {
          #ifdef VERBOSE
            printf("\tterm: term DIV factor\n");
          #endif
          (yyval.arith_node) = new Arithmetic(DIV, "", 0.0, (yyvsp[(1) - (3)].arith_node), (yyvsp[(3) - (3)].arith_node));
        ;}
    break;

  case 32:
#line 471 "parser.ypp"
    {
            #ifdef VERBOSE
              printf("\tfactor: T_INT\n");
            #endif
            (yyval.arith_node) = new Arithmetic(NOOP, "", atof(yytext), NULL, NULL);
          ;}
    break;

  case 33:
#line 478 "parser.ypp"
    {
            #ifdef VERBOSE
              printf("\tfactor: T_DOUBLE\n");
            #endif
            (yyval.arith_node) = new Arithmetic(NOOP, "", atof(yytext), NULL, NULL);
          ;}
    break;

  case 34:
#line 485 "parser.ypp"
    {
            #ifdef VERBOSE
              printf("\tfactor: ID\n");
            #endif
            (yyval.arith_node) = new Arithmetic(NOOP, yytext, 0.0, NULL, NULL);
            var.push_back((yyval.arith_node));
          ;}
    break;

  case 35:
#line 493 "parser.ypp"
    {
            #ifdef VERBOSE
              printf("\tfactor: L_PAREN sum R_PAREN\n");
            #endif
            (yyval.arith_node) = (yyvsp[(2) - (3)].arith_node);
          ;}
    break;


/* Line 1267 of yacc.c.  */
#line 1901 "parser.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 501 "parser.ypp"


// only used for blocks of type B. note that except the root of this subtree,
// every other node is Arithmetic
void copySubtreeParse(Block *b) {
#ifdef ASSERTS
  assert(b != NULL);
  assert(b -> t == B);
#endif
  Arithmetic *la, *ra, *ga;
  Block *b1; Arithmetic *a;

  /* left */
  ga = b -> G -> LA;
  if (ga == NULL) {
    la = NULL;
  } else {
    la = new Arithmetic(ga, b, LEFT);
    if (!(la -> var).empty()) { // if it's a variable
      var.push_back(la);
    }
  }
  b -> LA = la;
  if (la != NULL) {
    copySubtreeParse(la);
  }

  /* right */
  ga = b -> G -> RA;
  if (ga == NULL) {
    ra = NULL;
  } else {
    ra = new Arithmetic(ga, b, LEFT);
    if (!(ra -> var).empty()) { // if it's a variable
      var.push_back(ra);
    }
  }
  b -> RA = ra;
  if (ra != NULL) {
    copySubtreeParse(ra);
  }
}

void copySubtreeParse(Arithmetic *a) {
#ifdef ASSERTS
  assert(a != NULL);
#endif
  Arithmetic *l, *r, *g;

  g = a -> G -> L;
  if (g == NULL) {
    l = NULL;
  } else {
    l = new Arithmetic(g, a, LEFT);
    if (!(l -> var).empty()) { // if it's a variable
      var.push_back(l);
    }
  }
  a -> L = l;
  if (l != NULL) {
    copySubtreeParse(l);
  }

  g = a -> G -> R;
  if (g == NULL) {
    r = NULL;
  } else {
    r = new Arithmetic(g, a, RIGHT);
    if (!(r -> var).empty()) { // if it's a variable
      var.push_back(r);
    }
  }
  a -> R = r;
  if (l != NULL) {
    copySubtreeParse(r);
  }
}

// find the block in v with bound d and children l and r
Block *find(vector<Block *> v, const double &d, Block *l, Block *r) {
  Block *b;
  for (vector<Block *>::iterator it = v.begin(); it != v.end(); it++) {
    b = (*it);
    if (b -> bound == d && b -> LB == l && b -> RB == r) {
      return b;
    }
  }
  return NULL;
}

// tests if a1 and a2 are the same in the sense that they compute the same values
bool matchArithmetic(Arithmetic *a1, Arithmetic *a2) {
  if (a1 -> op != a2 -> op) return false;
  bool b1, b2;
  if (a1 -> op == NOOP) {
    if (!(a1 -> var).empty()) {
      if ((a1 -> var).compare((a2 -> var)) != 0) {
        return false;
      }
    } else {
      if (a1 -> val != a2 -> val) {
        return false;
      }
    }
  }
  if (a1 -> L) {
    if (a2 -> L == NULL) return false;
    b1 = matchArithmetic(a1 -> L, a2 -> L);
  }
  if (a1 -> R) {
    if (a2 -> R == NULL) return false;
    b2 = matchArithmetic(a1 -> R, a2 -> R);
  }
  if (b1 == false || b2 == false)
    return false;

  return true;
}

// find the block with children functioning similar to l and r
Block *find(vector<Block *> v, Arithmetic *l, Arithmetic *r) {
  // It's known that blocks in v are of type B
  Block *b;
  for (vector<Block *>::iterator it = v.begin(); it != v.end(); it++) {
    b = (*it);
    // go through the children of b recursively and check if they are same as l
    // and r, semantically. And not syntactically matching the pointer
    // addresses!
    if (matchArithmetic(b -> LA, l) && matchArithmetic(b -> RA, r)) {
      return b;
    }
  }
  return NULL;
}

// remove a from the vector "var"
void removeFromVar(Arithmetic *a) {
  for (vector<Arithmetic *>::iterator it = var.begin(); it != var.end(); it++) {
    if (*it == a) {
      var.erase(it);
      break;
    }
  }
}

// delete the unwanted arithmetics (which don't propagate to anybody) in the
// subtree rooted at a
void recursiveDelete(Arithmetic *a) {
  if (a -> L) {
    recursiveDelete(a -> L);
  }
  if (a -> R) {
    recursiveDelete(a -> R);
  }
  /* if it is a variable, remove from "var" */
  if (!(a -> var).empty()) {
    removeFromVar(a);
  }
  delete a;
}

// delete the unwanted blocks (which don't propagate to anybody) in the
// subtree rooted at b
void recursiveDelete(Block *b) {
  map<Block *, Direction> &m = b -> propagate;
  switch (b -> t) {
  case F:
  case T:
    delete b;
    break;
  case B:
    if (b -> LA) {
      recursiveDelete(b -> LA);
    }
    if (b -> RA) {
      recursiveDelete(b -> RA);
    }
    delete b;
    break;
  default:
    if (b -> LB) {
      m = (b -> LB) -> propagate;
      m.erase(m.find(b));
      if (m.empty()) {
        recursiveDelete(b -> LB);
      }
    }
    if (b -> RB) {
      m = (b -> RB) -> propagate;
      m.erase(m.find(b));
      if (m.empty()) {
        recursiveDelete(b -> RB);
      }
    }
    delete b;
    break;
  }
}

/**
 * this is a utility function for converting the input property to Negation Normal
 * Form. It pushes the negation sign inwards recursively
 */
Block *pushNeg(Block *b) {
  if (b == NULL) {
    printf("error! pushing neg inside NULL\n");
    return NULL;
  }
  Block *lb, *rb, *ret;
  Block *lb1, *rb1, *rb2, *temp;
  lb = b -> LB;
  rb = b -> RB;
  switch (b -> t) {
  case T:
    if (fal != NULL) {
      ret = fal;
    } else {
      ret = fal = new Block(NOOP, F, -0.1, (Block *)NULL, (Block *)NULL);
    }
    break;

  case F:
    if (tru != NULL) {
      ret = tru;
    } else {
      ret = tru = new Block(NOOP, T, -0.1, (Block *)NULL, (Block *)NULL);
    }
    break;

  case B:
    switch (b -> op) {
    case EQ:
      ret = find(neq, b -> LA, b -> RA);
      if (ret == NULL) {
        ret = new Block(NEQ, B, -0.1, b);
        copySubtreeParse(ret);
        ret -> G = ret;
        neq.push_back(ret);
      }
      break;

    case NEQ:
      ret = find(eq, b -> LA, b -> RA);
      if (ret == NULL) {
        ret = new Block(EQ, B, -0.1, b);
        copySubtreeParse(ret);
        ret -> G = ret;
        eq.push_back(ret);
      }
      break;

    case LT:
      ret = find(gte, b -> LA, b -> RA);
      if (ret == NULL) {
        ret = new Block(GTE, B, -0.1, b);
        copySubtreeParse(ret);
        ret -> G = ret;
        gte.push_back(ret);
      }
      break;

    case LTE:
      ret = find(gt, b -> LA, b -> RA);
      if (ret == NULL) {
        ret = new Block(GT, B, -0.1, b);
        copySubtreeParse(ret);
        ret -> G = ret;
        gt.push_back(ret);
      }
      break;

    case GT:
      ret = find(lte, b -> LA, b -> RA);
      if (ret == NULL) {
        ret = new Block(LTE, B, -0.1, b);
        copySubtreeParse(ret);
        ret -> G = ret;
        lte.push_back(ret);
      }
      break;

    case GTE:
      ret = find(lt, b -> LA, b -> RA);
      if (ret == NULL) {
        ret = new Block(LT, B, -0.1, b);
        copySubtreeParse(ret);
        ret -> G = ret;
        lt.push_back(ret);
      }
      break;

    default:
      assert(0);
    }

    break;

  case D_LR:
    lb1 = pushNeg(lb);
    rb1 = pushNeg(rb);

    ret = find(clr, -0.1, lb1, rb1);
    if (ret == NULL) {
      ret = new Block(NOOP, C_LR, -0.1, lb1, rb1);
      clr.push_back(ret);
    } 

    break;

  case C_LR:
    lb1 = pushNeg(lb);
    rb1 = pushNeg(rb);

    ret = find(dlr, -0.1, lb1, rb1);
    if (ret == NULL) {
      ret = new Block(NOOP, D_LR, -0.1, lb1, rb1);
      dlr.push_back(ret);
    } 

    break;

  case F_RF:
    rb1 = pushNeg(rb);

    ret = find(grf, b -> bound, NULL, rb1);
    if (ret == NULL) {
      ret = new Block(NOOP, G_RF, b -> bound, NULL, rb1);
      grf.push_back(ret);
    }

    break;

  case G_RF:
    rb1 = pushNeg(rb);

    ret = find(frf, b -> bound, NULL, rb1);
    if (ret == NULL) {
      ret = new Block(NOOP, F_RF, b -> bound, NULL, rb1);
      frf.push_back(ret);
    }

    break;

  case U_LRF:
    // ~(pUq) = [~q U (~p /\ ~q)] \/ G(~q)
    lb1 = pushNeg(lb);
    rb1 = pushNeg(rb);

    // ~p /\ ~q
    rb2 = find(clr, -0.1, lb1, rb1);
    if (rb2 == NULL) {
      rb2 = new Block(NOOP, C_LR, -0.1, lb1, rb1);
      clr.push_back(rb2);
    }

    // ~q U (~p /\ ~q)
    temp = find(ulrf, b -> bound, lb1, rb2);
    if (temp == NULL) {
      lb1 = new Block(NOOP, U_LRF, b -> bound, lb1, rb2);
      ulrf.push_back(lb1);
    } else {
      lb1 = temp;
    }

    // G(~q)
    rb2 = find(grf, b -> bound, NULL, rb1);
    if (rb2 == NULL) {
      rb2 = new Block(NOOP, G_RF, b -> bound, NULL, rb1);
      grf.push_back(rb2);
    }

    // the rest...
    ret = find(dlr, -0.1, lb1, rb2);
    if (ret == NULL) {
      ret = new Block(NOOP, D_LR, -0.1, lb1, rb2);
      dlr.push_back(ret);
    }

    break;
  }

  if ((b->propagate).empty()) { // you only created the negated version of b.
                                // now, b (and its subtree) is not needed, if b
                                // doesn't propagate to anybody!
    recursiveDelete(b); // delete b from its children's propagates, delete b,
                        // call the function on the children if their propagates
                        // are empty
  }

  return ret;
}


//Block *pushNeg(Block *b) {
////LogicalRelationalNode *pushNeg(LogicalRelationalNode *n)
//  if (b == NULL) {
//    printf("error! pushing neg inside NULL\n");
//    return NULL;
//  }
//  switch (b -> t) {
//  case T:
//    b -> t = F;
//    break;
//
//  case F:
//    b -> t = T;
//    break;
//
//  case B:
//    switch (b -> op) {
//    case EQ:
//      b -> op = NEQ;
//      break;
//    case NEQ:
//      b -> op = EQ;
//      break;
//    case LT:
//      b -> op = GTE;
//      break;
//    case LTE:
//      b -> op = GT;
//      break;
//    case GT:
//      b -> op = LTE;
//      break;
//    case GTE:
//      b -> op = LT;
//      break;
//    default:
//      assert(0);
//    }
//    break;
//
//  case D_LR:
//    b -> t = C_LR;
//    b -> LB = pushNeg(b -> LB);
//    b -> RB = pushNeg(b -> RB);
//    break;
//
//  case C_LR:
//    b -> t = D_LR;
//    b -> LB = pushNeg(b -> LB);
//    b -> RB = pushNeg(b -> RB);
//    break;
//
//  case F_RF:
//    b -> t = G_RF;
//    b -> RB = pushNeg(b -> RB);
//    break;
//
//  case G_RF:
//    b -> t = F_RF;
//    b -> RB = pushNeg(b -> RB);
//    break;
//
//  case U_LRF:
//    // get accurate value for blockCount, by first counting the number of
//    // "block"s in the left and right subtree (p and q), and second, subtracting
//    // this number from blockCount and starting over...don't think of using bL
//    // and bR - just delete them at the end!
//    // ~(pUq) = [~q U (~p /\ ~q)] | G(~q)
//    Block *b1, *b2;
//    Block *bL = pushNeg(b -> LB);
//    Block *bR = pushNeg(b -> RB);
//
//    // copying ~q
//    b2 = new Block(*bR);
//    copySubtreeParse(b2);
//    b2 -> G = b2;
//
//    b2 = new Block(NOOP, C_LR, -0.1, bL, b2); // ~p /\ ~q
//
//    // copying ~q
//    b1 = new Block(*bR);
//    copySubtreeParse(b1);
//    b1 -> G = b1;
//
//    b1 = new Block(NOOP, U_LRF, b -> bound, b1, b2);
//
//    b2 = new Block(NOOP, G_RF, b -> bound, NULL, bR);
//
//    b = new Block(NOOP, D_LR, -0.1, b1, b2);
//
//    break;
//  }
//  return b;
//}

// assign ids to blocks
void assignIds(Block *b) {
  if (b -> id >= 0) return; // revisited!

  if (b -> LB) {
    assignIds(b -> LB);
  }
  b -> id = blockCount;
  blockCount++;
  if (b -> RB) {
    assignIds(b -> RB);
  }
}

#ifdef VERBOSE
  /**
   * traverse the entire subtree rooted at "root" in a breadth-first manner and
   * print out some information
   */
  void traverse(Node *root) {
    /* do a bfs */
    Q_el *head, *end;
    Node *current;
  
    head = new Q_el();
    head -> n = root;
    head -> next = NULL;
    end = head;
  
    Arithmetic *a; string var; double val;
    Block *b;
    while (head != NULL) {
      current = head -> n;
      if (current->left == NULL && current->right == NULL) {
        /* leaf node */
        if (current->getType() == ARITHMETIC) {
          a = current -> getA();
          var = a -> getVar(); val = a -> getVal();
          if (!var.empty()) {
            cout << var << "  ";
          } else {
            cout << val << "  ";
          }
        } else {
          b = current -> getB();
          if (b -> getType() == T) {
            cout << "TRUE  ";
          } else if (b -> getType() == F) {
            cout << "FALSE  ";
          } else {
            assert(0);
          }
        }
      } else {
        /* non-leaf node */
        if (current->getType() == ARITHMETIC) {
          a = current -> getA();
          cout << a -> getOp() << "  ";
        } else {
          b = current -> getB();
          cout << b -> getOp() << ":" << b -> getBound() << "  ";
        }
        if (current->left != NULL) {
          end->next = new Q_el();
          end->next->n = current->left;
          end = end->next;
          end->next = NULL;
        }
        if (current->right != NULL) {
          end->next = new Q_el();
          end->next->n = current->right;
          end = end->next;
          end->next = NULL;
        }
      }
      head = head -> next;
    }
    printf("\n");
  }
#endif

/**
 * this is called when a parse error is encountered by Bison
 */
int yyerror(const char *s) {
  printf("%s\n", s);
}

