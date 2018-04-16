#include <stdio.h>
#include <string.h>

#include "Parser_init.h"
#include "ParserLL1.h"
#include "Token_Data.h"
#include "LinkedList.h"


#define SYMBOL_MAX_LEN 25
// ANSI escape codes to print to console
#define TEXT_RED	"\x1B[31m"
#define TEXT_GRN	"\x1B[32m"
#define TEXT_YLW	"\x1B[33m"
#define TEXT_BLD	"\x1B[1m"
#define TEXT_RST	"\x1B[0m"


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
	SYMBOL_V_CALL_PARAM_LIST,
	SYMBOL_V_CALL_PARAM_LIST_REM,
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
	SYMBOL_V_VAL,
};
int len_variable_symbols = 42;

int start_symbol = SYMBOL_V_MAIN_FUNCTION;
int empty_symbol = SYMBOL_V_EPSILON;
int end_symbol = SYMBOL_T_ENDOFINPUT;

int forget_terminal_symbols[] = {
	SYMBOL_T_SEMICOLON
};
int len_forget_terminal_symbols = 1;

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
	"call_param_list",
	"call_param_list_rem",
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
	"val",
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

	ParserLL1_add_rule(psr_ptr, 1,	SYMBOL_V_MAIN_FUNCTION,			(int[]) {SYMBOL_T_KW_MAIN  ,  SYMBOL_T_SQO  ,  SYMBOL_T_SQC  ,  SYMBOL_V_STMT_OR_DEF_LIST  ,  SYMBOL_T_KW_END} , 5 );
	ParserLL1_add_rule(psr_ptr, 2,	SYMBOL_V_STMT_OR_DEF_LIST,		(int[]) {SYMBOL_V_STMT_OR_DEF  ,  SYMBOL_V_STMT_OR_DEF_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 3,	SYMBOL_V_STMT_OR_DEF_LIST_REM,	(int[]) {SYMBOL_V_STMT_OR_DEF  ,  SYMBOL_V_STMT_OR_DEF_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 4,	SYMBOL_V_STMT_OR_DEF_LIST_REM,	(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 5,	SYMBOL_V_STMT_OR_DEF,			(int[]) {SYMBOL_V_STMT} , 1 );
	ParserLL1_add_rule(psr_ptr, 6,	SYMBOL_V_STMT_OR_DEF,			(int[]) {SYMBOL_V_DEF} , 1 );
	ParserLL1_add_rule(psr_ptr, 7,	SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_DECL} , 1 );
	ParserLL1_add_rule(psr_ptr, 8,	SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_ASSIGN} , 1 );
	ParserLL1_add_rule(psr_ptr, 9,	SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_COND} , 1 );
	ParserLL1_add_rule(psr_ptr, 10,	SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_READ} , 1 );
	ParserLL1_add_rule(psr_ptr, 11,	SYMBOL_V_STMT,					(int[]) {SYMBOL_V_STMT_PRINT} , 1 );
	ParserLL1_add_rule(psr_ptr, 12,	SYMBOL_V_STMT,					(int[]) {SYMBOL_V_CALL, SYMBOL_T_SEMICOLON} , 2 );


	ParserLL1_add_rule(psr_ptr, 13,	SYMBOL_V_STMT_DECL,				(int[]) {SYMBOL_V_TYPE  ,  SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM  ,  SYMBOL_T_SEMICOLON} , 4 );
	ParserLL1_add_rule(psr_ptr, 14,	SYMBOL_V_ID_LIST,				(int[]) {SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 15,	SYMBOL_V_ID_LIST,				(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 16,	SYMBOL_V_ID_LIST_REM,			(int[]) {SYMBOL_T_COMMA  ,  SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM} , 3 );
	ParserLL1_add_rule(psr_ptr, 17,	SYMBOL_V_ID_LIST_REM,			(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 18,	SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_INT} , 1 );
	ParserLL1_add_rule(psr_ptr, 19,	SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_REAL} , 1 );
	ParserLL1_add_rule(psr_ptr, 20,	SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_STRING} , 1 );
	ParserLL1_add_rule(psr_ptr, 21,	SYMBOL_V_TYPE,					(int[]) {SYMBOL_T_KW_MATRIX} , 1 );


	ParserLL1_add_rule(psr_ptr, 22,	SYMBOL_V_STMT_ASSIGN,			(int[]) {SYMBOL_V_LVAL  ,  SYMBOL_T_ASSIGNOP  ,  SYMBOL_V_EXPR_ARITH  ,  SYMBOL_T_SEMICOLON} , 4 );
	ParserLL1_add_rule(psr_ptr, 23,	SYMBOL_V_LVAL,					(int[]) {SYMBOL_T_ID} , 1 );
	ParserLL1_add_rule(psr_ptr, 24,	SYMBOL_V_LVAL,					(int[]) {SYMBOL_T_SQO  ,  SYMBOL_T_ID  ,  SYMBOL_V_ID_LIST_REM  ,  SYMBOL_T_SQC} , 4 );


	ParserLL1_add_rule(psr_ptr, 25,	SYMBOL_V_EXPR_ARITH,			(int[]) {SYMBOL_V_EXPR_TERM  ,  SYMBOL_V_EXPR_ADDEND} ,2  );
	ParserLL1_add_rule(psr_ptr, 26,	SYMBOL_V_EXPR_ADDEND,			(int[]) {SYMBOL_T_PLUS  ,  SYMBOL_V_EXPR_TERM  ,  SYMBOL_V_EXPR_ADDEND} , 3 );
	ParserLL1_add_rule(psr_ptr, 27,	SYMBOL_V_EXPR_ADDEND,			(int[]) {SYMBOL_T_MINUS  ,  SYMBOL_V_EXPR_TERM  ,  SYMBOL_V_EXPR_ADDEND} , 3 );
	ParserLL1_add_rule(psr_ptr, 28,	SYMBOL_V_EXPR_ADDEND,			(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 29,	SYMBOL_V_EXPR_TERM,				(int[]) {SYMBOL_V_EXPR_FACTOR  ,  SYMBOL_V_EXPR_MULTIPLIER} , 2 );
	ParserLL1_add_rule(psr_ptr, 30,	SYMBOL_V_EXPR_MULTIPLIER,		(int[]) {SYMBOL_T_MUL  ,  SYMBOL_V_EXPR_FACTOR  ,  SYMBOL_V_EXPR_MULTIPLIER} , 3 );
	ParserLL1_add_rule(psr_ptr, 31,	SYMBOL_V_EXPR_MULTIPLIER,		(int[]) {SYMBOL_T_DIV  ,  SYMBOL_V_EXPR_FACTOR  ,  SYMBOL_V_EXPR_MULTIPLIER} , 3 );
	ParserLL1_add_rule(psr_ptr, 32,	SYMBOL_V_EXPR_MULTIPLIER,		(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 33,	SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_T_OP  ,  SYMBOL_V_EXPR_ARITH  ,  SYMBOL_T_CL} , 3 );
	ParserLL1_add_rule(psr_ptr, 34,	SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_V_NUM_VAL_WM} , 1 );
	ParserLL1_add_rule(psr_ptr, 35,	SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_T_STR} , 1 );
	ParserLL1_add_rule(psr_ptr, 36,	SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_T_SIZE  ,  SYMBOL_T_ID} , 2 );
	ParserLL1_add_rule(psr_ptr, 37,	SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_V_MATRIX_LITERAL} , 1 );
	ParserLL1_add_rule(psr_ptr, 38,	SYMBOL_V_EXPR_FACTOR,			(int[]) {SYMBOL_V_CALL} , 1 );
	ParserLL1_add_rule(psr_ptr, 39,	SYMBOL_V_MATRIX_LITERAL,		(int[]) {SYMBOL_T_SQO  ,  SYMBOL_V_MATRIX_ROW_LIST  ,  SYMBOL_T_SQC} , 3 );
	ParserLL1_add_rule(psr_ptr, 40,	SYMBOL_V_MATRIX_ROW_LIST,		(int[]) {SYMBOL_V_MATRIX_NUM_LIST  ,  SYMBOL_V_MATRIX_ROW_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 41,	SYMBOL_V_MATRIX_ROW_LIST_REM,	(int[]) {SYMBOL_T_SEMICOLON  ,  SYMBOL_V_MATRIX_ROW_LIST} , 2 );
	ParserLL1_add_rule(psr_ptr, 42,	SYMBOL_V_MATRIX_ROW_LIST_REM,	(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 43,	SYMBOL_V_MATRIX_NUM_LIST,		(int[]) {SYMBOL_T_NUM  ,  SYMBOL_V_MATRIX_NUM_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 44,	SYMBOL_V_MATRIX_NUM_LIST_REM,	(int[]) {SYMBOL_T_COMMA  ,  SYMBOL_V_MATRIX_NUM_LIST} , 2 );
	ParserLL1_add_rule(psr_ptr, 45,	SYMBOL_V_MATRIX_NUM_LIST_REM,	(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 46,	SYMBOL_V_CALL,					(int[]) {SYMBOL_T_FUNID  ,  SYMBOL_T_OP  ,  SYMBOL_V_CALL_PARAM_LIST  ,  SYMBOL_T_CL} , 4 );


	ParserLL1_add_rule(psr_ptr, 47,	SYMBOL_V_STMT_COND,				(int[]) {SYMBOL_T_KW_IF  ,  SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL  ,  SYMBOL_V_STMT_LIST  ,  SYMBOL_V_STMT_COND_REM} , 6 );
	ParserLL1_add_rule(psr_ptr, 48,	SYMBOL_V_STMT_COND_REM,			(int[]) {SYMBOL_T_KW_ENDIF, SYMBOL_T_SEMICOLON} , 2 );
	ParserLL1_add_rule(psr_ptr, 49,	SYMBOL_V_STMT_COND_REM,			(int[]) {SYMBOL_T_KW_ELSE  ,  SYMBOL_V_STMT_LIST  ,  SYMBOL_T_KW_ENDIF, SYMBOL_T_SEMICOLON} , 4 );
	ParserLL1_add_rule(psr_ptr, 50,	SYMBOL_V_STMT_LIST,				(int[]) {SYMBOL_V_STMT  ,  SYMBOL_V_STMT_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 51,	SYMBOL_V_STMT_LIST_REM,			(int[]) {SYMBOL_V_STMT  ,  SYMBOL_V_STMT_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 52,	SYMBOL_V_STMT_LIST_REM,			(int[]) {SYMBOL_V_EPSILON} , 1 );


	ParserLL1_add_rule(psr_ptr, 53,	SYMBOL_V_STMT_READ,				(int[]) {SYMBOL_T_KW_READ  ,  SYMBOL_T_OP  ,  SYMBOL_T_ID  ,  SYMBOL_T_CL  ,  SYMBOL_T_SEMICOLON} , 5 );
	ParserLL1_add_rule(psr_ptr, 54,	SYMBOL_V_STMT_PRINT,			(int[]) {SYMBOL_T_KW_PRINT  ,  SYMBOL_T_OP  ,  SYMBOL_T_ID  ,  SYMBOL_T_CL  ,  SYMBOL_T_SEMICOLON} , 5 );


	ParserLL1_add_rule(psr_ptr, 55,	SYMBOL_V_EXPR_LOG,				(int[]) {SYMBOL_V_EXPR_BOOL} , 1 );
	ParserLL1_add_rule(psr_ptr, 56,	SYMBOL_V_EXPR_LOG,				(int[]) {SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL  ,  SYMBOL_V_LOG_OP_BIN  ,  SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL} , 7 );
	ParserLL1_add_rule(psr_ptr, 57,	SYMBOL_V_EXPR_LOG,				(int[]) {SYMBOL_T_NOT  ,  SYMBOL_T_OP  ,  SYMBOL_V_EXPR_LOG  ,  SYMBOL_T_CL} , 4 );
	ParserLL1_add_rule(psr_ptr, 58,	SYMBOL_V_LOG_OP_BIN,			(int[]) {SYMBOL_T_AND} , 1 );
	ParserLL1_add_rule(psr_ptr, 59,	SYMBOL_V_LOG_OP_BIN,			(int[]) {SYMBOL_T_OR} , 1 );


	ParserLL1_add_rule(psr_ptr, 60,	SYMBOL_V_EXPR_BOOL,				(int[]) {SYMBOL_V_NUM_VAL_WOM  ,  SYMBOL_V_BOOL_OP  ,  SYMBOL_V_NUM_VAL_WOM} , 3 );
	ParserLL1_add_rule(psr_ptr, 61,	SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_LT} , 1 );
	ParserLL1_add_rule(psr_ptr, 62,	SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_LE} , 1 );
	ParserLL1_add_rule(psr_ptr, 63,	SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_GT} , 1 );
	ParserLL1_add_rule(psr_ptr, 64,	SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_GE} , 1 );
	ParserLL1_add_rule(psr_ptr, 65,	SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_EQ} , 1 );
	ParserLL1_add_rule(psr_ptr, 66,	SYMBOL_V_BOOL_OP,				(int[]) {SYMBOL_T_NE} , 1 );


	ParserLL1_add_rule(psr_ptr, 67,	SYMBOL_V_DEF,					(int[]) {SYMBOL_T_KW_FUNCTION  ,  SYMBOL_T_SQO  ,  SYMBOL_V_PARAM_LIST  ,  SYMBOL_T_SQC  ,  SYMBOL_T_ASSIGNOP  ,  SYMBOL_T_FUNID  ,  SYMBOL_T_SQO  ,  SYMBOL_V_PARAM_LIST  ,  SYMBOL_T_SQC  ,  SYMBOL_V_STMT_OR_DEF_LIST  ,  SYMBOL_T_KW_END  ,  SYMBOL_T_SEMICOLON} , 12 );
	ParserLL1_add_rule(psr_ptr, 68,	SYMBOL_V_PARAM_LIST,			(int[]) {SYMBOL_V_TYPE  ,  SYMBOL_T_ID  ,  SYMBOL_V_PARAM_LIST_REM} , 3 );
	ParserLL1_add_rule(psr_ptr, 69,	SYMBOL_V_PARAM_LIST_REM,		(int[]) {SYMBOL_T_COMMA  ,  SYMBOL_V_TYPE  ,  SYMBOL_T_ID  ,  SYMBOL_V_PARAM_LIST_REM} , 4 );
	ParserLL1_add_rule(psr_ptr, 70,	SYMBOL_V_PARAM_LIST_REM,		(int[]) {SYMBOL_V_EPSILON} , 1 );


	ParserLL1_add_rule(psr_ptr, 71,	SYMBOL_V_NUM_VAL_WM,			(int[]) {SYMBOL_T_NUM} , 1 );
	ParserLL1_add_rule(psr_ptr, 72,	SYMBOL_V_NUM_VAL_WM,			(int[]) {SYMBOL_T_RNUM} , 1 );
	ParserLL1_add_rule(psr_ptr, 73,	SYMBOL_V_NUM_VAL_WM,			(int[]) {SYMBOL_T_ID  ,  SYMBOL_V_NUM_VAL_WM_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 74,	SYMBOL_V_NUM_VAL_WM_REM,		(int[]) {SYMBOL_T_SQO  ,  SYMBOL_T_NUM  ,  SYMBOL_T_COMMA  ,  SYMBOL_T_NUM  ,  SYMBOL_T_SQC} , 5 );
	ParserLL1_add_rule(psr_ptr, 75,	SYMBOL_V_NUM_VAL_WM_REM,		(int[]) {SYMBOL_V_EPSILON} , 1 );
	ParserLL1_add_rule(psr_ptr, 76,	SYMBOL_V_NUM_VAL_WOM,			(int[]) {SYMBOL_T_NUM} , 1 );
	ParserLL1_add_rule(psr_ptr, 77,	SYMBOL_V_NUM_VAL_WOM,			(int[]) {SYMBOL_T_RNUM} , 1 );
	ParserLL1_add_rule(psr_ptr, 78,	SYMBOL_V_NUM_VAL_WOM,			(int[]) {SYMBOL_T_ID} , 1 );
	ParserLL1_add_rule(psr_ptr, 79,	SYMBOL_V_VAL,					(int[]) {SYMBOL_V_NUM_VAL_WM} , 1 );
	ParserLL1_add_rule(psr_ptr, 80,	SYMBOL_V_VAL,					(int[]) {SYMBOL_T_STR} , 1 );
	ParserLL1_add_rule(psr_ptr, 81,	SYMBOL_V_VAL,					(int[]) {SYMBOL_V_MATRIX_LITERAL} , 1 );


	ParserLL1_add_rule(psr_ptr, 82,	SYMBOL_V_CALL_PARAM_LIST,		(int[]) {SYMBOL_V_VAL  ,  SYMBOL_V_CALL_PARAM_LIST_REM} , 2 );
	ParserLL1_add_rule(psr_ptr, 83,	SYMBOL_V_CALL_PARAM_LIST_REM,	(int[]) {SYMBOL_T_COMMA  ,  SYMBOL_V_VAL  ,  SYMBOL_V_CALL_PARAM_LIST_REM} , 3 );
	ParserLL1_add_rule(psr_ptr, 84,	SYMBOL_V_CALL_PARAM_LIST_REM,	(int[]) {SYMBOL_V_EPSILON} , 1 );

}

void Parser_init_add_rules_from_file(ParserLL1 *psr_ptr, const char *filename){
	FILE *file_ptr = fopen(filename, "r");
	if(file_ptr == NULL){
		fprintf(stderr, "Unable to open file %s\n", filename);
		return;
	}

	// Additional len for checking len limit and nu;=l
	int len_buffer = SYMBOL_MAX_LEN + 2;
	char buffer[len_buffer];

	int state = 0;
	// 0 > lhs > 1 > = > 2 > rhs > 2 > . > 0

	int variable_symbol;
	LinkedList *expansion_symbol_list = LinkedList_new();

	int cur_rule_num = 1;

	while(1){
		memset(buffer, 0, len_buffer);

		int status = fscanf(file_ptr, " %25s", buffer);

		// printf("%d\t%s\n", state, buffer);

		if(status == EOF){
			break;
		}

		if(buffer[len_buffer-1] != '\0'){
			// Longer than max len
			fprintf(stderr, "Symbol %s is longer than %d chars\n", buffer, SYMBOL_MAX_LEN);
			break;
		}


		if(state == 0){
			// Try to read lhs
			variable_symbol = name_to_symbol(buffer);
			if(variable_symbol == -1){
				fprintf(stderr, "Unrecognised symbol %s\n", buffer);
				break;
			}
			state = 1;
		}
		else if(state == 1){
			// Try to read =
			if(strcmp(buffer, "=") != 0){
				fprintf(stderr, "Expected \"=\", got %s\n", buffer);
				break;
			}
			state = 2;
		}
		else if(state == 2){
			// Try to read rhs
			int symbol = name_to_symbol(buffer);
			if(symbol == -1){

				if(strcmp(buffer, ".") == 0){
					// Create and add rule

					int len_expansion_symbols = LinkedList_get_size(expansion_symbol_list);
					int expansion_symbols[len_expansion_symbols];

					for (int i = 0; i < len_expansion_symbols; ++i){
						expansion_symbols[i] = (long) LinkedList_pop(expansion_symbol_list);
					}

					ParserLL1_add_rule(psr_ptr, cur_rule_num, variable_symbol, expansion_symbols, len_expansion_symbols);
					cur_rule_num++;

					state = 0;
				}

				else{
					fprintf(stderr, "Unrecognised symbol %s\n", buffer);
					break;
				}
			}

			else{
				LinkedList_pushback(expansion_symbol_list, (void*)(long)symbol);
			}
		}
	}

	LinkedList_destroy(expansion_symbol_list);

	fclose(file_ptr);
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

int name_to_symbol(char *string){
	for (int i = 0; i < len_terminal_symbols + len_variable_symbols; ++i){
		if(strcmp(string, symbol_names[i]) == 0){
			return i;
		}
	}

	return -1;
}

static void print_node(ParseTree_Node *node_ptr, int *index, FILE* file_ptr){
	int symbol = node_ptr->symbol;
	Token *tkn_ptr = node_ptr->tkn_ptr;

	int col[] = {5,11,4,20,20,20,4,20};

	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",		col[0], 		(*index)++);

	if(tkn_ptr != NULL){
		// terminal leaf
		char buffer_token[col[3]+2];	memset(buffer_token, 0, col[3]+2);
		char buffer_value[col[4]+2];	memset(buffer_value, 0, col[4]+2);

		if(tkn_ptr->type == TOKEN_NUM || tkn_ptr->type == TOKEN_RNUM){
			snprintf(buffer_token, col[3]+2, "%s", "-------------------------");
			token_to_value(tkn_ptr, buffer_value, col[4]+2);
			if(buffer_value[col[4]]!='\0'){
				buffer_value[col[4]] = '\0';
				buffer_value[col[4]-1] = buffer_value[col[4]-2] = buffer_value[col[4]-3] = '.';
			}
		}
		else{
			token_to_string(tkn_ptr, buffer_token, col[3]+2);
			snprintf(buffer_value, col[4]+2, "%s", "-------------------------");
			if(buffer_token[col[3]]!='\0'){
				buffer_token[col[3]] = '\0';
				buffer_token[col[3]-1] = buffer_token[col[3]-2] = buffer_token[col[3]-3] = '.';
			}
		}

		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1], token_to_name(tkn_ptr));
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",		col[2],			tkn_ptr->line);
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3], buffer_token);
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4], buffer_value);

		if(node_ptr->parent){
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5], symbol_to_name(node_ptr->parent->symbol));
		}
		else{
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],  "ROOT");
		}

		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"Yes");
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7], symbol_to_name(symbol));
	}
	else{
		// Non leaf or uninitialized leaf
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"-------------------------");
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");

		if(node_ptr->parent){
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5], symbol_to_name(node_ptr->parent->symbol));
		}
		else{
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"ROOT");
		}

		if(node_ptr->child == NULL){
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"Yes");
		}
		else{
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"No");
		}

		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7], symbol_to_name(symbol));
	}

	fprintf(file_ptr, " |\n");
}

