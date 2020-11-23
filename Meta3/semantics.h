//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef SEMANTICS_H 
#define SEMANTICS_H
#include "symbol_table.h"
void startSemanticAnalysis(node* ast_root);
void buildSymbolTables(node* ast_root);
void handle_varDecs(node *n);
void handle_funcDefs(node* n);
void handle_funcDecs(node* n);
void add_funcBody_syms_to_table(sym_table* st, node* funcBodyNode);
int isDeclared(sym *s, sym_table *st);
int check_params_list_types(sym *s0, sym *s1);

#endif