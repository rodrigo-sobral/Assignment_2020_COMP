%{
    //PL5_COMPILADORES@2020
    //Eduardo Filipe Ferreira Cruz          2018285164
    //Rodrigo Fernando Henriques Sobral     2018298209
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"

    /*The extern keywor, in C, is used to tell the compiler that the
     variable that we are declaring was defined elsewhere.*/
    extern int lineNum,colNum; //verificar se yyleng n precisa de ser declarado como extern
    int errorFlag=0; 

%}

%union{
    char *str;
};

%token CHAR
%token INT
%token SHORT
%token DOUBLE
%token VOID
%token IF
%token ELSE
%token WHILE
%token RETURN
%token LPAR
%token RPAR
%token LBRACE
%token RBRACE
%token SEMI
%token COMMA
%token AND
%token OR
%token BITWISEAND
%token BITWISEOR
%token BITWISEXOR
%token NE
%token EQ
%token GE
%token GT
%token LE
%token LT
%token NOT
%token ASSIGN
%token MUL
%token PLUS
%token MINUS
%token DIV
%token MOD
%token <str> RESERVED ID CHRLIT REALLIT INTLIT

%left   COMMA
%right  ASSIGN
%left   OR   
%left   AND
%left   NE  EQ  GE  GT  LE  LT   
%left   MOD DIV MUL
%right  NOT

%%
program:    functionsAndDeclarations    {}
    ;
functionsAndDeclarations:   functionsAndDeclarations functionDefinition {}
    |   functionsAndDeclarations functionDeclaration    {}
    |   functionsAndDeclarations declaration    {}
    ;
functionDefinition: typeSpec functionDeclarator functionBody    {}
    ;
functionBody:   LBRACE RBRACE   {}
    |   LBRACE declarationsAndStatements RBRACE {}
    ;
declarationsAndStatements:  statement declarationsAndStatements {}
    |   declaration declarationsAndStatements   {}
    |   statement   {}
    |   declaration {}
    ;
functionDeclaration:    typeSpec functionDeclarator SEMI    {}
    ;
functionDeclarator: ID LPAR parameterList RPAR  {}
    ;
parameterList:  parameterDeclaration parametersAux   {}
    ;
parametersAux:  /*empty*/     {}
    |   parametersAux COMMA parameterDeclaration {}
    ;
parameterDeclaration:  typeSpec {}
    |   typeSpec ID {}
    ;
declaration:  typeSpec declarator declaratorsAux SEMI    {}
    |   error SEMI
    ;
declaratorsAux: /*empty*/     {}
    |   declaratorsAux COMMA declarator  {}
    ;
typeSpec:   CHAR    {}
    |   INT {}
    |   VOID    {}
    |   SHORT   {}
    |   DOUBLE  {}
    ;
declarator: ID  {}
    |   ID ASSIGN expr  {}
    ;
statement:  SEMI    {}  
    |   expr SEMI {}
    |   error SEMI  {}
    |   LBRACE statementsAux RBRACE   {}
    |   LBRACE error RBRACE {}
    |   IF LPAR expr RPAR statement   {}
    |   IF LPAR expr RPAR statement ELSE statement   {}
    |   WHILE LPAR expr RPAR statement   {}
    |   RETURN SEMI {}
    |   RETURN expr SEMI    {}
    ;
statementsAux:  /*empty*/   {}
    |   statementsAux statement {}
    ;
expr:   expr ASSIGN expr    {}
    |   expr COMMA expr {}
    |   expr PLUS expr   {}
    |   expr MINUS expr   {}
    |   expr MUL expr   {}
    |   expr DIV expr   {}
    |   expr MOD expr   {}
    |   expr OR expr   {}
    |   expr AND expr   {}
    |   expr BITWISEAND expr   {}
    |   expr BITWISEOR expr   {}
    |   expr BITWISEXOR expr   {}
    |   expr EQ expr   {}
    |   expr NE expr   {}
    |   expr LE expr   {}
    |   expr GE expr   {}
    |   expr LT expr   {}
    |   expr GT expr   {}
    |   PLUS expr %prec NOT   {}
    |   MINUS expr %prec NOT    {}
    |   NOT expr   {}
    |   ID LPAR RPAR   {}
    |   ID LPAR expr exprsAux RPAR   {}
    |   ID LPAR error RPAR  {}
    |   ID   {}
    |   INTLIT  {}
    |   CHRLIT  {}
    |   REALLIT {}
    |   LPAR expr RPAR  {}
    |   LPAR error RPAR {}
    ;
exprsAux:   /*empty*/   {}
    |   exprsAux COMMA expr {}
    ;
%%

void yyerror (char *s) { //sintax errors
    printf("Line %d, col %d: %s: %s\n", lineNum,colNum-yyleng,s,yytext);
    errorFlag=1; //syntax error happened!
}