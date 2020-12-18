//PL5_COMPILADORES@2020
//Eduardo Filipe Ferreira Cruz          2018285164
//Rodrigo Fernando Henriques Sobral     2018298209
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "ast.h"
#include "semantics.h"

struct sym_table* st_root; 

void startSemanticAnalysis(node* ast_root) {
    buildSymbolTables(ast_root);    
}

void buildSymbolTables(node* ast_root) {
    node* ast_node=ast_root->child; //lista ligada de func e variaveis globais

    st_root=create_global_table();
    /*funcDefinition funcDeclaration declaration */
    while(ast_node){
        if(strcmp(ast_node->str,"FuncDefinition")==0){
            handle_funcDefs(ast_node->child); /*ast_node->child is typespec node of funcDef*/
        } else if(strcmp(ast_node->str,"FuncDeclaration")==0){
            handle_funcDecs(ast_node->child);
        }else if(strcmp(ast_node->str,"Declaration")==0){
            handle_varDecs(ast_node->child);
        }    
        ast_node=ast_node->next;
    }
}

void handle_varDecs(node *n) {
    node *aux=n;//typedef
    _type expr_type;
    sym *s, *s_aux, *s_aux0;
    int flag=0; //
    if(str_to_type(aux->str)==voidlit) {flag=1;}
    s=create_sym(aux->next->tk->value,str_to_type(aux->str),0,0); 
    
    aux=aux->next; //id
    if(aux->next!=NULL){
        aux=aux->next; //expr
        expr_type=get_statement_type(aux,st_root);
        if(!flag) {
            if(checkConflitingTypes(s->type,expr_type)){ 
                printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", n->next->tk->lineNum,  n->next->tk->colNum,type_to_str(expr_type),type_to_str(s->type));
            }
        }
    }
    if((s_aux=get_sym(s,st_root))==NULL){
        s_aux0=create_sym(n->next->tk->value,str_to_type(n->str),1,0);
        if((s_aux=get_sym(s_aux0,st_root))==NULL){
            if(!flag)
                add_sym(st_root,s);
        }
        else{
            //TODO:
            /*
            printf("Line %d, col %d: Conflicting types (got %s", n->next->tk->lineNum,  n->next->tk->colNum,type_to_str(s->type)); 
            printf(", expected %s",type_to_str(s_aux->type));
            print_param_list(s_aux);
            printf(")\n");
            return;
            */
        }
    }
    else{ //already declared
        if(!flag&&checkConflitingTypes(s->type,s_aux->type)){
           printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", n->next->tk->lineNum,  n->next->tk->colNum,type_to_str(s->type),type_to_str(s_aux->type)); 
        }
    }
    if(flag){ printf("Line %d, col %d: Invalid use of void type in declaration\n",n->next->tk->lineNum, n->next->tk->colNum); free_sym(s);}
}

