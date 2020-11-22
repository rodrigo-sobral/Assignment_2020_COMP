//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209

#ifndef AST_H 
#define AST_H

/*linked lists tree structure*/
typedef struct node{
    char *str; //name
    //token *tk; //token value, lineNum,colNum
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
node *createNode(char *str);
node *getCopyNode(node *n);
node *getDeclarationNodes(node* n,node *typeSpecNode);
int isNullNode(node *n);
void addNext(node *n, node *next);
void addChild(node *n, node *child);
static void preOrder_(node *n, int h);
void printTree(void);
static void freeTree_(node *n); 
void freeTree(void);
token* createToken(char* str, int lineNum, int colNum);

#endif