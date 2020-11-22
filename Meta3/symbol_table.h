//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef SYMBOL_TABLE_H 
#define SYMBOL_TABLE_H

typedef enum {intlit, charlit, reallit, voidlit} _type;

/*Structures*/
typedef struct sym {
    char *name; // symbol name e.g. putchar, return, ...
    _type type;
    struct param* param_list; //list head
    int isFunc; //to distinguish between function and variable
    int isDec; //to flag func or variable declaration
    struct sym *next; 
} sym;

typedef struct param{
    _type type;
    struct param *next;
}param;

typedef struct sym_table{
    char* name; //e.g. Function main, Global, ...
    struct sym *sym_list; //list head
    struct sym_table *next;
} sym_table;

sym_table* create_global_table(void);
sym_table* create_sym_table(char* name);
sym *create_sym(char *name,_type type, int isfunc, int isdec);
param* create_param(_type type);

void add_sym(sym* s, sym* snext);
void add_param(sym *s, _type type);

struct sym* astToTable(struct node*);

#endif