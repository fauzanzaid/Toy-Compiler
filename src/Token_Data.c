#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Token_Data.h"
#include "Token.h"

#define TOKEN_VALUE_MAX_CHAR 25

// ANSI escape codes to print to console
#define TEXT_RED	"\x1B[31m"
#define TEXT_GRN	"\x1B[32m"
#define TEXT_YLW	"\x1B[33m"
#define TEXT_BLD	"\x1B[1m"
#define TEXT_RST	"\x1B[0m"

char *token_names[] = {
	"COMMA",
	"SEMICOLON",
	"OP",
	"CL",
	"SQC",
	"SQO",
	"PLUS",
	"MINUS",
	"MUL",
	"DIV",
	"SIZE",
	"WHITESPACE",
	"COMMENT",
	"ID",
	"FUNID",
	"KW_INT",
	"KW_REAL",
	"KW_STRING",
	"KW_MATRIX",
	"KW_FUNCTION",
	"KW_END",
	"KW_IF",
	"KW_ELSE",
	"KW_ENDIF",
	"KW_READ",
	"KW_PRINT",
	"KW_MAIN",
	"NUM",
	"RNUM",
	"STR",
	"AND",
	"OR",
	"NOT",
	"LT",
	"LE",
	"GT",
	"GE",
	"ASSIGNOP",
	"EQ",
	"NE",
	"UNKOWN",
	"ERROR",
	"EOT"
};


///////////////
// Functions //
///////////////

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

// void pretty_print_token(Token *tkn_ptr){
// 	printf(TEXT_BLD "%d\t%d\t" TEXT_RST, tkn_ptr->line, tkn_ptr->column);
// 	if (tkn_ptr->type == TOKEN_COMMA) 				printf("(Comma)      ");
// 	else if (tkn_ptr->type == TOKEN_SEMICOLON)		printf("(Semicolon)  ");
// 	else if (tkn_ptr->type == TOKEN_OP)				printf("(Op)         ");
// 	else if (tkn_ptr->type == TOKEN_CL)				printf("(Cl)         ");
// 	else if (tkn_ptr->type == TOKEN_SQC)			printf("(Sqc)        ");
// 	else if (tkn_ptr->type == TOKEN_SQO)			printf("(Sqo)        ");
// 	else if (tkn_ptr->type == TOKEN_PLUS)			printf("(Plus)       ");
// 	else if (tkn_ptr->type == TOKEN_MINUS)			printf("(Minus)      ");
// 	else if (tkn_ptr->type == TOKEN_MUL)			printf("(Mul)        ");
// 	else if (tkn_ptr->type == TOKEN_DIV)			printf("(Div)        ");
// 	else if (tkn_ptr->type == TOKEN_SIZE)			printf("(Size)       ");
// 	else if (tkn_ptr->type == TOKEN_WHITESPACE)		printf("(Whitespace) ");
// 	else if (tkn_ptr->type == TOKEN_COMMENT)		printf("(Comment)    ");
// 	else if (tkn_ptr->type == TOKEN_ID)				printf("(Id)         %s", tkn_ptr->data->string);
// 	else if (tkn_ptr->type == TOKEN_FUNID)			printf("(Funid)      %s", tkn_ptr->data->string);
// 	else if (tkn_ptr->type == TOKEN_KW_INT)			printf("(Kw_int)     ");
// 	else if (tkn_ptr->type == TOKEN_KW_REAL)		printf("(Kw_real)    ");
// 	else if (tkn_ptr->type == TOKEN_KW_STRING)		printf("(Kw_string)  ");
// 	else if (tkn_ptr->type == TOKEN_KW_MATRIX)		printf("(Kw_matrix)  ");
// 	else if (tkn_ptr->type == TOKEN_KW_FUNCTION)	printf("(Kw_function)");
// 	else if (tkn_ptr->type == TOKEN_KW_END)			printf("(Kw_end)     ");
// 	else if (tkn_ptr->type == TOKEN_KW_IF)			printf("(Kw_if)      ");
// 	else if (tkn_ptr->type == TOKEN_KW_ELSE)		printf("(Kw_else)    ");
// 	else if (tkn_ptr->type == TOKEN_KW_ENDIF)		printf("(Kw_endif)   ");
// 	else if (tkn_ptr->type == TOKEN_KW_READ)		printf("(Kw_read)    ");
// 	else if (tkn_ptr->type == TOKEN_KW_PRINT)		printf("(Kw_print)   ");
// 	else if (tkn_ptr->type == TOKEN_KW_MAIN)		printf("(Kw_main)    "); 
// 	else if (tkn_ptr->type == TOKEN_NUM)			printf("(Num)        %d", tkn_ptr->data->integer);
// 	else if (tkn_ptr->type == TOKEN_RNUM)			printf("(Rnum)       %d.%d", tkn_ptr->data->integer, tkn_ptr->data->fraction);
// 	else if (tkn_ptr->type == TOKEN_STR)			printf("(Str)        %s", tkn_ptr->data->string);
// 	else if (tkn_ptr->type == TOKEN_AND)			printf("(And)        ");
// 	else if (tkn_ptr->type == TOKEN_OR)				printf("(Or)         ");
// 	else if (tkn_ptr->type == TOKEN_NOT)			printf("(Not)        ");
// 	else if (tkn_ptr->type == TOKEN_LT)				printf("(Lt)         ");
// 	else if (tkn_ptr->type == TOKEN_LE)				printf("(Le)         ");
// 	else if (tkn_ptr->type == TOKEN_GT)				printf("(Gt)         ");
// 	else if (tkn_ptr->type == TOKEN_GE)				printf("(Ge)         ");
// 	else if (tkn_ptr->type == TOKEN_ASSIGNOP)		printf("(Assignop)   ");
// 	else if (tkn_ptr->type == TOKEN_EQ)				printf("(Eq)         ");
// 	else if (tkn_ptr->type == TOKEN_NE)				printf("(Ne)         ");
// 	else if (tkn_ptr->type == TOKEN_UNKOWN)			printf("(Unkown)     ");
// 	else if (tkn_ptr->type == TOKEN_ERROR)			printf("(Error)      ");
// 	else if (tkn_ptr->type == TOKEN_EOT)			printf("(Eot)        ");
// 	else	printf("(%d)", tkn_ptr->type);
// 	printf("\n");
// }

