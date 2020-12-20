//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef CODE_GENERATOR_H 
#define CODE_GENERATOR_H

#include "ast.h"
#include "symbol_table.h"
#include "semantics.h"
#include "symbol_table.h"

void generate_llvm_code(node* ast_root);
void start_gen(node* ast_root);
/***************************************************************************************************************/
void get_funcDefs_code(node *n);
void get_funcDecs_code(node *n);
void get_varDecs_code(node *n);
/******************************************************************************************/
void print_funcBody_code(node* funcBody,int);
void print_statList(node* statList, int);
void handle_statement(node* statement,int);
void print_params_types(node *paramList);
/*void print_params(node *paramList);*/
void alloca_params(node *paramList);
char* type_to_llvm(_type t);
void cast_llvm_type(char* got, char *expected, node *n,int);
void handle_funcCall(node * callNode,int);
void print_if(node* ifNode,int);
void print_while(node *whileNode,int printFlag);
void print_and_or_condition(node *and_or, int printFlag);
void print_2nd_op_AndOr(node *and_or, int op1,int printFlag);
int count_params(node *paramList);
int isComparison(node *n);
int get_chrlit_ascii_value(char* value);
int octal_to_int(char* str);
int power(int x, int y);
char* adapt_double_value(node *num);
void handle_Global_varDef(node* statement);
#endif