//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

//Abstract Syntax Tree Root
node *root;

/*Functions*/
void initTree(node *n)
{
    root = n; //set tree root
}

int programIsEmpty(void){
    if(root==NULL)
        return 1;
    else
        return 0;    
}

node *createNode(char *str, token *tk)
{
    node *n;
    if ((n = (node *)malloc(sizeof(node))) == NULL)
    {
        fprintf(stderr, "Error allocating memory");
        exit(-1);
    }
    n->next = NULL;
    n->child = NULL;
    n->str = strdup(str);
    n->tk=tk; //VERIFICAR SE ESTE ASSIGNMENT FUNCIONA!
    return n;
}

node *getCopyNode(node *n){
    return createNode(n->str,n->tk);
}

node *getDeclarationNodes(node* n,node *typeSpecNode){
    //recebe o 1º nó d'uma lista ligada de declarator's
    //e recebe o nó com o typespec da declaration
    node *decAux=n;
    node *aux;
    while(decAux!=NULL){
        aux=decAux->child;
        decAux->child=getCopyNode(typeSpecNode);
        decAux->child->next=aux;

        decAux=decAux->next; //iterate no next declaration node
    }

    return n;
}

int isNullNode(node *n){
    if(n!=NULL&&(strcmp(n->str,"Null")==0))
        return 1;
    else
        return 0;
}

void addNext(node *n, node *next)
{
    if (n == NULL || next == NULL)
        return;
    node *aux = n;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    aux->next = next;
}

void addChild(node *n, node *child)
{
    if (n->child == NULL)
        n->child = child;
    else
        addNext(n->child, child);
}

void printTree(void)
{
    preOrder_(root, 0);
}

void preOrder_(node *n, int h){ //used to be static :'(
    int i;
    if (n != NULL)
    {
        for (i = 0; i < h; i++)
            printf("..");
        printf("%s\n", n->str);
        preOrder_(n->child, h + 1);
        preOrder_(n->next, h);
    }
}

void freeTree(void){
    freeTree_(root);
}

void freeTree_(node *n) //used to be static :'(
{
    if (n != NULL)
    {
        if (n->child != NULL)
            freeTree_(n->child);
        if (n->next != NULL)
            freeTree_(n->next);
        //free (heap) allocated memory
        free(n->str); //str
        free(n);
    }
}



/*create token struct*/
token* createToken(char* str, int lineNum, int colNum){
    token *tk;
    if ((tk = (token *)malloc(sizeof(token))) == NULL)
    {
        fprintf(stderr, "Error allocating memory");
        exit(-1);
    }
    if(str!=NULL){
        tk->value = strdup(str);
    }
    else{
        tk->value=NULL;
    }
    tk->lineNum=lineNum;
    tk->colNum=colNum;
    return tk;
}