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
    int lexicalErrorFlag=0;
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
char        {if(printTokenFlag){printf("CHAR\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return CHAR;}}
int         {if(printTokenFlag){printf("INT\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return INT;}}
short       {if(printTokenFlag){printf("SHORT\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return SHORT;}}
double      {if(printTokenFlag){printf("DOUBLE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return DOUBLE;}}
void        {if(printTokenFlag){printf("VOID\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return VOID;}}
if          {if(printTokenFlag){printf("IF\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return IF;}}
else        {if(printTokenFlag){printf("ELSE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return ELSE;}}
while       {if(printTokenFlag){printf("WHILE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return WHILE;}}
return      {if(printTokenFlag){printf("RETURN\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(NULL,lineNum,colNum-yyleng);return RETURN;}}
{reserved}  {if(printTokenFlag){printf("RESERVED(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng); return RESERVED;}}
"("         {if(printTokenFlag){printf("LPAR\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return LPAR;}}
")"         {if(printTokenFlag){printf("RPAR\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return RPAR;}}
"{"         {if(printTokenFlag){printf("LBRACE\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return LBRACE;}}
"}"         {if(printTokenFlag){printf("RBRACE\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return RBRACE;}}
";"         {if(printTokenFlag){printf("SEMI\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return SEMI;}} 
","         {if(printTokenFlag){printf("COMMA\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return COMMA;}} 
"&&"        {if(printTokenFlag){printf("AND\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return AND;}}
"||"        {if(printTokenFlag){printf("OR\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return OR;}}
"&"         {if(printTokenFlag){printf("BITWISEAND\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return BITWISEAND;}}
"|"         {if(printTokenFlag){printf("BITWISEOR\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return BITWISEOR;}}
"^"         {if(printTokenFlag){printf("BITWISEXOR\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return BITWISEXOR;}}
"!="        {if(printTokenFlag){printf("NE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return NE;}}
"=="        {if(printTokenFlag){printf("EQ\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return EQ;}}
">="        {if(printTokenFlag){printf("GE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return GE;}}
">"         {if(printTokenFlag){printf("GT\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return GT;}}
"<="        {if(printTokenFlag){printf("LE\n");} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return LE;}}
"<"         {if(printTokenFlag){printf("LT\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return LT;}}
"!"         {if(printTokenFlag){printf("NOT\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return NOT;}}
"="         {if(printTokenFlag){printf("ASSIGN\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return ASSIGN;}}
"*"         {if(printTokenFlag){printf("MUL\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return MUL;}}
"+"         {if(printTokenFlag){printf("PLUS\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return PLUS;}}
"-"         {if(printTokenFlag){printf("MINUS\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return MINUS;}}
"/"         {if(printTokenFlag){printf("DIV\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return DIV;}}
"%"         {if(printTokenFlag){printf("MOD\n");} colNum++; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng);return MOD;}}
 
'{char}'		        {if(printTokenFlag) printf("CHRLIT(%s)\n", yytext); colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng); return CHRLIT;}}
'([^\n\'\\]|\\.)*'		{printf("Line %d, col %d: invalid char constant (%s)\n", lineNum, colNum, yytext); colNum+=yyleng; lexicalErrorFlag=1;}
'([^\n\'\\]|\\.)*(\\)?	{printf("Line %d, col %d: unterminated char constant\n", lineNum, colNum); colNum+=yyleng; lexicalErrorFlag=1;}

({digits}(("."{digits}?{exp}?)|{exp}))|("."{digits}{exp}?)  {if(printTokenFlag){printf("REALLIT(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng); return REALLIT;}}
{digits}                                                    {if(printTokenFlag){printf("INTLIT(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng); return INTLIT;}}
{id}                                                        {if(printTokenFlag){printf("ID(%s)\n",yytext);} colNum+=yyleng; if(parseFlag){yylval.tk = createToken(yytext,lineNum,colNum-yyleng); return ID;}} 

"//".*$             ;
"//".*              ;
"/*"                {BEGIN COMMENT; auxL=lineNum; auxC=colNum; colNum+=yyleng;}
<COMMENT>"*/"       {BEGIN 0; colNum+=yyleng;}
<COMMENT>\r?\n|\r   {colNum=1;lineNum++;}
<COMMENT><<EOF>>    {printf("Line %d, col %d: unterminated comment\n",auxL,auxC); return 0; lexicalErrorFlag=1;}
<COMMENT>.          {colNum++;}

{ws}        {colNum+=yyleng;}
\r?\n|\r    {colNum=1;lineNum++;}
<<EOF>>     {return 0;}
.           {printf("Line %d, col %d: illegal character (%s)\n",lineNum,colNum,yytext); colNum++; lexicalErrorFlag=1;}


%%
int main(int argc, char *argv[]){
    if(argc==2){
        if(strcmp(argv[1],"-l")==0){
            //do only lexical analysis and stdout tokens and error_messages from lexical analysis   
            printTokenFlag=1; 
            parseFlag=0; //parsing off
            yylex(); 
        } else if(strcmp(argv[1],"-e1")==0) {
            //do lexical analysis only and stdout lexical errors
            parseFlag=0; //parsing off
            yylex();
        } else if(strcmp(argv[1],"-t")==0){
            //do lexical and sintax analysis
            yyparse();   
            if(!sintaxErrorFlag) { //if no LEXIAL error occurred 
                printTree();
            }
            freeTree();//dont forget to free up memory!            
        } else if(strcmp(argv[1],"-e2")==0) {
            yyparse(); //stdout lexical and sintax errors
            freeTree();//dont forget to free up memory!  
        } else if(strcmp(argv[1],"-s")==0){
            yyparse(); //stdout lexical and sintax errors 
            if(!sintaxErrorFlag){ //if no syntax error occurred 
                /*FAZER ANÁLISE SEMÂNTICA*/
                /*IMPRIMIR TABELAS DE SIMBOLOS*/
                /*IMPRIMIR AST ANOTADA */
                startSemanticAnalysis(root);
                printSymTables();
                printTree(1); //with anotations
            }
            freeTree();
            freeTables();
        } else if(strcmp(argv[1],"-e3")==0) {
            yyparse(); //stdout lexical and sintax errors 
            if(!sintaxErrorFlag){ //if no syntax error occurred 
                /*FAZER ANÁLISE SEMÂNTICA se n houverem erros sintaticos*/
                startSemanticAnalysis(root);
            }
            freeTree();
            freeTables();
        } 
    } else{
        yyparse(); //stdout lexical and sintax errors 
        if(!sintaxErrorFlag&&!lexicalErrorFlag) { //if no syntax error occurred 
            startSemanticAnalysis(root); /*FAZER ANÁLISE SEMÂNTICA*/
            if(/*se n houver nenhum erro semantico*/){

                /*GERAR CÓDIGO LLVM*/

            }
        }

        /*
        SÓ DEVE SER GERADO CÓDIGO LLVM
        CASO NÃO HAJA NENHUM ERRO,
        NEM SEMANTICO NEM LEXICAL NEM SINTATICO
        */

        /*
        TODO:
        implementar deteção de ocorrência de erros semânticos,
        utilizando uma flag, 
        APÓS TERMINADAS A META 3!
        */


        freeTree();
        freeTables();
    }
    return 0;
}



int yywrap(){ return 1; }
