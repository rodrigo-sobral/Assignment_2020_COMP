//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef CODE_GENERATOR_H 
#define CODE_GENERATOR_H
#include "ast.h"
#include "symbol_table.h"
#include "semantics.h"

void generate_llvm_code(node* ast_root);
void start_gen(node* ast_root);
/***************************************************************************************************************/
void get_funcDefs_code(node *n);
void get_funcDecs_code(node *n);
void get_varDecs_code(node *n);
/******************************************************************************************/
void print_funcBody_code(node* funcBody);
void handle_statement(node* statement);
void print_params_types(node *paramList);
void print_params(node *paramList);
void alloca_params(node *paramList);
char* type_to_llvm(_type t);
void cast_llvm_type(char* got, char *expected, node *n);
void handle_funcCall(node * callNode);
void print_if(node* ifNode);
#endif