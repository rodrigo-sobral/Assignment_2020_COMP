//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "ast.h"
#include "semantics.h"

struct sym_table* st_root; 

void startSemanticAnalysis(node* ast_root) {
    buildSymbolTables(ast_root);    
}

void buildSymbolTables(node* ast_root) {
    node* ast_node=ast_root->child; //lista ligada de func e variaveis globais

    st_root=create_global_table();
    /*funcDefinition funcDeclaration declaration */
    while(ast_node){
        if(strcmp(ast_node->str,"FuncDefinition")==0){
            handle_funcDefs(ast_node->child); /*ast_node->child is typespec node of funcDef*/
        } else if(strcmp(ast_node->str,"FuncDeclaration")==0){
            handle_funcDecs(ast_node->child);
        }else if(strcmp(ast_node->str,"Declaration")==0){
            handle_varDecs(ast_node->child);
        }    
        ast_node=ast_node->next;
    }
}

void handle_varDecs(node *n) {
    node *aux=n;//typedef
    _type expr_type;
    sym *s;
    s=create_sym(aux->next->tk->value,str_to_type(aux->str),0,0); 
    aux=aux->next; //id
    if(aux->next!=NULL){
        aux=aux->next; //expr
        expr_type=get_statement_type(aux,st_root);
        if(checkConflitingTypes(s->type,expr_type,aux->tk->lineNum, aux->tk->colNum)){
            free_sym(s);
        }
    }

    if (isDeclared(s, st_root)) printf("Line %d, col %d: Symbol %s already defined\n",n->tk->lineNum, n->tk->colNum ,s->name);
    else add_sym(st_root,s);
}

void handle_funcDecs(node* n) {
    //recebe head da lista ligada com nós:
    //typespec-->funcDeclarator
    sym *funcDec;
    node* aux=n, *paramAux;
    _type retType; //func return type
    char *funcName;

    retType=str_to_type(aux->str); //get func return type from typespec node

    //move to id
    aux=aux->next; //id
    funcName=strdup(aux->tk->value); //id(value)

    funcDec=create_sym(funcName,retType,1,0);
  
    if(isDeclared(funcDec,st_root)){
        //TODO:THROW ERROR!! função já declarada
        free_sym(funcDec);
        return;
    }
    else{
        //add param and 
        //add to global symlist
        //move to paramList node
        aux=aux->next; //paramlist
        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        aux=aux->child;
        while(aux){ //iterate through paramList childs
            paramAux=aux->child; //paramDec content (typespec-->[option: id])
            add_param(funcDec,str_to_type(paramAux->str)); //add param to sym paramlist
            aux=aux->next;
        }
        add_sym(st_root,funcDec);
    }

}

