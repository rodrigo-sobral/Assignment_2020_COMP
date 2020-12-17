//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "code_generator.h"

int count=1; //para variáveis intermédias
char buffer[1000]; //aux
int retFlag=0;
_type funcRetType;

void generate_llvm_code(node* ast_root){
    //incluir funções standard do C
    printf("declare i32 @putchar(i32 nocapture) nounwind\n");  //nocapture e nounwind para dizer q a funcao esta na biblioteca standard do c
    //printf("declare i32 @getchar(void no capture) nounwind\n");  //para dizer q a funcao esta na biblioteca standard do c
    //TODO: 
    printf("\n");

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
    retFlag=0; //reset retFlag
    //receives typedef node as n
    node *aux=n; //typedef node
    _type t=str_to_type(aux->str);
    funcRetType=t;
    printf("define %s @%s(",type_to_llvm(t),aux->next->tk->value);
    aux=aux->next; //id node
    print_params(aux->next); //paramList node
    printf(") ");
    aux=aux->next; //paramList node
    aux=aux->next; //funcBody node
    //funcBody
    printf("{\n");

    //alocar parametros
    alloca_params(n->next->next);
    print_funcBody_code(aux);
    if(!retFlag){
        if(t!=voidlit)
            printf("\tret %s 0\n", type_to_llvm(t));
        else
            printf("\tret void\n");
        
    }
    printf("}\n\n");
    count=1; //reset counter
}

void get_funcDecs_code(node *n){ //FUNC DECS
    //receives typedef node as n
    node *aux=n; //typedef
    printf("declare %s @%s(",type_to_llvm(str_to_type(aux->str)),aux->next->tk->value);
    aux=aux->next; //id
    print_params_types(aux->next); //paramList node
    printf(")\n");

    //nota: acho q se podem omitir declarações de funções definidas..
}

void get_varDecs_code(node *n){ //VAR DECS
//COMO DECLARAR VARIAVEIS GLOBAIS EM LLVM? :(
    node *aux=n; //typespec node
    _type t=str_to_type(aux->str); //var type

    aux=aux->next;//id
    if(aux->next!=NULL){
        //VAR DEFINITION
        handle_statement(aux->next);
        printf("@%s = global %s ", aux->tk->value,type_to_llvm(t));
        printf("%s\n",aux->next->llvm_name);
        sprintf(buffer,"@%s",aux->tk->value);
        assign_llvm_name(aux, buffer);
        count=1;
    }
    else{
        //VAR DECLARATION
        printf("@%s = common global %s 0\n", aux->tk->value,type_to_llvm(t));
        sprintf(buffer,"@%s",aux->tk->value);
        assign_llvm_name(aux, buffer);
    }
}
/******************************************************************************************/
void print_funcBody_code(node* funcBody){
    //count==1 se funcao n devolver nada, count==2 se a função tiver um tipo de retorno diferente de void 
    node *funcDecAndStats=funcBody->child,*aux; 
    // ^^lista ligada de declarations and statements do func body
    _type t;
    while(funcDecAndStats){
        if(strcmp(funcDecAndStats->str,"Declaration")==0){
            aux=funcDecAndStats->child; //typedef 
            t=str_to_type(aux->str); //typedef of declared var
            aux=aux->next;//id (var name)
            printf("\t%%%s = alloca %s\n", aux->tk->value, type_to_llvm(t));
            sprintf(buffer,"%%%s",aux->tk->value);
            assign_llvm_name(aux, buffer);
            if(aux->next!=NULL){
                //atribuicao de valor à variavel declarada
                //TODO:
                //expressões...func calls...
                //traduzir as expressões todas e no final fazer o store com o valor resultante
                //...
                handle_statement(aux->next);
                //FAZER CASTING!!!!!!!!!!!
                cast_llvm_type(type_to_llvm(aux->next->type), type_to_llvm(t),aux->next);
                printf("\tstore %s ",type_to_llvm(t));
                printf("%s, ",aux->next->llvm_name);
                printf("%s* %%%s\n",type_to_llvm(t),aux->tk->value);
                //...
                
            }
        }
        else{
            //pode ser:
            //while, if else, statlist, return 
            //STATEMENTS
            handle_statement(funcDecAndStats);
        }

        funcDecAndStats=funcDecAndStats->next;
    }

}

