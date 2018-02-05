#include "Lexer_init.h"
#include "Lexer.h"
#include "Token.h"
#include "Token_Data.h"
#include "Dfa_init.h"

///////////////
// Functions //
///////////////


void success_evaluate_function(Token *tkn_ptr, int state, char *string, int len_string){
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
		tkn_ptr->type = TOKEN_UNKOWN;
		// 
	}

	else if(state == STATE_F_ID){
		tkn_ptr->type = TOKEN_ID;
		// 
	}

	else if(state == STATE_F_FUNID_OR_KW){
		tkn_ptr->type = TOKEN_UNKOWN;
		// 
	}

	else if(state == STATE_F_NUM){
		tkn_ptr->type = TOKEN_NUM;
		// 
	}

	else if(state == STATE_F_RNUM){
		tkn_ptr->type = TOKEN_RNUM;
		// 
	}

	else if(state == STATE_F_STR){
		tkn_ptr->type = TOKEN_STR;
		// 
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
}

void error_evaluate_function(Token *tkn_ptr){
	tkn_ptr->type = TOKEN_ERROR;
}