static void traverse_inorder(ParseTree_Node *cur_node_ptr, int *index, FILE *file_ptr){
	if(cur_node_ptr == NULL){
		return;
	}

	ParseTree_Node *child_node_ptr = cur_node_ptr->child;
	traverse_inorder(child_node_ptr, index, file_ptr);

	print_node(cur_node_ptr, index, file_ptr);

	while(child_node_ptr != NULL){
		child_node_ptr = child_node_ptr->sibling;
		traverse_inorder(child_node_ptr, index, file_ptr);
	}
}

void print_parse_tree(ParseTree *tree, FILE *file_ptr){
	int col[] = {5,11,4,20,20,20,4,20};

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"-------------------------");
	fprintf(file_ptr, "--\n");

	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"S No");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"Lexeme");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"Line");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"Token");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"Value");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"Parent");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"Leaf");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"Current");
	fprintf(file_ptr, " |\n");

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"-------------------------");
	fprintf(file_ptr, "--\n");

	int index = 1;

	traverse_inorder(tree, &index, file_ptr);

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"-------------------------");
	fprintf(file_ptr, "--\n");
}

void print_parse_tree_preorder(ParseTree *tree, FILE *file_ptr){
	int col[] = {5,11,4,20,20,20,4,20};

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"-------------------------");
	fprintf(file_ptr, "--\n");

	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"S No");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"Lexeme");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"Line");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"Token");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"Value");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"Parent");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"Leaf");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"Current");
	fprintf(file_ptr, " |\n");

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"-------------------------");
	fprintf(file_ptr, "--\n");

	int index = 1;

	ParseTree_Node *cur_node_ptr = tree;
	while(cur_node_ptr != NULL){
		print_node(cur_node_ptr, &index, file_ptr);
		cur_node_ptr = ParseTree_Node_move_preorder(cur_node_ptr);
	}

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"-------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"-------------------------");
	fprintf(file_ptr, "--\n");
}
