#include <stdio.h>

#include "Parser_init.h"
#include "ParserLL1.h"
#include "Token_Data.h"

int terminal_symbols[] = {
	SYMBOL_T_COMMA,
	SYMBOL_T_SEMICOLON,
	SYMBOL_T_OP,
	SYMBOL_T_CL,
	SYMBOL_T_SQC,
	SYMBOL_T_SQO,
	SYMBOL_T_PLUS,
	SYMBOL_T_MINUS,
	SYMBOL_T_MUL,
	SYMBOL_T_DIV,
	SYMBOL_T_SIZE,
	SYMBOL_T_WHITESPACE,
	SYMBOL_T_COMMENT,
	SYMBOL_T_ID,
	SYMBOL_T_FUNID,
	SYMBOL_T_KW_INT,
	SYMBOL_T_KW_REAL,
	SYMBOL_T_KW_STRING,
	SYMBOL_T_KW_MATRIX,
	SYMBOL_T_KW_FUNCTION,
	SYMBOL_T_KW_END,
	SYMBOL_T_KW_IF,
	SYMBOL_T_KW_ELSE,
	SYMBOL_T_KW_ENDIF,
	SYMBOL_T_KW_READ,
	SYMBOL_T_KW_PRINT,
	SYMBOL_T_KW_MAIN,
	SYMBOL_T_NUM,
	SYMBOL_T_RNUM,
	SYMBOL_T_STR,
	SYMBOL_T_AND,
	SYMBOL_T_OR,
	SYMBOL_T_NOT,
	SYMBOL_T_LT,
	SYMBOL_T_LE,
	SYMBOL_T_GT,
	SYMBOL_T_GE,
	SYMBOL_T_ASSIGNOP,
	SYMBOL_T_EQ,
	SYMBOL_T_NE,
	SYMBOL_T_ENDOFINPUT
};
int len_terminal_symbols = 41;

int variable_symbols[] = {
	SYMBOL_V_EPSILON,
	SYMBOL_V_BOOL_OP,
	SYMBOL_V_CALL,
	SYMBOL_V_DEF,
	SYMBOL_V_EXPR_ADDEND,
	SYMBOL_V_EXPR_ARITH,
	SYMBOL_V_EXPR_BOOL,
	SYMBOL_V_EXPR_FACTOR,
	SYMBOL_V_EXPR_LOG,
	SYMBOL_V_EXPR_MULTIPLIER,
	SYMBOL_V_EXPR_TERM,
	SYMBOL_V_ID_LIST,
	SYMBOL_V_ID_LIST_REM,
	SYMBOL_V_LOG_OP_BIN,
	SYMBOL_V_LVAL,
	SYMBOL_V_MAIN_FUNCTION,
	SYMBOL_V_MATRIX_LITERAL,
	SYMBOL_V_MATRIX_NUM_LIST,
	SYMBOL_V_MATRIX_NUM_LIST_REM,
	SYMBOL_V_MATRIX_ROW_LIST,
	SYMBOL_V_MATRIX_ROW_LIST_REM,
	SYMBOL_V_NUM_VAL_WM,
	SYMBOL_V_NUM_VAL_WM_REM,
	SYMBOL_V_NUM_VAL_WOM,
	SYMBOL_V_PARAM_LIST,
	SYMBOL_V_PARAM_LIST_REM,
	SYMBOL_V_STMT,
	SYMBOL_V_STMT_ASSIGN,
	SYMBOL_V_STMT_COND,
	SYMBOL_V_STMT_COND_REM,
	SYMBOL_V_STMT_DECL,
	SYMBOL_V_STMT_LIST,
	SYMBOL_V_STMT_LIST_REM,
	SYMBOL_V_STMT_OR_DEF,
	SYMBOL_V_STMT_OR_DEF_LIST,
	SYMBOL_V_STMT_OR_DEF_LIST_REM,
	SYMBOL_V_STMT_PRINT,
	SYMBOL_V_STMT_READ,
	SYMBOL_V_TYPE,
};
int len_variable_symbols = 39;

int start_symbol = SYMBOL_V_MAIN_FUNCTION;
int empty_symbol = SYMBOL_V_EPSILON;
int end_symbol = SYMBOL_T_ENDOFINPUT;

