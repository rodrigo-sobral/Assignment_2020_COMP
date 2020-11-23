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

void startSemanticAnalysis(node* ast_root){
    buildSymbolTables(ast_root);    
}

void buildSymbolTables(node* ast_root){
    node* ast_node=ast_root->child; //lista ligada de func e variaveis globais

    st_root=create_global_table();
    /*funcDefinition funcDeclaration declaration */
    while(ast_node!=NULL){
        if(strcmp(ast_node->str,"FuncDefinition")==0){
            handle_funcDefs(ast_node->child); /*ast_node->child is typespec node of funcDef*/
        }
        else if(strcmp(ast_node->str,"FuncDeclaration")==0){
            handle_funcDecs(ast_node->child);
        }
        else if(strcmp(ast_node->str,"Declaration")==0){
            handle_varDecs(ast_node->child);
        }    


        ast_node=ast_node->next;
    }
}

void handle_varDecs(node *n){
    node *aux=n;//typedef
    _type type;
    sym *s;

    type=str_to_type(aux->str);
    aux=aux->next; //id
    s=create_sym(aux->tk->value,type,0,0); 
    if(aux->next!=NULL){
        aux=aux->next; //expr
        s->isDef=1;
        /* TODO: tratar de inicializações ..FAZER VERIFICAÇÕES SOBRE AS EXPRESSIONS..POR CAUSA DOS TIPOS E DAS DECLARAÇÕES*/
        /*TODO: THROW ERROR SE O TYPE FOR INVALIDO e dar free no symbolo s*/
    }
    /*TODO:verificar se o symbolo já foi declarado antes!*/
    add_sym(st_root,s);
}

void handle_funcDecs(node* n){
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
    /*VERIFICAÇÃO SE A FUNC JÁ FOI DECLARADA PARA VERIFICACAO DE ERROS*/
    if(isDeclared(funcDec,st_root)){
        //THROW ERROR!!
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
        while(aux!=NULL){ //iterate through paramList childs
            paramAux=aux->child; //paramDec content (typespec-->[option: id])
            add_param(funcDec,str_to_type(paramAux->str)); //add param to sym paramlist
            aux=aux->next;
        }
        add_sym(st_root,funcDec);
    }

}

