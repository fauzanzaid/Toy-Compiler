#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "Ast_init.h"
#include "Parser_init.h"
#include "Type_init.h"
#include "Icg_init.h"


///////////////
// Constants //
///////////////

char *ast_operator_names[] = {
	"",
	"plus",
	"minus",
	"mul",
	"div",
	"size",
	"id",
	"funid",
	"kw_int",
	"kw_real",
	"kw_string",
	"kw_matrix",
	"kw_read",
	"kw_print",
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
	"assign",
	"eq",
	"ne",
	"call",
	"cond",
	"decl",
	"def",
	"id_list",
	"matrix",
	"matrix_element",
	"def_param_list",
	"call_param_list",
	"stmt_list",
	"stmt_or_def_list",
	"unknown",
};

int len_ast_operator_names = 39;


////////////////////////
// Private Prototypes //
////////////////////////

static int remove_children(ParseTree_Node *node_ptr, int *symbol_indices, int len_symbol_indices);


///////////////
// Functions //
///////////////

char *ast_operator_to_name(int op){
	if(op>0 && op<len_ast_operator_names)
		return ast_operator_names[op];

	else
		return ast_operator_names[len_ast_operator_names-1];
}

ParseTree_Node_Attr *ParseTree_Node_Attr_new(){
	ParseTree_Node_Attr *atr_ptr = malloc( sizeof(ParseTree_Node_Attr) );

	atr_ptr->inh = NULL;
	atr_ptr->type = NULL;
	atr_ptr->entry = NULL;

	atr_ptr->result_value = NULL;
	atr_ptr->result_2_value = NULL;
	atr_ptr->result_entry = NULL;

	atr_ptr->code = NULL;

	return atr_ptr;
}


void ParseTree_Node_Attr_destroy(ParseTree_Node_Attr *atr_ptr){
	if(atr_ptr->type != NULL){
		Type_destroy(atr_ptr->type);
	}

	if(atr_ptr->result_value != NULL){
		free(atr_ptr->result_value);
	}

	if(atr_ptr->result_2_value != NULL){
		free(atr_ptr->result_2_value);
	}

	if(atr_ptr->code != NULL){
		quad_list_destroy(atr_ptr->code);
	}

	free(atr_ptr);
}