char *symbol_names[] = {
	"comma",
	"semicolon",
	"op",
	"cl",
	"sqc",
	"sqo",
	"plus",
	"minus",
	"mul",
	"div",
	"size",
	"whitespace",
	"comment",
	"id",
	"funid",
	"kw_int",
	"kw_real",
	"kw_string",
	"kw_matrix",
	"kw_function",
	"kw_end",
	"kw_if",
	"kw_else",
	"kw_endif",
	"kw_read",
	"kw_print",
	"kw_main",
	"num",
	"rnum",
	"str",
	"and",
	"or",
	"not",
	"lt",
	"le",
	"gt",
	"ge",
	"assignop",
	"eq",
	"ne",
	"endofinput",
	"epsilon",
	"bool_op",
	"call",
	"def",
	"expr_addend",
	"expr_arith",
	"expr_bool",
	"expr_factor",
	"expr_log",
	"expr_multiplier",
	"expr_term",
	"id_list",
	"id_list_rem",
	"log_op_bin",
	"lval",
	"main_function",
	"matrix_literal",
	"matrix_num_list",
	"matrix_num_list_rem",
	"matrix_row_list",
	"matrix_row_list_rem",
	"num_val_wm",
	"num_val_wm_rem",
	"num_val_wom",
	"param_list",
	"param_list_rem",
	"stmt",
	"stmt_assign",
	"stmt_cond",
	"stmt_cond_rem",
	"stmt_decl",
	"stmt_list",
	"stmt_list_rem",
	"stmt_or_def",
	"stmt_or_def_list",
	"stmt_or_def_list_rem",
	"stmt_print",
	"stmt_read",
	"type",
	"unknown"
};

char *symbol_strings[] = {
	",",
	";",
	"(",
	")",
	"]",
	"[",
	"+",
	"-",
	"*",
	"/",
	"@",
	"whitespace",
	"comment",
	"id",
	"funid",
	"int",
	"real",
	"string",
	"matrix",
	"function",
	"end",
	"if",
	"else",
	"endif",
	"read",
	"print",
	"_main",
	"int literal",
	"real literal",
	"string literal",
	".and.",
	".or.",
	".not.",
	"<",
	"<=",
	">",
	">=",
	"=",
	"==",
	"=/=",
	"end of input",
	"unknown"
};


///////////////
// Functions //
///////////////