void handle_funcDecs(node* n) {
    //recebe head da lista ligada com nós:
    //typespec
    sym *funcDec, *sym_aux, *s_aux0;
    sym_table *funcDefTable;
    node* aux=n, *paramDec;
    _type retType; //func return type
    char *funcName;
    int flag=0;

    retType=str_to_type(aux->str); //get func return type from typespec node

    aux=aux->next; //id
    funcName=strdup(aux->tk->value); //id(value)
    funcDec=create_sym(funcName,retType,1,0);
  
    if((sym_aux=get_sym(funcDec,st_root))!=NULL){ //se já existir declarada
        funcDefTable=create_sym_table(funcName);
        add_sym(funcDefTable, create_sym("return", retType, 0, 0)); //return sym
        funcDefTable->isDef=0; //not defined 
        /*OBTER PARÂMETROS DOS NÓS DA DEFINIÇÃO*/
        aux=aux->next; //paramList
        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        paramDec=aux->child; //paramDec
        if(paramDec!=NULL){
            if(str_to_type(paramDec->child->str)==voidlit){
                if(paramDec->child->next!=NULL){flag= 1; printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);}
                else if(paramDec->next!=NULL){
                    flag= 1; printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);
                }
            }
            add_param(funcDec,str_to_type(paramDec->child->str)); //add paramtype to param list of funcDec symbol            
            paramDec=paramDec->next; //next paramdeclaration node
        }
        while(paramDec!=NULL){ //iterate through paramList childs
            //paramDec content (typespec-->[option: id])
            if(str_to_type(paramDec->child->str)==voidlit&&!flag) { //param type
               flag=1; printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);                
            }
            if(paramDec->child->next!=NULL){
                if(isVarNameInSymList(paramDec->child->next->tk->value,funcDefTable)){
                    printf("Line %d, col %d: Symbol %s already defined\n", paramDec->child->next->tk->lineNum, paramDec->child->next->tk->colNum, paramDec->child->next->tk->value);
                }
                else{
                    add_sym(funcDefTable,create_sym(paramDec->child->next->tk->value,str_to_type(paramDec->child->str),0,1));
                }                        
            }
            add_param(funcDec,str_to_type(paramDec->child->str)); //add paramtype to param list of funcDec symbol
            paramDec=paramDec->next;
        }

        free_sym_table(funcDefTable);

        /*COMPARE return types and PARAMETER TYPES WITH DECLARED ONES*/
        if(!flag&&(!check_params_list_types(funcDec,sym_aux)||sym_aux->type!=funcDec->type)){ 
            printf("Line %d, col %d: Conflicting types (got %s", n->next->tk->lineNum,  n->next->tk->colNum,type_to_str(funcDec->type)); 
            print_param_list(funcDec);
            printf(", expected %s",type_to_str(sym_aux->type));
            print_param_list(sym_aux);
            printf(")\n");
        }

        free_sym(funcDec);
    }
    else{
        /**************************************************************/
        /*OBTER PARÂMETROS DOS NÓS DA DEFINIÇÃO*/
        aux=aux->next; //paramList

        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        paramDec=aux->child; //paramDec
        if(paramDec!=NULL){
            if(str_to_type(paramDec->child->str)==voidlit){
                if(paramDec->child->next!=NULL){flag=1; printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);}
                else if(paramDec->next!=NULL){
                    flag= 1; printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);
                }
            }
            add_param(funcDec,str_to_type(paramDec->child->str)); //add paramtype to param list of funcDec symbol            
            paramDec=paramDec->next; //next paramdeclaration node
        }
        while(paramDec!=NULL){ //iterate through paramList childs
            //paramDec content (typespec-->[option: id])
            if(str_to_type(paramDec->child->str)==voidlit&&!flag) { //param type
                printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);   
                flag=1;             
            }
            add_param(funcDec,str_to_type(paramDec->child->str)); //add paramtype to param list of funcDec symbol
            paramDec=paramDec->next;
        }

        s_aux0=create_sym(funcName,retType,0,0);
        if((sym_aux=get_sym(s_aux0,st_root))!=NULL){
            delete_sym_from_table(sym_aux->name,st_root);
            /*
            //declared as a variable TODO:
            printf("Line %d, col %d: Conflicting types (got %s", n->next->tk->lineNum,  n->next->tk->colNum,type_to_str(funcDec->type)); 
            print_param_list(funcDec);
            printf(", expected %s",type_to_str(s_aux0->type));
            printf(")\n");
            free_sym(s_aux0);
            free_sym(funcDec);
            return;
            */
        }
    /***************************************************************************/



        //create new sym_table
        funcDefTable=create_sym_table(funcName);
        add_sym(funcDefTable, create_sym("return", retType, 0, 0)); //return sym
        funcDefTable->isDef=0; //not defined        

        /*OBTER PARÂMETROS DOS NÓS DA DEFINIÇÃO*/
        //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
        paramDec=aux->child; //paramDec
        if(paramDec!=NULL){
            if(str_to_type(paramDec->child->str)!=voidlit) {
                if(paramDec->child->next!=NULL){add_sym(funcDefTable,create_sym(paramDec->child->next->tk->value,str_to_type(paramDec->child->str),0,1));}          
            }
            paramDec=paramDec->next; //next paramdeclaration node
        }
        while(paramDec!=NULL){ //iterate through paramList childs
            //paramDec content (typespec-->[option: id])
            if(str_to_type(paramDec->child->str)!=voidlit) { //param type
                if(paramDec->child->next!=NULL){
                    if(isVarNameInSymList(paramDec->child->next->tk->value,funcDefTable)){
                        printf("Line %d, col %d: Symbol %s already defined\n", paramDec->child->next->tk->lineNum, paramDec->child->next->tk->colNum, paramDec->child->next->tk->value);
                    }
                    else{
                        add_sym(funcDefTable,create_sym(paramDec->child->next->tk->value,str_to_type(paramDec->child->str),0,1));
                    }                        
                }
            }                              
            paramDec=paramDec->next; //next paramdeclaration node
        }
        free_sym_table(funcDefTable);

        if(!flag){
            add_sym(st_root,funcDec);

            //create new sym_table
            funcDefTable=create_sym_table(funcName);
            add_sym(funcDefTable, create_sym("return", retType, 0, 0)); //return sym
            funcDefTable->isDef=0; //not defined
            add_sym_table(funcDefTable);
        }
    }
}