ParseTree_Node *prune_parse_tree(ParseTree_Node *node_ptr){

	switch(node_ptr->rule_num){

		// Terminal symbols
		case 0:
		{
			switch(node_ptr->symbol){

				case SYMBOL_T_PLUS:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_PLUS;
					break;
				}

				case SYMBOL_T_MINUS:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_MINUS;
					break;
				}

				case SYMBOL_T_MUL:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_MUL;
					break;
				}

				case SYMBOL_T_DIV:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_DIV;
					break;
				}

				case SYMBOL_T_SIZE:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_SIZE;
					break;
				}

				case SYMBOL_T_ID:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_ID;
					break;
				}

				case SYMBOL_T_FUNID:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_FUNID;
					break;
				}

				case SYMBOL_T_KW_INT:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_KW_INT;
					break;
				}

				case SYMBOL_T_KW_REAL:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_KW_REAL;
					break;
				}

				case SYMBOL_T_KW_STRING:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_KW_STRING;
					break;
				}

				case SYMBOL_T_KW_MATRIX:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_KW_MATRIX;
					break;
				}

				case SYMBOL_T_KW_READ:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_KW_READ;
					break;
				}

				case SYMBOL_T_KW_PRINT:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_KW_PRINT;
					break;
				}

				case SYMBOL_T_NUM:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_NUM;
					break;
				}

				case SYMBOL_T_RNUM:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_RNUM;
					break;
				}

				case SYMBOL_T_STR:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_STR;
					break;
				}

				case SYMBOL_T_AND:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_AND;
					break;
				}

				case SYMBOL_T_OR:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_OR;
					break;
				}

				case SYMBOL_T_NOT:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_NOT;
					break;
				}

				case SYMBOL_T_LT:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_LT;
					break;
				}

				case SYMBOL_T_LE:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_LE;
					break;
				}

				case SYMBOL_T_GT:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_GT;
					break;
				}

				case SYMBOL_T_GE:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_GE;
					break;
				}

				case SYMBOL_T_ASSIGNOP:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_ASSIGN;
					break;
				}

				case SYMBOL_T_EQ:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_EQ;
					break;
				}

				case SYMBOL_T_NE:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_NE;
					break;
				}

				default:
				{
					node_ptr->atr_ptr->op = AST_OPERATOR_UNKNOWN;
					printf("=%d=\n", node_ptr->symbol);
					break;
				}
			}

			break;
		}

		//  1  (main_function) = KW_MAIN  SQO  SQC  (stmt_or_def_list)  END
		case 1:
		{
			remove_children( node_ptr, (int[]) {0,1,2,4} , 4 );

			ParseTree_Node *child_3 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 3);

			ParseTree_Node_destroy(node_ptr);

			child_3 = prune_parse_tree( child_3 );

			node_ptr = child_3;

			break;
		}

		//  2  (stmt_or_def_list) = (stmt_or_def)  (stmt_or_def_list_rem)
		case 2:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			if(child_1 != NULL)
				ParseTree_Node_attach_sibling(child_0, child_1);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = AST_OPERATOR_STMT_OR_DEF_LIST;

			break;
		}

		//  3  (stmt_or_def_list_rem_1) = (stmt_or_def)  (stmt_or_def_list_rem_2)
		case 3:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			if(child_1 != NULL)
				ParseTree_Node_attach_sibling(node_ptr, child_1);

			break;
		}

		//  4  (stmt_or_def_list_rem_1) = EPSILON
		case 4:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		//  5  (stmt_or_def) = (stmt)
		case 5:

		//  6  (stmt_or_def) = (def)
		case 6:

		//  7  (stmt) = (stmt_decl)
		case 7:

		//  8  (stmt) = (stmt_assign)
		case 8:

		//  9  (stmt) = (stmt_cond)
		case 9:

		// 10  (stmt) = (stmt_read)
		case 10:

		// 11  (stmt) = (stmt_print)
		case 11:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}



		// 12  (stmt) = (call)  SEMICOLON
		case 12:
		{
			remove_children( node_ptr, (int[]) {1} , 1 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 13  (stmt_decl) = (type)  ID  (id_list_rem)  SEMICOLON
		case 13:
		{
			remove_children( node_ptr, (int[]) {3} , 1 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );


			ParseTree_Node *child_new = ParseTree_Node_new(SYMBOL_UNKNOWN, NULL);
			child_new->atr_ptr->op = AST_OPERATOR_ID_LIST;

			if(child_2 != NULL)
				ParseTree_Node_attach_sibling(child_1, child_2);
			ParseTree_Node_attach_child_left_end(child_new, child_1);
			ParseTree_Node_attach_child_left_end(node_ptr, child_new);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			// if(child_2 != NULL)
				// ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			// ParseTree_Node_attach_child_left_end(node_ptr, child_1);
			// ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = AST_OPERATOR_DECL;

			break;
		}

		// 14  (id_list) = ID  (id_list_rem)
		case 14:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			if(child_1 != NULL)
				ParseTree_Node_attach_sibling(child_0, child_1);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = AST_OPERATOR_ID_LIST;

			break;
		}

		// 15  (id_list) = EPSILON
		case 15:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 16  (id_list_rem) = COMMA  ID  (id_list_rem)
		case 16:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_1;

			if(child_2 != NULL)
				ParseTree_Node_attach_sibling(node_ptr, child_2);

			break;
		}

		// 17  (id_list_rem) = EPSILON
		case 17:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 18  (type) = KW_INT
		case 18:

		// 19  (type) = KW_REAL
		case 19:

		// 20  (type) = KW_STRING
		case 20:

		// 21  (type) = KW_MATRIX
		case 21:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 22  (stmt_assign) = (lval)  ASSIGNOP  (expr_arith)  SEMICOLON
		case 22:
		{
			remove_children( node_ptr, (int[]) {3} , 1 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );

			node_ptr = child_1;

			ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			break;
		}

		// 23  (lval) = ID
		case 23:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 24  (lval) = SQO  ID  (id_list_rem)  SQC
		case 24:
		{
			remove_children( node_ptr, (int[]) {0,3} , 2 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );

			if(child_2 != NULL)
				ParseTree_Node_attach_sibling(child_1, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, child_1);

			node_ptr->atr_ptr->op = AST_OPERATOR_ID_LIST;

			break;
		}

		// 25  (expr_arith) = (expr_term)  (expr_addend)
		case 25:

		// 29  (expr_term) = (expr_factor)  (expr_multiplier)
		case 29:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1->atr_ptr->inh = child_0;
			child_1 = prune_parse_tree( child_1 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_1;

			break;
		}

		// 26  (expr_addend_1) = PLUS  (expr_term)  (expr_addend_2)
		case 26:

		// 27  (expr_addend_1) = MINUS  (expr_term)  (expr_addend_2)
		case 27:

		// 30  (expr_multiplier_1) = MUL  (expr_factor)  (expr_multiplier_2)
		case 30:

		// 31  (expr_multiplier_1) = DIV  (expr_factor)  (expr_multiplier_2)
		case 31:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			ParseTree_Node *inh = node_ptr->atr_ptr->inh;

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			ParseTree_Node_attach_child_left_end(child_0, child_1);
			ParseTree_Node_attach_child_left_end(child_0, inh);

			child_2->atr_ptr->inh = child_0;
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_2;

			break;
		}


		// 28  (expr_addend) = EPSILON
		case 28:

		// 32  (expr_multiplier) = EPSILON
		case 32:
		{
			ParseTree_Node *inh = node_ptr->atr_ptr->inh;

			ParseTree_Node_destroy(node_ptr);

			node_ptr = inh;

			break;
		}

		// 33  (expr_factor) = OP  (expr_arith)  CL
		case 33:
		{
			remove_children( node_ptr, (int[]) {0,2} , 2 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			ParseTree_Node_destroy(node_ptr);

			child_1 = prune_parse_tree( child_1 );

			node_ptr = child_1;

			break;
		}

		// 34  (expr_factor) = (num_val_wm)
		case 34:

		// 35  (expr_factor) = STR
		case 35:

		// 37  (expr_factor) = (matrix_literal)
		case 37:

		// 38  (expr_factor) = (call)
		case 38:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 36  (expr_factor) = SIZE ID
		case 36:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			ParseTree_Node_attach_child_left_end(node_ptr, child_1);

			break;
		}

		// 39  (matrix_literal) = SQO  (matrix_row_list)  SQC
		case 39:
		{
			remove_children( node_ptr, (int[]) {0,2} , 2 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_1 = prune_parse_tree( child_1 );

			// ParseTree_Node_destroy(node_ptr);

			// node_ptr = child_1;

			ParseTree_Node_attach_child_left_end(node_ptr, child_1);

			node_ptr->atr_ptr->op = AST_OPERATOR_MATRIX;

			break;
		}

		// 40  (matrix_row_list) = (matrix_num_list)  (matrix_row_list_rem)
		case 40:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );


			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			if(child_1 != NULL)
				ParseTree_Node_attach_child_left_end(node_ptr, child_1);

			break;
		}

		// 41  (matrix_row_list_rem) = SEMICOLON  (matrix_row_list)
		case 41:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			ParseTree_Node_destroy(node_ptr);

			child_1 = prune_parse_tree( child_1 );

			node_ptr = child_1;

			break;
		}

		// 42  (matrix_row_list_rem) = EPSILON
		case 42:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 43  (matrix_num_list) = NUM  (matrix_num_list_rem)
		case 43:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );


			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			if(child_1 != NULL)
				ParseTree_Node_attach_sibling(node_ptr, child_1);

			break;
		}

		// 44  (matrix_num_list_rem) = COMMA  (matrix_num_list)
		case 44:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			ParseTree_Node_destroy(node_ptr);

			child_1 = prune_parse_tree( child_1 );

			node_ptr = child_1;

			break;
		}

		// 45  (matrix_num_list_rem) = EPSILON
		case 45:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 46  (call) = FUNID  OP  (call_param_list)  CL
		case 46:
		{
			remove_children( node_ptr, (int[]) {1,3} , 2 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_0 = prune_parse_tree( child_0 );
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = AST_OPERATOR_CALL;

			break;
		}

		// 47  (stmt_cond) = KW_IF  OP  (expr_log)  CL  (stmt_list)  (stmt_cond_rem)
		case 47:
		{
			remove_children( node_ptr, (int[]) {0,1,3} , 3 );

			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);
			ParseTree_Node *child_4 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 4);
			ParseTree_Node *child_5 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 5);

			child_2 = prune_parse_tree( child_2 );
			child_4 = prune_parse_tree( child_4 );
			child_5 = prune_parse_tree( child_5 );

			if(child_5 != NULL)
				ParseTree_Node_attach_child_left_end(node_ptr, child_5);
			ParseTree_Node_attach_child_left_end(node_ptr, child_4);
			ParseTree_Node_attach_child_left_end(node_ptr, child_2);

			node_ptr->atr_ptr->op = AST_OPERATOR_COND;

			break;
		}

		// 48  (stmt_cond_rem) = KW_ENDIF  SEMICOLON
		case 48:
		{
			remove_children( node_ptr, (int[]) {0,1} , 2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 49  (stmt_cond_rem) = KW_ELSE  (stmt_list)  KW_ENDIF
		case 49:
		{
			remove_children( node_ptr, (int[]) {0,2} , 2 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			ParseTree_Node_destroy(node_ptr);

			child_1 = prune_parse_tree( child_1 );

			node_ptr = child_1;

			break;
		}

		// 50  (stmt_list) = (stmt)  (stmt_list_rem)
		case 50:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			if(child_1 != NULL)
				ParseTree_Node_attach_sibling(child_0, child_1);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = AST_OPERATOR_STMT_LIST;

			break;
		}

		// 51  (stmt_list_rem_1) = (stmt)  (stmt_list_rem_2)
		case 51:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );


			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			if(child_1 != NULL)
				ParseTree_Node_attach_sibling(node_ptr, child_1);

			break;
		}

		// 52  (stmt_list_rem_1) = EPSILON
		case 52:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 53  (stmt_read) = KW_READ  OP  ID  CL  SEMICOLON
		case 53:
		{
			remove_children( node_ptr, (int[]) {1,3,4} , 3 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_0 = prune_parse_tree( child_0 );
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			ParseTree_Node_attach_child_left_end(node_ptr, child_2);

			break;
		}

		// 54  (stmt_print) = KW_PRINT  OP  ID  CL  SEMICOLON
		case 54:
		{
			remove_children( node_ptr, (int[]) {1,3,4} , 3 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_0 = prune_parse_tree( child_0 );
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			ParseTree_Node_attach_child_left_end(node_ptr, child_2);

			break;
		}

		// 55  (expr_log) = (expr_bool)
		case 55:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 56  (expr_log) = OP  (expr_log)  CL  (log_op_bin)  OP  (expr_log)  CL
		case 56:
		{
			remove_children( node_ptr, (int[]) {0,2,4,6} , 4 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_3 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 3);
			ParseTree_Node *child_5 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 5);

			child_1 = prune_parse_tree( child_1 );
			child_3 = prune_parse_tree( child_3 );
			child_5 = prune_parse_tree( child_5 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_3;

			ParseTree_Node_attach_child_left_end(node_ptr, child_5);
			ParseTree_Node_attach_child_left_end(node_ptr, child_1);

			break;
		}

		// 57  (expr_log) = NOT  OP  (expr_log)  CL
		case 57:
		{
			remove_children( node_ptr, (int[]) {1,3} , 2 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_0 = prune_parse_tree( child_0 );
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			ParseTree_Node_attach_child_left_end(node_ptr, child_2);

			break;
		}

		// 58  (log_op_bin) = AND
		case 58:

		// 59  (log_op_bin) = OR
		case 59:

		// 61  (bool_op) = LT
		case 61:

		// 62  (bool_op) = LE
		case 62:

		// 63  (bool_op) = GT
		case 63:

		// 64  (bool_op) = GE
		case 64:

		// 65  (bool_op) = EQ
		case 65:

		// 66  (bool_op) = NE
		case 66:

		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 60  (expr_bool) = (num_val_wom_1)  (bool_op)  (num_val_wom_2)
		case 60:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_1;

			ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			break;
		}

		// 67  (def) = KW_FUNCTION  SQO  (param_list_1)  SQC  ASSIGNOP  FUNID  SQO  (param_list_2)  SQC  (stmt_or_def_list)  END  SEMICOLON
		case 67:
		{
			remove_children( node_ptr, (int[]) {0,1,3,4,6,8,10,11} , 8 );

			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);
			ParseTree_Node *child_5 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 5);
			ParseTree_Node *child_7 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 7);
			ParseTree_Node *child_9 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 9);

			child_2 = prune_parse_tree( child_2 );
			child_5 = prune_parse_tree( child_5 );
			child_7 = prune_parse_tree( child_7 );
			child_9 = prune_parse_tree( child_9 );

			ParseTree_Node_attach_child_left_end(node_ptr, child_9);
			ParseTree_Node_attach_child_left_end(node_ptr, child_7);
			ParseTree_Node_attach_child_left_end(node_ptr, child_5);
			ParseTree_Node_attach_child_left_end(node_ptr, child_2);

			node_ptr->atr_ptr->op = AST_OPERATOR_DEF;

			break;
		}

		// 68  (param_list) = (type)  ID  (param_list_rem)
		case 68:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );

			if(child_2 != NULL)
				ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, child_1);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = AST_OPERATOR_DEF_PARAM_LIST;

			break;
		}

		// 69  (param_list_rem_1) = COMMA  (type)  ID  (param_list_rem_2)
		case 69:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);
			ParseTree_Node *child_3 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 3);

			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );
			child_3 = prune_parse_tree( child_3 );

			if(child_3 != NULL)
				ParseTree_Node_attach_child_left_end(node_ptr, child_3);
			ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, child_1);

			node_ptr->atr_ptr->op = AST_OPERATOR_DEF_PARAM_LIST;

			break;
		}

		// 70  (param_list_rem_1) = EPSILON
		case 70:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 71  (num_val_wm) = NUM
		case 71:

		// 72  (num_val_wm) = RNUM
		case 72:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 73  (num_val_wm) = ID  (num_val_wm_rem)
		case 73:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			if(child_1 != NULL){
				node_ptr->atr_ptr->op = AST_OPERATOR_MATRIX_ELEMENT;
				ParseTree_Node_attach_child_left_end(node_ptr, child_1);
			}

			break;
		}

		// 74  (num_val_wm_rem) = SQO  NUM_1  COMMA  NUM_2  SQC
		case 74:
		{
			remove_children( node_ptr, (int[]) {0,2,4} , 3 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_3 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 3);

			child_1 = prune_parse_tree( child_1 );
			child_3 = prune_parse_tree( child_3 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_1;

			ParseTree_Node_attach_sibling(node_ptr, child_3);

			break;
		}

		// 75  (num_val_wm_rem) = EPSILON
		case 75:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 76  (num_val_wom) = NUM
		case 76:

		// 77  (num_val_wom) = RNUM
		case 77:

		// 78  (num_val_wom) = ID
		case 78:

		// 79  (val) = (num_val_wm)
		case 79:

		// 80  (val) = STR
		case 80:

		// 81  (val) = (matrix_literal)
		case 81:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

			break;
		}

		// 82  (call_param_list) = (val)  (call_param_list_rem)
		case 82:
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);

			child_0 = prune_parse_tree( child_0 );
			child_1 = prune_parse_tree( child_1 );

			if(child_1 != NULL)
				ParseTree_Node_attach_child_left_end(node_ptr, child_1);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = AST_OPERATOR_CALL_PARAM_LIST;

			break;
		}

		// 83  (call_param_list_rem) = COMMA  (val)  (call_param_list_rem)
		case 83:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

			ParseTree_Node *child_1 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);

			child_1 = prune_parse_tree( child_1 );
			child_2 = prune_parse_tree( child_2 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_1;

			if(child_2 != NULL)
				ParseTree_Node_attach_sibling(node_ptr, child_2);

			break;
		}

		// 84  (call_param_list_rem) = EPSILON
		case 84:
		{
			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		default:
		{
			break;
		}

	}

	return node_ptr;
}

