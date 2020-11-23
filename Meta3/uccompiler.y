%{
    //PL5_COMPILADORES@2020
    //Eduardo Filipe Ferreira Cruz          2018285164
    //Rodrigo Fernando Henriques Sobral     2018298209
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
    #include "ast.h"

    int yylex (void);
    void yyerror(char* s);
    /*The extern keywor, in C, is used to tell the compiler that the
     variable that we are declaring was defined elsewhere.*/
    extern int lineNum,colNum,yyleng; 
    extern char* yytext;
    int sintaxErrorFlag=0; 
    char buffer[100]; //aux
%}

%union{
    struct token* tk;
    struct node* n;
};

%token <tk> CHAR
%token <tk>  INT
%token <tk> SHORT
%token <tk>  DOUBLE
%token <tk>  VOID
%token <tk> IF
%token <tk>  ELSE
%token <tk>  WHILE
%token <tk>  RETURN
%token <tk> LPAR
%token <tk>  RPAR
%token <tk>  LBRACE
%token <tk>  RBRACE
%token <tk>  SEMI
%token <tk>  COMMA
%token <tk>  AND
%token <tk>  OR
%token <tk>  BITWISEAND
%token <tk>  BITWISEOR
%token <tk>  BITWISEXOR
%token <tk>  NE
%token <tk>  EQ
%token <tk>  GE
%token <tk>  GT
%token <tk>  LE
%token <tk>  LT
%token <tk>  NOT
%token <tk> ASSIGN
%token <tk> MUL
%token <tk>  PLUS
%token <tk>  MINUS
%token <tk> DIV
%token <tk> MOD
%token <tk> RESERVED ID CHRLIT REALLIT INTLIT

%left   COMMA
%right  ASSIGN
%left   OR   
%left   AND
%left   BITWISEOR
%left   BITWISEXOR
%left   BITWISEAND
%left   NE  EQ
%left   GE  GT  LE  LT   
%left   PLUS MINUS
%left   MOD DIV MUL
%right  NOT

%nonassoc THEN
%nonassoc ELSE


%type <n> program functionsAndDeclarations functionDefinition functionBody declarationsAndStatements functionDeclaration functionDeclarator parameterList parametersAux parameterDeclaration declaration declaratorsAux typeSpec declarator statement expr statementsAux statError exprList exprComplete functionCall 
%%
program:    functionsAndDeclarations    {$$=createNode("Program",createToken(NULL,lineNum,colNum)); initTree($$); addChild($$,$1);}
    ;
functionsAndDeclarations:   functionsAndDeclarations functionDefinition  {$$=$1; addNext($$,$2);}
    |   functionsAndDeclarations functionDeclaration     {$$=$1; addNext($$,$2);}
    |   functionsAndDeclarations declaration  {$$=$1; addNext($$,$2);}
    |   functionDefinition  {$$=$1;}
    |   functionDeclaration     {$$=$1;}
    |   declaration  {$$=$1;}
    ;
functionDefinition: typeSpec functionDeclarator functionBody    {$$=createNode("FuncDefinition",createToken(NULL,lineNum,colNum)); addChild($$,$1);addChild($$,$2);addChild($$,$3);}
    ;
functionBody:   LBRACE RBRACE   {$$=createNode("FuncBody",$1);}
    |   LBRACE declarationsAndStatements RBRACE {$$=createNode("FuncBody",$1); addChild($$,$2);}
    ;
declarationsAndStatements:  statement declarationsAndStatements {if(isNullNode($1)){$$=$2;} else{$$=$1; addNext($$,$2);}}
    |   declaration declarationsAndStatements   {if(isNullNode($1)){$$=$2;} else{$$=$1; addNext($$,$2);}}
    |   statement   {if(isNullNode($1)){$$=NULL;} else{$$=$1;}}
    |   declaration {if(isNullNode($1)){$$=NULL;} else{$$=$1;}}
    ;
functionDeclaration:    typeSpec functionDeclarator SEMI    {$$=createNode("FuncDeclaration",createToken(NULL,lineNum,colNum)); addChild($$,$1); addChild($$,$2);}
    ;
