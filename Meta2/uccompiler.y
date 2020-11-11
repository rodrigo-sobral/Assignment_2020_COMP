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
    extern int lineNum,colNum, yyleng; 
    extern char* yytext;
    int errorFlag=0; 
    char buffer[100]; //aux
%}

%union{
    char* str;
    struct node* n;
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
program:    functionsAndDeclarations    {$$=createNode("Program"); initTree($$); addChild($$,$1);}
    ;
functionsAndDeclarations:   functionsAndDeclarations functionDefinition  {$$=$1; addNext($$,$2);}
    |   functionsAndDeclarations functionDeclaration     {$$=$1; addNext($$,$2);}
    |   functionsAndDeclarations declaration  {$$=$1; addNext($$,$2);}
    |   functionDefinition  {$$=$1;}
    |   functionDeclaration     {$$=$1;}
    |   declaration  {$$=$1;}
    ;
functionDefinition: typeSpec functionDeclarator functionBody    {$$=createNode("FuncDefinition"); addChild($$,$1);addChild($$,$2);addChild($$,$3);}
    ;
functionBody:   LBRACE RBRACE   {$$=createNode("FuncBody");}
    |   LBRACE declarationsAndStatements RBRACE {$$=createNode("FuncBody"); addChild($$,$2);}
    ;
declarationsAndStatements:  statement declarationsAndStatements {if(isNullNode($1)){$$=$2;} else{$$=$1; addNext($$,$2);}}
    |   declaration declarationsAndStatements   {if(isNullNode($1)){$$=$2;} else{$$=$1; addNext($$,$2);}}
    |   statement   {if(isNullNode($1)){$$=NULL;} else{$$=$1;}}
    |   declaration {if(isNullNode($1)){$$=NULL;} else{$$=$1;}}
    ;
functionDeclaration:    typeSpec functionDeclarator SEMI    {$$=createNode("FuncDeclaration"); addChild($$,$1); addChild($$,$2);}
    ;
functionDeclarator: ID LPAR parameterList RPAR  {sprintf(buffer, "Id(%s)", $1); $$=createNode(buffer); addNext($$,$3);} 
    ;
parameterList:  parameterDeclaration parametersAux   {$$=createNode("ParamList"); addChild($$,$1); if($2!=NULL){addChild($$,$2);}}
    ;
parametersAux:      {$$=NULL;}
    |   parametersAux COMMA parameterDeclaration {if($1!=NULL){$$=$1; addNext($$,$3);} else{$$=$3;}}
    ; 
parameterDeclaration:  typeSpec {$$=createNode("ParamDeclaration"); addChild($$,$1);}
    |   typeSpec ID {$$=createNode("ParamDeclaration"); addChild($$,$1); sprintf(buffer, "Id(%s)", $2); addChild($$,createNode(buffer));}
    ;
declaration:  typeSpec declarator declaratorsAux SEMI    {$$=createNode("Declaration"); addChild($$,$1); addChild($$,$2); if($3!=NULL){addNext($$,getDeclarationNodes($3,$1));}}
    |   error SEMI {$$=NULL;}
    ;
declaratorsAux: /*epsilon*/     {$$=NULL;}
    |   declaratorsAux COMMA declarator  {struct node *n;  if($1!=NULL){$$=$1; n=createNode("Declaration"); addChild(n,$3); addNext($$,n);} else{n=createNode("Declaration"); addChild(n,$3); $$=n;}}
    ;
typeSpec:   CHAR    {$$=createNode("Char");}
    |   INT {$$=createNode("Int");}
    |   VOID    {$$=createNode("Void");}
    |   SHORT   {$$=createNode("Short");}
    |   DOUBLE  {$$=createNode("Double");}
    ;
declarator: ID  {sprintf(buffer, "Id(%s)", $1); $$=createNode(buffer);}
    |   ID ASSIGN exprComplete  {sprintf(buffer, "Id(%s)", $1); $$=createNode(buffer); addNext($$,$3);}
    ;
statement:  SEMI    {$$=createNode("Null");}   
    |   exprComplete SEMI {$$=$1;}
    |   LBRACE statementsAux RBRACE   {if($2!=NULL){if(($2)->next!=NULL){$$=createNode("StatList"); addChild($$,$2);} else{$$=$2;}} else{$$=NULL;}}
    |   LBRACE error RBRACE {$$=NULL;}
    |   LBRACE RBRACE   {$$=createNode("Null");}
    |   IF LPAR exprComplete RPAR statError %prec THEN  {$$=createNode("If"); addChild($$,$3); addChild($$,$5); addChild($$,createNode("Null"));}
    |   IF LPAR exprComplete RPAR statError ELSE statError   {$$=createNode("If"); addChild($$,$3); addChild($$,$5); addChild($$,$7);}
    |   WHILE LPAR exprComplete RPAR statError   {$$=createNode("While"); addChild($$,$3); addChild($$,$5);}
    |   RETURN SEMI {$$=createNode("Return"); addChild($$,createNode("Null"));}
    |   RETURN exprComplete SEMI    {$$=createNode("Return"); addChild($$,$2);}
    ;
statementsAux:  statError   {if(isNullNode($1)){$$=NULL;} else{$$=$1;} }
    |   statementsAux statError {if(isNullNode($2)){$$=$1;} else{$$=$1; addNext($$,$2);}}
    ;
statError: statement    {$$=$1;}
    |   error SEMI  {$$=NULL;}
    ;
expr:   expr ASSIGN expr    {$$= createNode("Store"); addChild($$,$1); addChild($$,$3); }
    |   expr PLUS expr   {$$= createNode("Add"); addChild($$,$1); addChild($$,$3); }
    |   expr MINUS expr   {$$= createNode("Sub"); addChild($$,$1); addChild($$,$3); }
    |   expr MUL expr   {$$= createNode("Mul"); addChild($$,$1); addChild($$,$3); }
    |   expr DIV expr   {$$= createNode("Div"); addChild($$,$1); addChild($$,$3); }
    |   expr MOD expr   {$$= createNode("Mod"); addChild($$,$1); addChild($$,$3); }
    |   expr OR expr   {$$= createNode("Or"); addChild($$,$1); addChild($$,$3); }
    |   expr AND expr   {$$= createNode("And"); addChild($$,$1); addChild($$,$3); }
    |   expr BITWISEAND expr   {$$= createNode("BitWiseAnd"); addChild($$,$1); addChild($$,$3); }
    |   expr BITWISEOR expr   {$$= createNode("BitWiseOr"); addChild($$,$1); addChild($$,$3); }
    |   expr BITWISEXOR expr   {$$= createNode("BitWiseXor"); addChild($$,$1); addChild($$,$3); }
    |   expr EQ expr   {$$= createNode("Eq"); addChild($$,$1); addChild($$,$3); }
    |   expr NE expr   {$$= createNode("Ne"); addChild($$,$1); addChild($$,$3); }
    |   expr LE expr   {$$= createNode("Le"); addChild($$,$1); addChild($$,$3); }
    |   expr GE expr   {$$= createNode("Ge"); addChild($$,$1); addChild($$,$3); }
    |   expr LT expr   {$$= createNode("Lt"); addChild($$,$1); addChild($$,$3); }
    |   expr GT expr   {$$= createNode("Gt"); addChild($$,$1); addChild($$,$3); }
    |   PLUS expr %prec NOT   {$$= createNode("Plus"); addChild($$,$2); }
    |   MINUS expr %prec NOT    {$$= createNode("Minus"); addChild($$,$2); }   
    |   NOT expr   {$$= createNode("Not"); addChild($$,$2); }
    |   functionCall    {$$=$1;}
    |   ID   {sprintf(buffer, "Id(%s)", $1); $$=createNode(buffer);}
    |   INTLIT  {sprintf(buffer, "IntLit(%s)", $1); $$=createNode(buffer);}
    |   CHRLIT  {sprintf(buffer, "ChrLit(%s)", $1); $$=createNode(buffer);}
    |   REALLIT {sprintf(buffer, "RealLit(%s)", $1); $$=createNode(buffer);}
    |   LPAR exprComplete RPAR  {$$=$2;}
    |   LPAR error RPAR {$$=NULL;}
    ;
functionCall: ID LPAR RPAR {$$=createNode("Call"); sprintf(buffer, "Id(%s)", $1); addChild($$,createNode(buffer));}
    |   ID LPAR exprList RPAR {$$=createNode("Call"); sprintf(buffer, "Id(%s)", $1); addChild($$,createNode(buffer)); addChild($$,$3);}
    |   ID LPAR error RPAR  {$$=NULL;}
    ;
exprList:   expr {$$=$1;}
    |   exprList COMMA expr  {$$= $1; addNext($$,$3);}
    ;
exprComplete: expr  {$$=$1;}
    |   exprComplete COMMA expr {$$= createNode("Comma"); addChild($$,$1); addChild($$,$3);}
    ;
%%

void yyerror (char *s) { //sintax errors
    if(strlen(yytext)==0) printf("Line %d, col %d: %s: %s\n", lineNum,colNum,s,yytext);
    else printf("Line %d, col %d: %s: %s\n", lineNum,colNum-yyleng,s,yytext);
    errorFlag=1; //syntax error happened!
}