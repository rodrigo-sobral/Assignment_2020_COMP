%X COMMENT

%{
	#include "y.tab.h"
	#include "ast.h"

	int line = 1;
	int col = 1;
	int start_line = 1;
	int start_col = 1;
	
	int flag_l = 0;
	int flag_1 = 0;
	int flag_t = 0;
	int flag_error = 0;

	extern node_type* root;
%}

letter				[a-zA-Z_]
number 				[0-9]
seq_escape			\\(n|t|\\|\'|\"|[0-7]{1,3})
reserved 			"auto"|"break"|"case"|"const"|"continue"|"default"|"do"|"double"|"enum"|"extern"|"float"|"goto"|"long"|"register"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"while"|"++"|"--"

%%

"/*"								{BEGIN COMMENT; start_line = line; start_col = col; col+=yyleng;}
<COMMENT>.							{col+=yyleng;}
<COMMENT>\n							{line++; col=1;}
<COMMENT><<EOF>>					{printf("Line %d, col %d: unterminated comment\n", start_line, start_col); yyterminate();}
<COMMENT>"*/"						{BEGIN 0; col+=yyleng;}

"&"									{if(flag_l) printf("AMP\n"); col+=yyleng; if(!flag_1) return AMP;}
"&&"								{if(flag_l) printf("AND\n"); col+=yyleng; if(!flag_1) return AND;}
"="									{if(flag_l) printf("ASSIGN\n"); col+=yyleng; if(!flag_1) return ASSIGN;}
"*"									{if(flag_l) printf("AST\n"); col+=yyleng; if(!flag_1) return AST;}
"char"								{if(flag_l) printf("CHAR\n"); col+=yyleng; if(!flag_1) return CHAR;}
","									{if(flag_l) printf("COMMA\n"); col+=yyleng; if(!flag_1) return COMMA;}
"/"									{if(flag_l) printf("DIV\n"); col+=yyleng; if(!flag_1) return DIV;}
"else"								{if(flag_l) printf("ELSE\n"); col+=yyleng; if(!flag_1) return ELSE;}
"=="								{if(flag_l) printf("EQ\n"); col+=yyleng; if(!flag_1) return EQ;}
"for"								{if(flag_l) printf("FOR\n"); col+=yyleng; if(!flag_1) return FOR;}
">="								{if(flag_l) printf("GE\n"); col+=yyleng; if(!flag_1) return GE;}
">"									{if(flag_l) printf("GT\n"); col+=yyleng; if(!flag_1) return GT;}
"if"								{if(flag_l) printf("IF\n"); col+=yyleng; if(!flag_1) return IF;}
"int"								{if(flag_l) printf("INT\n"); col+=yyleng; if(!flag_1) return INT;}
"{"									{if(flag_l) printf("LBRACE\n"); col+=yyleng; if(!flag_1) return LBRACE;}
"<="								{if(flag_l) printf("LE\n"); col+=yyleng; if(!flag_1) return LE;}
"("									{if(flag_l) printf("LPAR\n"); col+=yyleng; if(!flag_1) return LPAR;}
"["									{if(flag_l) printf("LSQ\n"); col+=yyleng; if(!flag_1) return LSQ;}
"<"									{if(flag_l) printf("LT\n"); col+=yyleng; if(!flag_1) return LT;}
"-"									{if(flag_l) printf("MINUS\n"); col+=yyleng; if(!flag_1) return MINUS;}
"%"									{if(flag_l) printf("MOD\n"); col+=yyleng; if(!flag_1) return MOD;}
"!="								{if(flag_l) printf("NE\n"); col+=yyleng; if(!flag_1) return NE;}
"!"									{if(flag_l) printf("NOT\n"); col+=yyleng; if(!flag_1) return NOT;}
"||"								{if(flag_l) printf("OR\n"); col+=yyleng; if(!flag_1) return OR;}
"+"									{if(flag_l) printf("PLUS\n"); col+=yyleng; if(!flag_1) return PLUS;}
"}"									{if(flag_l) printf("RBRACE\n"); col+=yyleng; if(!flag_1) return RBRACE;}
"return"							{if(flag_l) printf("RETURN\n"); col+=yyleng; if(!flag_1) return RETURN;}
")"									{if(flag_l) printf("RPAR\n"); col+=yyleng; if(!flag_1) return RPAR;}
"]"									{if(flag_l) printf("RSQ\n"); col+=yyleng; if(!flag_1) return RSQ;}
";"									{if(flag_l) printf("SEMI\n"); col+=yyleng; if(!flag_1) return SEMI;}
"void"								{if(flag_l) printf("VOID\n"); col+=yyleng; if(!flag_1) return VOID;}

{reserved}							{yylval.token = strdup(yytext); if(flag_l) printf("RESERVED(%s)\n", yytext); col+=yyleng; if(!flag_1) return RESERVED;}
{letter}({letter}|{number})*		{yylval.token = strdup(yytext); if(flag_l) printf("ID(%s)\n", yytext); col+=yyleng; if(!flag_1) return ID;}
{number}+							{yylval.token = strdup(yytext); if(flag_l) printf("INTLIT(%s)\n", yytext); col+=yyleng; if(!flag_1) return INTLIT;}
\'([^\n\'\\]|{seq_escape})\'		{yylval.token = strdup(yytext); if(flag_l) printf("CHRLIT(%s)\n", yytext); col+=yyleng; if(!flag_1) return CHRLIT;}
\"([^\n\"\\]|{seq_escape})*\"		{yylval.token = strdup(yytext); if(flag_l) printf("STRLIT(%s)\n", yytext); col+=yyleng; if(!flag_1) return STRLIT;}

\'([^\n\'\\]|\\.|\\[0-9]{1,3})*(\\)?	{printf("Line %d, col %d: unterminated char constant\n", line, col); col+=yyleng;}
\'([^\n\'\\]|\\.|\\[0-9]{1,3})*\'		{printf("Line %d, col %d: invalid char constant (%s)\n", line, col, yytext); col+=yyleng;}
\"([^\n\"\\]|\\.|\\[0-9]{1,3})*(\\)?	{printf("Line %d, col %d: unterminated string constant\n", line, col); col+=yyleng;}
\"([^\n\"\\]|\\.|\\[0-9]{1,3})*\"		{printf("Line %d, col %d: invalid string constant (%s)\n", line, col, yytext); col+=yyleng;}

" "|\t								{col+=yyleng;}
<<EOF>>								{col+=yyleng; return yytext[0];}
\n									{line++; col=1;}
.									{printf("Line %d, col %d: illegal character (%c)\n", line, col, yytext[0]); col+=yyleng;}

%%

int main(int argc, char* argv[]) {
	if(argc > 1) {
		if(strcmp(argv[1], "-l") == 0) {
			flag_l = 1;
			flag_1 = 1;
		}

		if(strcmp(argv[1], "-1") == 0)
			flag_1 = 1;

		if(strcmp(argv[1], "-t") == 0)
			flag_t = 1;
	}

	if(flag_1) {
		yylex();
	} else {
		yyparse();

		if(flag_t && !flag_error)
			print_ast(root, 0);

		clear_ast(root);
	}

	return 0;
}

int yywrap() {
	return 1;
}