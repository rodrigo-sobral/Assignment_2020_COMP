%{
    //PL5_COMPILADORES@2020
    //Eduardo Filipe Ferreira Cruz          2018285164
    //Rodrigo Fernando Henriques Sobral     2018298209
    #include <string.h>
    #include "y.tab.h"
    #include "ast.h"
    #include "semantics.h"

    int printTokenFlag=0; //if 0(default) print error_messages from lexical_analysis only; else print all tokens and error_messages from lexical_analysis
    int parseFlag=1; //if 1(default) execute parser

    int lineNum=1,colNum=1; //line counter, column counter
    int auxL,auxC;    //to save aux values 
    
    extern struct node *root;
    extern int sintaxErrorFlag;
%}


%X COMMENT
letter  [A-Za-z_]
digit   [0-9]
digits  {digit}+
signal  [+-]?
exp [Ee]{signal}{digits}
reserved    "++"|"--"|"["|"]"|"auto"|"break"|"case"|"const"|"continue"|"default"|"do"|"enum"|"extern"|"float"|"for"|"goto"|"inline"|"long"|"register"|"restrict"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"_Bool"|"_Complex"|"_Imaginary"                
id  {letter}({letter}|{digit})*
ws  [\t ]+
esc \\n|\\t|\\\\|\\\'|\\\"|\\[0-7]{1,3}
char [^\n\'\\]|{esc} 


%%
char        {if(printTokenFlag){printf("CHAR\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return CHAR;}}
int         {if(printTokenFlag){printf("INT\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return INT;}}
short       {if(printTokenFlag){printf("SHORT\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return SHORT;}}
double      {if(printTokenFlag){printf("DOUBLE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return DOUBLE;}}
void        {if(printTokenFlag){printf("VOID\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return VOID;}}
if          {if(printTokenFlag){printf("IF\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return IF;}}
else        {if(printTokenFlag){printf("ELSE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return ELSE;}}
while       {if(printTokenFlag){printf("WHILE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return WHILE;}}
return      {if(printTokenFlag){printf("RETURN\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return RETURN;}}
{reserved}  {if(printTokenFlag){printf("RESERVED(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum); return RESERVED;}}
"("         {if(printTokenFlag){printf("LPAR\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return LPAR;}}
")"         {if(printTokenFlag){printf("RPAR\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return RPAR;}}
"{"         {if(printTokenFlag){printf("LBRACE\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return LBRACE;}}
"}"         {if(printTokenFlag){printf("RBRACE\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return RBRACE;}}
";"         {if(printTokenFlag){printf("SEMI\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return SEMI;}} 
","         {if(printTokenFlag){printf("COMMA\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return COMMA;}} 
"&&"        {if(printTokenFlag){printf("AND\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return AND;}}
"||"        {if(printTokenFlag){printf("OR\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return OR;}}
"&"         {if(printTokenFlag){printf("BITWISEAND\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return BITWISEAND;}}
"|"         {if(printTokenFlag){printf("BITWISEOR\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return BITWISEOR;}}
"^"         {if(printTokenFlag){printf("BITWISEXOR\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return BITWISEXOR;}}
"!="        {if(printTokenFlag){printf("NE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return NE;}}
"=="        {if(printTokenFlag){printf("EQ\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return EQ;}}
">="        {if(printTokenFlag){printf("GE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return GE;}}
">"         {if(printTokenFlag){printf("GT\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return GT;}}
"<="        {if(printTokenFlag){printf("LE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return LE;}}
"<"         {if(printTokenFlag){printf("LT\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return LT;}}
"!"         {if(printTokenFlag){printf("NOT\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return NOT;}}
"="         {if(printTokenFlag){printf("ASSIGN\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return ASSIGN;}}
"*"         {if(printTokenFlag){printf("MUL\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return MUL;}}
"+"         {if(printTokenFlag){printf("PLUS\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return PLUS;}}
"-"         {if(printTokenFlag){printf("MINUS\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return MINUS;}}
"/"         {if(printTokenFlag){printf("DIV\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return DIV;}}
"%"         {if(printTokenFlag){printf("MOD\n");} colNum++; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum);return MOD;}}
 
'{char}'		        {if(printTokenFlag) printf("CHRLIT(%s)\n", yytext); colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum); return CHRLIT;}}
'([^\n\'\\]|\\.)*'		{printf("Line %d, col %d: invalid char constant (%s)\n", lineNum, colNum, yytext); colNum+=yyleng;}
'([^\n\'\\]|\\.)*(\\)?	{printf("Line %d, col %d: unterminated char constant\n", lineNum, colNum); colNum+=yyleng;}

({digits}(("."{digits}?{exp}?)|{exp}))|("."{digits}{exp}?)  {if(printTokenFlag){printf("REALLIT(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum); return REALLIT;}}
{digits}                                                    {if(printTokenFlag){printf("INTLIT(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum); return INTLIT;}}
{id}                                                        {if(printTokenFlag){printf("ID(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum); return ID;}} 

"//".*$             ;
"//".*              ;
"/*"                {BEGIN COMMENT; auxL=lineNum; auxC=colNum; colNum+=yyleng;}
<COMMENT>"*/"       {BEGIN 0; colNum+=yyleng;}
<COMMENT>\r?\n|\r   {colNum=1;lineNum++;}
<COMMENT><<EOF>>    {printf("Line %d, col %d: unterminated comment\n",auxL,auxC); return 0;}
<COMMENT>.          {colNum++;}

{ws}        {colNum+=yyleng;}
\r?\n|\r    {colNum=1;lineNum++;}
<<EOF>>     {return 0;}
.           {printf("Line %d, col %d: illegal character (%s)\n",lineNum,colNum,yytext); colNum++;}


%%
int main(int argc, char *argv[]){
    if(argc==2){
        if(strcmp(argv[1],"-s")==0){
            yyparse(); //stdout lexical and sintax errors 
            if(!sintaxErrorFlag){ //if no syntax error occurred 
                /*FAZER ANÁLISE SEMÂNTICA*/
                /*IMPRIMIR TABELAS DE SIMBOLOS*/
                /*IMPRIMIR AST ANOTADA */
                startSemanticAnalysis(root);
                printSymTables();
            }
            /*LIBERTAR MEMORIA DA AST E DAS TABELAS DE SÍMBOLOS (desalocar tokens tb pela tabela)*/
        }
        else if(strcmp(argv[1],"-t")==0){
            //do lexical and sintax analysis
            yyparse();   
            
            if(!sintaxErrorFlag){ //if no syntax error occurred 
                printTree();
            }
            freeTree();//dont forget to free up memory!            
        }      
    }
    else{
        yyparse(); //stdout lexical and sintax errors 
        if(!sintaxErrorFlag){ //if no syntax error occurred 
            /*FAZER ANÁLISE SEMÂNTICA*/
            startSemanticAnalysis(root);
        }
        /*LIBERTAR MEMORIA DA AST E DAS TABELAS DE SÍMBOLOS (desalocar tokens tb pela tabela)*/
    }
    return 0;
}



int yywrap(){ return 1; }