functionDeclarator: ID LPAR parameterList RPAR  {sprintf(buffer, "Id(%s)", $1->value); $$=createNode(buffer,$1); addNext($$,$3);} 
    ;
parameterList:  parameterDeclaration parametersAux   {$$=createNode("ParamList",createToken(NULL,lineNum,colNum)); addChild($$,$1); if($2!=NULL){addChild($$,$2);}}
    ;
parametersAux:      {$$=NULL;}
    |   parametersAux COMMA parameterDeclaration {if($1!=NULL){$$=$1; addNext($$,$3);} else{$$=$3;}}
    ; 
parameterDeclaration:  typeSpec {$$=createNode("ParamDeclaration",createToken(NULL,lineNum,colNum)); addChild($$,$1);}
    |   typeSpec ID {$$=createNode("ParamDeclaration",createToken(NULL,lineNum,colNum)); addChild($$,$1); sprintf(buffer, "Id(%s)",$2->value); addChild($$,createNode(buffer,$2));}
    ;
declaration:  typeSpec declarator declaratorsAux SEMI    {$$=createNode("Declaration",createToken(NULL,lineNum,colNum)); addChild($$,$1); addChild($$,$2); if($3!=NULL){addNext($$,getDeclarationNodes($3,$1));}}
    |   error SEMI {$$=NULL;}
    ;
declaratorsAux: /*epsilon*/     {$$=NULL;}
    |   declaratorsAux COMMA declarator  {struct node *n;  if($1!=NULL){$$=$1; n=createNode("Declaration",createToken(NULL,lineNum,colNum)); addChild(n,$3); addNext($$,n);} else{n=createNode("Declaration",createToken(NULL,lineNum,colNum)); addChild(n,$3); $$=n;}}
    ;
typeSpec:   CHAR    {$$=createNode("Char",$1);}
    |   INT {$$=createNode("Int",$1);}
    |   VOID    {$$=createNode("Void",$1);}
    |   SHORT   {$$=createNode("Short",$1);}
    |   DOUBLE  {$$=createNode("Double",$1);}
    ;
declarator: ID  {sprintf(buffer, "Id(%s)", $1->value); $$=createNode(buffer,$1);}
    |   ID ASSIGN exprComplete  {sprintf(buffer, "Id(%s)", $1->value); $$=createNode(buffer,$1); addNext($$,$3);}
    ;
statement:  SEMI    {$$=createNode("Null",$1);}   
    |   exprComplete SEMI {$$=$1;}
    |   LBRACE statementsAux RBRACE   {if($2!=NULL){if(($2)->next!=NULL){$$=createNode("StatList",$1); addChild($$,$2);} else{$$=$2;}} else{$$=createNode("Null",$1);}}
    |   LBRACE error RBRACE {$$=NULL;}
    |   LBRACE RBRACE   {$$=createNode("Null",$1);}
    |   IF LPAR exprComplete RPAR statError %prec THEN  {$$=createNode("If",$1); addChild($$,$3); addChild($$,$5); addChild($$,createNode("Null",$1));}
    |   IF LPAR exprComplete RPAR statError ELSE statError   {$$=createNode("If",$1); addChild($$,$3); addChild($$,$5); addChild($$,$7);}
    |   WHILE LPAR exprComplete RPAR statError   {$$=createNode("While",$1); addChild($$,$3); addChild($$,$5);}
    |   RETURN SEMI {$$=createNode("Return",$1); addChild($$,createNode("Null",$1));}
    |   RETURN exprComplete SEMI    {$$=createNode("Return",$1); addChild($$,$2);}
    ;
statementsAux:  statError   {if(isNullNode($1)){$$=NULL;} else{$$=$1;}}
    |   statementsAux statError {if($1!=NULL){$$=$1; if(!isNullNode($2)){addNext($$,$2);}} else{if(!isNullNode($2)){$$=$2;} else{$$=NULL;}}}
    ;
statError: statement    {$$=$1;}
    |   error SEMI  {$$=NULL;}
    ;
