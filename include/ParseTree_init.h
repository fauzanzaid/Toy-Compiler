#ifndef INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5
#define INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5

#include "ParseTree.h"


///////////
// Types //
///////////

typedef enum {
	OPERATOR_STMT_OR_DEF_LIST = 1,
	OPERATOR_DECL,
	OPERATOR_ID_LIST,
	OPERATOR_TYPE,
	OPERATOR_ASSIGN,
	OPERATOR_MATRIX_ROW_LIST,
	OPERATOR_MATRIX_NUM_LIST,
	OPERATOR_CALL,
	OPERATOR_BRANCH,
	OPERATOR_STMT_LIST,
	OPERATOR_DEF,
	OPERATOR_PARAM_LIST,
	OPERATOR_INDEX,
}Operator_type;

typedef struct ParseTree_Node_Attr
{
	int op;
	void *type;	int type_valid;
	void *inh;	int inh_valid;
	void *syn;	int syn_valid;
	void *code;	int code_valid;
} ParseTree_Node_Attr;


///////////////
// Functions //
///////////////

ParseTree_Node *prune_parse_tree(ParseTree_Node *tree_ptr);


#endif
