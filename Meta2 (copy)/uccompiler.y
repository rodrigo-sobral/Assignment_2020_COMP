%{
	#include <stdio.h>
	#include "ast.h"

	void yyerror(char *s);
	int yylex();

	extern int yyleng;
	extern char* yytext;

	extern int line;
	extern int col;
	extern int flag_error;

	node_type* root = NULL; 		/* this node represents the root of the AST */
	node_type* aux_node = NULL; 	/* auxiliary node used to build new nodes */
%}

%union {
	char* token;
	struct node_type* node;
}

%token AMP AND ASSIGN AST CHAR COMMA DIV ELSE EQ FOR GE GT IF INT LBRACE LE
%token LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RETURN RPAR RSQ SEMI VOID

%token <token> RESERVED ID INTLIT CHRLIT STRLIT

%type <node> Start FunctionDefinition FunctionBody DeclarationList StatementList FunctionDeclaration FunctionDeclarator ParameterList
ParameterDeclaration Declaration DeclaratorList TypeSpec Declarator AstList ErrStatement Statement StatList ExprOpt GeneralExpr Expr ExprList

%left COMMA
%right ASSIGN
%left OR
%left AND
%left EQ NE
%left GT LT GE LE
%left PLUS MINUS
%left AST DIV MOD
%right NOT AMP
%left LPAR RPAR LSQ RSQ
%nonassoc IFX
%nonassoc ELSE

%%

Start:
	Start FunctionDefinition 												{aux_node = $1; aux_node = aux_node->children; add_sibling(aux_node,$2); $$ = $1;}
	| Start FunctionDeclaration 											{aux_node = $1; aux_node = aux_node->children; add_sibling(aux_node,$2); $$ = $1;}
	| Start Declaration 													{aux_node = $1; aux_node = aux_node->children; add_sibling(aux_node,$2); $$ = $1;}
	| FunctionDefinition 													{$$ = create_node("Program", NULL); add_child($$, $1); root = $$;}
	| FunctionDeclaration													{$$ = create_node("Program", NULL); add_child($$, $1); root = $$;}
	| Declaration 															{$$ = create_node("Program", NULL); add_child($$, $1); root = $$;}
	;
