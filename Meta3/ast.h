//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef AST_H 
#define AST_H
#include "symbol_table.h"
/*linked lists tree structure*/
typedef struct node{
    char *str; //name
    struct token *tk; //tk has-> value, lineNum,colNum
    /*for tree notation*/
    _type type;
    struct param* param_list; //list head
    /**/
    struct node *next;
    struct node *child; 
} node;

/*token structure*/
typedef struct token{
    char* value;
    int lineNum;
    int colNum;
}token;

/*Function Declaration*/
void initTree(node* n);
int programIsEmpty(void);
node *createNode(char *str, token *tk);
node *getCopyNode(node *n);
node *getDeclarationNodes(node* n,node *typeSpecNode);
int isNullNode(node *n);
void addNext(node *n, node *next);
void addChild(node *n, node *child);
void preOrder_(node *n, int h,int anotate);
void printTree(int anotate);
void freeTree_(node *n); 
void freeTree(void);
token* createToken(char* str, int lineNum, int colNum);
void add_param_to_node(node *n, _type type);
#endif