static int remove_children(ParseTree_Node *node_ptr, int *symbol_indices, int len_symbol_indices){
	for (int i = 0; i < len_symbol_indices; ++i){
		int status = ParseTree_Node_remove_child_by_symbol_index(node_ptr, symbol_indices[i]);
		if (status == -1)
			return -1;
	}
	return 0;
}


///////////
// Print //
///////////

static void print_node(ParseTree_Node *node_ptr, int *index, FILE* file_ptr){
	int symbol = node_ptr->symbol;
	Token *tkn_ptr = node_ptr->tkn_ptr;

	int col[] = {5,16,4,20,20,20,10,20};

	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",		col[0], 		(*index)++);

	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1], ast_operator_to_name(node_ptr->atr_ptr->op));


	if(tkn_ptr != NULL){
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

		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",	col[2],			tkn_ptr->line);
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	buffer_token);
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	buffer_value);
	}
	else{
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"-------------------------");
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"-------------------------");
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"-------------------------");
	}


	if(node_ptr->parent){
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5], ast_operator_to_name(node_ptr->parent->atr_ptr->op));
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

	if(node_ptr->atr_ptr->type != NULL){
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7], type_to_name(((Type *)node_ptr->atr_ptr->type)->type_enum));
	}
	else{
		fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7], "-------------------------");
	}

	fprintf(file_ptr, " |\n");
}

void print_abstract_tree_preorder(ParseTree *tree, FILE *file_ptr){
	int col[] = {5,16,4,20,20,20,10,20};

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
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"Operator");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"Line");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"Lexeme");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"Value");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"Parent");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"Leaf");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"Type");
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