void handle_statement(node* statement){
    int loopi;
    //node* aux=statement;
    if(statement!=NULL){
        //PLUS
        if(strcmp(statement->str,"Plus")==0){
            //1 nó filho
            handle_statement(statement->child);
            assign_llvm_name(statement, statement->child->llvm_name);
        }
        //MINUS
        else if(strcmp(statement->str,"Minus")==0){
            //1 nó filho
            handle_statement(statement->child);
            printf("\t%%%d = sub i32 0, %s\n", count, statement->child->llvm_name);
            sprintf(buffer,"%%%d",count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        //NOT
        else if(strcmp(statement->str,"Not")==0){
            //1 nó filho
            handle_statement(statement->child);
            //statement->type sempre igual a int
            printf("\t%%%d = icmp ne %s %s, 0\n",count,type_to_llvm(statement->type),statement->child->llvm_name);
            printf("\t%%%d = xor i1 %%%d, true\n", count+1, count);
	        printf("\t%%%d = zext i1 %%%d to i32\n", count+2, count+1);
            sprintf(buffer,"%%%d", count+2);
            assign_llvm_name(statement, buffer);
            count+=3;
        }
        //CALL
        else if(strcmp(statement->str,"Call")==0){
            handle_funcCall(statement);
        }
        //STORE
        else if(strcmp(statement->str,"Store")==0){
            handle_statement(statement->child->next);
            //cast if necessary
            cast_llvm_type(type_to_llvm(statement->child->next->type), type_to_llvm(statement->child->type),statement->child->next);
            printf("\tstore %s ",type_to_llvm(statement->child->type));
            printf("%s, ",statement->child->next->llvm_name);
            printf("%s* %%%s\n",type_to_llvm(statement->child->type),statement->child->tk->value);
        }
        //COMMA
        else if(strcmp(statement->str,"Comma")==0){
            //evaluates first expr and discard result, evaluates second expr and returns result
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            assign_llvm_name(statement, statement->child->next->llvm_name);
            //TODO:?
        }
        //OPERATIONS
        else if(strcmp(statement->str,"Add")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            printf("\t%%%d = add %s %s, %s\n",count,type_to_llvm(statement->type),statement->child->llvm_name,statement->child->next->llvm_name);
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Mul")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            printf("\t%%%d = mul %s %s, %s\n",count,type_to_llvm(statement->type),statement->child->llvm_name,statement->child->next->llvm_name);
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Div")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            printf("\t%%%d = div %s %s, %s\n",count,type_to_llvm(statement->type),statement->child->llvm_name,statement->child->next->llvm_name);
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Sub")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            printf("\t%%%d = sub %s %s, %s\n",count,type_to_llvm(statement->type),statement->child->llvm_name,statement->child->next->llvm_name);
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Mod")==0){
            //urem ou srem ?
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            printf("\t%%%d = srem %s %s, %s\n",count,type_to_llvm(statement->type),statement->child->llvm_name,statement->child->next->llvm_name);
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        //COMPARISONS
        else if(strcmp(statement->str,"Eq")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            if(statement->child->type==reallit||statement->child->next->type==reallit){
                cast_llvm_type(type_to_llvm(statement->child->type),"double",statement->child);
                cast_llvm_type(type_to_llvm(statement->child->next->type),"double",statement->child->next);
                printf("\t%%%d = fcmp eq %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            else{
                printf("\t%%%d = icmp eq %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Ne")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            if(statement->child->type==reallit||statement->child->next->type==reallit){
                cast_llvm_type(type_to_llvm(statement->child->type),"double",statement->child);
                cast_llvm_type(type_to_llvm(statement->child->next->type),"double",statement->child->next);
                printf("\t%%%d = fcmp ne %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            else{
                printf("\t%%%d = icmp ne %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Le")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            if(statement->child->type==reallit||statement->child->next->type==reallit){
                cast_llvm_type(type_to_llvm(statement->child->type),"double",statement->child);
                cast_llvm_type(type_to_llvm(statement->child->next->type),"double",statement->child->next);
                printf("\t%%%d = fcmp sle %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            else{
                printf("\t%%%d = icmp sle %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Ge")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            if(statement->child->type==reallit||statement->child->next->type==reallit){
                cast_llvm_type(type_to_llvm(statement->child->type),"double",statement->child);
                cast_llvm_type(type_to_llvm(statement->child->next->type),"double",statement->child->next);
                printf("\t%%%d = fcmp sge %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            else{
                printf("\t%%%d = icmp sge %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Lt")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            if(statement->child->type==reallit||statement->child->next->type==reallit){
                cast_llvm_type(type_to_llvm(statement->child->type),"double",statement->child);
                cast_llvm_type(type_to_llvm(statement->child->next->type),"double",statement->child->next);
                printf("\t%%%d = fcmp slt %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            else{
                printf("\t%%%d = icmp slt %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        else if(strcmp(statement->str,"Gt")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            if(statement->child->type==reallit||statement->child->next->type==reallit){
                cast_llvm_type(type_to_llvm(statement->child->type),"double",statement->child);
                cast_llvm_type(type_to_llvm(statement->child->next->type),"double",statement->child->next);
                printf("\t%%%d = fcmp sgt %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            else{
                printf("\t%%%d = icmp sgt %s %s, %s\n", count, type_to_llvm(statement->type),statement->child->llvm_name, statement->child->next->llvm_name);
            }
            sprintf(buffer,"%%%d", count);
            assign_llvm_name(statement, buffer);
            count++;
        }
        //
        else if(strcmp(statement->str,"BitWiseAnd")==0||strcmp(statement->str,"BitWiseOr")==0||strcmp(statement->str,"BitWiseXor")==0||strcmp(statement->str,"Or")==0||strcmp(statement->str,"And")==0){
            handle_statement(statement->child);
            handle_statement(statement->child->next);
            //TODO:
        }
        //
        else if(strcmp(statement->str,"StatList")==0){
            print_funcBody_code(statement);
        }
        //
        else if(strcmp(statement->str,"While")==0){
            printf("br lable");


            printf()
        }
        //
        else if(strcmp(statement->str,"If")==0){        
            print_if(statement);
        }
        //RETURN
        else if(strcmp(statement->str,"Return")==0){
            retFlag=1; //set retFlag
            if(strcmp(statement->child->str,"Null")==0){
                printf("\tret void\n");
            }
            else{
                //TODO: Vou ter de fazer CAST aqui tb!
                handle_statement(statement->child);
                cast_llvm_type(type_to_llvm(statement->child->type),type_to_llvm(funcRetType),statement->child);
                printf("\tret %s %s\n", type_to_llvm(funcRetType),statement->child->llvm_name);
                count=1; //reset counter
            }            
        }

        else if(isTerminal(statement)){
            if(strncmp(statement->str,"Id",2)==0){
                //se for ID
                printf("\t%%%d = load %s, %s* %%%s\n",count,type_to_llvm(statement->type),type_to_llvm(statement->type),statement->tk->value);
                sprintf(buffer,"%%%d",count);
                assign_llvm_name(statement, buffer);
                count++;
            }
            else{
                if(strncmp(statement->str,"ChrLit",6)==0){
                    //ChrLit('a')
                    sprintf(buffer,"%d",(int)statement->str[8]);
                    assign_llvm_name(statement, buffer);
                }
                else{ //intlit
                   assign_llvm_name(statement, statement->tk->value); 
                }
            }
        }

    }
    

}

void cast_llvm_type(char* got, char *expected, node* n){
    if(strcmp(got,expected)==0){
        return;
    }
    //got double expected int
    else if(strcmp(got,"double")==0&&strcmp(expected,"i32")==0){
        printf("\t%%%d = fptosi double %s to i32\n",count,n->llvm_name);
        sprintf(buffer,"%%%d", count);
        assign_llvm_name(n, buffer);
        count++;

    }
    //got int expected double
    else if(strcmp(expected,"double")==0&&strcmp(got,"i32")==0){
        printf("\t%%%d = sitofp i32 %s to double\n",count,n->llvm_name);
        sprintf(buffer,"%%%d", count);
        assign_llvm_name(n, buffer);
        count++;
    }
    else{
        //TODO: ?
    }
}

void handle_funcCall(node *callNode){
    node *aux=callNode;
    param *p_aux;
    aux=aux->child->next; //first argument node
    p_aux=callNode->child->param_list;
    while(aux!=NULL){
        handle_statement(aux);
        cast_llvm_type(type_to_llvm(aux->type),type_to_llvm(p_aux->type),aux); //CAST if necessary
        aux=aux->next;
        p_aux=p_aux->next;
    }
    aux=callNode->child->next;//back to first argument node

    printf("\tcall %s @%s(",type_to_llvm(callNode->type),callNode->child->tk->value);
    //print arguments
    if(aux!=NULL){
        printf("%s %s",type_to_llvm(aux->type),aux->llvm_name);
        aux=aux->next;
    }
    while(aux!=NULL){
        printf(", %s %s",type_to_llvm(aux->type),aux->llvm_name);
        aux=aux->next;
    }
    printf(")\n");
}

void print_if(node* ifNode){

}


void print_params_types(node *paramList){ //types only..for func declarations
    node *paramDec=paramList->child;
    if(paramDec!=NULL){
        if(str_to_type(paramDec->child->str)!=voidlit){
            printf("%s",type_to_llvm(str_to_type(paramDec->child->str)));
        }
        paramDec=paramDec->next; //next paramDec
    }
    while(paramDec!=NULL){
        printf(", %s",type_to_llvm(str_to_type(paramDec->child->str)));
        paramDec=paramDec->next; //next paramDec
    }

    //TODO: vale a pena omitir o void ou não?
}

void print_params(node *paramList){ //types and var names...for func definitions
    _type t;
    node *paramDec=paramList->child;
    if(paramDec!=NULL){
        t=str_to_type(paramDec->child->str);
        if(t!=voidlit){
            printf("%s",type_to_llvm(t));
            if(paramDec->child->next!=NULL){
                printf(" %%%s",paramDec->child->next->tk->value);
            }
        }
        paramDec=paramDec->next; //next paramDec
    }
    while(paramDec!=NULL){
        t=str_to_type(paramDec->child->str);
        printf(" , %s",type_to_llvm(t));
        if(t!=voidlit&&paramDec->child->next!=NULL){
            printf(" %%%s",paramDec->child->next->tk->value);
        }
        paramDec=paramDec->next; //next paramDec
    }

}

void alloca_params(node *paramList){
    _type t;
    node *paramDec=paramList->child;
    while(paramDec!=NULL){
        t=str_to_type(paramDec->child->str);
        if(t!=voidlit&&paramDec->child->next!=NULL){
            printf("\t%%%s = alloca %s\n",paramDec->child->next->tk->value,type_to_llvm(t));
            sprintf(buffer,"%%%s",paramDec->child->next->tk->value);
            assign_llvm_name(paramDec->child->next, buffer);
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




