//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "ast.h"

extern struct sym_table* st_root; 
/*
st=symtable
s=sym
*/

sym_table* create_sym_table(char* name) {
    sym_table *st;
    if ((st = (sym_table *)malloc(sizeof(sym_table))) == NULL) {
        fprintf(stderr, "Error allocating memory");
        exit(-1);
    }
    st->next = NULL;
    st->sym_list = NULL;
    st->name = strdup(name);
    return st;
}

sym_table* create_global_table(void) {
    sym *aux;
    sym_table* st=create_sym_table("Global");
    aux=  create_sym("putchar", intlit, 1,0);
    add_param(aux, intlit);
    add_sym(st, aux);
    
    aux= create_sym("getchar", intlit, 1,0);
    add_param(aux, voidlit);
    add_sym(st, aux);

    /*
    aux= create_sym("main", intlit, 1, 0);
    add_param(aux, voidlit);
    add_sym(st, aux);
    */
        
    return st;
}

void add_sym_table(sym_table *st) {
    sym_table *aux=st_root;
    if(aux!=NULL){
        while(aux->next!=NULL) aux=aux->next;
        aux->next=st;
    } else st_root=st;
}

sym_table *get_sym_table(char* name) { 
    //searches and returns sym_table if found (doesnt include global table!)
    sym_table *aux;
    if(st_root!=NULL){
        aux=st_root->next;
        while(aux!=NULL){
            if(strcmp(aux->name,name)==0) return aux;
            aux=aux->next;
        }
    } return NULL;
}

/********************************************************/
sym *create_sym(char *name,_type type, int isfunc, int isparam) {
    sym *s;
    if ((s = (sym *)malloc(sizeof(sym))) == NULL) {
        fprintf(stderr, "Error allocating memory");
        exit(-1);
    }
    s->param_list=NULL;
    s->next = NULL;
    s->isParam=isparam;
    s->isFunc=isfunc;
    s->type = type;
    s->name = strdup(name);
    return s;
}

void add_sym(sym_table* st, sym* s) {
    sym *aux=NULL;
    if(st->sym_list!=NULL){
        aux=st->sym_list;
        while(aux->next!=NULL) aux=aux->next;
        aux->next=s;
    } else st->sym_list=s;
}

sym* get_sym(sym* s,sym_table* st) {
    sym *aux=st->sym_list;
    while(aux!=NULL){
        if(strcmp(aux->name,s->name)==0 && s->isFunc==aux->isFunc) return aux; 
        aux=aux->next;
    } return NULL;
}


void free_sym(sym *s) {
    if (s!=NULL) {
        free(s->name);
        free_param_list(s->param_list);
        free(s);
    }
}
/*************************************************/
void add_param(sym *s, _type type) {
    param *aux;
    if(s->param_list){
        aux=s->param_list;
        while(aux->next)  aux=aux->next;
        aux->next=create_param(type);
    }
    else s->param_list=create_param(type);
}

param* create_param(_type type) {
    param* p;
    p=(param *)malloc(sizeof(param));
    p->type=type;
    p->next=NULL;
    return p;
}
/**********************************************/
void printSymTables(void) {
    printGlobal(st_root); //print global table
    printFunctions(st_root->next); //print functions tables
}

void printGlobal(sym_table* st) { //table
    sym* s= st->sym_list;		
    printf("===== Global Symbol Table =====\n");
	while(s != NULL) {
        printf("%s\t%s", s->name, type_to_str(s->type));
        print_param_list(s);
        printf("\n");
        s=s->next;
    } printf("\n");
}

void printFunctions(sym_table* st) { //tables
    sym* s;
    if(st!=NULL){
        if(st->isDef){
            printf("===== Function %s Symbol Table =====\n", st->name);
            s=st->sym_list;	
            while(s != NULL) {
                printf("%s\t%s",s->name,type_to_str(s->type));
                if(!s->isFunc&&s->isParam){ printf("\tparam");}
                printf("\n");
                s = s->next;
            } printf("\n");
        }
        printFunctions(st->next);
    }
}

void free_param_list(param* p) {
    param *aux;
    if(p!=NULL){
        aux=p;
        while(p!=NULL){
            aux=p->next;
            free(p);
            p=aux;
        }
    }
}

void free_sym_table(sym_table* st){
    sym *s=st->sym_list,*s_aux;
    while(s!=NULL){
        s_aux=s->next;
        free_sym(s);
        s=s_aux;
    }
    free(st->name);
    free(st);
}

/**********************************************/
_type str_to_type(char* str) {
    if(strcmp(str,"Char")==0||strcmp(str,"char")==0) return charlit;
    else if(strcmp(str,"Void")==0||strcmp(str,"void")==0) return voidlit;
    else if(strcmp(str,"Int")==0||strcmp(str,"int")==0) return intlit;
    else if(strcmp(str,"Short")==0||strcmp(str,"short")==0) return shortlit;
    else if(strcmp(str,"Double")==0||strcmp(str,"double")==0) return reallit;
    else{return undef;}
}

char* type_to_str(_type t) {
    switch(t){
        case charlit: return "char";
        case intlit: return "int";
        case reallit: return "double";
        case voidlit: return "void";
        case shortlit: return "short";
        case undef: return "undef";
        default: return "undef";
    }
}
/*****************************************************/
void freeTables(void){
    sym_table *st=st_root,*st_aux;
    while(st!=NULL){
        st_aux=st->next;
        free_sym_table(st);
        st=st_aux;
    }
}
/********************************************************/
int isVarNameInSymList(char* name ,sym_table* st){
    sym* aux=st->sym_list;
    while(aux!=NULL){
        if(!(aux->isFunc)&&(strcmp(aux->name,name)==0)){ return 1;}
        aux=aux->next;
    }
    return 0;
}

/************************************************************/
void print_param_list(sym *s){
    param *param_list;
    param_list= s->param_list;
    if(param_list!=NULL){
        printf("(%s",type_to_str(param_list->type));
        param_list=param_list->next;
        while(param_list!=NULL){
            printf(",%s",type_to_str(param_list->type));
            param_list=param_list->next;
        }
        printf(")");
    }
}

int isLocalVar(char *id,char *funcName){
    return isVarNameInSymList(id,get_sym_table(funcName));
}

/*********************************************************************/

void delete_sym_from_table(char *name,sym_table *st){
    sym *aux;
    sym *bef;
    if(st->sym_list!=NULL){
        aux=st->sym_list;
        bef=aux;
        aux=aux->next;
        while(aux!=NULL){
            if(strcmp(aux->name,name)==0){
                //delete
                bef->next=aux->next;
                free_sym(aux);
                return;
            }
            bef=aux;
            aux=aux->next;
        }
    }
}

