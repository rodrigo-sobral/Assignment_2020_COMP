%{
    //PL5_COMPILADORES@2020
    //Eduardo Filipe Ferreira Cruz  2018285164
    //Rodrigo ... Sobral    2018298209
    #include <string.h>

    int printFlag=0; //if 0(default), print error_messages only; else print all tokens and error_messages
    int lineNum=1,colNum=1; //line counter, collumn counter
    int auxL,auxC;    //to save aux values 
%}
%X COMMENT
letter  [A-Za-z_]
digit   [0-9]
digits  {digit}+
signal  [+-]?
exp [Ee]{signal}{digits}
reserved    "++"|"--"|"["|"]"|"auto"|"break"|"case"|"const"|"continue"|"default"|"do"|"enum"|"extern"|"float"|"for"|"float"|"goto"|"long"|"register"|"return"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"inline"|"inline"|"_Bool"|"_Complex"|"_Imaginary"
id  {letter}({letter}|{digit})*
ws  [\t ]+
esc ("\\n"|"\\t"|"\\"|"\\\'"|"\\\""|"\\ooo")
char ([^']|{esc})    
%%
char    {if(printFlag){printf("CHAR\n");} colNum+=yyleng;}
int {if(printFlag){printf("INT\n");} colNum+=yyleng;}
short   {if(printFlag){printf("SHORT\n");} colNum+=yyleng;}
double  {if(printFlag){printf("DOUBLE\n");} colNum+=yyleng;}
void    {if(printFlag){printf("VOID\n");} colNum+=yyleng;}
if  {if(printFlag){printf("IF\n");} colNum+=yyleng;}
else    {if(printFlag){printf("ELSE\n");} colNum+=yyleng;}
while   {if(printFlag){printf("WHILE\n");} colNum+=yyleng;}
return  {if(printFlag){printf("RETURN\n");} colNum+=yyleng;}
{reserved}  {if(printFlag){printf("RESERVED(%s)\n",yytext);} colNum+=yyleng;}
"(" {if(printFlag){printf("LPAR\n");} colNum++;}
")" {if(printFlag){printf("RPAR\n");} colNum++;}
"{" {if(printFlag){printf("LBRACE\n");} colNum++;}
"}" {if(printFlag){printf("RBRACE\n");} colNum++;}
";" {if(printFlag){printf("SEMI\n");} colNum++;} 
"," {if(printFlag){printf("COMMA\n");} colNum++;} 
"&&"    {if(printFlag){printf("AND\n");} colNum+=yyleng;}
"||"    {if(printFlag){printf("OR\n");} colNum+=yyleng;}
"&" {if(printFlag){printf("BITWISEAND\n");} colNum++;}
"|" {if(printFlag){printf("BITWISEOR\n");} colNum++;}
"^" {if(printFlag){printf("BITWISEXOR\n");} colNum++;}
"!="    {if(printFlag){printf("NE\n");} colNum+=yyleng;}
"=="    {if(printFlag){printf("EQ\n");} colNum+=yyleng;}
">="    {if(printFlag){printf("GE\n");} colNum+=yyleng;}
">" {if(printFlag){printf("GT\n");} colNum++;}
"<="    {if(printFlag){printf("LE\n");} colNum+=yyleng;}
"<" {if(printFlag){printf("LT\n");} colNum++;}
"!" {if(printFlag){printf("NOT\n");} colNum++;}
"=" {if(printFlag){printf("ASSIGN\n");} colNum++;}
"*" {if(printFlag){printf("MUL\n");} colNum++;}
"+" {if(printFlag){printf("PLUS\n");} colNum++;}
"-" {if(printFlag){printf("MINUS\n");} colNum++;}
"/" {if(printFlag){printf("DIV\n");} colNum++;}
"%" {if(printFlag){printf("MOD\n");} colNum++;}
 
\'{char}\'  {if(printFlag){printf("CHRLIT(%s)\n",yytext);} colNum+=yyleng;}
\'{char}{char}+\'   {printf("Line %d, col %d: invalid char constant (%s)\n",lineNum,colNum,yytext); colNum+=yyleng;}
\"{char}*\" {printf("Line %d, col %d: invalid char constant (%s)\n",lineNum,colNum,yytext); colNum+=yyleng;} 
\'{char}* {printf("Line %d, col %d: unterminated char constant\n",lineNum,colNum); colNum+=yyleng;}

({signal}{digits}(("."{digits}?{exp}?)|{exp}))|("."{digits}{exp}?)  {if(printFlag){printf("REALLIT(%s)\n",yytext);} colNum+=yyleng;}
{signal}{digits}    {if(printFlag){printf("INTLIT(%s)\n",yytext);} colNum+=yyleng;}
{id}    {if(printFlag){printf("ID(%s)\n",yytext);} colNum+=yyleng;} 
 
"//".*$    {;}
"/*"    {BEGIN COMMENT; auxL=lineNum; colNum+=yyleng;}
<COMMENT>"*/"   {BEGIN 0; auxC=colNum; colNum+=yyleng;}
<COMMENT>\r?\n {colNum=1;lineNum++;}
<COMMENT><<EOF>>    {BEGIN 0; printf("Line %d, col %d: unterminated comment\n",auxL,auxC); colNum+=yyleng;}
<COMMENT>.  {colNum++;}

{ws}    {colNum+=yyleng;}
\r?\n  {colNum=1;lineNum++;}
<<EOF>> {return 0;}
.   {printf("Line %d, col %d: illegal character (%s)\n",lineNum,colNum,yytext); colNum++;}
%%
int main(int argc, char *argv[]){
    if(argc==2){
        if(strcmp(argv[1],"-l")==0){
            printFlag=1; //stdout print tokens and error_messages
        }
    }
    yylex();
    return 0;
}

int yywrap(){
    return 1;
}
