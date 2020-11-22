//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "ast.h"

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
    aux=  create_sym("putchar", intlit, 1, 1);
    add_param(aux, intlit);
    add_sym(st, aux);
    
    aux= create_sym("getchar", intlit, 1, 1);
    add_param(aux, intlit);
    add_sym(st, aux);
    
    aux= create_sym("main", intlit, 1, 1);
    add_param(aux, intlit);
    add_sym(st, aux);
    
    return st;
}
/********************************************************/
sym *create_sym(char *name,_type type, int isfunc, int isdec){
    sym *s;
    if ((s = (sym *)malloc(sizeof(sym))) == NULL) {
        fprintf(stderr, "Error allocating memory");
        exit(-1);
    }
    s->param_list=NULL;
    s->next = NULL;
    s->isDec=isdec;
    s->isFunc=isfunc;
    s->type = type;
    s->name = strdup(name);
    return s;
}

void add_sym(sym_table* st, sym* s){
    sym *aux=NULL;
    if(st->sym_list!=NULL){
        aux=st->sym_list;
        while(aux->next!=NULL) aux=aux->next;
        aux->next=s;
    }
    else{
        st->sym_list=s;
    }

}
/*************************************************/
void add_param(sym *s, _type type){
    param *aux;
    if(s->param_list!=NULL){
        aux=s->param_list;
        while(aux->next!=NULL)  aux=aux->next;
        aux->next=create_param(type);
    }
    else{
        s->param_list=create_param(type);
    }
}

param* create_param(_type type){
    param* p;
    p=(param *)malloc(sizeof(param));
    p->type=type;
    p->next=NULL;
    return p;
}
/**********************************************/
/*
sym_table* astToTable(node *ast) {
	sym_table* aux_t;

	if (ast != NULL) {
        if(strcmp(ast->str,"Program") == 0) {
            syms_table= create_sym_table();

		} else if (strcmp(ast->str,"Declaration") == 0) {

		} else if (strcmp(ast->str,"ParamList") == 0) {

		} else if (strcmp(ast->str,"FuncDeclaration") == 0) {

		} else if (strcmp(ast->str,"FuncDefinition") == 0) {

		} else if (strcmp(ast->str,"FuncBody") == 0) {
			
		} else if(strcmp(ast->str,"IntLit") == 0) {

		} else if(strcmp(ast->str,"ChrLit") == 0) {

		} else if(strcmp(ast->str,"Id") == 0) {

		} else if(strcmp(ast->str,"Call") == 0) {

		} else if(strcmp(ast->str,"Store") == 0) {

		} else if(strcmp(ast->str, "Comma") == 0) {

		} else if(strcmp(ast->str, "Not") == 0) {

		} else if(strcmp(ast->str, "Lt") == 0 || strcmp(ast->str, "Gt") == 0 || strcmp(ast->str, "Le") == 0 || strcmp(ast->str, "Ge") == 0) {

		} else if(strcmp(ast->str, "Eq") == 0 || strcmp(ast->str, "Ne") == 0) {

		} else if(strcmp(ast->str, "Or") == 0 || strcmp(ast->str, "And") == 0) {

		} else if(strcmp(ast->str, "Mul") == 0 || strcmp(ast->str, "Div") == 0 || strcmp(ast->str, "Mod") == 0) {

		} else if(strcmp(ast->str, "Plus") == 0 || strcmp(ast->str, "Minus") == 0) {

		} else if(strcmp(ast->str, "Add") == 0) {

		} else if(strcmp(ast->str, "Sub") == 0) {
			
		} else if(strcmp(ast->str, "If") == 0) {
						
		} else if(strcmp(ast->str, "Return") == 0) {
			
		} else {

		}
    }

	return NULL;
}
*/