void handle_funcDefs(node* n) {
    //recebe head da lista ligada com nós:
    //typespec-->funcDeclarator-->funcBody
    sym *funcDef;
    sym_table *funcDefTable;
    node* aux=n, *paramAux;
    _type retType; //func return type
    char *funcName;

    retType=str_to_type(aux->str); //get func0 return type from typespec node

    //move to id
    aux=aux->next;
    funcName=strdup(aux->tk->value); //id(value)
    funcDef=create_sym(funcName,retType,1,0);
    /*VERIFICAÇÃO SE A FUNC JÁ FOI DECLARADA PARA VERIFICACAO DE ERROS*/

    if(isDeclared(funcDef,st_root)) {
        //move to paramList node
        aux=aux->next; //paramList
        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        paramAux=aux->child; //paramDec
        while(paramAux){ //iterate through paramList childs
           //paramDec content (typespec-->[option: id])
            add_param(funcDef,str_to_type(paramAux->child->str)); //add param to sym paramlist
            paramAux=paramAux->next;
        }

        //if params types are not equal
        if(!check_params_list_types(funcDef,get_sym(funcDef,st_root))){
            free_sym(funcDef);
            return;
        }
        
        /*checks if there's already a sym_table for this funcDef, if not create new sym_table for this funcdef, else throw error*/
        if(get_sym_table(funcName)){
            /*DONE:THROW FUNCTION ALREADY DEFINED ERROR*/
            printf("Line %d, col %d: Symbol %s already defined\n", 0, 0, funcName);
            free_sym(funcDef);
            return;
        } else {
            //create new sym_table
            funcDefTable=create_sym_table(funcName);
            add_sym(funcDefTable, create_sym("return", retType, 0, 0)); //return sym
            
            /*ADD PARAMETER VARIABLES SYMS TO FUNCDEF SYM_TABLE*/
            paramAux=aux->child; //paramDec
            while(paramAux!=NULL){ //iterate through paramList childs
                //paramDec content (typespec-->[option: id])
                if(str_to_type(paramAux->child->str)!=voidlit){
                    if(paramAux->child->next!=NULL){ //var name
                        add_sym(funcDefTable,create_sym(paramAux->child->next->tk->value,str_to_type(paramAux->child->str),0,1)); //parameter variable sym
                    }
                    else {
                        //DONE: throw error declaração de função sem nome de variaveis nos parâmetros
                        printf("Line %d, col %d: Lvalue required\n", 0, 0); 
                    } 
                }               
                paramAux=paramAux->next; //next paramdeclaration node
            }
                
            /*ENTRAR NO FUNCBODY*/
            aux=aux->next; //funcBody node
            /*FAZER ANALISE SEMANTICA PARA DECLARATIONS AND STATEMENTS DO FUNCBODY!*/
            add_funcBody_syms_to_table(funcDefTable,aux); //<-esta funcao deve fazer a analise semantica destes erros^
            add_sym_table(funcDefTable);
        }
    }
    else if(strcmp(funcName,"main")==0){ //main doesnt need to be explicitly declared
        //move to paramList node
        aux=aux->next;//paramList
        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        paramAux=aux->child; //paramDec
        while(paramAux){ //iterate through paramList childs
           //paramDec content (typespec-->[option: id])
            add_param(funcDef,str_to_type(paramAux->child->str)); //add param to sym paramlist
            paramAux=paramAux->next;
        }
        //add main declaration to global symtable
        add_sym(st_root,funcDef);
        //create new sym_table
        funcDefTable=create_sym_table(funcName);
        add_sym(funcDefTable,create_sym("return",retType,0,0)); //return sym

        /*ADD PARAMETER VARIABLES SYMS TO FUNCDEF SYM_TABLE*/
        paramAux=aux->child; //paramDec
        while(paramAux){ //iterate through paramList childs
            //paramDec content (typespec-->[option: id])
            if(str_to_type(paramAux->child->str)!=voidlit){
                if(paramAux->child->next){ //var name                   
                    if (isDeclared(funcDef , funcDefTable))
                        printf("Line %d, col %d: Symbol %s already defined\n", paramAux->child->next->tk->lineNum, paramAux->child->next->tk->colNum, paramAux->child->next->tk->value);
                    //add this variable to symtable of this function
                    add_sym(funcDefTable,create_sym(paramAux->child->next->tk->value,str_to_type(paramAux->child->str),0,1)); //parameter variable sym
                }
                else{
                    //TODO: throw error declaração de função sem nome de variaveis nos parâmetros
                    printf("**semantic error: funcDef sem nomes de variaveis**\n"); //temporary
                }   
            }             
            paramAux=paramAux->next; //next paramdeclaration node
        }

        aux=aux->next; //funcBody node
        
        /*FAZER ANALISE SEMANTICA PARA DECLARATIONS AND STATEMENTS DO FUNCBODY!*/
        add_funcBody_syms_to_table(funcDefTable,aux); //<-esta funcao deve fazer a analise semantica destes erros^
        add_sym_table(funcDefTable);
    }
    else if(isBeforeMainFunc(funcDef)){
        add_sym(st_root,funcDef); //add to global table 
        //move to paramList node
        aux=aux->next; //paramList
        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        paramAux=aux->child; //paramDec
        while(paramAux){ //iterate through paramList childs
           //paramDec content (typespec-->[option: id])
            add_param(funcDef,str_to_type(paramAux->child->str)); //add param to sym paramlist
            paramAux=paramAux->next;
        }
      
        /*checks if there's already a sym_table for this funcDef, if not create new sym_table for this funcdef, else throw error*/
        if(get_sym_table(funcName)){
            /*DONE:THROW FUNCTION ALREADY DEFINED ERROR*/
            printf("Line %d, col %d: Symbol %s already defined\n", 0, 0, funcName);
            free_sym(funcDef);
            return;
        } else {
            //create new sym_table
            funcDefTable=create_sym_table(funcName);
            add_sym(funcDefTable, create_sym("return", retType, 0, 0)); //return sym
            
            /*ADD PARAMETER VARIABLES SYMS TO FUNCDEF SYM_TABLE*/
            paramAux=aux->child; //paramDec
            while(paramAux){ //iterate through paramList childs
                //paramDec content (typespec-->[option: id])
                if(str_to_type(paramAux->child->str)!=voidlit){
                    if(paramAux->child->next){ //var name
                        add_sym(funcDefTable,create_sym(paramAux->child->next->tk->value,str_to_type(paramAux->child->str),0,1)); //parameter variable sym
                    }
                    else {
                        //DONE: throw error declaração de função sem nome de variaveis nos parâmetros
                        printf("Line %d, col %d: Lvalue required\n", 0, 0); 
                    } 
                }               
                paramAux=paramAux->next; //next paramdeclaration node
            }
                
            /*ENTRAR NO FUNCBODY*/
            aux=aux->next; //funcBody node
            /*FAZER ANALISE SEMANTICA PARA DECLARATIONS AND STATEMENTS DO FUNCBODY!*/
            add_funcBody_syms_to_table(funcDefTable,aux); //<-esta funcao deve fazer a analise semantica destes erros^
            add_sym_table(funcDefTable);
        }
    }
    else{
        //TODO:throw FUNCTION UNDECLARED error
        printf("Line %d, col %d: Symbol %s is not a function\n", 0, 0, funcDefTable->name);
        free_sym(funcDef);
        return;
    }    
}

