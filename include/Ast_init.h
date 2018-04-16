#ifndef INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5
#define INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5

#include "ParseTree.h"


#include "Token_Data.h"


///////////
// Types //
///////////

typedef enum {

	OPERATOR_PLUS = 1,
	OPERATOR_MINUS,
	OPERATOR_MUL,
	OPERATOR_DIV,
	OPERATOR_SIZE,
	OPERATOR_ID,
	OPERATOR_FUNID,
	OPERATOR_KW_INT,
	OPERATOR_KW_REAL,
	OPERATOR_KW_STRING,
	OPERATOR_KW_MATRIX,
	OPERATOR_KW_READ,
	OPERATOR_KW_PRINT,
	OPERATOR_NUM,
	OPERATOR_RNUM,
	OPERATOR_STR,
	OPERATOR_AND,
	OPERATOR_OR,
	OPERATOR_NOT,
	OPERATOR_LT,
	OPERATOR_LE,
	OPERATOR_GT,
	OPERATOR_GE,
	OPERATOR_ASSIGN,
	OPERATOR_EQ,
	OPERATOR_NE,

	OPERATOR_CALL,
	OPERATOR_COND,
	OPERATOR_DECL,
	OPERATOR_DEF,
	OPERATOR_ID_LIST,
	OPERATOR_MATRIX,
	OPERATOR_MATRIX_ELEMENT,
	OPERATOR_DEF_PARAM_LIST,
	OPERATOR_CALL_PARAM_LIST,
	OPERATOR_STMT_LIST,
	OPERATOR_STMT_OR_DEF_LIST,

	OPERATOR_UNKNOWN = -1,

}Operator_type;

typedef struct ParseTree_Node_Attr
{
	int op;
	void *inh;
	void *type;
	void *entry;
	void *code;
} ParseTree_Node_Attr;


///////////////
// Constants //
///////////////

extern char *operator_names[];


///////////////
// Functions //
///////////////

char *operator_to_name(int op);

ParseTree_Node *prune_parse_tree(ParseTree_Node *tree_ptr);


///////////
// Print //
///////////

void print_abstract_tree_preorder(ParseTree *tree, FILE *file_ptr);


#endif