void handle_funcDefs(node* n) {
    //recebe head da lista ligada com nós:
    //typespec-->funcDeclarator-->funcBody
    sym *funcDef, *sym_aux, *s_aux0;
    sym_table *funcDefTable;
    node* aux=n, *paramDec;
    _type retType; //func return type
    char *funcName;
    int flag=0;

    retType=str_to_type(aux->str); //get func return type from typespec node

    aux=aux->next;//id node
    funcName=strdup(aux->tk->value); //id(value)
    funcDef=create_sym(funcName,retType,1,0);

    /*OBTER PARÂMETROS DOS NÓS DA DEFINIÇÃO*/
    aux=aux->next; //paramList
    //move to paramList childs (linked list nodes: paramdec-->paramdec-->paramdec-->(...))
    paramDec=aux->child; //paramDec
    if(paramDec!=NULL){
        if(str_to_type(paramDec->child->str)==voidlit){
            if(paramDec->child->next!=NULL){flag=1; printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);}
            else if(paramDec->next!=NULL){
                flag=1; printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);
            }       
        }
        add_param(funcDef,str_to_type(paramDec->child->str)); //add paramtype to param list of funcDec symbol            
        paramDec=paramDec->next; //next paramdeclaration node
    }
    while(paramDec!=NULL){ //iterate through paramList childs
        //paramDec content (typespec-->[option: id])
        if(str_to_type(paramDec->child->str)==voidlit&&!flag) { //param type
            printf("Line %d, col %d: Invalid use of void type in declaration\n", paramDec->child->tk->lineNum, paramDec->child->tk->colNum);   
            flag=1;             
        }
        add_param(funcDef,str_to_type(paramDec->child->str)); //add paramtype to param list of funcDec symbol
        paramDec=paramDec->next;
    }

    /*VERIFICAÇÃO SE A FUNC JÁ FOI DECLARADA ou definida*/
    if((funcDefTable=get_sym_table(funcName))!=NULL){//SE JÁ EXISTIR TABELA DE SÍMBOLOS PARA ESTA TABELA É PQ ELA JÁ FOI DECLARADA OU DEFINIDA ANTERIORMENTE

        if(!funcDefTable->isDef){ //se a função so tiver sido declarada
            sym_aux=get_sym(funcDef,st_root); 
            //if params types are not equal
            if(!flag&&(!check_params_list_types(funcDef,sym_aux)||sym_aux->type!=funcDef->type)){
                printf("Line %d, col %d: Conflicting types (got %s", n->next->tk->lineNum,  n->next->tk->colNum,type_to_str(funcDef->type)); 
                print_param_list(funcDef);
                printf(", expected %s",type_to_str(sym_aux->type));
                print_param_list(sym_aux);
                printf(")\n");
                flag=1;
                return;
            }
        }
        
        if(!flag&&funcDefTable->sym_list->next!=NULL){ //se a função já tiver sido definida
            printf("Line %d, col %d: Symbol %s already defined\n", n->next->tk->lineNum, n->next->tk->colNum, funcName);
            /*******************SÓ PARA FAZER A ANOTAÇÃO DA ÁRVORE E ANÁLISE SEMÂNTICA DO CÓDIGO DA FUNÇÃO******************/
            /********NOTE: NÃO É ADICIONADA NENHUMA TABELA À LISTA DE TABELAS****************/
            funcDefTable=create_sym_table(funcName);
            add_sym(funcDefTable, create_sym("return", retType, 0, 0)); //return sym
            return;
        }
        /*ADD PARAMETER VARIABLES SYMS TO FUNCDEF SYM_TABLE*/
        if(!flag) funcDefTable->isDef=1;//set as defined
        paramDec=aux->child; //paramDec
        if(paramDec!=NULL){
            if(paramDec->child->next!=NULL&&str_to_type(paramDec->child->str)!=voidlit){add_sym(funcDefTable,create_sym(paramDec->child->next->tk->value,str_to_type(paramDec->child->str),0,1));}        
            paramDec=paramDec->next; //next paramdeclaration node
        }
        while(paramDec!=NULL){ //iterate through paramList childs
            if(str_to_type(paramDec->child->str)!=voidlit) { //param type
                if(paramDec->child->next!=NULL){
                    if(isVarNameInSymList(paramDec->child->next->tk->value,funcDefTable)){
                        printf("Line %d, col %d: Symbol %s already defined\n", paramDec->child->next->tk->lineNum, paramDec->child->next->tk->colNum, paramDec->child->next->tk->value);
                    }
                    else{
                        add_sym(funcDefTable,create_sym(paramDec->child->next->tk->value,str_to_type(paramDec->child->str),0,1));
                    }                        
                }
            }                              
            paramDec=paramDec->next; //next paramdeclaration node
        }            
        /*************************/  
                    
        /*ENTRAR NO FUNCBODY*/
        aux=aux->next; //funcBody node
        /*FAZER ANALISE SEMANTICA PARA DECLARATIONS AND STATEMENTS DO FUNCBODY!*/
        if(!flag)
            add_funcBody_syms_to_table(funcDefTable,aux); //<-esta funcao deve fazer a analise semantica destes erros^
        /***********************************************************************************************************************/
 
        //free_sym(funcDef);
        return;
    }
    else{ //se ainda n tiver sido declarada ou definida

        s_aux0=create_sym(funcName,retType,0,0);
        if((sym_aux=get_sym(s_aux0,st_root))!=NULL){
            delete_sym_from_table(sym_aux->name,st_root); //delete var from table to be replaced with funcdefinition
            /*
            //declared as a variable TODO:
            printf("Line %d, col %d: Conflicting types (got %s", n->next->tk->lineNum,  n->next->tk->colNum,type_to_str(funcDef->type)); 
            print_param_list(funcDef);
            printf(", expected %s",type_to_str(s_aux0->type));
            printf(")\n");
            free_sym(s_aux0);
            free_sym(funcDef);
            return;
            */
        }



        if(!flag){
            add_sym(st_root,funcDef); //add to global table 
        }
        //create new sym_table
        funcDefTable=create_sym_table(funcName);
        add_sym(funcDefTable, create_sym("return", retType, 0, 0)); //return sym       
        /*ADD PARAMETER VARIABLES SYMS TO FUNCDEF SYM_TABLE*/
        paramDec=aux->child; //paramDec
        if(paramDec!=NULL){
            if(str_to_type(paramDec->child->str)!=voidlit) {
                if(paramDec->child->next!=NULL){add_sym(funcDefTable,create_sym(paramDec->child->next->tk->value,str_to_type(paramDec->child->str),0,1));}          
            }
            paramDec=paramDec->next; //next paramdeclaration node
        }
        while(paramDec!=NULL){ //iterate through paramList childs
            if(str_to_type(paramDec->child->str)!=voidlit) { //param type
                if(paramDec->child->next!=NULL){
                    if(isVarNameInSymList(paramDec->child->next->tk->value,funcDefTable)){
                        printf("Line %d, col %d: Symbol %s already defined\n", paramDec->child->next->tk->lineNum, paramDec->child->next->tk->colNum, paramDec->child->next->tk->value);
                    }
                    else{
                        add_sym(funcDefTable,create_sym(paramDec->child->next->tk->value,str_to_type(paramDec->child->str),0,1));
                    }                        
                }
            }                              
            paramDec=paramDec->next; //next paramdeclaration node
        }            
        /*************************/           
        /*ENTRAR NO FUNCBODY*/
        aux=aux->next; //funcBody node
        /*FAZER ANALISE SEMANTICA PARA DECLARATIONS AND STATEMENTS DO FUNCBODY!*/
        if(!flag){
            funcDefTable->isDef=1;//set as defined 
            add_funcBody_syms_to_table(funcDefTable,aux); //<-esta funcao deve fazer a analise semantica destes erros^
            add_sym_table(funcDefTable);
        }
    }
}

