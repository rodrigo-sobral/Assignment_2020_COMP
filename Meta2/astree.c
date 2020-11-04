//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include "astree.h"

//Abstract Syntax Tree Root
static node *root; 

/*Functions*/
void initTree(node* n){
    root = n; //set tree root
}

node* createNode(char* str){
    node* n;
    if ((n = (node *)malloc(sizeof(node))) == NULL){
        fprintf(stderr,"Error allocating memory");
        exit(-1);
    }
    n->next = NULL;
    n->child = NULL;
    n->str = strdup(str);
    return n;
}

void addNext(node* n, node* next){
    if(n==NULL||next==NULL)
        return;
    node *aux = n;
    while(aux->next!=NULL){
        aux = aux->next;
    }
    aux->next = next;
}

void addChild(node* n, node* child){
    if(n->child==NULL)
        n->child = child;
    else
        addNext(n->child,child);
}

void printTree(void){
    preOrder_(root,1);
}

static void preOrder_(node* n, int h){ 
    int i;
    if (n != NULL){
        for (i = 0; i < h;i++)
            printf(".."); 
        printf("%d ", n->str);
        preOrder_(n->child,h+1);
        preOrder_(n->next,h);
    }    
}

void freeAll(void){
    freeAll_(root);
}

static void freeAll_(node* n){
    if (n != NULL){
        if(n->child!=NULL)
            freeAll_(n->child);
        if(n->next!=NULL)
            freeAll_(n->next);
        //free (heap) allocated memory
        free(n->str); //str
        free(n);
    }
}
