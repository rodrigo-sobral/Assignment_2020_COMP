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
    node *aux=n;
    node *aux1=aux->next;
    node *d=createNode("Declaration");
    node *declaration=d;
    addChild(declaration,getCopyNode(typeSpecNode));
    aux->next=NULL;
    addChild(declaration,aux);
    while(aux1!=NULL){
        declaration=createNode("Declaration");
        addChild(declaration,getCopyNode(typeSpecNode));
        aux=aux1;
        aux1->next=NULL;
        addChild(declaration,aux);
        addNext(d,declaration);
        aux1=aux->next;
    }
    return d;
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

node* listStatements(node *n){
    //recebe o nó inicial da lista ligada com nós statements
    //função agrupa statements em StatList e devolve ponteiro para nó StatList (se o nr de nós statements for >=2)
    int count=0;
    node *aux=n;
    node *statList=createNode("StatList");
    while(aux!=NULL){
        count++;
        aux=aux->next;
    }

    if(count>=2){
        addChild(statList,n);
        return statList;
    }
    else{
        return n;
    }
}