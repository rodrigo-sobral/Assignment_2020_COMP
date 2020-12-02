//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef SYMBOL_TABLE_H 
#define SYMBOL_TABLE_H
#include "symbol_table.h"
typedef enum {none,intlit, charlit, reallit, voidlit, shortlit,undef} _type;

/*Structures*/
typedef struct sym {
    char *name; // symbol name e.g. putchar, return, ...
    _type type;
    struct param* param_list; //list head
    //struct declar* declar_list; //list head
    int isFunc; //to distinguish between function and variable
    int isParam; //to flag var as parameter
    struct sym *next; 
} sym;

typedef struct param{
    _type type;
    struct param *next;
}param;

typedef struct sym_table{
    char* name; //e.g. Function main, Global, ...
    int isDef;
    struct sym *sym_list; //list head
    struct sym_table *next;
} sym_table;


//  CREATERS
sym_table* create_global_table(void);
sym_table* create_sym_table(char* name);
sym *create_sym(char *name,_type type, int isfunc, int isparam);
param* create_param(_type type);

//  GETTERS
sym_table *get_sym_table(char* name);
sym* get_sym(sym* s,sym_table* st);

//  ADDERS
void add_sym_table(sym_table *st);
void add_sym(sym_table* st, sym* s);
void add_param(sym *s, _type type);

//  CONVERTES
_type str_to_type(char* str);
char* type_to_str(_type t);

//  FREERES
void free_sym(sym *s);
void free_param_list(param* p);
void free_sym_table(sym_table* st);
void freeTables(void);

//  PRINTERS
void printFunctions(sym_table* st);
void printGlobal(sym_table* st);
void printSymTables(void);

//CHECKERs
int isVarNameInSymList(char* name ,sym_table* st);

#endif