void handle_funcDefs(node* n){
    //recebe head da lista ligada com nós:
    //typespec-->funcDeclarator-->funcBody
    sym *funcDef;
    sym_table *funcDefTable;
    node* aux=n, *paramAux;
    _type retType; //func return type
    char *funcName;

    retType=str_to_type(aux->str); //get func return type from typespec node

    //move to id
    aux=aux->next;
    funcName=strdup(aux->tk->value); //id(value)
    funcDef=create_sym(funcName,retType,1,1);
    /*VERIFICAÇÃO SE A FUNC JÁ FOI DECLARADA PARA VERIFICACAO DE ERROS*/
    if(isDeclared(funcDef,st_root)){
        //move to paramList node
        aux=aux->next;//paramList
        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        paramAux=aux->child; //paramCec
        while(paramAux!=NULL){ //iterate through paramList childs
           //paramDec content (typespec-->[option: id])
            add_param(funcDef,str_to_type(paramAux->child->str)); //add param to sym paramlist
            paramAux=paramAux->next;
        }

        if(!check_params_list_types(funcDef,get_sym(funcDef,st_root))){
            //if params types are not equal
            //TODO: THROW ERROR parametros da definicao diferentes da declaracao
            return;
            free_sym(funcDef);
        }
        
        /*checks if there's already a sym_table for this funcDef, if not create new sym_table for this funcdef, else throw error*/
        if(get_sym_table(funcName)!=NULL){
            /*TODO:THROW FUNCTION ALREADY DEFINED ERROR*/
            free_sym(funcDef);
            return;
        }
        else{
            //create new sym_table
            funcDefTable=create_sym_table(funcName);
            add_sym(funcDefTable,create_sym("return",retType,0,0)); //return sym

            /*ADD PARAMETER VARIABLES SYMS TO FUNCDEF SYM_TABLE*/
            paramAux=aux->child; //paramDec
            while(paramAux!=NULL){ //iterate through paramList childs
                //paramDec content (typespec-->[option: id])
                if(paramAux->child->next!=NULL){ //var name
                    /*TODO:FAZER VERIFICAÇAO SEMANTICA SE NOME DA VAR (paramAux->child->next->tk->value) é igual ao utilizado na funcDec desta funcao.. */
                    //add this variable to symtable of this function
                    add_sym(funcDefTable,create_sym(paramAux->child->next->tk->value,str_to_type(paramAux->child->str),0,0)); //parameter variable sym
                }
                else{
                    //TODO: throw error declaração de função sem nome de variaveis nos parâmetros
                    printf("**semantic error: funcDef sem nomes de variaveis**\n"); //temporary
                }                
                paramAux=paramAux->next; //next paramdeclaration node
            }
                

            /*TODO: ENTRAR NO FUNCBODY*/
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
        paramAux=aux->child; //paramCec
        while(paramAux!=NULL){ //iterate through paramList childs
           //paramDec content (typespec-->[option: id])
            add_param(funcDef,str_to_type(paramAux->child->str)); //add param to sym paramlist
            //if(paramAux->child->next!=NULL){ //var name
                /*TODO:FAZER VERIFICAÇAO SEMANTICA SE NOME DA VAR (paramAux->child->next->tk->value) é igual ao utilizado na funcDec desta funcao.. */
                //add this variable to symtable of this function
            //}
            paramAux=paramAux->next;
        }
        //add main declaration to global symtable
        add_sym(st_root,funcDef);
        //create new sym_table
        funcDefTable=create_sym_table(funcName);
        add_sym(funcDefTable,create_sym("return",retType,0,0)); //return sym

        /*ADD PARAMETER VARIABLES SYMS TO FUNCDEF SYM_TABLE*/
        paramAux=aux->child; //paramDec
        while(paramAux!=NULL){ //iterate through paramList childs
            //paramDec content (typespec-->[option: id])
            if(paramAux->child->next!=NULL){ //var name
                /*TODO:FAZER VERIFICAÇAO SEMANTICA SE NOME DA VAR (paramAux->child->next->tk->value) é igual ao utilizado na funcDec desta funcao.. */
                //add this variable to symtable of this function
                add_sym(funcDefTable,create_sym(paramAux->child->next->tk->value,str_to_type(paramAux->child->str),0,0)); //parameter variable sym
            }
            else{
                //TODO: throw error declaração de função sem nome de variaveis nos parâmetros
                printf("**semantic error: funcDef sem nomes de variaveis**\n"); //temporary
            }                
            paramAux=paramAux->next; //next paramdeclaration node
        }

        /*TODO: ENTRAR NO FUNCBODY*/
        aux=aux->next; //funcBody node
        /*FAZER ANALISE SEMANTICA PARA DECLARATIONS AND STATEMENTS DO FUNCBODY!*/
        add_funcBody_syms_to_table(funcDefTable,aux); //<-esta funcao deve fazer a analise semantica destes erros^
        add_sym_table(funcDefTable);
    }
    else{
        //TODO:throw FUNCTION UNDECLARED error
        free_sym(funcDef);
        return;
    }    
}

void add_funcBody_syms_to_table(sym_table* st, node* funcBodyNode){
    node *funcDecAndStats=funcBodyNode->child;
    node *aux;
    _type type;
    sym *s;
    // ^^lista ligada de declarations and statements do func body
    while(funcDecAndStats!=NULL){
        if(strcmp(funcDecAndStats->str,"Declaration")==0){
            /*TODO:verificar se o symbolo já foi declarado */
            aux=funcDecAndStats->child; //typedef
            type=str_to_type(aux->str);
            aux=aux->next; //id
            s=create_sym(aux->tk->value,type,0,0); 
            if(aux->next!=NULL){
                //var definition
                aux=aux->next; //expr
                s->isDef=1;
                /*FAZER VERIFICAÇÕES SOBRE AS EXPRESSIONS..POR CAUSA DOS TIPOS E DAS DECLARAÇÕES*/
                /*TODO: THROW ERROR SE O TYPE FOR INVALIDO e dar free no symbolo s*/
            }
            add_sym(st,s);
        }
        //else{
            /*TODO: TRATAR DOS STATEMENTS LATER..É O Q DÁ MAIS TRABALHO..RECURSIVIDADE..? :P !!!!!*/
        //}
        funcDecAndStats=funcDecAndStats->next;
    }
}

int isDeclared(sym *s, sym_table *st){
    //check if s is declared in st, if sym is a function and sym is before main func, return declared=1
    sym *symAux=st->sym_list;
    while(symAux!=NULL){
        if(strcmp(s->name,symAux->name)==0&&s->isFunc==symAux->isFunc){
            return 1;
        }
        symAux=symAux->next;
    }
    return 0; 
}

int check_params_list_types(sym *s0, sym *s1){
    param *list0=s0->param_list;
    param *list1=s1->param_list;

    while(list0!=NULL&&list1!=NULL){
        if(list0->type!=list1->type){
            return 0; //not equal
        }
        list1=list1->next;
        list0=list0->next;
    }
    if(list1!=NULL||list0!=NULL){
        //throw error ?
        return 0; //one sym has more parameters than the other
    }
    return 1;    
}
