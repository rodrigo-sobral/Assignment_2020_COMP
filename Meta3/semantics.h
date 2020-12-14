//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef SEMANTICS_H 
#define SEMANTICS_H
#include "symbol_table.h"

//  STARTES
void startSemanticAnalysis(node* ast_root);
void buildSymbolTables(node* ast_root);

//  HANDLERS
void handle_varDecs(node *n);
void handle_funcDecs(node* n);
void handle_funcDefs(node* n);

//  ADDERS
void add_funcBody_syms_to_table(sym_table* st, node* funcBodyNode);
void add_stat_decs_syms_to_table(sym_table* st, node* stats_decs);

//  CHECKERS
int isDeclared(sym *s, sym_table *st);
int check_params_list_types(sym *s0, sym *s1);
int isTerminal(node *n);
int isBeforeMainFunc(sym *s);
int checkConflitingTypes(_type expectedType,_type gotType);

//  COUNTERS
int paramsCounter(struct param* param_list, int flag);

//  GETTERS
_type get_statement_type(node* statement, sym_table *st);
_type get_operation_type(node * operation,sym_table *st);
_type get_comparisons_type(node * operation,sym_table *st);
_type get_store_type(node *store, sym_table*st);
_type get_funcCall_type(node *call,sym_table*st);
_type getTerminalType(node *n,sym_table *st);
_type get_bitwise_type(node *operation, sym_table *st);
#endif