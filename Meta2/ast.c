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

void listStatements(node* funcBody){
    /*percorrer a lista de filhos do nó funcBody (q apenas contém statements e/ou declarations)
     e caso encontre grupos de statements (>=2) criar nós statList e atualizar lista ligada de filhos de funcBody*/
    int count = 0,flag=0;
    node *aux=funcBody->child; //nó inicial da lista ligada de filhos de funcBody 
    node *dec=NULL; //para guardar ultimo nó Declaration
    node *statList;
    if(aux!=NULL){
        if(strcmp(aux->str,"Declaration")!=0){ //se for diferente de Declaration
            count++;
            //caso em q o 1º filho de funcBody é um nó Statement
            flag=1;
        }
        else{
            dec=aux;
        }
        while(aux->next != NULL){
            
            if(strcmp(aux->next->str,"Declaration")!=0){ //se for diferente a Declaration
                count++;                
            }
            else{
                if(count>=2){
                    statList=createNode("StatList");                    
                    if(flag){ //para o caso do 1º filho de funcBody ser nó Statement
                        statList->child=(funcBody->child);
                        funcBody->child=statList;
                        flag=0;
                    }
                    else{ 
                        statList->child=(dec->next);
                        dec->next=statList;
                    }                    
                    statList->next=aux->next;
                    aux->next=NULL;
                    aux=statList;                    
                }
                flag=0;
                count=0;
                dec=aux->next;
            } 
            aux = aux->next;            
        }

        if(count>=2){    //para o caso de qnd o último nó de funcBody é um Statement             
            statList=createNode("StatList");
            statList->child=(dec->next);
            dec->next=statList;
            statList->next=NULL;                  
        }
    }


}