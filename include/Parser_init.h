#ifndef INCLUDE_GUARD_05C155452FCD41C4A40FCE69D00879F3
#define INCLUDE_GUARD_05C155452FCD41C4A40FCE69D00879F3


#include <stdio.h>


#include "ParserLL1.h"
#include "ParseTree.h"
#include "Token.h"


///////////
// Types //
///////////

typedef enum {
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
	SYMBOL_T_ENDOFINPUT,
	
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

	SYMBOL_UNKNOWN = -1,
}Symbol_type;


///////////////////
// Symbol Arrays //
///////////////////

extern int variable_symbols[];
extern int len_variable_symbols;
extern int terminal_symbols[];
extern int len_terminal_symbols;
extern int start_symbol;
extern int empty_symbol;
extern int end_symbol;
extern int forget_terminal_symbols[];
extern int len_forget_terminal_symbols;
extern char *symbol_names[];
extern char *symbol_strings[];

///////////////
// Functions //
///////////////

void Parser_init_add_rules(ParserLL1 *psr_ptr);

void Parser_init_add_rules_from_file(ParserLL1 *psr_ptr, const char *filename);

int token_to_symbol(Token *tkn_ptr);

char *symbol_to_string(int symbol);

char *symbol_to_name(int symbol);

int name_to_symbol(char *string);

void print_parse_tree(ParseTree *tree, FILE *file_ptr);

void print_parse_tree_preorder(ParseTree *tree, FILE *file_ptr);

#endif
