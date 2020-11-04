%{
    //PL5_COMPILADORES@2020
    //Eduardo Filipe Ferreira Cruz          2018285164
    //Rodrigo Fernando Henriques Sobral     2018298209
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"
    #include "astree.h"

    int yylex (void);
    void yyerror(char* s);
    /*The extern keywor, in C, is used to tell the compiler that the
     variable that we are declaring was defined elsewhere.*/
    extern int lineNum,colNum, yyleng; 
    extern char* yytext;
    int errorFlag=0; 
    char buffer[100]; //aux
    //nota:  no error semi meter yyerrok; ? n entendo bem o q faz
%}

%union{
    char* str;
    node* n;
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

%type <n> program functionsAndDeclarations functionDefinition functionBody declarationsAndStatements functionDeclaration functionDeclarator parameterList parametersAux parameterDeclaration declaration declaratorsAux typeSpec declarator statement expr statementsAux exprsAux
%%
program:    functionsAndDeclarations    {n=createNode("Program"); initTree(n); addChild(n,$1); $$=n;}
    ;
functionsAndDeclarations:   functionsAndDeclarations functionDefinition  {$$=$1; addNext($1,$2);}
    |   functionsAndDeclarations functionDeclaration     {$$=$1; addNext($1,$2);}
    |   functionsAndDeclarations declaration  {$$=$1; addNext($1,$2);}
    |   functionDefinition  {addChild($$,$1);}
    |   functionDeclaration     {}
    |   declaration  {}
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
parametersAux:  /*epsilon*/     {}
    |   parametersAux COMMA parameterDeclaration {}
    ;
parameterDeclaration:  typeSpec {}
    |   typeSpec ID {sprintf(buffer, "Id(%s)", $2);}
    ;
declaration:  typeSpec declarator declaratorsAux SEMI    {}
    |   error SEMI  {$$=NULL;}
    ;
declaratorsAux: /*epsilon*/     {}
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
    |   expr SEMI {$$=$1;}
    |   error SEMI  {$$=NULL;}
    |   LBRACE statementsAux RBRACE   {}
    |   LBRACE error RBRACE {$$=NULL;}
    |   IF LPAR expr RPAR statement   {}
    |   IF LPAR expr RPAR statement ELSE statement   {}
    |   WHILE LPAR expr RPAR statement   {}
    |   RETURN SEMI {}
    |   RETURN expr SEMI    {}
    ;
statementsAux:  /*epsilon*/   {}
    |   statementsAux statement {}
    ;
expr:   expr ASSIGN expr    {node* n= createNode("Assign"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr COMMA expr {node* n= createNode("Comma"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr PLUS expr   {node* n= createNode("Add"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr MINUS expr   {node* n= createNode("Sub"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr MUL expr   {node* n= createNode("Mul"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr DIV expr   {node* n= createNode("Div"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr MOD expr   {node* n= createNode("Mod"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr OR expr   {node* n= createNode("Or"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr AND expr   {node* n= createNode("And"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr BITWISEAND expr   {node* n= createNode("BitWiseAnd"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr BITWISEOR expr   {node* n= createNode("BitWiseOr"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr BITWISEXOR expr   {node* n= createNode("BitWiseXor"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr EQ expr   {node* n= createNode("Eq"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr NE expr   {node* n= createNode("Ne"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr LE expr   {node* n= createNode("Le"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr GE expr   {node* n= createNode("Ge"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr LT expr   {node* n= createNode("Lt"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   expr GT expr   {node* n= createNode("Gt"); addChild(n,$1); addChild(n,$3); $$=n;}
    |   PLUS expr %prec NOT   {node* n= createNode("Plus"); addChild(n,$2); $$=n;}
    |   MINUS expr %prec NOT    {node* n= createNode("Minus"); addChild(n,$2); $$=n;}
    |   NOT expr   {node* n= createNode("Not"); addChild(n,$2); $$=n;}
    |   ID LPAR RPAR   {sprintf(buffer, "Id(%s)", $1); $$= createNode(buffer);}
    |   ID LPAR expr exprsAux RPAR   {node* n; sprintf(buffer, "Id(%s)", $1); n=createNode(buffer); addChild(n,$3);  addChild(n,$4); $$=n;}
    |   ID LPAR error RPAR  {$$=NULL;}
    |   ID   {sprintf(buffer, "Id(%s)", $1); $$= createNode(buffer);}
    |   INTLIT  {sprintf(buffer, "IntLit(%s)", $1); $$= createNode(buffer);}
    |   CHRLIT  {sprintf(buffer, "ChrLit(%s)", $1); $$= createNode(buffer);}
    |   REALLIT {sprintf(buffer, "RealLit(%s)", $1); $$= createNode(buffer);}
    |   LPAR expr RPAR  {$$=$2;}
    |   LPAR error RPAR {$$=NULL;}
    ;
exprsAux:   /*epsilon*/   {}
    |   exprsAux COMMA expr {/*addNext*/}
    ;
%%

void yyerror (char *s) { //sintax errors
    printf("Line %d, col %d: %s: %s\n", lineNum,colNum-yyleng,s,yytext);
    errorFlag=1; //syntax error happened!
}