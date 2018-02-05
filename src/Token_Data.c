#include <stdlib.h>

#include "Token_Data.h"
#include "Token.h"

Token_Data *Token_Data_new(){
	Token_Data *tdt_ptr = malloc( sizeof(Token_Data) );
	tdt_ptr->string == NULL;
	tdt_ptr->len_string = 0;
	tdt_ptr->integer = 0;
	tdt_ptr->fraction = 0;
}

void Token_Data_destroy(Token_Data *tdt_ptr){
	if(tdt_ptr->string != NULL){
		free(tdt_ptr->string);
	}
	free(tdt_ptr);
}
