#include <string.h>
#include <stdlib.h>

#include "Lexer.h"
#include "Token.h"
#include "HashTable.h"
#include "Token_Data.h"
#include "Lexer_init.h"
#include "Dfa_init.h"


////////////
// Arrays //
////////////

char *error_evaluate_error_strings[] = {
	/*STATE_N_START*/			"Unexpected character, not in language",
	/*STATE_F_COMMA*/			"",
	/*STATE_F_SEMICOLON*/		"",
	/*STATE_F_OP*/				"",
	/*STATE_F_CL*/				"",
	/*STATE_F_SQO*/				"",
	/*STATE_F_SQC*/				"",
	/*STATE_F_PLUS*/			"",
	/*STATE_F_MINUS*/			"",
	/*STATE_F_MUL*/				"",
	/*STATE_F_DIV*/				"",
	/*STATE_F_SIZE*/			"",
	/*STATE_F_WHITESPACE*/		"",
	/*STATE_N_A_COMMENT*/		"",
	/*STATE_F_COMMENT*/			"",
	/*STATE_F_ID_OR_KW*/		"Expected identifier or keyword",
	/*STATE_F_ID*/				"Expected identifier",
	/*STATE_N_A_FUNID_OR_KW*/	"Expected function identifier or keyword",
	/*STATE_F_FUNID_OR_KW*/		"Expected function identifier or keyword",
	/*STATE_F_NUM*/				"",
	/*STATE_N_A_RNUM*/			"Expected real literal",
	/*STATE_N_B_RNUM*/			"Expected real literal",
	/*STATE_F_RNUM*/			"",
	/*STATE_N_A_STR*/			"Expected string literal",
	/*STATE_N_B_STR*/			"Expected string literal",
	/*STATE_F_STR*/				"",
	/*STATE_N_LOGICAL*/			"Expected logical operator",
	/*STATE_N_A_AND*/			"Expected .and.",
	/*STATE_N_B_AND*/			"Expected .and.",
	/*STATE_N_C_AND*/			"Expected .and.",
	/*STATE_F_AND*/				"",
	/*STATE_N_A_OR*/			"Expected .or.",
	/*STATE_N_B_OR*/			"Expected .or.",
	/*STATE_F_OR*/				"",
	/*STATE_N_A_NOT*/			"Expected .not.",
	/*STATE_N_B_NOT*/			"Expected .not.",
	/*STATE_N_C_NOT*/			"Expected .not.",
	/*STATE_F_NOT*/				"",
	/*STATE_F_LT*/				"",
	/*STATE_F_LE*/				"",
	/*STATE_F_GT*/				"",
	/*STATE_F_GE*/				"",
	/*STATE_F_ASSIGNOP*/		"",
	/*STATE_F_EQ*/				"",
	/*STATE_N_A_NE*/			"Expected =/=",
	/*STATE_F_NE*/				"",
	/*STATE_F_EOT*/				"",
};

char *success_evaluate_error_strings[] = {
	"Identifier is longer than 20 characters",
	"String literal is longer than 20 characters",
};


///////////////
// Constants //
///////////////

int KEYWORD_INT_VALUE = TOKEN_KW_INT;
int KEYWORD_REAL_VALUE = TOKEN_KW_REAL;
int KEYWORD_STRING_VALUE = TOKEN_KW_STRING;
int KEYWORD_MATRIX_VALUE = TOKEN_KW_MATRIX;
int KEYWORD_FUNCTION_VALUE = TOKEN_KW_FUNCTION;
int KEYWORD_END_VALUE = TOKEN_KW_END;
int KEYWORD_IF_VALUE = TOKEN_KW_IF;
int KEYWORD_ELSE_VALUE = TOKEN_KW_ELSE;
int KEYWORD_ENDIF_VALUE = TOKEN_KW_ENDIF;
int KEYWORD_READ_VALUE = TOKEN_KW_READ;
int KEYWORD_PRINT_VALUE = TOKEN_KW_PRINT;
int KEYWORD_MAIN_VALUE = TOKEN_KW_MAIN;

