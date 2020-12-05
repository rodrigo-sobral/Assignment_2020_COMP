/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CHAR = 258,
    INT = 259,
    SHORT = 260,
    DOUBLE = 261,
    VOID = 262,
    IF = 263,
    ELSE = 264,
    WHILE = 265,
    RETURN = 266,
    LPAR = 267,
    RPAR = 268,
    LBRACE = 269,
    RBRACE = 270,
    SEMI = 271,
    COMMA = 272,
    AND = 273,
    OR = 274,
    BITWISEAND = 275,
    BITWISEOR = 276,
    BITWISEXOR = 277,
    NE = 278,
    EQ = 279,
    GE = 280,
    GT = 281,
    LE = 282,
    LT = 283,
    NOT = 284,
    ASSIGN = 285,
    MUL = 286,
    PLUS = 287,
    MINUS = 288,
    DIV = 289,
    MOD = 290,
    RESERVED = 291,
    ID = 292,
    CHRLIT = 293,
    REALLIT = 294,
    INTLIT = 295,
    THEN = 296
  };
#endif
/* Tokens.  */
#define CHAR 258
#define INT 259
#define SHORT 260
#define DOUBLE 261
#define VOID 262
#define IF 263
#define ELSE 264
#define WHILE 265
#define RETURN 266
#define LPAR 267
#define RPAR 268
#define LBRACE 269
#define RBRACE 270
#define SEMI 271
#define COMMA 272
#define AND 273
#define OR 274
#define BITWISEAND 275
#define BITWISEOR 276
#define BITWISEXOR 277
#define NE 278
#define EQ 279
#define GE 280
#define GT 281
#define LE 282
#define LT 283
#define NOT 284
#define ASSIGN 285
#define MUL 286
#define PLUS 287
#define MINUS 288
#define DIV 289
#define MOD 290
#define RESERVED 291
#define ID 292
#define CHRLIT 293
#define REALLIT 294
#define INTLIT 295
#define THEN 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "uccompiler.y"

    struct token* tk;
    struct node* n;

#line 144 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
