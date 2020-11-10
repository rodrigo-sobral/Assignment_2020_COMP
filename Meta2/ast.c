//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

//Abstract Syntax Tree Root
static node *root;

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

node *createNode(char *str)
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
    return n;
}

node *getCopyNode(node *n){
    return createNode(n->str);
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

static void preOrder_(node *n, int h)
{
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

void freeAll(void)
{
    freeAll_(root);
}

static void freeAll_(node *n)
{
    if (n != NULL)
    {
        if (n->child != NULL)
            freeAll_(n->child);
        if (n->next != NULL)
            freeAll_(n->next);
        //free (heap) allocated memory
        free(n->str); //str
        free(n);
    }
}