void add_funcBody_syms_to_table(sym_table* st, node* funcBodyNode) {
    node *funcDecAndStats=funcBodyNode->child; 
    node *aux;
    _type expr_type;
    sym *s;
    // ^^lista ligada de declarations and statements do func body
    while(funcDecAndStats){
        if(strcmp(funcDecAndStats->str,"Declaration")==0){
            /*TODO:verificar se o symbolo já foi declarado */
            aux=funcDecAndStats->child; //typedef            
            s=create_sym(aux->next->tk->value,str_to_type(aux->str),0,0); 
            aux=aux->next; //id
            if(isDeclared(s,st)){
                //TODO: THROW ERROR váriável q se está a declarar já foi declarada
                printf("Line %d, col %d: Symbol %s already defined\n", 0, 0, funcDecAndStats->str);
            }
            else{
                if(aux->next!=NULL){
                    //var definition
                    aux=aux->next; //expr
                    expr_type=get_statement_type(aux,st_root);
                    if(checkConflitingTypes(s->type,expr_type,aux->tk->lineNum, aux->tk->colNum)){
                        free_sym(s);
                        funcDecAndStats=funcDecAndStats->next;
                        continue;
                    }
                }
                add_sym(st,s); //adiciona sym à table apenas se este n tiver sido declarado!
            }
            
        }
        else{
            expr_type=get_statement_type(funcDecAndStats, st);
        }
        funcDecAndStats=funcDecAndStats->next;
    }
}

int isDeclared(sym *s, sym_table *st) {
    //check if s is declared in st, if sym is a function and sym is before main func, return declared=1
    sym *symAux=st->sym_list;
    while(symAux!=NULL){
        if(strcmp(s->name,symAux->name)==0 && s->isFunc==symAux->isFunc) return 1;
        symAux=symAux->next;
    }
    return 0; 
}

int check_params_list_types(sym *sym_defined, sym *sym_declared) {
    param *list0=sym_defined->param_list;
    param *list1=sym_declared->param_list;

    while(list0 && list1){
        checkConflitingTypes(list0->type,list1->type,0, 0);         
        list0=list0->next;
        list1=list1->next;
    }
    if(list1 || list0) {
        //throw error ? one sym has more parameters than the other
        printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n", 0, 0, sym_declared->name, paramsCounter(sym_defined->param_list), paramsCounter(sym_declared->param_list));
        return 0; 
    }
    return 1;    
}

int paramsCounter(struct param* param_list) {
    int counter=0;
    param* param_list_aux= param_list;
    while (param_list_aux) {
        counter++;
        param_list_aux= param_list_aux->next;
    }
    return counter;    
}

