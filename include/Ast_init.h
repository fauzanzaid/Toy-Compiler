#ifndef INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5
#define INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5


#include <stdio.h>


#include "ParseTree.h"


#include "Token_Data.h"


///////////
// Types //
///////////

typedef enum {

	AST_OPERATOR_PLUS = 1,
	AST_OPERATOR_MINUS,
	AST_OPERATOR_MUL,
	AST_OPERATOR_DIV,
	AST_OPERATOR_SIZE,
	AST_OPERATOR_ID,
	AST_OPERATOR_FUNID,
	AST_OPERATOR_KW_INT,
	AST_OPERATOR_KW_REAL,
	AST_OPERATOR_KW_STRING,
	AST_OPERATOR_KW_MATRIX,
	AST_OPERATOR_KW_READ,
	AST_OPERATOR_KW_PRINT,
	AST_OPERATOR_NUM,
	AST_OPERATOR_RNUM,
	AST_OPERATOR_STR,
	AST_OPERATOR_AND,
	AST_OPERATOR_OR,
	AST_OPERATOR_NOT,
	AST_OPERATOR_LT,
	AST_OPERATOR_LE,
	AST_OPERATOR_GT,
	AST_OPERATOR_GE,
	AST_OPERATOR_ASSIGN,
	AST_OPERATOR_EQ,
	AST_OPERATOR_NE,

	AST_OPERATOR_CALL,
	AST_OPERATOR_COND,
	AST_OPERATOR_DECL,
	AST_OPERATOR_DEF,
	AST_OPERATOR_ID_LIST,
	AST_OPERATOR_MATRIX,
	AST_OPERATOR_MATRIX_ELEMENT,
	AST_OPERATOR_DEF_PARAM_LIST,
	AST_OPERATOR_CALL_PARAM_LIST,
	AST_OPERATOR_STMT_LIST,
	AST_OPERATOR_STMT_OR_DEF_LIST,

	AST_OPERATOR_UNKNOWN = -1,

}AstOperator_type;

typedef struct ParseTree_Node_Attr
{
	int op;

	void *inh;
	void *type;
	void *entry;

	void *result_value;
	void *result_entry;
	void *code;
} ParseTree_Node_Attr;


///////////////
// Constants //
///////////////

extern char *ast_operator_names[];


///////////////
// Functions //
///////////////

char *ast_operator_to_name(int op);

ParseTree_Node *prune_parse_tree(ParseTree_Node *tree_ptr);


///////////
// Print //
///////////

void print_abstract_tree_preorder(ParseTree *tree, FILE *file_ptr);


#endif