char KEYWORD_INT_KEY[] = "int";
char KEYWORD_REAL_KEY[] = "real";
char KEYWORD_STRING_KEY[] = "string";
char KEYWORD_MATRIX_KEY[] = "matrix";
char KEYWORD_FUNCTION_KEY[] = "function";
char KEYWORD_END_KEY[] = "end";
char KEYWORD_IF_KEY[] = "if";
char KEYWORD_ELSE_KEY[] = "else";
char KEYWORD_ENDIF_KEY[] = "endif";
char KEYWORD_READ_KEY[] = "read";
char KEYWORD_PRINT_KEY[] = "print";
char KEYWORD_MAIN_KEY[] = "_main";


///////////////
// Hashtable //
///////////////

HashTable *keyword_table;

static int string_hash_function(void *ptr){
	char *string = (char *)ptr;
	int hash = 5381;
	int c;
	while(c = *string++)
		hash = ((hash << 5) + hash) + c;

	if(hash<0)
		return -hash;
	else
		return hash;
}

static int string_key_compare(void *ptr_a, void *ptr_b){
	return strcmp((char *)ptr_a, (char *)ptr_b);
}

void keyword_table_init(){
	keyword_table = HashTable_new(13, string_hash_function, string_key_compare);

	HashTable_add(keyword_table, (void *)&KEYWORD_INT_KEY, (void *)&KEYWORD_INT_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_REAL_KEY, (void *)&KEYWORD_REAL_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_STRING_KEY, (void *)&KEYWORD_STRING_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_MATRIX_KEY, (void *)&KEYWORD_MATRIX_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_FUNCTION_KEY, (void *)&KEYWORD_FUNCTION_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_END_KEY, (void *)&KEYWORD_END_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_IF_KEY, (void *)&KEYWORD_IF_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_ELSE_KEY, (void *)&KEYWORD_ELSE_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_ENDIF_KEY, (void *)&KEYWORD_ENDIF_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_READ_KEY, (void *)&KEYWORD_READ_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_PRINT_KEY, (void *)&KEYWORD_PRINT_VALUE);
	HashTable_add(keyword_table, (void *)&KEYWORD_MAIN_KEY, (void *)&KEYWORD_MAIN_VALUE);
}




///////////////
// Functions //
///////////////