void Parser_init_add_rules(ParserLL1 *psr_ptr){

	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MAIN_FUNCTION,			(int[]) {SYMBOL_T_KW_MAIN  ,  SYMBOL_T_SQO  ,  SYMBOL_T_SQC  ,  SYMBOL_V_STMT_OR_DEF_LIST  ,  SYMBOL_T_KW_END} , 5 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_OR_DEF_LIST,		(int[]) {SYMBOL_V_STMT_OR_DEF  ,  SYMBOL_V_STMT_OR_DEF_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_OR_DEF_LIST_REM,	(int[]) {SYMBOL_V_STMT_OR_DEF  ,  SYMBOL_V_STMT_OR_DEF_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_OR_DEF_LIST_REM,	(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_OR_DEF,			(int[]) {SYMBOL_V_STMT} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_OR_DEF,			(int[]) {SYMBOL_V_DEF} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_DECL} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_ASSIGN} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_COND} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_READ} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_PRINT} , 1 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_DECL,				(int[]) {SYMBOL_V_TYPE  ,  SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM  ,  SYMBOL_T_SEMICOLON} , 4 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_ID_LIST,				(int[]) {SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_ID_LIST,				(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_ID_LIST_REM,			(int[]) {SYMBOL_T_COMMA  ,  SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_ID_LIST_REM,			(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_INT} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_REAL} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_STRING} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_MATRIX} , 1 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_ASSIGN,			(int[]) {SYMBOL_V_LVAL  ,  SYMBOL_T_ASSIGNOP  ,  SYMBOL_V_EXPR_ARITH  ,  SYMBOL_T_SEMICOLON} , 4 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_LVAL,					(int[]) {SYMBOL_T_ID} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_LVAL,					(int[]) {SYMBOL_T_SQO  ,  SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM  ,  SYMBOL_T_SQC} , 4 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_ARITH,			(int[]) {SYMBOL_V_EXPR_TERM  ,  SYMBOL_V_EXPR_ADDEND} ,2  );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_ADDEND,			(int[]) {SYMBOL_T_PLUS  ,  SYMBOL_V_EXPR_TERM  ,  SYMBOL_V_EXPR_ADDEND} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_ADDEND,			(int[]) {SYMBOL_T_MINUS  ,  SYMBOL_V_EXPR_TERM  ,  SYMBOL_V_EXPR_ADDEND} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_ADDEND,			(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_TERM,				(int[]) {SYMBOL_V_EXPR_FACTOR  ,  SYMBOL_V_EXPR_MULTIPLIER} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_MULTIPLIER,		(int[]) {SYMBOL_T_MUL  ,  SYMBOL_V_EXPR_FACTOR  ,  SYMBOL_V_EXPR_MULTIPLIER} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_MULTIPLIER,		(int[]) {SYMBOL_T_DIV  ,  SYMBOL_V_EXPR_FACTOR  ,  SYMBOL_V_EXPR_MULTIPLIER} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_MULTIPLIER,		(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_T_OP  ,  SYMBOL_V_EXPR_ARITH  ,  SYMBOL_T_CL} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_V_NUM_VAL_WM} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_T_STR} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_T_SIZE  ,  SYMBOL_T_ID} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_V_MATRIX_LITERAL} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_V_CALL} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MATRIX_LITERAL,		(int[]) {SYMBOL_T_SQO  ,  SYMBOL_V_MATRIX_ROW_LIST  ,  SYMBOL_T_SQC} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MATRIX_ROW_LIST,		(int[]) {SYMBOL_V_MATRIX_NUM_LIST  ,  SYMBOL_V_MATRIX_ROW_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MATRIX_ROW_LIST_REM,	(int[]) {SYMBOL_T_SEMICOLON  ,  SYMBOL_V_MATRIX_ROW_LIST} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MATRIX_ROW_LIST_REM,	(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MATRIX_NUM_LIST,		(int[]) {SYMBOL_T_NUM  ,  SYMBOL_V_MATRIX_NUM_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MATRIX_NUM_LIST_REM,	(int[]) {SYMBOL_T_COMMA  ,  SYMBOL_V_MATRIX_NUM_LIST} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_MATRIX_NUM_LIST_REM,	(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_CALL,					(int[]) {SYMBOL_T_FUNID  ,  SYMBOL_T_OP  ,  SYMBOL_V_ID_LIST  ,  SYMBOL_T_CL} , 4 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_COND,				(int[]) {SYMBOL_T_KW_IF  ,  SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL  ,  SYMBOL_V_STMT_LIST  ,  SYMBOL_V_STMT_COND_REM} , 6 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_COND_REM,			(int[]) {SYMBOL_T_KW_ENDIF} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_COND_REM,			(int[]) {SYMBOL_T_KW_ELSE  ,  SYMBOL_V_STMT_LIST  ,  SYMBOL_T_KW_ENDIF} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_LIST,				(int[]) {SYMBOL_V_STMT  ,  SYMBOL_V_STMT_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_LIST_REM,			(int[]) {SYMBOL_V_STMT  ,  SYMBOL_V_STMT_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_LIST_REM,			(int[]) {SYMBOL_V_EPSILON} , 1 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_READ,				(int[]) {SYMBOL_T_KW_READ  ,  SYMBOL_T_OP  ,  SYMBOL_V_NUM_VAL_WOM  ,  SYMBOL_T_CL  ,  SYMBOL_T_SEMICOLON} , 5 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_PRINT,			(int[]) {SYMBOL_T_KW_PRINT  ,  SYMBOL_T_OP  ,  SYMBOL_T_ID  ,  SYMBOL_T_CL  ,  SYMBOL_T_SEMICOLON} , 5 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_LOG,				(int[]) {SYMBOL_V_EXPR_BOOL} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_LOG,				(int[]) {SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL  ,  SYMBOL_V_LOG_OP_BIN  ,  SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL} , 7 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_LOG,				(int[]) {SYMBOL_T_NOT  ,  SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL} , 4 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_LOG_OP_BIN,			(int[]) {SYMBOL_T_AND} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_LOG_OP_BIN,			(int[]) {SYMBOL_T_OR} , 1 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_EXPR_BOOL,				(int[]) {SYMBOL_V_NUM_VAL_WM  ,  SYMBOL_V_BOOL_OP  ,  SYMBOL_V_NUM_VAL_WM} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_LT} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_LE} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_GT} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_GE} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_EQ} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_NE} , 1 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_DEF,					(int[]) {SYMBOL_T_KW_FUNCTION  ,  SYMBOL_T_SQO  ,  SYMBOL_V_PARAM_LIST  ,  SYMBOL_T_SQC  ,  SYMBOL_T_ASSIGNOP  ,  SYMBOL_T_FUNID  ,  SYMBOL_T_SQO  ,  SYMBOL_V_PARAM_LIST  ,  SYMBOL_T_SQC  ,  SYMBOL_V_STMT_OR_DEF_LIST  ,  SYMBOL_T_KW_END} , 11 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_PARAM_LIST,			(int[]) {SYMBOL_V_TYPE  ,  SYMBOL_T_ID  ,  SYMBOL_V_PARAM_LIST_REM} , 3 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_PARAM_LIST,			(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_PARAM_LIST_REM,		(int[]) {SYMBOL_T_COMMA  ,  SYMBOL_V_TYPE  ,  SYMBOL_T_ID  ,  SYMBOL_V_PARAM_LIST_REM} , 4 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_PARAM_LIST_REM,		(int[]) {SYMBOL_V_EPSILON} , 1 );


	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WM,			(int[]) {SYMBOL_T_NUM} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WM,			(int[]) {SYMBOL_T_RNUM} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WM,			(int[]) {SYMBOL_T_ID  ,  SYMBOL_V_NUM_VAL_WM_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WM_REM,		(int[]) {SYMBOL_T_SQO  ,  SYMBOL_T_NUM  ,  SYMBOL_T_COMMA  ,  SYMBOL_T_NUM  ,  SYMBOL_T_SQC} , 5 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WM_REM,		(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WOM,			(int[]) {SYMBOL_T_NUM} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WOM,			(int[]) {SYMBOL_T_RNUM} , 1 );
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_NUM_VAL_WOM,			(int[]) {SYMBOL_T_ID} , 1 );


}

