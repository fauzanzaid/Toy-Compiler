#ifndef INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5
#define INCLUDE_GUARD_DC95772F90A448D9A205820533F9CED5

#include "ParseTree.h"


///////////
// Types //
///////////

typedef struct ParseTree_Node_Attr
{
	void *inh;	int inh_valid;
	void *syn;	int syn_valid;
	void *code;	int code_valid;
} ParseTree_Node_Attr;


#endif