int token_ignore(Token *tkn_ptr){
	if (tkn_ptr->type == TOKEN_WHITESPACE)	return 1;
	if (tkn_ptr->type == TOKEN_COMMENT)	return 1;
	return 0;
}

char *token_to_name(Token *tkn_ptr){
	return token_names[tkn_ptr->type];
}

void token_to_value(Token *tkn_ptr, char *buffer, int len_buffer){
	if (tkn_ptr->type == TOKEN_ID)			snprintf(buffer, len_buffer, "%s", tkn_ptr->data->string);
	else if (tkn_ptr->type == TOKEN_FUNID)	snprintf(buffer, len_buffer, "%s", tkn_ptr->data->string);
	else if (tkn_ptr->type == TOKEN_NUM)	snprintf(buffer, len_buffer, "%d", tkn_ptr->data->integer);
	else if (tkn_ptr->type == TOKEN_RNUM)	snprintf(buffer, len_buffer, "%d.%d", tkn_ptr->data->integer, tkn_ptr->data->fraction);
	else if (tkn_ptr->type == TOKEN_STR)	snprintf(buffer, len_buffer, "%s", tkn_ptr->data->string);
}

void pretty_print_token(Token *tkn_ptr){
	printf(TEXT_BLD "%4d:%-3d" TEXT_RST, tkn_ptr->line, tkn_ptr->column);
	printf(" ");

	printf(TEXT_BLD TEXT_GRN "%-11.11s" TEXT_RST, token_to_name(tkn_ptr));
	printf(" ");

	// Get value of token if it exists
	// Add characters for \0 and truncation check
	int len_buffer = TOKEN_VALUE_MAX_CHAR + 2;
	char buffer[len_buffer];
	memset(buffer, '\0', len_buffer);
	token_to_value(tkn_ptr, buffer, len_buffer);

	if(buffer[len_buffer-2] != '\0'){
		// value string truncate
		buffer[len_buffer-2] = '\0';
		printf("\"" TEXT_YLW "" "%s" TEXT_RST "...\"", buffer);
	}
	else if(buffer[0] != '\0'){
		// Print value string as is
		printf("\"" TEXT_YLW "" "%s" TEXT_RST "\"", buffer);
	}

	printf("\n");
}
