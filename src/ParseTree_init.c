#include <stdlib.h>

#include "ParseTree_init.h"
#include "Parser_init.h"


////////////////////////
// Private Prototypes //
////////////////////////

static int remove_children(ParseTree_Node *node_ptr, int *symbol_indices, int len_symbol_indices);


////////////////
// Funcations //
////////////////

ParseTree_Node_Attr *ParseTree_Node_Attr_new(){
	ParseTree_Node_Attr *atr_ptr = malloc( sizeof(ParseTree_Node_Attr) );

	atr_ptr->type = NULL;	atr_ptr->type_valid = 0;
	atr_ptr->inh = NULL;	atr_ptr->inh_valid = 0;
	atr_ptr->syn = NULL;	atr_ptr->syn_valid = 0;
	atr_ptr->code = NULL;	atr_ptr->code_valid = 0;

	return atr_ptr;
}


void ParseTree_Node_Attr_destroy(ParseTree_Node_Attr *atr_ptr){
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
					node_ptr->atr_ptr->op = OPERATOR_PLUS;
					break;
				}

				case SYMBOL_T_MINUS:
				{
					node_ptr->atr_ptr->op = OPERATOR_MINUS;
					break;
				}

				case SYMBOL_T_MUL:
				{
					node_ptr->atr_ptr->op = OPERATOR_MUL;
					break;
				}

				case SYMBOL_T_DIV:
				{
					node_ptr->atr_ptr->op = OPERATOR_DIV;
					break;
				}

				case SYMBOL_T_SIZE:
				{
					node_ptr->atr_ptr->op = OPERATOR_SIZE;
					break;
				}

				case SYMBOL_T_ID:
				{
					node_ptr->atr_ptr->op = OPERATOR_ID;
					break;
				}

				case SYMBOL_T_FUNID:
				{
					node_ptr->atr_ptr->op = OPERATOR_FUNID;
					break;
				}

				case SYMBOL_T_KW_INT:
				{
					node_ptr->atr_ptr->op = OPERATOR_KW_INT;
					break;
				}

				case SYMBOL_T_KW_REAL:
				{
					node_ptr->atr_ptr->op = OPERATOR_KW_REAL;
					break;
				}

				case SYMBOL_T_KW_STRING:
				{
					node_ptr->atr_ptr->op = OPERATOR_KW_STRING;
					break;
				}

				case SYMBOL_T_KW_MATRIX:
				{
					node_ptr->atr_ptr->op = OPERATOR_KW_MATRIX;
					break;
				}

				case SYMBOL_T_KW_READ:
				{
					node_ptr->atr_ptr->op = OPERATOR_KW_READ;
					break;
				}

				case SYMBOL_T_KW_PRINT:
				{
					node_ptr->atr_ptr->op = OPERATOR_KW_PRINT;
					break;
				}

				case SYMBOL_T_NUM:
				{
					node_ptr->atr_ptr->op = OPERATOR_NUM;
					break;
				}

				case SYMBOL_T_RNUM:
				{
					node_ptr->atr_ptr->op = OPERATOR_RNUM;
					break;
				}

				case SYMBOL_T_STR:
				{
					node_ptr->atr_ptr->op = OPERATOR_STR;
					break;
				}

				case SYMBOL_T_AND:
				{
					node_ptr->atr_ptr->op = OPERATOR_AND;
					break;
				}

				case SYMBOL_T_OR:
				{
					node_ptr->atr_ptr->op = OPERATOR_OR;
					break;
				}

				case SYMBOL_T_NOT:
				{
					node_ptr->atr_ptr->op = OPERATOR_NOT;
					break;
				}

				case SYMBOL_T_LT:
				{
					node_ptr->atr_ptr->op = OPERATOR_LT;
					break;
				}

				case SYMBOL_T_LE:
				{
					node_ptr->atr_ptr->op = OPERATOR_LE;
					break;
				}

				case SYMBOL_T_GT:
				{
					node_ptr->atr_ptr->op = OPERATOR_GT;
					break;
				}

				case SYMBOL_T_GE:
				{
					node_ptr->atr_ptr->op = OPERATOR_GE;
					break;
				}

				case SYMBOL_T_ASSIGNOP:
				{
					node_ptr->atr_ptr->op = OPERATOR_ASSIGN;
					break;
				}

				case SYMBOL_T_EQ:
				{
					node_ptr->atr_ptr->op = OPERATOR_EQ;
					break;
				}

				case SYMBOL_T_NE:
				{
					node_ptr->atr_ptr->op = OPERATOR_NE;
					break;
				}

				default:
				{
					node_ptr->atr_ptr->op = OPERATOR_UNKNOWN;
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

			node_ptr->atr_ptr->op = OPERATOR_STMT_OR_DEF_LIST;

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
			remove_children( node_ptr, (int[]) {0} , 1 );

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
			child_new->atr_ptr->op = OPERATOR_ID_LIST;

			if(child_2 != NULL)
				ParseTree_Node_attach_sibling(child_1, child_2);
			ParseTree_Node_attach_child_left_end(child_new, child_1);
			ParseTree_Node_attach_child_left_end(node_ptr, child_new);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			// if(child_2 != NULL)
				// ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			// ParseTree_Node_attach_child_left_end(node_ptr, child_1);
			// ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = OPERATOR_DECL;

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

			node_ptr->atr_ptr->op = OPERATOR_ID_LIST;

			break;
		}

		// 15  (id_list) = EPSILON
		case 15:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

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
			remove_children( node_ptr, (int[]) {0} , 1 );

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

			node_ptr->atr_ptr->op = OPERATOR_ID_LIST;

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

			child_1 = prune_parse_tree( child_1 );
			child_2->atr_ptr->inh = child_1;
			child_2 = prune_parse_tree( child_2 );
			child_0 = prune_parse_tree( child_0 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = child_0;

			ParseTree_Node_attach_child_left_end(node_ptr, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, inh);

			break;
		}


		// 28  (expr_addend) = EPSILON
		case 28:

		// 32  (expr_multiplier) = EPSILON
		case 32:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

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

			node_ptr->atr_ptr->op = OPERATOR_MATRIX;

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
				ParseTree_Node_attach_sibling(node_ptr, child_1);

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
			remove_children( node_ptr, (int[]) {0} , 1 );

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
			remove_children( node_ptr, (int[]) {0} , 1 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 46  (call) = FUNID  OP  ID  (id_list_rem)  CL
		case 46:
		{
			remove_children( node_ptr, (int[]) {1,4} , 2 );

			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);
			ParseTree_Node *child_2 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 2);
			ParseTree_Node *child_3 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 3);

			child_0 = prune_parse_tree( child_0 );
			child_2 = prune_parse_tree( child_2 );
			child_3 = prune_parse_tree( child_3 );


			ParseTree_Node *child_new = ParseTree_Node_new(SYMBOL_UNKNOWN, NULL);
			child_new->atr_ptr->op = OPERATOR_ID_LIST;

			if(child_3 != NULL)
				ParseTree_Node_attach_sibling(child_2, child_3);
			ParseTree_Node_attach_child_left_end(child_new, child_2);
			ParseTree_Node_attach_child_left_end(node_ptr, child_new);
			ParseTree_Node_attach_child_left_end(node_ptr, child_0);

			node_ptr->atr_ptr->op = OPERATOR_CALL;

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

			node_ptr->atr_ptr->op = OPERATOR_COND;

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

			node_ptr->atr_ptr->op = OPERATOR_STMT_LIST;

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
			remove_children( node_ptr, (int[]) {0} , 1 );

			ParseTree_Node_destroy(node_ptr);

			node_ptr = NULL;

			break;
		}

		// 53  (stmt_read) = KW_READ  OP  (num_val_wom)  CL  SEMICOLON
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

		// 54  (stmt_print) = KW_PRINT  OP  (num_val_wm)  CL  SEMICOLON
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

		// 60  (expr_bool) = (num_val_wm_1)  (bool_op)  (num_val_wm_2)
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

			node_ptr->atr_ptr->op = OPERATOR_DEF;

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

			node_ptr->atr_ptr->op = OPERATOR_PARAM_LIST;

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

			node_ptr->atr_ptr->op = OPERATOR_PARAM_LIST;

			break;
		}

		// 70  (param_list_rem_1) = EPSILON
		case 70:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

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

			if(child_1 != NULL)
				ParseTree_Node_attach_child_left_end(node_ptr, child_1);

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

			ParseTree_Node_attach_child_left_end(node_ptr, child_3);
			ParseTree_Node_attach_child_left_end(node_ptr, child_1);

			node_ptr->atr_ptr->op = OPERATOR_INDEX;

			break;
		}

		// 75  (num_val_wm_rem) = EPSILON
		case 75:
		{
			remove_children( node_ptr, (int[]) {0} , 1 );

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
		{
			ParseTree_Node *child_0 = ParseTree_Node_detach_child_by_symbol_index(node_ptr, 0);

			ParseTree_Node_destroy(node_ptr);

			child_0 = prune_parse_tree( child_0 );

			node_ptr = child_0;

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