_type get_statement_type(node* statement, sym_table *st) {
    node *aux=statement;
    _type t_aux;
    if(strcmp(statement->str,"Plus")==0 || strcmp(statement->str,"Minus")==0 || strcmp(statement->str,"Not")==0){
        //1 nó filho
        if(!isTerminal(aux->child)){
            return get_statement_type(aux->child,st);
        }
        else{
            t_aux=getTerminalType(aux->child,st);
            if(!(t_aux==reallit||t_aux==intlit)){
                //TODO: THROW ERROR invalid statement type (por exemplo fazer: -'a' )
                printf("error\n");
                return undef;
            }
        }
    }
    else if(strcmp(statement->str,"Call")==0){
        return get_funcCall_type(statement,st);
    }
    else if(strcmp(statement->str,"Store")==0){
        return get_store_type(statement,st);
    }
    else if(strcmp(statement->str,"Comma")==0){
        //evaluates first expr and discard result, evaluates second expr and returns result
        get_statement_type(statement->child,st); //evaluates and discards
        return get_statement_type(statement->child->next,st); //evaluates and returns
    }
    else if(strcmp(statement->str,"Add")==0||strcmp(statement->str,"Sub")==0||strcmp(statement->str,"Mul")==0||strcmp(statement->str,"Div")==0||strcmp(statement->str,"Mod")==0){
        return get_operation_type(statement,st);
    }
    else if(strcmp(statement->str,"Or")==0||strcmp(statement->str,"And")==0||strcmp(statement->str,"BitWiseAnd")==0||strcmp(statement->str,"BitWiseOr")==0||strcmp(statement->str,"BitWiseXor")==0||strcmp(statement->str,"Eq")==0||strcmp(statement->str,"Ne")==0||strcmp(statement->str,"Le")==0||strcmp(statement->str,"Ge")==0||strcmp(statement->str,"Lt")==0||strcmp(statement->str,"Gt")==0){
        return intlit;
    }
    else if(strcmp(statement->str,"StatList")==0){
        add_funcBody_syms_to_table(st, statement); 
        return voidlit; //doesnt matter here..
    }
    else if(strcmp(statement->str,"While")==0){
        //check confliting types in expr of while(expr)
        checkConflitingTypes(intlit,get_statement_type(statement->child,st),statement->child->tk->lineNum,statement->child->tk->colNum);
        add_funcBody_syms_to_table(st, statement); 
        return voidlit; //doesnt matter here..
    }
    else if(strcmp(statement->str,"If")==0){
        checkConflitingTypes(intlit,get_statement_type(statement->child,st),statement->child->tk->lineNum,statement->child->tk->colNum);
        add_funcBody_syms_to_table(st, statement); 
        return voidlit; //doesnt matter here..
    }
    else if(strcmp(statement->str,"Return")==0){
        //verificar se tipo coincide q se espera returnar, coincide
        checkConflitingTypes(st->sym_list->type,get_statement_type(statement->child,st),statement->child->tk->lineNum,statement->child->tk->colNum);
        return st->sym_list->type; //doesnt matter here..
    }
    else if(isTerminal(statement)){
        //intlit ou realit ou undef..
        return getTerminalType(statement,st);
    }
    else if(strcmp(statement->str,"Null")==0){
        return voidlit;
    }
}

_type get_operation_type(node * operation,sym_table *st) {
    node *n_aux=operation->child;
    _type type0, type1; //operation only has 2 nodes
    type0=get_statement_type(n_aux,st);
    type1=get_statement_type(n_aux->next,st);
    if(type0==undef||type1==undef){
        return undef;
    }
    else if(type0==reallit||type1==reallit){
        return reallit;
    }
    else{
        return intlit;
    }
}

_type get_store_type(node *store, sym_table*st) {
    node *n_aux= store->child; //store variable node (Id) 
    sym *s_aux, *storedSym; 
    _type t_aux;
    s_aux= create_sym(n_aux->tk->value, undef, 0, 0);
    storedSym= get_sym(s_aux, st);
    if(storedSym==NULL){
        storedSym==get_sym(s_aux,st_root);
        if(storedSym==NULL){ 
            //TODO: THROW ERROR VARIÁVEL NAO ESTÀ DECLARADA (nome da variável n declara: n_aux->tk->value)
            //linenum colnum: n_aux->tk->lineNum e  n_aux->tk->colNum
            printf("Line %d, col %d: Symbol %s already defined\n",n_aux->tk->lineNum, n_aux->tk->colNum , n_aux->tk->value);
            return undef;
        }
    }
    if(storedSym->type==charlit) { t_aux=intlit; } //chars são considerados ints
    else { t_aux=storedSym->type; }
    if ( t_aux != get_statement_type(n_aux->next, st) ) {
        //TODO: THROW ERROR tentar associar a uma variável um tipo q n é aquele q foi declarado para essa variável
        //tipo: int i; i=2.3; <--
        //linenum e colnum: n_aux->next->tk->lineNum e n_aux->next->tk->colNum 
        printf("Line %d, col %d: Conflicting  types (got %s, expected %s)\n", n_aux->next->tk->lineNum, n_aux->next->tk->colNum, type_to_str(get_statement_type(n_aux->next, st)), type_to_str(t_aux));
        return undef;
    } else {
        return t_aux;
    }

}