int token_to_symbol(Token *tkn_ptr){
	if(tkn_ptr->type == TOKEN_EOT)
		return SYMBOL_T_ENDOFINPUT;

	if(tkn_ptr->type == TOKEN_COMMA)
		return SYMBOL_T_COMMA;

	if(tkn_ptr->type == TOKEN_SEMICOLON)
		return SYMBOL_T_SEMICOLON;

	if(tkn_ptr->type == TOKEN_OP)
		return SYMBOL_T_OP;

	if(tkn_ptr->type == TOKEN_CL)
		return SYMBOL_T_CL;

	if(tkn_ptr->type == TOKEN_SQC)
		return SYMBOL_T_SQC;

	if(tkn_ptr->type == TOKEN_SQO)
		return SYMBOL_T_SQO;

	if(tkn_ptr->type == TOKEN_PLUS)
		return SYMBOL_T_PLUS;

	if(tkn_ptr->type == TOKEN_MINUS)
		return SYMBOL_T_MINUS;

	if(tkn_ptr->type == TOKEN_MUL)
		return SYMBOL_T_MUL;

	if(tkn_ptr->type == TOKEN_DIV)
		return SYMBOL_T_DIV;

	if(tkn_ptr->type == TOKEN_SIZE)
		return SYMBOL_T_SIZE;

	if(tkn_ptr->type == TOKEN_WHITESPACE)
		return SYMBOL_T_WHITESPACE;

	if(tkn_ptr->type == TOKEN_COMMENT)
		return SYMBOL_T_COMMENT;

	if(tkn_ptr->type == TOKEN_ID)
		return SYMBOL_T_ID;

	if(tkn_ptr->type == TOKEN_FUNID)
		return SYMBOL_T_FUNID;

	if(tkn_ptr->type == TOKEN_KW_INT)
		return SYMBOL_T_KW_INT;

	if(tkn_ptr->type == TOKEN_KW_REAL)
		return SYMBOL_T_KW_REAL;

	if(tkn_ptr->type == TOKEN_KW_STRING)
		return SYMBOL_T_KW_STRING;

	if(tkn_ptr->type == TOKEN_KW_MATRIX)
		return SYMBOL_T_KW_MATRIX;

	if(tkn_ptr->type == TOKEN_KW_FUNCTION)
		return SYMBOL_T_KW_FUNCTION;

	if(tkn_ptr->type == TOKEN_KW_END)
		return SYMBOL_T_KW_END;

	if(tkn_ptr->type == TOKEN_KW_IF)
		return SYMBOL_T_KW_IF;

	if(tkn_ptr->type == TOKEN_KW_ELSE)
		return SYMBOL_T_KW_ELSE;

	if(tkn_ptr->type == TOKEN_KW_ENDIF)
		return SYMBOL_T_KW_ENDIF;

	if(tkn_ptr->type == TOKEN_KW_READ)
		return SYMBOL_T_KW_READ;

	if(tkn_ptr->type == TOKEN_KW_PRINT)
		return SYMBOL_T_KW_PRINT;

	if(tkn_ptr->type == TOKEN_KW_MAIN)
		return SYMBOL_T_KW_MAIN;

	if(tkn_ptr->type == TOKEN_NUM)
		return SYMBOL_T_NUM;

	if(tkn_ptr->type == TOKEN_RNUM)
		return SYMBOL_T_RNUM;

	if(tkn_ptr->type == TOKEN_STR)
		return SYMBOL_T_STR;

	if(tkn_ptr->type == TOKEN_AND)
		return SYMBOL_T_AND;

	if(tkn_ptr->type == TOKEN_OR)
		return SYMBOL_T_OR;

	if(tkn_ptr->type == TOKEN_NOT)
		return SYMBOL_T_NOT;

	if(tkn_ptr->type == TOKEN_LT)
		return SYMBOL_T_LT;

	if(tkn_ptr->type == TOKEN_LE)
		return SYMBOL_T_LE;

	if(tkn_ptr->type == TOKEN_GT)
		return SYMBOL_T_GT;

	if(tkn_ptr->type == TOKEN_GE)
		return SYMBOL_T_GE;

	if(tkn_ptr->type == TOKEN_ASSIGNOP)
		return SYMBOL_T_ASSIGNOP;

	if(tkn_ptr->type == TOKEN_EQ)
		return SYMBOL_T_EQ;

	if(tkn_ptr->type == TOKEN_NE)
		return SYMBOL_T_NE;

	return SYMBOL_UNKNOWN;
}

