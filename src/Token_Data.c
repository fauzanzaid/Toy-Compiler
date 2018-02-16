#include <stdio.h>
#include <stdlib.h>

#include "Token_Data.h"
#include "Token.h"

Token_Data *Token_Data_new(){
	Token_Data *tdt_ptr = malloc( sizeof(Token_Data) );
	tdt_ptr->string = NULL;
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

void pretty_print_token(Token *tkn_ptr){
	printf("%d\t%d\t", tkn_ptr->line, tkn_ptr->column);
	if (tkn_ptr->type == TOKEN_COMMA) 				printf("(Comma)      ");
	else if (tkn_ptr->type == TOKEN_SEMICOLON)		printf("(Semicolon)  ");
	else if (tkn_ptr->type == TOKEN_OP)				printf("(Op)         ");
	else if (tkn_ptr->type == TOKEN_CL)				printf("(Cl)         ");
	else if (tkn_ptr->type == TOKEN_SQC)			printf("(Sqc)        ");
	else if (tkn_ptr->type == TOKEN_SQO)			printf("(Sqo)        ");
	else if (tkn_ptr->type == TOKEN_PLUS)			printf("(Plus)       ");
	else if (tkn_ptr->type == TOKEN_MINUS)			printf("(Minus)      ");
	else if (tkn_ptr->type == TOKEN_MUL)			printf("(Mul)        ");
	else if (tkn_ptr->type == TOKEN_DIV)			printf("(Div)        ");
	else if (tkn_ptr->type == TOKEN_SIZE)			printf("(Size)       ");
	else if (tkn_ptr->type == TOKEN_WHITESPACE)		printf("(Whitespace) ");
	else if (tkn_ptr->type == TOKEN_COMMENT)		printf("(Comment)    ");
	else if (tkn_ptr->type == TOKEN_ID)				printf("(Id)         %s", tkn_ptr->data->string);
	else if (tkn_ptr->type == TOKEN_FUNID)			printf("(Funid)      %s", tkn_ptr->data->string);
	else if (tkn_ptr->type == TOKEN_KW_INT)			printf("(Kw_int)     ");
	else if (tkn_ptr->type == TOKEN_KW_REAL)		printf("(Kw_real)    ");
	else if (tkn_ptr->type == TOKEN_KW_STRING)		printf("(Kw_string)  ");
	else if (tkn_ptr->type == TOKEN_KW_MATRIX)		printf("(Kw_matrix)  ");
	else if (tkn_ptr->type == TOKEN_KW_FUNCTION)	printf("(Kw_function)");
	else if (tkn_ptr->type == TOKEN_KW_END)			printf("(Kw_end)     ");
	else if (tkn_ptr->type == TOKEN_KW_IF)			printf("(Kw_if)      ");
	else if (tkn_ptr->type == TOKEN_KW_ELSE)		printf("(Kw_else)    ");
	else if (tkn_ptr->type == TOKEN_KW_ENDIF)		printf("(Kw_endif)   ");
	else if (tkn_ptr->type == TOKEN_KW_READ)		printf("(Kw_read)    ");
	else if (tkn_ptr->type == TOKEN_KW_PRINT)		printf("(Kw_print)   ");
	else if (tkn_ptr->type == TOKEN_KW_MAIN)		printf("(Kw_main)    "); 
	else if (tkn_ptr->type == TOKEN_NUM)			printf("(Num)        %d", tkn_ptr->data->integer);
	else if (tkn_ptr->type == TOKEN_RNUM)			printf("(Rnum)       %d.%d", tkn_ptr->data->integer, tkn_ptr->data->fraction);
	else if (tkn_ptr->type == TOKEN_STR)			printf("(Str)        %s", tkn_ptr->data->string);
	else if (tkn_ptr->type == TOKEN_AND)			printf("(And)        ");
	else if (tkn_ptr->type == TOKEN_OR)				printf("(Or)         ");
	else if (tkn_ptr->type == TOKEN_NOT)			printf("(Not)        ");
	else if (tkn_ptr->type == TOKEN_LT)				printf("(Lt)         ");
	else if (tkn_ptr->type == TOKEN_LE)				printf("(Le)         ");
	else if (tkn_ptr->type == TOKEN_GT)				printf("(Gt)         ");
	else if (tkn_ptr->type == TOKEN_GE)				printf("(Ge)         ");
	else if (tkn_ptr->type == TOKEN_ASSIGNOP)		printf("(Assignop)   ");
	else if (tkn_ptr->type == TOKEN_EQ)				printf("(Eq)         ");
	else if (tkn_ptr->type == TOKEN_NE)				printf("(Ne)         ");
	else if (tkn_ptr->type == TOKEN_UNKOWN)			printf("(Unkown)     ");
	else if (tkn_ptr->type == TOKEN_ERROR)			printf("(Error)      ");
	else if (tkn_ptr->type == TOKEN_EOT)			printf("(Eot)        ");
	printf("\n");
}
