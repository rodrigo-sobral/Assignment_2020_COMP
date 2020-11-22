//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "ast.h"

struct sym_table* st_root; 

void semanticAnalysis(node* ast_root){
    buildSymbolTables(ast_root);
    /*recursão*/    
}

void buildSymbolTables(node* ast_root){
    node* ast_node=ast_root->child; //lista ligada de func e variaveis globais
    st_root=create_global_table();
    /*funcDefinition funcDeclaration declaration */
    while(ast_node!=NULL){
        st_root
        ast_node=ast_node->next;
    }

}

