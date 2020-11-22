//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef SYMBOL_TABLE_H 
#define SYMBOL_TABLE_H

typedef enum {intlit, charlit, reallit} sym_type;

/*Structures*/
typedef struct sym{
    char *name; // symbol name e.g. putchar, return, ...
    sym_type type;
    struct sym *next; 
    int isFunc; //to distinguish between function and variable
    int isDef; //to flag func or variable definition
}sym;

typedef struct sym_table{
    char* name; //e.g. Function main, Global, ...
    struct sym *head;
    struct sym_table *next;
}sym_table;


#endif