void add_funcBody_syms_to_table(sym_table* st, node* funcBodyNode) {
    node *funcDecAndStats=funcBodyNode->child; 
    node *aux;
    _type expr_type;
    sym *s;
    int flag=0;
    // ^^lista ligada de declarations and statements do func body
    while(funcDecAndStats){
        if(strcmp(funcDecAndStats->str,"Declaration")==0){
            /*verifica se o symbolo já foi declarado */
            aux=funcDecAndStats->child; //typedef 
            if(str_to_type(aux->str)==voidlit){flag=1;}           
            s=create_sym(aux->next->tk->value,str_to_type(aux->str),0,0);
            
            aux=aux->next; //id            
            if(aux->next!=NULL){
                //var definition
                aux=aux->next; //expr
                expr_type=get_statement_type(aux,st);
                if(!flag)
                    if(expr_type!=undef&&checkConflitingTypes(s->type,expr_type)){
                        printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", funcDecAndStats->child->next->tk->lineNum,  funcDecAndStats->child->next->tk->colNum, type_to_str(expr_type), type_to_str(s->type)); //TODO:                     
                    }
            }

            if(!isDeclared(s,st)){
                if(!flag)
                    add_sym(st,s); //adiciona sym à table  
            }
            else{
                printf("Line %d, col %d: Symbol %s already defined\n", funcDecAndStats->child->next->tk->lineNum, funcDecAndStats->child->next->tk->colNum, funcDecAndStats->child->next->tk->value);
            }
            if(flag){printf("Line %d, col %d: Invalid use of void type in declaration\n",funcDecAndStats->child->next->tk->lineNum,funcDecAndStats->child->next->tk->colNum); free_sym(s);}
              
        }
        else{
            expr_type=get_statement_type(funcDecAndStats, st);
        }
        funcDecAndStats=funcDecAndStats->next;
    }
}

int isDeclared(sym *s, sym_table *st) {
    //check if s is declared in st, if sym is a function and sym is before main func, return declared=1
    sym *symAux=st->sym_list;
    while(symAux!=NULL){
        if(strcmp(s->name,symAux->name)==0 && s->isFunc==symAux->isFunc) return 1;
        symAux=symAux->next;
    }
    return 0; 
}

int check_params_list_types(sym *sym_defined, sym *sym_declared) {
    param *list0=sym_defined->param_list;
    param *list1=sym_declared->param_list;

    while(list0!=NULL && list1!=NULL){
        if(list1->type!=list0->type){
            return 0; //different param types
        }  
        list0=list0->next;
        list1=list1->next;
    }
    if(list1==NULL&&list0==NULL) {
        return 1;
    }
    else{
        return 0;
    }  
}

int paramsCounter(struct param* param_list, int flag) {
    int counter=0;
    param* param_list_aux= param_list;
    while (param_list_aux) {
        if(flag){// não conta voids nos parametros
            if(param_list_aux->type!=voidlit){counter++;}
        } 
        else{ //conta voids nos parametros
            counter++;
        }
        param_list_aux= param_list_aux->next;
    }
    return counter;    
}