expr:   expr ASSIGN expr    {$$= createNode("Store",$2); addChild($$,$1); addChild($$,$3); }
    |   expr PLUS expr   {$$= createNode("Add",$2); addChild($$,$1); addChild($$,$3); }
    |   expr MINUS expr   {$$= createNode("Sub",$2); addChild($$,$1); addChild($$,$3); }
    |   expr MUL expr   {$$= createNode("Mul",$2); addChild($$,$1); addChild($$,$3); }
    |   expr DIV expr   {$$= createNode("Div",$2); addChild($$,$1); addChild($$,$3); }
    |   expr MOD expr   {$$= createNode("Mod",$2); addChild($$,$1); addChild($$,$3); }
    |   expr OR expr   {$$= createNode("Or",$2); addChild($$,$1); addChild($$,$3); }
    |   expr AND expr   {$$= createNode("And",$2); addChild($$,$1); addChild($$,$3); }
    |   expr BITWISEAND expr   {$$= createNode("BitWiseAnd",$2); addChild($$,$1); addChild($$,$3); }
    |   expr BITWISEOR expr   {$$= createNode("BitWiseOr",$2); addChild($$,$1); addChild($$,$3); }
    |   expr BITWISEXOR expr   {$$= createNode("BitWiseXor",$2); addChild($$,$1); addChild($$,$3); }
    |   expr EQ expr   {$$= createNode("Eq",$2); addChild($$,$1); addChild($$,$3); }
    |   expr NE expr   {$$= createNode("Ne",$2); addChild($$,$1); addChild($$,$3); }
    |   expr LE expr   {$$= createNode("Le",$2); addChild($$,$1); addChild($$,$3); }
    |   expr GE expr   {$$= createNode("Ge",$2); addChild($$,$1); addChild($$,$3); }
    |   expr LT expr   {$$= createNode("Lt",$2); addChild($$,$1); addChild($$,$3); }
    |   expr GT expr   {$$= createNode("Gt",$2); addChild($$,$1); addChild($$,$3); }
    |   PLUS expr %prec NOT   {$$= createNode("Plus",$1); addChild($$,$2); }
    |   MINUS expr %prec NOT    {$$= createNode("Minus",$1); addChild($$,$2); }   
    |   NOT expr   {$$= createNode("Not",$1); addChild($$,$2); }
    |   functionCall    {$$=$1;}
    |   ID   {sprintf(buffer, "Id(%s)", $1->value); $$=createNode(buffer,$1);}
    |   INTLIT  {sprintf(buffer, "IntLit(%s)", $1->value); $$=createNode(buffer,$1);}
    |   CHRLIT  {sprintf(buffer, "ChrLit(%s)", $1->value); $$=createNode(buffer,$1);}
    |   REALLIT {sprintf(buffer, "RealLit(%s)", $1->value); $$=createNode(buffer,$1);}
    |   LPAR exprComplete RPAR  {$$=$2;}
    |   LPAR error RPAR {$$=NULL;}
    ;
functionCall: ID LPAR RPAR {$$=createNode("Call",$1); sprintf(buffer, "Id(%s)", $1->value); addChild($$,createNode(buffer,$1));}
    |   ID LPAR exprList RPAR {$$=createNode("Call",$1); sprintf(buffer, "Id(%s)", $1->value); addChild($$,createNode(buffer,$1)); addChild($$,$3);}
    |   ID LPAR error RPAR  {$$=NULL;}
    ;
exprList:   expr {$$=$1;}
    |   exprList COMMA expr  {$$= $1; addNext($$,$3);}
    ;
exprComplete: expr  {$$=$1;}
    |   exprComplete COMMA expr {$$= createNode("Comma",$2); addChild($$,$1); addChild($$,$3);}
    ;
%%

void yyerror (char *s) { //sintax errors
    if(strlen(yytext)==0) printf("Line %d, col %d: %s: %s\n", lineNum,colNum,s,yytext);
    else printf("Line %d, col %d: %s: %s\n", lineNum,colNum-yyleng,s,yytext);
    sintaxErrorFlag=1; //syntax error happened!
}