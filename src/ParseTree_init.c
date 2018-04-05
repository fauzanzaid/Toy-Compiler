#include <stdlib.h>

#include "ParseTree_init.h"


ParseTree_Node_Attr *ParseTree_Node_Attr_new(){
	ParseTree_Node_Attr *atr_ptr = malloc( sizeof(ParseTree_Node_Attr) );

	atr_ptr->inh = NULL;	atr_ptr->inh_valid = 0;
	atr_ptr->syn = NULL;	atr_ptr->syn_valid = 0;
	atr_ptr->code = NULL;	atr_ptr->code_valid = 0;

	return atr_ptr;
}

void ParseTree_Node_Attr_destroy(ParseTree_Node_Attr *atr_ptr){
	free(atr_ptr);
}