char *symbol_to_name(int symbol){
	if(symbol >= 0 && symbol < (len_variable_symbols + len_terminal_symbols)){
		return symbol_names[symbol];
	}
	return symbol_names[len_variable_symbols + len_terminal_symbols];
}

char *symbol_to_string(int symbol){
	if(symbol >= 0 && symbol < len_terminal_symbols){
		return symbol_strings[symbol];
	}
	return symbol_strings[len_terminal_symbols];
}

void token_to_value(Token *tkn_ptr, char *buffer, int len_buffer){
	if (tkn_ptr->type == TOKEN_ID)			snprintf(buffer, len_buffer, "%s", tkn_ptr->data->string);
	else if (tkn_ptr->type == TOKEN_FUNID)	snprintf(buffer, len_buffer, "%s", tkn_ptr->data->string);
	else if (tkn_ptr->type == TOKEN_NUM)	snprintf(buffer, len_buffer, "%d", tkn_ptr->data->integer);
	else if (tkn_ptr->type == TOKEN_RNUM)	snprintf(buffer, len_buffer, "%d.%d", tkn_ptr->data->integer, tkn_ptr->data->fraction);
	else if (tkn_ptr->type == TOKEN_STR)	snprintf(buffer, len_buffer, "%s", tkn_ptr->data->string);
}
