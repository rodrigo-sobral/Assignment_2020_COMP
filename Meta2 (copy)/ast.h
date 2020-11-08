#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

typedef struct node_type {
	char* type;
	char* token;

	struct node_type* children;
	struct node_type* siblings;
} node_type;

node_type* create_node(char*, char*);

void add_child(node_type*, node_type*);

void add_sibling(node_type*, node_type*);

int have_sibling(node_type*);

int is_null_node(node_type*);

node_type* add_typespec_to_declarations(node_type*, node_type*);

void print_ast(node_type*, int);

void clear_ast(node_type*);

#endif // AST_H_INCLUDED
