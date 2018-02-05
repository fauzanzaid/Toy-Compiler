#ifndef INCLUDE_GUARD_87FBD8CD2A8147A894EC087904C9BCF0
#define INCLUDE_GUARD_87FBD8CD2A8147A894EC087904C9BCF0

#include "Token.h"


///////////
// Types //
///////////

typedef enum {
	TOKEN_COMMA,
	TOKEN_SEMICOLON,
	TOKEN_OP,
	TOKEN_CL,
	TOKEN_SQC,
	TOKEN_SQO,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_SIZE,
	
	TOKEN_WHITESPACE,
	TOKEN_COMMENT,

	TOKEN_ID,
	TOKEN_FUNID,
	
	TOKEN_KW_INT,
	TOKEN_KW_REAL,
	TOKEN_KW_STRING,
	TOKEN_KW_MATRIX,
	TOKEN_KW_FUNCTION,
	TOKEN_KW_END,
	TOKEN_KW_IF,
	TOKEN_KW_ELSE,
	TOKEN_KW_ENDIF,
	TOKEN_KW_READ,
	TOKEN_KW_PRINT,
	TOKEN_KW_MAIN,
	
	TOKEN_NUM,
	TOKEN_RNUM,
	TOKEN_STR,
	
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_NOT,
	
	TOKEN_LT,
	TOKEN_LE,
	TOKEN_GT,
	TOKEN_GE,
	TOKEN_ASSIGNOP,
	TOKEN_EQ,
	TOKEN_NE,

	TOKEN_UNKOWN,
	TOKEN_ERROR,
	TOKEN_EOT
} Token_type;


/////////////////////
// Data Structures //
/////////////////////

typedef struct Token_Data{
	//TOKEN_ID
	//TOKEN_FUNID		
	char *string;
	int len_string;
		
	//TOKEN_NUM
	//TOKEN_RNUM
	//TOKEN_STR
	int integer;
	int fraction;
} Token_Data;


///////////////
// Functions //
///////////////

Token_Data *Token_Data_new();
void Token_Data_destroy(Token_Data *tdt_ptr);

void pretty_print_token(Token *tkn_ptr);


#endif
