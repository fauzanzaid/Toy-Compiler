#include "Parser_init.h"
#include "ParserLL1.h"

int variable_symbols[] = {
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
	SYMBOL_T_NE
};
int len_variable_symbols = 41;

int terminal_symbols[] = {
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
int len_terminal_symbols = 38;

int start_symbol = SYMBOL_V_MAIN_FUNCTION;
int empty_symbol = SYMBOL_V_EPSILON;


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
	ParserLL1_add_rule(psr_ptr, SYMBOL_V_STMT_PRINT,			(int[]) {SYMBOL_T_KW_PRINT  ,  SYMBOL_T_OP  ,  SYMBOL_V_NUM_VAL_WM  ,  SYMBOL_T_CL  ,  SYMBOL_T_SEMICOLON} , 5 );


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
