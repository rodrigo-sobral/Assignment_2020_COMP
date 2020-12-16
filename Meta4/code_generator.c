//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"


void generate_llvm_code(node* ast_root){
    //incluir funções standard do C
    printf("declare i32 @putchar(i32 nocapture) nounwind\n");  //nocapture e nounwind para dizer q a funcao esta na biblioteca standard do c
    printf("declare i32 @getchar(void no capture) nounwind\n\n");  //para dizer q a funcao esta na biblioteca standard do c

    start_gen(ast_root);
}


void start_gen(node* ast_root){
    node* ast_node=ast_root->child; //lista ligada de func e variaveis globais
    /*GLOBAL*/
    /*funcDefinition funcDeclaration declaration */
    while(ast_node){
        if(strcmp(ast_node->str,"FuncDefinition")==0){
            get_funcDefs_code(ast_node->child); /*ast_node->child is typespec node of funcDef*/
        } else if(strcmp(ast_node->str,"FuncDeclaration")==0){
            get_funcDecs_code(ast_node->child); /*ast_node->child is typespec node of funcDec*/
        }else if(strcmp(ast_node->str,"Declaration")==0){
            get_varDecs_code(ast_node->child); /*ast_node->child is typespec node of funcDec*/
        }    
        ast_node=ast_node->next;
    }
}
/***************************************************************************************************************/
void get_funcDefs_code(node *n){ //FUNC DEFS
    //receives typedef node as n
    node *aux=n; //typedef node
    printf("define %s @%s(",type_to_llvm(str_to_type(aux->str)),aux->next->tk->value);
    aux=aux->next; //id node
    print_params(aux->next); //paramList node
    printf(")\n");
    aux=aux->next; //paramList node
    aux=aux->next; //funcBody node
    //funcBody
    printf("{");
    print_funcBody_code(aux);
    printf("}\n");
}

void get_funcDecs_code(node *n){ //FUNC DECS
    //receives typedef node as n
    node *aux=n; //typedef
    printf("declare %s @%s(",type_to_llvm(str_to_type(aux->str)),aux->next->tk->value);
    aux=aux->next; //id
    print_params_types(aux->next); //paramList node
    printf(")\n");
}

void get_varDecs_code(node *n){ //VAR DECS
//COMO DECLARAR VARIAVEIS GLOBAIS EM LLVM? :(
    node *aux=n; //typespec node
	printf("@%s = common global %s 0\n", aux->next->tk->value,type_to_llvm(str_to_type(aux->str)));
}
/******************************************************************************************/
void print_funcBody_code(node* funcBody){
    node *funcDecAndStats=funcBody->child,*aux; 
    _type t;
    // ^^lista ligada de declarations and statements do func body
    while(funcDecAndStats){
        if(strcmp(funcDecAndStats->str,"Declaration")==0){
            aux=funcDecAndStats->child; //typedef 
            
            printf("%%%s = alloca %s\n", aux->next->tk->value, type_to_llvm(str_to_type(aux->str)));
        }
        else{
            //pode ser:
            //while, if else, statlist, return 

        }

        funcDecAndStats=funcDecAndStats->next;
    }


}


void print_params_types(node *paramList){
    node *paramDec=paramList->child;
    if(paramDec!=NULL){
        printf("%s",type_to_llvm(str_to_type(paramDec->child)));
        paramDec=paramDec->next; //next paramDec
    }
    while(paramDec!=NULL){
        printf(", %s",type_to_llvm(str_to_type(paramDec->child)));
        paramDec=paramDec->next; //next paramDec
    }

    //TODO: questão para prof amanha, vale a pena omitir o void ou não?

}

void print_params(node *paramList){
    _type t;
    node *paramDec=paramList->child;
    if(paramDec!=NULL){
        t=str_to_type(paramDec->child);
        printf("%s ",type_to_llvm(t));
        if(t!=voidlit&&paramDec->child->next!=NULL){
            printf("%%%s",paramDec->child->next->tk->value);
        }
        paramDec=paramDec->next; //next paramDec
    }
    while(paramDec!=NULL){
        printf(", %s ",type_to_llvm(t));
        if(t!=voidlit&&paramDec->child->next!=NULL){
            printf("%%%s",paramDec->child->next->tk->value);
        }
        paramDec=paramDec->next; //next paramDec
    }

}

char* type_to_llvm(_type t){
    switch(t){
        case charlit: return "i32";
        case intlit: return "i32";
        case reallit: return "double";
        case voidlit: return "void";
        case shortlit: return "i32";
        default: return "";
    }
}