_type get_statement_type(node* statement, sym_table *st) {
    node *aux=statement;
    _type t_aux,t_aux0;
    if(strcmp(statement->str,"Plus")==0 || strcmp(statement->str,"Minus")==0){
        //1 nó filho
        t_aux=get_statement_type(aux->child,st);
        if(strcmp(statement->child->str,"Call")!=0 && statement->child->param_list!=NULL){
            printf("Line %d, col %d: Operator %s cannot be applied to type %s",statement->tk->lineNum,statement->tk->colNum,statement->tk->value,type_to_str(t_aux));
            print_param_list_node(statement->child);
            printf("\n");
        }
        else if(t_aux==undef||t_aux==voidlit){
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",statement->tk->lineNum,statement->tk->colNum,statement->tk->value,type_to_str(t_aux));
        }
        statement->type=t_aux;
        return t_aux;
    }
    else if(strcmp(statement->str,"Not")==0){
        //1 nó filho
        t_aux=get_statement_type(aux->child,st);
        if(strcmp(statement->child->str,"Call")!=0 && statement->child->param_list!=NULL){
            printf("Line %d, col %d: Operator %s cannot be applied to type %s",statement->tk->lineNum,statement->tk->colNum,statement->tk->value,type_to_str(t_aux));
            print_param_list_node(statement->child);
            printf("\n");
        }
        else if(t_aux==voidlit || t_aux==undef || t_aux==reallit){
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",statement->tk->lineNum,statement->tk->colNum,statement->tk->value,type_to_str(t_aux));
        }
        statement->type=intlit;
        return intlit;
    }
    else if(strcmp(statement->str,"Call")==0){
        t_aux=get_funcCall_type(statement,st);
        statement->type=t_aux;
        return t_aux;
    }
    else if(strcmp(statement->str,"Store")==0){
        t_aux=get_store_type(statement,st);
        statement->type=t_aux;
        return t_aux;
    }
    else if(strcmp(statement->str,"Comma")==0){
        //evaluates first expr and discard result, evaluates second expr and returns result
        t_aux0=get_statement_type(statement->child,st); //evaluates and discards
        t_aux=get_statement_type(statement->child->next,st); //evaluates and returns
        if(statement->child->param_list!=NULL||statement->child->next->param_list!=NULL||t_aux0==undef||t_aux==undef){
            printf("Line %d, col %d: Operator %s cannot be applied to types %s",statement->tk->lineNum,statement->tk->colNum,statement->tk->value,type_to_str(t_aux0)); 
            print_param_list_node(statement->child);
            printf(", %s",type_to_str(t_aux));
            print_param_list_node(statement->child->next);
            printf("\n");
            statement->type=undef;
            return undef;
        }
        statement->type=t_aux;
        return t_aux;
    }
    else if(strcmp(statement->str,"Add")==0||strcmp(statement->str,"Sub")==0||strcmp(statement->str,"Mul")==0||strcmp(statement->str,"Div")==0){
        t_aux=get_operation_type(statement,st);
        statement->type=t_aux;
        return t_aux;
    }
    else if(strcmp(statement->str,"Eq")==0||strcmp(statement->str,"Ne")==0||strcmp(statement->str,"Le")==0||strcmp(statement->str,"Ge")==0||strcmp(statement->str,"Lt")==0||strcmp(statement->str,"Gt")==0){
        t_aux=get_comparisons_type(statement, st);
        statement->type=t_aux;
        return t_aux;
    }
    else if(strcmp(statement->str,"BitWiseAnd")==0||strcmp(statement->str,"BitWiseOr")==0||strcmp(statement->str,"BitWiseXor")==0||strcmp(statement->str,"Mod")==0||strcmp(statement->str,"Or")==0||strcmp(statement->str,"And")==0){
        t_aux=get_bitwise_type(statement,st);
        statement->type=t_aux;
        return t_aux;
    }
    else if(strcmp(statement->str,"StatList")==0){
        add_stat_decs_syms_to_table(st, statement->child); 
        return voidlit; //doesnt matter here..
    }
    else if(strcmp(statement->str,"While")==0){
        //check confliting types in expr of while(expr)
        //TODO: incluir charlit tb..?
        t_aux=get_statement_type(statement->child,st);
        if(checkConflitingTypes(intlit,t_aux)){
            printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", statement->child->tk->lineNum, statement->child->tk->colNum,type_to_str(t_aux),type_to_str(intlit));
        }
        add_stat_decs_syms_to_table(st, statement->child->next); 
        return voidlit; //doesnt matter here..
    }
    else if(strcmp(statement->str,"If")==0){        
        t_aux=get_statement_type(statement->child,st);
        if(checkConflitingTypes(intlit,t_aux)){
            printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", statement->child->tk->lineNum, statement->child->tk->colNum,type_to_str(t_aux),type_to_str(intlit));
        }
        add_stat_decs_syms_to_table(st, statement->child->next); 
        return voidlit; //doesnt matter here..
    }
    else if(strcmp(statement->str,"Return")==0){
        //verificar se tipo coincide q se espera returnar, coincide
        t_aux=get_statement_type(statement->child,st);
        if(strcmp(statement->child->str,"Call")!=0 && statement->child->param_list!=NULL){
            printf("Line %d, col %d: Conflicting types (got %s", statement->child->tk->lineNum, statement->child->tk->colNum,type_to_str(t_aux)); 
            print_param_list_node(statement->child);
            printf(", expected %s",type_to_str(st->sym_list->type));
            printf(")\n");
        }
        else if(checkConflitingTypes(st->sym_list->type,t_aux)){
            if(strcmp(statement->child->str,"Null")==0){
                printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", statement->tk->lineNum, statement->tk->colNum, type_to_str(t_aux), type_to_str(st->sym_list->type));
            }
            else{
                printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", statement->child->tk->lineNum, statement->child->tk->colNum, type_to_str(t_aux), type_to_str(st->sym_list->type));
            }
        }
        return st->sym_list->type; //doesnt matter here..
    }
        else if(strcmp(statement->str,"Null")==0){
        return voidlit;
    }
    else if(isTerminal(statement)){
        //intlit ou reallit ou undef ou id..
        return getTerminalType(statement,st);
    }
    else{
        return undef;
    }
}