_type get_funcCall_type(node *call,sym_table*st) {
    node *n_aux;
    sym * s_aux, *funcSym;
    param* funcCall_params,*p_aux0,*p_aux1; //da func call
    int count=0,i;
    n_aux=call->child; //function name node->str = Id(name)
    s_aux=create_sym(n_aux->tk->value,undef,1,0);
    funcSym=get_sym(s_aux,st_root);
    n_aux=n_aux->next; //1º argumento da call
    if(funcSym){
        //calcular tipos dos parametros dos argumentos da funccall e add à lista de param do sym_auxiliar
        while(n_aux){
            add_param(s_aux,get_statement_type(n_aux,st));            
            n_aux=n_aux->next; //next funccall arg
        }
        //verificar tipos dos parametros
        p_aux0=funcSym->param_list; //lista com tipos de parametros da funcao declarada no global
        p_aux1=s_aux->param_list; //lista com tipos de parametros da func call 
        if(paramsCounter(p_aux0)!=paramsCounter(p_aux1)){
            //TODO:  THROW ERROR nr de parametros da function call diferente do nr de parametros declarados para esssa funcção
            printf("error");
        }
        while(p_aux0&&p_aux1){
            count++;
            if(p_aux0->type!=p_aux1->type){                
                n_aux=call; n_aux=n_aux->child; //1º nó dos parametros da funcCall
                for(i=1;i<=count;i++) n_aux=n_aux->next;
                //TODO: THROW ERROR : tipo do parametro no funcCall diferente daquele q foi declarado!
                //numlinha e numcoluna do erro vai ser: n_aux->tk->lineNum e n_aux->tk->colNum !!! 
                printf("error"); 
            }
            p_aux1=p_aux1->next;
            p_aux0=p_aux0->next;
        }
        free_sym(s_aux);
        return funcSym->type;
    }
    else{
        //TODO: THROW ERROR função chamada não está declarada!
        free_sym(s_aux);
        return undef;
    }
}



int getTerminalType(node *n,sym_table *st) {
    sym *aux0,*aux1;
    if(strncmp(n->str,"Id",2)==0){
        aux1=create_sym(n->tk->value,undef,0,0);
        if((aux0=get_sym(aux1,st))==NULL){ //if not in local table, search in global table
            if((aux0=get_sym(aux1,st_root))==NULL){
                //not in global table...
                //TODO: THROW ERROR VARIABLE NOT DECLARED
                return undef;
            } else{
                if(aux0->type==charlit) {return intlit;}
                else {return aux0->type;}
            }
        }
        else{
            if(aux0->type==charlit) {return intlit;}
            else{return aux0->type;}
        }
        free_sym(aux1);
    }
    else if(strncmp(n->str,"ChrLit",6)==0){
        //return charlit;
        return intlit;
    } else if(strncmp(n->str,"IntLit",6)==0){
        return intlit;
    } else if(strncmp(n->str,"RealLit",7)==0){
        return reallit;
    }
}
int isTerminal(node *n) {
    if(n->tk->value) return 1;    
    else return 0;
}

int isBeforeMainFunc(sym *s){
    sym *aux=st_root->sym_list;
    while(aux!=NULL){
        if(strcmp(s->name,aux->name)==0&&aux->isFunc==s->isFunc){return 1;}
        else if(strcmp("main",aux->name)==0&&aux->isFunc){return 0;}
        aux=aux->next;
    }
    return 1;
}

int checkConflitingTypes(_type expectedType,_type gotType,int line, int col){
    _type aux0,aux1;
    if(expectedType==charlit||expectedType==shortlit){aux0=intlit;}
    else{aux0=expectedType;}
    if(gotType==charlit||gotType==shortlit){aux1=intlit;}
    else{aux1=gotType;}
    if(aux0==aux1){
        return 0; //no conflict
    }
    else if(aux0==reallit&&aux1==intlit){
        return 0; //no conflict
    }
    else{
        printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", line, col, type_to_str(gotType), type_to_str(expectedType));
        return 1;
    }
}