FunctionDefinition:
	TypeSpec FunctionDeclarator FunctionBody 								{aux_node = create_node("FuncDefinition", NULL); add_sibling($1,$2); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	;
FunctionBody:
	LBRACE DeclarationList StatementList RBRACE 							{$$ = create_node("FuncBody", NULL); add_sibling($2,$3); add_child($$,$2);}
	| LBRACE DeclarationList RBRACE 										{$$ = create_node("FuncBody", NULL); add_child($$,$2);}
	| LBRACE StatementList RBRACE 											{$$ = create_node("FuncBody", NULL); if(is_null_node($2) == 0) {add_child($$,$2);}}
	| LBRACE RBRACE 														{$$ = create_node("FuncBody", NULL);}
	| LBRACE error RBRACE 													{$$ = create_node("Error", NULL);} /* an Error Node is created */
	;
DeclarationList:
	DeclarationList Declaration 											{add_sibling($1,$2); $$ = $1;}
	| Declaration 															{$$ = $1;}
	;
StatementList:
	StatementList ErrStatement 												{if(is_null_node($1)) {$$ = $2;} else if(is_null_node($2)) {$$ = $1;} else {add_sibling($1,$2); $$ = $1;}}
	| Statement 															{$$ = $1;}
	;
FunctionDeclaration:
	TypeSpec FunctionDeclarator SEMI 										{aux_node = create_node("FuncDeclaration", NULL); add_sibling($1,$2); add_child(aux_node,$1); $$ = aux_node;}
	;
FunctionDeclarator:
	AstList ID LPAR ParameterList RPAR 										{aux_node = create_node("ParamList", NULL); add_child(aux_node,$4); add_sibling($1,create_node("Id", $2)); add_sibling($1,aux_node); $$ = $1;}
	| ID LPAR ParameterList RPAR 											{$$ = create_node("Id", $1); aux_node = create_node("ParamList", NULL); add_child(aux_node,$3); add_sibling($$,aux_node);}
	;
ParameterList:
	ParameterList COMMA ParameterDeclaration 								{add_sibling($1,$3); $$ = $1;}
	| ParameterDeclaration 													{$$ = $1;}
	;
ParameterDeclaration:
	TypeSpec AstList ID 													{$$ = create_node("ParamDeclaration", NULL); aux_node = create_node("Id", $3); add_sibling($1,$2); add_sibling($1,aux_node); add_child($$,$1);}
	| TypeSpec ID 															{$$ = create_node("ParamDeclaration", NULL); aux_node = create_node("Id", $2); add_sibling($1,aux_node); add_child($$,$1);}
	| TypeSpec AstList 														{$$ = create_node("ParamDeclaration", NULL); add_sibling($1,$2); add_child($$,$1);}
	| TypeSpec 																{$$ = create_node("ParamDeclaration", NULL); add_child($$,$1);}
	;
Declaration:
	TypeSpec DeclaratorList SEMI 											{$$ = add_typespec_to_declarations($1,$2);}
	| error SEMI 															{$$ = create_node("Error", NULL);} /* an Error Node is created */
	;
DeclaratorList:
	DeclaratorList COMMA Declarator 										{add_sibling($1,$3); $$ = $1;}
	| Declarator 															{$$ = $1;}
	;
TypeSpec:
	CHAR 																	{$$ = create_node("Char", NULL);}
	| INT 																	{$$ = create_node("Int", NULL);}
	| VOID 																	{$$ = create_node("Void", NULL);}
	;
Declarator:
	AstList ID LSQ INTLIT RSQ 												{$$ = create_node("ArrayDeclaration", NULL); aux_node = create_node("Id", $2); add_sibling($1,aux_node); add_sibling($1,create_node("IntLit", $4)); add_child($$,$1);}
	| ID LSQ INTLIT RSQ 													{$$ = create_node("ArrayDeclaration", NULL); aux_node = create_node("Id", $1); add_sibling(aux_node,create_node("IntLit", $3)); add_child($$,aux_node);}
	| AstList ID 															{$$ = create_node("Declaration", NULL); aux_node = create_node("Id", $2); add_sibling($1,aux_node); add_child($$,$1);}
	| ID 																	{$$ = create_node("Declaration", NULL); aux_node = create_node("Id", $1); add_child($$,aux_node);}
	;
AstList:
	AstList AST 															{aux_node = create_node("Pointer", NULL); add_sibling($1,aux_node); $$ = $1;}
	| AST																	{$$ = create_node("Pointer", NULL);}
	;
ErrStatement:
	ExprOpt SEMI 															{$$ = $1;}

	| LBRACE StatList RBRACE 												{if(have_sibling($2) == 1){$$ = create_node("StatList", NULL); add_child($$,$2);} else {$$ = $2;}}
	| LBRACE RBRACE 														{$$ = create_node("Null", NULL);}
	| LBRACE error RBRACE 													{$$ = create_node("Error", NULL);} /* an Error Node is created */

	| IF LPAR GeneralExpr RPAR ErrStatement %prec IFX 						{$$ = create_node("If", NULL); aux_node = create_node("Null", NULL); add_sibling($3,$5); add_sibling($3,aux_node); add_child($$,$3);}
	| IF LPAR GeneralExpr RPAR ErrStatement ELSE ErrStatement 				{aux_node = create_node("If", NULL); add_sibling($3,$5); add_sibling($3,$7); add_child(aux_node,$3); $$ = aux_node;}

	| FOR LPAR ExprOpt SEMI ExprOpt SEMI ExprOpt RPAR ErrStatement 			{aux_node = create_node("For", NULL); add_sibling($3,$5); add_sibling($3,$7); add_sibling($3,$9); add_child(aux_node,$3); $$ = aux_node;}

	| RETURN ExprOpt SEMI 													{aux_node = create_node("Return", NULL); add_child(aux_node,$2); $$ = aux_node;}

	| error SEMI 															{$$ = create_node("Error", NULL);} /* an Error Node is created */
	;
Statement:
	ExprOpt SEMI 															{$$ = $1;}

	| LBRACE StatList RBRACE 												{if(have_sibling($2) == 1){$$ = create_node("StatList", NULL); add_child($$,$2);} else {$$ = $2;}}
	| LBRACE RBRACE 														{$$ = create_node("Null", NULL);}
	| LBRACE error RBRACE 													{$$ = create_node("Error", NULL);} /* an Error Node is created */

	| IF LPAR GeneralExpr RPAR ErrStatement %prec IFX 						{$$ = create_node("If", NULL); aux_node = create_node("Null", NULL); add_sibling($3,$5); add_sibling($3,aux_node); add_child($$,$3);}
	| IF LPAR GeneralExpr RPAR ErrStatement ELSE ErrStatement 				{aux_node = create_node("If", NULL); add_sibling($3,$5); add_sibling($3,$7); add_child(aux_node,$3); $$ = aux_node;}

	| FOR LPAR ExprOpt SEMI ExprOpt SEMI ExprOpt RPAR ErrStatement 			{aux_node = create_node("For", NULL); add_sibling($3,$5); add_sibling($3,$7); add_sibling($3,$9); add_child(aux_node,$3); $$ = aux_node;}

	| RETURN ExprOpt SEMI 													{aux_node = create_node("Return", NULL); add_child(aux_node,$2); $$ = aux_node;}
	;
StatList:
	StatList ErrStatement 													{if(is_null_node($1)) {$$ = $2;} else if(is_null_node($2)) {$$ = $1;} else {add_sibling($1,$2); $$ = $1;}}
	| ErrStatement 															{$$ = $1;}
	;
ExprOpt:
	GeneralExpr 															{$$ = $1;}
	| 																		{$$ = create_node("Null", NULL);}
	;
GeneralExpr:
	GeneralExpr COMMA Expr 													{aux_node = create_node("Comma", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr 																	{$$ = $1;}
	;
Expr:
	Expr ASSIGN Expr 														{aux_node = create_node("Store", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}

	| Expr AND Expr 														{aux_node = create_node("And", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr OR Expr 															{aux_node = create_node("Or", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}

	| Expr EQ Expr 															{aux_node = create_node("Eq", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr NE Expr 															{aux_node = create_node("Ne", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr LT Expr 															{aux_node = create_node("Lt", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr GT Expr 															{aux_node = create_node("Gt", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr LE Expr 															{aux_node = create_node("Le", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr GE Expr 															{aux_node = create_node("Ge", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}

	| Expr PLUS Expr 														{aux_node = create_node("Add", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr MINUS Expr 														{aux_node = create_node("Sub", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr AST Expr 														{aux_node = create_node("Mul", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr DIV Expr 														{aux_node = create_node("Div", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}
	| Expr MOD Expr 														{aux_node = create_node("Mod", NULL); add_sibling($1,$3); add_child(aux_node,$1); $$ = aux_node;}

	| AMP Expr 																{$$ = create_node("Addr", NULL); add_child($$,$2);}
	| AST Expr %prec NOT													{$$ = create_node("Deref", NULL); add_child($$,$2);}
	| PLUS Expr %prec NOT													{$$ = create_node("Plus", NULL); add_child($$,$2);}
	| MINUS Expr %prec NOT													{$$ = create_node("Minus", NULL); add_child($$,$2);}
	| NOT Expr 																{$$ = create_node("Not", NULL); add_child($$,$2);}

	| Expr LSQ GeneralExpr RSQ 												{$$ = create_node("Deref", NULL); aux_node = create_node("Add", NULL); add_sibling($1,$3); add_child(aux_node,$1); add_child($$,aux_node);}

	| ID LPAR ExprList RPAR 												{$$ = create_node("Call", NULL); aux_node = create_node("Id", $1); add_sibling(aux_node,$3); add_child($$,aux_node);}
	| ID LPAR RPAR 															{$$ = create_node("Call", NULL); aux_node = create_node("Id", $1); add_child($$,aux_node);}
	| ID LPAR error RPAR 													{$$ = create_node("Error", NULL);} /* an Error Node is created */
	
	| ID 																	{$$ = create_node("Id", $1);}
	| INTLIT 																{$$ = create_node("IntLit", $1);}
	| CHRLIT 																{$$ = create_node("ChrLit", $1);}
	| STRLIT 																{$$ = create_node("StrLit", $1);}
	| LPAR GeneralExpr RPAR													{$$ = $2;}
	| LPAR error RPAR														{$$ = create_node("Error", NULL);} /* an Error Node is created */
	;
ExprList:
	ExprList COMMA Expr 													{add_sibling($1,$3); $$ = $1;}
	| Expr 																	{$$ = $1;}
	;

%%

void yyerror(char *s) {
	flag_error = 1;
	printf("Line %d, col %d: %s: %s\n", line, col-yyleng, s, yytext);
}