void add_stat_decs_syms_to_table(sym_table* st, node* stats_decs) {
    //receives statment ou declaration as node stats_decs
    node *stat_dec=stats_decs; 
    node *aux;
    _type expr_type;
    sym *s;
    int flag=0;
    // ^^lista ligada de declarations and statements do func body
    while(stat_dec!=NULL){
        if(strcmp(stat_dec->str,"Declaration")==0){
            /*verifica se o symbolo já foi declarado */
            aux=stat_dec->child; //typedef 
            if(str_to_type(aux->str)==voidlit){flag=1;}           
            s=create_sym(aux->next->tk->value,str_to_type(aux->str),0,0); 
                       
            aux=aux->next; //id
    
            if(aux->next!=NULL){
                //var definition
                aux=aux->next; //expr
                expr_type=get_statement_type(aux,st);
                if(!flag)
                    if(expr_type!=undef&&checkConflitingTypes(s->type,expr_type)){
                        printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", stat_dec->child->next->tk->lineNum,  stat_dec->child->next->tk->colNum, type_to_str(expr_type), type_to_str(s->type)); //TODO:
                    }
            }
            if(!isDeclared(s,st)){
                if(!flag)
                    add_sym(st,s); //adiciona sym à table  
            }
            else{
                printf("Line %d, col %d: Symbol %s already defined\n", stat_dec->child->next->tk->lineNum, stat_dec->child->next->tk->colNum, stat_dec->child->next->tk->value);
            }
            if(flag){ printf("Line %d, col %d: Invalid use of void type in declaration\n",stat_dec->child->next->tk->lineNum,stat_dec->child->next->tk->colNum); free_sym(s);}            
        }
        else{
            expr_type=get_statement_type(stat_dec, st);
        }
        stat_dec=stat_dec->next;
    }
}

_type get_operation_type(node * operation,sym_table *st){
    node *n_aux=operation->child;
    _type type0, type1; //operation only has 2 nodes
    type0=get_statement_type(n_aux,st);
    type1=get_statement_type(n_aux->next,st);
    if(n_aux->param_list!=NULL||n_aux->next->param_list!=NULL){
        //se algum dos nós for o ids de uma função
        printf("Line %d, col %d: Operator %s cannot be applied to types %s",operation->tk->lineNum,operation->tk->colNum,operation->tk->value,type_to_str(type0)); 
        print_param_list_node(n_aux);
        printf(", %s",type_to_str(type1));
        print_param_list_node(n_aux->next);
        printf("\n");
        operation->type=undef;
        return undef;
    }
    else if(type0==undef||type1==undef){
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",operation->tk->lineNum,operation->tk->colNum,operation->tk->value,type_to_str(type0),type_to_str(type1));
        operation->type=undef;
        return undef;
    }
    else if(type0==voidlit||type1==voidlit){
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",operation->tk->lineNum,operation->tk->colNum,operation->tk->value,type_to_str(type0),type_to_str(type1));
        operation->type=undef;
        return undef;
    }    
    else if(type0==reallit||type1==reallit){
        operation->type=reallit;
        return reallit;
    }
    else if(type0==shortlit&&type1==shortlit){
        operation->type=shortlit;
        return shortlit;
    }
    else if(type0==shortlit&&type1==charlit){
        operation->type=shortlit;
        return shortlit;
    }
    else if(type1==shortlit&&type0==charlit){
        operation->type=shortlit;
        return shortlit;
    }
    else if(type0==charlit&&type1==charlit){
        operation->type=charlit;
        return charlit;
    }
    else{
        operation->type=intlit;
        return intlit;
    }
}

_type get_bitwise_type(node *operation, sym_table *st){
    node *n_aux=operation->child;
    _type type0, type1; //operation only has 2 nodes
    type0=get_statement_type(n_aux,st);
    type1=get_statement_type(n_aux->next,st);
    if(n_aux->param_list!=NULL||n_aux->next->param_list!=NULL){
        //se algum dos nós for o ids de uma função
        printf("Line %d, col %d: Operator %s cannot be applied to types %s",operation->tk->lineNum,operation->tk->colNum,operation->tk->value,type_to_str(type0)); 
        print_param_list_node(n_aux);
        printf(", %s",type_to_str(type1));
        print_param_list_node(n_aux->next);
        printf("\n");
    }
    else if(type0==reallit||type1==reallit||type0==voidlit||type1==voidlit||type0==undef||type1==undef){
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",operation->tk->lineNum,operation->tk->colNum,operation->tk->value,type_to_str(type0),type_to_str(type1));
    }
    operation->type=intlit;
    return intlit;
}

