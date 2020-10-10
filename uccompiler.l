%{
    /********************************NOTAS*********************************************
    [EDUARDO]:
        O que fiz até agora foi apenas incluir os tokens e dar o respetivo print de
        acordo com o pedido no enunciado. 
        ws == whitespaces..tabs e/ou espaços..nao incluí o newline pq o newline é preciso ser interpretado separadamente para dar reset ao contador de colunas colNum
        Em relação ao INTLIT vou enviar ao prof um mail a perguntar se é suposto aceitar
            apenas numeros decimais ou se os hexadecimais e os octa tb contam..
        Testar o REALLINT e CHRLIT só para verificar q está a funcionar bem....
        Testar CHRLIT para o caso em q o char tem \t ou \n, escritos..
        PARTE MAIS COMPLICADA: deteçao e controlo dos erros lexicais
        Atençao: na linguagem UC nao existem arrays logo nao existem strings! "a" tem de ser
            interpretado como invalid char constant..assim como 'abc'
    ===================================================================================
    [RODRIGO]:
    

    ***********************************************************************************/
    //PL5_COMPILADORES@2020
    //Eduardo Filipe Ferreira Cruz  2018285164
    //Rodrigo ... Sobral    ...
    #include <string.h>

    int printFlag=0; //if 0(default), print error_messages only; else print all tokens and error_messages
    int lineNum=1,colNum=1; //contador de linha, contador de coluna
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
char    {if(printFlag){printf("CHAR\n");}}
int {if(printFlag){printf("INT\n");}}
short   {if(printFlag){printf("SHORT\n");}}
double  {if(printFlag){printf("DOUBLE\n");}}
void    {if(printFlag){printf("VOID\n");}}
if  {if(printFlag){printf("IF\n");}}
else    {if(printFlag){printf("ELSE\n");}}
while   {if(printFlag){printf("WHILE\n");}}
return  {if(printFlag){printf("RETURN\n");}}
{reserved}  {if(printFlag){printf("RESERVED(%s)\n",yytext);}}
"(" {if(printFlag){printf("LPAR\n");}}
")" {if(printFlag){printf("RPAR\n");}}
"{" {if(printFlag){printf("LBRACE\n");}}
"}" {if(printFlag){printf("RBRACE\n");}}
";" {if(printFlag){printf("SEMI\n");}} 
"," {if(printFlag){printf("COMMA\n");}} 
"&&"    {if(printFlag){printf("AND\n");}}
"||"    {if(printFlag){printf("OR\n");}}
"&" {if(printFlag){printf("BITWISEAND\n");}}
"|" {if(printFlag){printf("BITWISEOR\n");}}
"^" {if(printFlag){printf("BITWISEXOR\n");}}
"!="    {if(printFlag){printf("NE\n");}}
"=="    {if(printFlag){printf("EQ\n");}}
">="    {if(printFlag){printf("GE\n");}}
">" {if(printFlag){printf("GT\n");}}
"<="    {if(printFlag){printf("LE\n");}}
"<" {if(printFlag){printf("LT\n");}}
"!" {if(printFlag){printf("NOT\n");}}
"=" {if(printFlag){printf("ASSIGN\n");}}
"*" {if(printFlag){printf("MUL\n");}}
"+" {if(printFlag){printf("PLUS\n");}}
"-" {if(printFlag){printf("MINUS\n");}}
"/" {if(printFlag){printf("DIV\n");}}
"%" {if(printFlag){printf("MOD\n");}}
 
\'{char}\'  {if(printFlag){printf("CHRLIT(%s)\n",yytext);}}
\'{char}{char}+\'   {printf("Line <num linha>, col <num coluna>: invalid char constant (<c>)\n");}
\"{char}*\" {printf("Line <num linha>, col <num coluna>: invalid char constant (<c>)\n");} 
\'{char}* {printf("Line <num linha>, col <num coluna>: unterminated char constant\n");}

({signal}{digits}(("."{digits}?{exp}?)|{exp}))|("."{digits}{exp}?)  {if(printFlag){printf("REALLIT(%s)\n",yytext);}}
{signal}{digits}    {if(printFlag){printf("INTLIT(%s)\n",yytext);}}
{id}    {if(printFlag){printf("ID(%s)\n",yytext);}} 
 
"//".*$    {;}
"/*"    {BEGIN COMMENT;}
<COMMENT>"*/"   {BEGIN 0;}
<COMMENT>\r?\n {colNum=1;lineNum++;}
<COMMENT><<EOF>>    {BEGIN 0; printf("Line <num linha>, col <num coluna>: unterminated comment\n");}
<COMMENT>.  {;}

{ws}    {;}
\r?\n  {colNum=1;lineNum++;}
<<EOF>> {return 0;}
.   {printf("Line <num linha>, col <num coluna>: illegal character (<c>)\n");}
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