char *success_evaluate_function(Token *tkn_ptr, int state, char *string, int len_string){
	if(state == STATE_F_COMMA){
		tkn_ptr->type = TOKEN_COMMA;
	}

	else if(state == STATE_F_SEMICOLON){
		tkn_ptr->type = TOKEN_SEMICOLON;
	}

	else if(state == STATE_F_OP){
		tkn_ptr->type = TOKEN_OP;
	}

	else if(state == STATE_F_CL){
		tkn_ptr->type = TOKEN_CL;
	}

	else if(state == STATE_F_SQO){
		tkn_ptr->type = TOKEN_SQO;
	}

	else if(state == STATE_F_SQC){
		tkn_ptr->type = TOKEN_SQC;
	}

	else if(state == STATE_F_PLUS){
		tkn_ptr->type = TOKEN_PLUS;
	}

	else if(state == STATE_F_MINUS){
		tkn_ptr->type = TOKEN_MINUS;
	}

	else if(state == STATE_F_MUL){
		tkn_ptr->type = TOKEN_MUL;
	}

	else if(state == STATE_F_DIV){
		tkn_ptr->type = TOKEN_DIV;
	}

	else if(state == STATE_F_SIZE){
		tkn_ptr->type = TOKEN_SIZE;
	}

	else if(state == STATE_F_WHITESPACE){
		tkn_ptr->type = TOKEN_WHITESPACE;
	}

	else if(state == STATE_F_COMMENT){
		tkn_ptr->type = TOKEN_COMMENT;
	}

	else if(state == STATE_F_ID_OR_KW){
		tkn_ptr->data->len_string = len_string;

		tkn_ptr->data->string = malloc( sizeof(char) * (1 + len_string) );
		memcpy(tkn_ptr->data->string, string, len_string);
		tkn_ptr->data->string[len_string] = '\0';

		Token_type *tkn_type_ptr = HashTable_get(keyword_table, tkn_ptr->data->string);

		if(tkn_type_ptr == NULL){
			// Enforce ID length limit
			if(len_string > 20){
				tkn_ptr->type = TOKEN_ERROR;
				return success_evaluate_error_strings[1];
			}
			else
				tkn_ptr->type = TOKEN_ID;
		}

		else{
			tkn_ptr->type = *tkn_type_ptr;
		}
	}

	else if(state == STATE_F_ID){
		tkn_ptr->data->len_string = len_string;

		tkn_ptr->data->string = malloc( sizeof(char) * (1 + len_string) );
		memcpy(tkn_ptr->data->string, string, len_string);
		tkn_ptr->data->string[len_string] = '\0';

		if(len_string > 20){
			tkn_ptr->type = TOKEN_ERROR;
			return success_evaluate_error_strings[0];
		}
		else
			tkn_ptr->type = TOKEN_ID;
	}

	else if(state == STATE_F_FUNID_OR_KW){
		tkn_ptr->data->len_string = len_string;

		tkn_ptr->data->string = malloc( sizeof(char) * (1 + len_string) );
		memcpy(tkn_ptr->data->string, string, len_string);
		tkn_ptr->data->string[len_string] = '\0';


		Token_type *tkn_type_ptr = HashTable_get(keyword_table, tkn_ptr->data->string);
		if(tkn_type_ptr == NULL)
			tkn_ptr->type = TOKEN_FUNID;
		else
			tkn_ptr->type = *tkn_type_ptr;
	}

	else if(state == STATE_F_NUM){
		tkn_ptr->type = TOKEN_NUM;

		char tmp[1+len_string];
		memcpy(tmp, string, len_string);
		tmp[len_string] = '\0';

		tkn_ptr->data->integer = atoi(tmp);
	}

	else if(state == STATE_F_RNUM){
		tkn_ptr->type = TOKEN_RNUM;

		char tmp[1+len_string];
		memcpy(tmp, string, len_string);
		tmp[len_string] = '\0';

		tkn_ptr->data->integer = atoi(tmp);
		tkn_ptr->data->fraction = atoi(&tmp[len_string - 2]);
	}

	else if(state == STATE_F_STR){
		int len_literal = len_string-2;	// Remove the two "

		tkn_ptr->data->string = malloc( sizeof(char) * (1 + len_literal) );
		memcpy(tkn_ptr->data->string, string+1, len_literal);
		tkn_ptr->data->string[len_literal] = '\0';

		// Enforce STR length limit
		if(len_literal>20){
			tkn_ptr->type = TOKEN_ERROR;
			return success_evaluate_error_strings[1];
		}
		else
			tkn_ptr->type = TOKEN_STR;
	}

	else if(state == STATE_F_AND){
		tkn_ptr->type = TOKEN_AND;
	}

	else if(state == STATE_F_OR){
		tkn_ptr->type = TOKEN_OR;
	}

	else if(state == STATE_F_NOT){
		tkn_ptr->type = TOKEN_NOT;
	}

	else if(state == STATE_F_LT){
		tkn_ptr->type = TOKEN_LT;
	}

	else if(state == STATE_F_LE){
		tkn_ptr->type = TOKEN_LE;
	}

	else if(state == STATE_F_GT){
		tkn_ptr->type = TOKEN_GT;
	}

	else if(state == STATE_F_GE){
		tkn_ptr->type = TOKEN_GE;
	}

	else if(state == STATE_F_ASSIGNOP){
		tkn_ptr->type = TOKEN_ASSIGNOP;
	}

	else if(state == STATE_F_EQ){
		tkn_ptr->type = TOKEN_EQ;
	}

	else if(state == STATE_F_NE){
		tkn_ptr->type = TOKEN_NE;
	}

	else if(state == STATE_F_EOT){
		tkn_ptr->type = TOKEN_EOT;
	}

	else{
		tkn_ptr->type = TOKEN_UNKOWN;
	}

	return NULL;
}

char *error_evaluate_function(Token *tkn_ptr, int state, char *string, int len_string){
	tkn_ptr->type = TOKEN_ERROR;

	tkn_ptr->data->len_string = len_string;

	tkn_ptr->data->string = malloc( sizeof(char) * (1 + len_string) );
	memcpy(tkn_ptr->data->string, string, len_string);
	tkn_ptr->data->string[len_string] = '\0';

	return error_evaluate_error_strings[state];
}