_type get_comparisons_type(node *operation, sym_table *st){
    node *n_aux=operation->child;
    _type type0, type1; //comparison only has 2 nodes
    type0=get_statement_type(n_aux,st);
    type1=get_statement_type(n_aux->next,st);
    if(n_aux->param_list!=NULL||n_aux->next->param_list!=NULL){
        //se algum dos nós for o ids de uma função
        printf("Line %d, col %d: Operator %s cannot be applied to types %s",operation->tk->lineNum,operation->tk->colNum,operation->tk->value,type_to_str(type0)); 
        print_param_list_node(n_aux);
        printf(", %s",type_to_str(type1));
        print_param_list_node(n_aux->next);
        printf("\n");
    }
    else if(type0==voidlit||type1==voidlit||(type0==undef&&type1!=undef)||(type0!=undef&&type1==undef)){
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",operation->tk->lineNum,operation->tk->colNum,operation->tk->value,type_to_str(type0),type_to_str(type1));
    }
    operation->type=intlit;
    return intlit;
}

_type get_store_type(node *store, sym_table*st) {
    node *n_aux= store->child; //store variable node (Id) 
    sym *s_aux, *storedSym; 
    _type expr_type;
    int flag=0;

    if(strncmp(store->child->str,"Id",2)!=0){
        printf("Line %d, col %d: Lvalue required\n",store->child->tk->lineNum,store->child->tk->colNum);
        store->child->type=get_statement_type(store->child, st);
        store->child->next->type=get_statement_type(store->child->next, st);//expr node        
        return store->child->type;
    }
 
    /*verificar se variável está declarada*/
    s_aux= create_sym(n_aux->tk->value, undef, 0, 0);
    storedSym= get_sym(s_aux, st);
    if(storedSym==NULL){
        storedSym=get_sym(s_aux,st_root);
        if(storedSym==NULL){
            free(s_aux);
            s_aux= create_sym(n_aux->tk->value, undef, 1, 0);
            storedSym= get_sym(s_aux, st_root);
            if(storedSym==NULL){
                //DONE: THROW ERROR VARIÁVEL NAO ESTÀ DECLARADA 
                printf("Line %d, col %d: Unknown symbol %s\n",n_aux->tk->lineNum, n_aux->tk->colNum , n_aux->tk->value);
                flag=1; 
            }
            else{
                //declared as function
                store->child->type=storedSym->type;
                expr_type=get_statement_type(n_aux->next, st);
                store->child->next->type=expr_type;//expr node
                get_statement_type(n_aux, st_root);

                printf("Line %d, col %d: Operator %s cannot be applied to types %s",store->tk->lineNum,store->tk->colNum,store->tk->value,type_to_str(store->child->type)); 
                print_param_list_node(n_aux);
                printf(", %s",type_to_str(expr_type));
                print_param_list_node(n_aux->next);
                printf("\n");
                free(s_aux);
                return undef;
            }
            
        }
    }
    free(s_aux);

    if(flag) store->child->type=undef; //var node
    else store->child->type=storedSym->type; //var node

    expr_type=get_statement_type(n_aux->next, st);
    store->child->next->type=expr_type;//expr node

    if(store->child->next->param_list!=NULL){
        printf("Line %d, col %d: Operator %s cannot be applied to types %s",store->tk->lineNum,store->tk->colNum,store->tk->value,type_to_str(store->child->type)); 
        print_param_list_node(n_aux);
        printf(", %s",type_to_str(expr_type));
        print_param_list_node(n_aux->next);
        printf("\n");
    }
    else if(checkConflitingTypes(store->child->type,expr_type)||store->child->type==undef){
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",store->tk->lineNum,store->tk->colNum,store->tk->value,type_to_str(store->child->type),type_to_str(expr_type));
    }

    return store->child->type;


}

_type get_funcCall_type(node *call,sym_table*st) {
    node *n_aux;
    sym * s_aux, *funcSym;
    param *p_aux0,*p_aux1; //da func call
    _type t_aux;
    int count=0,i;
    n_aux=call->child; //function name node->str = Id(name)
    s_aux=create_sym(n_aux->tk->value,undef,1,0);
    funcSym=get_sym(s_aux,st_root);
    n_aux=n_aux->next; //1º argumento da func
    int flag=0;

    //calcular tipos dos parametros dos argumentos da funccall e add à lista de param do sym_auxiliar
    //if(n_aux==NULL){add_param(s_aux,voidlit);} //se a func n tiver argumentos..add param type void
    while(n_aux!=NULL){
        t_aux=get_statement_type(n_aux,st); 
        add_param(s_aux,t_aux);
        n_aux->type=t_aux;       
        n_aux=n_aux->next; //next funccall arg
    }
    p_aux1=s_aux->param_list; //lista com tipos de parametros da func call 

    if(funcSym!=NULL){//se a funcao estiver declarada
        //verificar tipos dos parametros
        p_aux0=funcSym->param_list; //lista com tipos de parametros da funcao declarada no global
        while(p_aux0&&p_aux1){
            /*get node line and col*/
            count++;
            n_aux=call; n_aux=n_aux->child; //1º nome da funcao
            for(i=1;i<=count;i++) n_aux=n_aux->next; 
            /************************/
            if(p_aux0->type!=voidlit)
                if(checkConflitingTypes(p_aux0->type,p_aux1->type)){
                    printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", n_aux->tk->lineNum, n_aux->tk->colNum, type_to_str(p_aux1->type), type_to_str(p_aux0->type));
                }       
            p_aux1=p_aux1->next;
            p_aux0=p_aux0->next;
        }
        p_aux1=s_aux->param_list;
        p_aux0=funcSym->param_list;
        if(paramsCounter(p_aux0,1) != paramsCounter(p_aux1,0)){
            //DONE:  THROW ERROR nr de parametros da function call diferente do nr de parametros declarados para esssa funcção
            printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n", call->child->tk->lineNum, call->child->tk->colNum, funcSym->name, paramsCounter(p_aux1,0), paramsCounter(p_aux0,1));
        }
        
        free_sym(s_aux);
        call->child->param_list=funcSym->param_list;
        call->child->type=funcSym->type;
        return funcSym->type;
    } else{
        //DONE: THROW ERROR função chamada não está declarada!
        s_aux->isFunc=0; //set as variable
        funcSym=get_sym(s_aux,st);
        if(funcSym!=NULL){ //declared as variable (not as function)
            flag=1;
            //printf("Line %d, col %d: Symbol %s is not a function\n", call->tk->lineNum, call->tk->colNum, call->tk->value);
            if(paramsCounter(p_aux1,0)!=0)
                printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n", call->child->tk->lineNum, call->child->tk->colNum, s_aux->name, paramsCounter(p_aux1,0), 0);
        }
        else{
            printf("Line %d, col %d: Unknown symbol %s\n", call->tk->lineNum, call->tk->colNum, call->tk->value);
            if(paramsCounter(p_aux1,0)!=0)
                printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n", call->child->tk->lineNum, call->child->tk->colNum, s_aux->name, paramsCounter(p_aux1,0), 0);
        }
        free_sym(s_aux);
        /*anotate func args types*/
        n_aux=call->child->next; //first arg
        while(n_aux){
            get_statement_type(n_aux,st);    //anotate tree   
            n_aux=n_aux->next; //next funccall arg 
        }
        /*************************/
        if(flag){
            call->child->type=funcSym->type;
            return funcSym->type;
        }
        else{
            call->child->type=undef; //func undef sem parametros
            return undef; //call undef
        }
    }
}

_type getTerminalType(node *n,sym_table *st) {
    sym *aux0,*aux1;
    if(strncmp(n->str,"Id",2)==0){
        aux1=create_sym(n->tk->value,undef,0,0);
        if((aux0=get_sym(aux1,st))==NULL){ //if not in local table, search in global table
            if((aux0=get_sym(aux1, st_root))==NULL) {
                //if variable not in global table
                //search for function with equal variable name
                free_sym(aux1);
                aux1=create_sym(n->tk->value,undef,1,0); //as function
                if((aux0=get_sym(aux1,st_root))==NULL){
                    //not in global table as a variable or as a function...
                    printf("Line %d, col %d: Unknown symbol %s\n", n->tk->lineNum, n->tk->colNum, n->tk->value);
                    n->type=undef;
                    free_sym(aux1);
                    return undef;
                }
                else{
                    n->type=aux0->type;
                    n->param_list=aux0->param_list;
                    free_sym(aux1);
                    return aux0->type;
                }
            } 
            else{
                n->type=aux0->type; free_sym(aux1); return aux0->type;
            }
        }
        else {
            n->type=aux0->type;
            free_sym(aux1);
            return aux0->type;
        }
    } else if(strncmp(n->str,"ChrLit",6)==0){
        //return charlit;
        n->type=intlit;
        return intlit;
    } else if(strncmp(n->str,"IntLit",6)==0){
        n->type=intlit;
        return intlit;
    } else if(strncmp(n->str,"RealLit",7)==0){
        n->type=reallit;
        return reallit;
    }
    else{
        return undef;
    }
}

int isTerminal(node *n) {
    if(strncmp(n->str,"Id",2)==0){
        return 1;
    } 
    else if(strncmp(n->str,"ChrLit",6)==0){
        return 1;
    } 
    else if(strncmp(n->str,"IntLit",6)==0){
        return 1;
    } 
    else if(strncmp(n->str,"RealLit",7)==0){
        return 1;
    }
    else{
        return 0;
    }
}

int isBeforeMainFunc(sym *s){
    sym *aux=st_root->sym_list;
    while(aux!=NULL){
        if(strcmp(s->name,aux->name)==0&&aux->isFunc==s->isFunc){return 1;}
        else if(strcmp("main",aux->name)==0&&aux->isFunc){return 0;}
        aux=aux->next;
    }
    return 1;
}

int checkConflitingTypes(_type expectedType,_type gotType){
    _type aux0,aux1;
    if(expectedType==charlit || expectedType==shortlit) { aux0=intlit; } 
    else{aux0=expectedType;}
    if(gotType==charlit || gotType==shortlit) { aux1=intlit; }
    else{aux1=gotType;}
    if(aux0==aux1){
        return 0; //no conflict
    } else if(aux0==reallit && aux1==intlit){
        return 0; //no conflict
    } else{        
        return 1;
    }
}