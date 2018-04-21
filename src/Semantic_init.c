#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedList.h"
#include "Token.h"
#include "ParseTree.h"
#include "SymbolEnv.h"


#include "Token_Data.h"
#include "Ast_init.h"
#include "Type_init.h"


#include "Semantic_init.h"


///////////////
// Constants //
///////////////

#define TEXT_RED	"\x1B[31m"
#define TEXT_GRN	"\x1B[32m"
#define TEXT_YLW	"\x1B[33m"
#define TEXT_BLU	"\x1B[34m"
#define TEXT_BLD	"\x1B[1m"
#define TEXT_RST	"\x1B[0m"


///////////////
// Functions //
///////////////


int Semantic_symbol_and_type_check(ParseTree_Node *node_ptr, SymbolEnv *env_ptr, int *flag_error, int flag_print_errors){

	int status;

	switch(node_ptr->atr_ptr->op)
	{
		case AST_OPERATOR_PLUS:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			// printf("== + ==\n");

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			// printf("   +    ");
			// print_type(child_0->atr_ptr->type);
			// printf("\n");
			// printf("   +    ");
			// print_type(child_1->atr_ptr->type);
			// printf("\n");

			Type *type_0_ptr = child_0->atr_ptr->type;
			Type *type_1_ptr = child_1->atr_ptr->type;



			if(type_0_ptr->type_enum == TYPE_ENUM_STR && type_1_ptr->type_enum == TYPE_ENUM_STR){
				// Not need to check compatibility for string concatenation
			}
			else if(
				type_0_ptr->type_enum != TYPE_ENUM_NUM &&
				type_0_ptr->type_enum != TYPE_ENUM_RNUM &&
				type_0_ptr->type_enum != TYPE_ENUM_MATRIX ||
				type_1_ptr->type_enum != TYPE_ENUM_NUM &&
				type_1_ptr->type_enum != TYPE_ENUM_RNUM &&
				type_1_ptr->type_enum != TYPE_ENUM_MATRIX ||
				Type_check_compatibility(type_0_ptr, type_1_ptr) == -1
			){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Operator " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support operands " TEXT_YLW "%s" TEXT_RST " and " TEXT_YLW "%s" TEXT_RST " of incompatible type\n", "+", type_to_name(type_0_ptr->type_enum), type_to_name(type_1_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			if( Type_check_completeness(type_0_ptr) == -1 || Type_check_completeness(type_1_ptr) == -1 ){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Operator " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support unknown sized operands of incomplete type\n", "+");
				}

				*flag_error = 1;
				return -1;
			}

			Type *type_ptr = Type_clone(type_0_ptr);
			node_ptr->atr_ptr->type = type_ptr;

			if(type_ptr->type_enum == TYPE_ENUM_STR){
				type_ptr->len_string = type_0_ptr->len_string + type_1_ptr->len_string;

				if(type_ptr->len_string > 18){

					if(flag_print_errors == 1){
						int line_number = node_ptr->tkn_ptr->line;
						int column_number = node_ptr->tkn_ptr->column;

						printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
						printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

						printf("String concatenation results in a string longer than 18 characters\n");
					}

					*flag_error = 1;
					return -1;
				}
			}

			// printf("   +    ");
			// print_type(type_ptr);
			// printf("\n");

			break;
		}

		case AST_OPERATOR_MINUS:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			Type *type_0_ptr = child_0->atr_ptr->type;
			Type *type_1_ptr = child_1->atr_ptr->type;

			if(
				type_0_ptr->type_enum != TYPE_ENUM_NUM &&
				type_0_ptr->type_enum != TYPE_ENUM_RNUM &&
				type_0_ptr->type_enum != TYPE_ENUM_MATRIX ||
				type_1_ptr->type_enum != TYPE_ENUM_NUM &&
				type_1_ptr->type_enum != TYPE_ENUM_RNUM &&
				type_1_ptr->type_enum != TYPE_ENUM_MATRIX ||
				Type_check_compatibility(type_0_ptr, type_1_ptr) == -1
			){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Operator " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support operands " TEXT_YLW "%s" TEXT_RST " and " TEXT_YLW "%s" TEXT_RST " of incompatible type\n", "-", type_to_name(type_0_ptr->type_enum), type_to_name(type_1_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			if( Type_check_completeness(type_0_ptr) == -1 || Type_check_completeness(type_1_ptr) == -1 ){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Operator " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support unknown sized operands of incomplete type\n", "-");
				}

				*flag_error = 1;
				return -1;
			}

			node_ptr->atr_ptr->type = Type_clone(type_0_ptr);

			break;
		}

		case AST_OPERATOR_MUL:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			Type *type_0_ptr = child_0->atr_ptr->type;
			Type *type_1_ptr = child_1->atr_ptr->type;

			if(
				type_0_ptr->type_enum != TYPE_ENUM_NUM &&
				type_0_ptr->type_enum != TYPE_ENUM_RNUM ||
				type_1_ptr->type_enum != TYPE_ENUM_NUM &&
				type_1_ptr->type_enum != TYPE_ENUM_RNUM ||
				Type_check_compatibility(type_0_ptr, type_1_ptr) == -1
			){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Operator " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support operands " TEXT_YLW "%s" TEXT_RST " and " TEXT_YLW "%s" TEXT_RST " of incompatible type\n", "*", type_to_name(type_0_ptr->type_enum), type_to_name(type_1_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			// Not required for multiplication
			// if( Type_check_completeness(type_0_ptr) == -1 || Type_check_completeness(type_1_ptr) == -1 ){
			// 	fprintf(stderr, "Incomplete operand types for *\n");
			// 	*flag_error = 1;
			// 	return -1;
			// }

			node_ptr->atr_ptr->type = Type_clone(type_0_ptr);

			break;
		}

		case AST_OPERATOR_DIV:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			Type *type_0_ptr = child_0->atr_ptr->type;
			Type *type_1_ptr = child_1->atr_ptr->type;

			if(
				type_0_ptr->type_enum != TYPE_ENUM_NUM &&
				type_0_ptr->type_enum != TYPE_ENUM_RNUM ||
				type_1_ptr->type_enum != TYPE_ENUM_NUM &&
				type_1_ptr->type_enum != TYPE_ENUM_RNUM ||
				Type_check_compatibility(type_0_ptr, type_1_ptr) == -1
			){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Operator " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support operands " TEXT_YLW "%s" TEXT_RST " and " TEXT_YLW "%s" TEXT_RST " of incompatible type\n", "/", type_to_name(type_0_ptr->type_enum), type_to_name(type_1_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			// Not required for division
			// if( Type_check_completeness(type_0_ptr) == -1 || Type_check_completeness(type_1_ptr) == -1 ){
			// 	fprintf(stderr, "Incomplete operand types for /\n");
			// 	*flag_error = 1;
			// 	return -1;
			// }

			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_RNUM);

			break;
		}

		case AST_OPERATOR_SIZE:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;


			if(Type_check_completeness(type_ptr) == -1){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Cannot get size of uninitialized variable " TEXT_YLW "%*s" TEXT_RST "\n", child_0->tkn_ptr->data->len_string, child_0->tkn_ptr->data->string);
				}

				*flag_error = 1;
				return -1;
			}


			if(type_ptr->type_enum == TYPE_ENUM_STR){
				node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);
			}

			else if(type_ptr->type_enum == TYPE_ENUM_MATRIX){
				node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_LIST);
				Type_add_list_element(node_ptr->atr_ptr->type, Type_new(TYPE_ENUM_NUM));
				Type_add_list_element(node_ptr->atr_ptr->type, Type_new(TYPE_ENUM_NUM));
			}

			else{

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Operator " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support operand " TEXT_YLW "%s" TEXT_RST " of incompatible type\n", "@", type_to_name(type_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			break;
		}

		case AST_OPERATOR_ID:
		{
			char *id = node_ptr->tkn_ptr->data->string;
			int len_id = node_ptr->tkn_ptr->data->len_string;

			// printf("== id: %*s ==\n", len_id, id);

			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
			if(etr_ptr == NULL){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Variable " TEXT_YLW "%*s" TEXT_RST " undeclared in this scope \n", len_id, id);
				}

				*flag_error = 1;
				return -1;
			}

			node_ptr->atr_ptr->entry = etr_ptr;
			node_ptr->atr_ptr->type = Type_clone( SymbolEnv_Entry_get_type(etr_ptr) );

			break;
		}

		case AST_OPERATOR_FUNID:
		{

			break;
		}

		case AST_OPERATOR_KW_INT:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);
			break;
		}

		case AST_OPERATOR_KW_REAL:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_RNUM);
			break;
		}

		case AST_OPERATOR_KW_STRING:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_STR);
			break;
		}

		case AST_OPERATOR_KW_MATRIX:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_MATRIX);
			break;
		}

		case AST_OPERATOR_KW_READ:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;

			if(
				type_ptr->type_enum != TYPE_ENUM_NUM &&
				type_ptr->type_enum != TYPE_ENUM_RNUM
			){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Function " TEXT_GRN TEXT_BLD "%s" TEXT_RST " does not support operand " TEXT_YLW "%s" TEXT_RST " of incompatible type\n", "read()", type_to_name(type_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			SymbolEnv_Entry_set_flag_initialized(child_0->atr_ptr->entry);

			break;
		}

		case AST_OPERATOR_KW_PRINT:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			break;
		}

		case AST_OPERATOR_NUM:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);
			break;
		}

		case AST_OPERATOR_RNUM:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_RNUM);
			break;
		}

		case AST_OPERATOR_STR:
		{
			// printf("== str ==\n");

			Type *type_ptr = Type_new(TYPE_ENUM_STR);
			type_ptr->len_string = node_ptr->tkn_ptr->data->len_string;

			// printf("   str    ");
			// print_type(type_ptr);
			// printf("\n");

			node_ptr->atr_ptr->type = type_ptr;
			break;
		}

		case AST_OPERATOR_ASSIGN:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			// printf("== assign ==\n");

			// Check both rval and lval
			int flag_error_local = 0;

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				flag_error_local = 1;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				flag_error_local = 1;

			if(flag_error_local == 1)
				return -1;

			// printf("   assign    ");
			// print_type(child_0->atr_ptr->type);
			// printf("\n");
			// printf("   assign    ");
			// print_type(child_1->atr_ptr->type);
			// printf("\n");

			Type *type_0_ptr = child_0->atr_ptr->type;
			Type *type_1_ptr = child_1->atr_ptr->type;

			if( Type_check_compatibility(type_0_ptr, type_1_ptr) == -1 ){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Cannot assign value " TEXT_YLW "%s" TEXT_RST " to variable " TEXT_YLW "%s" TEXT_RST " of incompatible type\n", type_to_name(type_1_ptr->type_enum), type_to_name(type_0_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			if( Type_check_completeness(type_1_ptr) == -1 ){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Incomplete type " TEXT_YLW "%s" TEXT_RST "\n", type_to_name(type_1_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}


			// Complete type of lval if incomplete
			if( Type_check_completeness(type_0_ptr) == -1 ){

				if(type_0_ptr->type_enum != TYPE_ENUM_LIST){

					Type *type_0_in_etr_ptr = SymbolEnv_Entry_get_type(child_0->atr_ptr->entry);
					Type *type_1_element_ptr;

					if(type_1_ptr->type_enum == TYPE_ENUM_LIST)
						type_1_element_ptr = LinkedList_peek(type_1_ptr->lst_ptr);
					else if(type_1_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL)
						type_1_element_ptr = LinkedList_peek(type_1_ptr->type_param_out_lst_ptr->lst_ptr);
					else
						type_1_element_ptr = type_1_ptr;


					if(type_0_ptr->type_enum == TYPE_ENUM_STR){
						type_0_ptr->len_string = type_1_element_ptr->len_string;
						type_0_in_etr_ptr->len_string = type_1_element_ptr->len_string;
					}

					else if(type_0_ptr->type_enum == TYPE_ENUM_MATRIX){
						type_0_ptr->num_rows = type_1_element_ptr->num_rows;
						type_0_in_etr_ptr->num_rows = type_1_element_ptr->num_rows;
						type_0_ptr->num_columns = type_1_element_ptr->num_columns;
						type_0_in_etr_ptr->num_columns = type_1_element_ptr->num_columns;
					}
				}

				else if(type_0_ptr->type_enum == TYPE_ENUM_LIST){

					LinkedListIterator *itr_0_ptr = LinkedListIterator_new(type_0_ptr->lst_ptr);
					LinkedListIterator_move_to_first(itr_0_ptr);
					// Type struct of AST_OPERATOR_ID_LIST
					Type *type_id_in_list_ptr = LinkedListIterator_get_item(itr_0_ptr);

					ParseTree_Node *id_node_ptr = child_0->child;
					// Type struct in tree node
					Type *type_id_in_tree_ptr = id_node_ptr->atr_ptr->type;
					// Type struct in symbol table
					Type *type_id_in_entry_ptr = SymbolEnv_Entry_get_type(id_node_ptr->atr_ptr->entry);


					if(type_1_ptr->type_enum == TYPE_ENUM_STR){
						type_id_in_list_ptr->len_string = type_1_ptr->len_string;
						type_id_in_tree_ptr->len_string = type_1_ptr->len_string;
						type_id_in_entry_ptr->len_string = type_1_ptr->len_string;
					}

					else if(type_1_ptr->type_enum == TYPE_ENUM_MATRIX){
						type_id_in_list_ptr->num_rows = type_1_ptr->num_rows;
						type_id_in_tree_ptr->num_rows = type_1_ptr->num_rows;
						type_id_in_entry_ptr->num_rows = type_1_ptr->num_rows;
						type_id_in_list_ptr->num_columns = type_1_ptr->num_columns;
						type_id_in_tree_ptr->num_columns = type_1_ptr->num_columns;
						type_id_in_entry_ptr->num_columns = type_1_ptr->num_columns;
					}

					else{

						LinkedListIterator *itr_1_ptr;

						if(type_1_ptr->type_enum == TYPE_ENUM_LIST)
							itr_1_ptr = LinkedListIterator_new(type_1_ptr->lst_ptr);
						else if(type_1_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL)
							itr_1_ptr = LinkedListIterator_new(type_1_ptr->type_param_out_lst_ptr->lst_ptr);


						Type *type_1_element_ptr = LinkedListIterator_get_item(itr_1_ptr);

						while(type_1_element_ptr != NULL){
							type_id_in_list_ptr = LinkedListIterator_get_item(itr_0_ptr);
							type_id_in_tree_ptr = id_node_ptr->atr_ptr->type;
							type_id_in_entry_ptr = SymbolEnv_Entry_get_type(id_node_ptr->atr_ptr->entry);
							type_1_element_ptr = LinkedListIterator_get_item(itr_1_ptr);


							if(type_1_element_ptr->type_enum == TYPE_ENUM_STR){
								type_id_in_list_ptr->len_string = type_1_element_ptr->len_string;
								type_id_in_tree_ptr->len_string = type_1_element_ptr->len_string;
								type_id_in_entry_ptr->len_string = type_1_element_ptr->len_string;
							}

							else if(type_1_element_ptr->type_enum == TYPE_ENUM_MATRIX){
								type_id_in_list_ptr->num_rows = type_1_element_ptr->num_rows;
								type_id_in_tree_ptr->num_rows = type_1_element_ptr->num_rows;
								type_id_in_entry_ptr->num_rows = type_1_element_ptr->num_rows;
								type_id_in_list_ptr->num_columns = type_1_element_ptr->num_columns;
								type_id_in_tree_ptr->num_columns = type_1_element_ptr->num_columns;
								type_id_in_entry_ptr->num_columns = type_1_element_ptr->num_columns;
							}


							LinkedListIterator_move_to_next(itr_0_ptr);
							id_node_ptr = id_node_ptr->sibling;
							LinkedListIterator_move_to_next(itr_1_ptr);
						}

						LinkedListIterator_destroy(itr_1_ptr);
					}

					LinkedListIterator_destroy(itr_0_ptr);

				}
			}


			// Set initialized flag
			if(type_0_ptr->type_enum != TYPE_ENUM_LIST){
				SymbolEnv_Entry_set_flag_initialized(child_0->atr_ptr->entry);
			}
			else{
				ParseTree_Node *id_node_ptr = child_0->child;
				while(id_node_ptr != NULL){
					SymbolEnv_Entry_set_flag_initialized(id_node_ptr->atr_ptr->entry);
					id_node_ptr = id_node_ptr->sibling;
				}
			}

			break;
		}

		case AST_OPERATOR_AND:
		case AST_OPERATOR_OR:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			break;
		}

		case AST_OPERATOR_NOT:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			break;
		}

		case AST_OPERATOR_LT:
		case AST_OPERATOR_LE:
		case AST_OPERATOR_GT:
		case AST_OPERATOR_GE:
		case AST_OPERATOR_EQ:
		case AST_OPERATOR_NE:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			Type *type_0_ptr = child_0->atr_ptr->type;
			Type *type_1_ptr = child_1->atr_ptr->type;

			if(
				( type_0_ptr->type_enum != TYPE_ENUM_NUM && type_0_ptr->type_enum != TYPE_ENUM_RNUM ) ||
				( type_1_ptr->type_enum != TYPE_ENUM_NUM && type_1_ptr->type_enum != TYPE_ENUM_RNUM )
			){
				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Cannot compare value of incompatible type(s)");
					if(type_0_ptr->type_enum != TYPE_ENUM_NUM && type_0_ptr->type_enum != TYPE_ENUM_RNUM)
						printf( TEXT_YLW " %s" TEXT_RST, type_to_name(type_0_ptr->type_enum));
					if(type_1_ptr->type_enum != TYPE_ENUM_NUM && type_1_ptr->type_enum != TYPE_ENUM_RNUM)
						printf( TEXT_YLW " %s" TEXT_RST, type_to_name(type_1_ptr->type_enum));
				}

				*flag_error = 1;
				return -1;
			}

			break;
		}

		case AST_OPERATOR_CALL:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			char *name = child_0->tkn_ptr->data->string;
			int len_name = child_0->tkn_ptr->data->len_string;

			// printf("== call: %*s ==\n", len_name, name);

			SymbolEnv_Entry *etr_def_ptr = SymbolEnv_entry_get_by_id(env_ptr, name, len_name);
			if(etr_def_ptr == NULL){

				if(flag_print_errors == 1){
					int line_number = child_0->tkn_ptr->line;
					int column_number = child_0->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Function " TEXT_YLW "%*s" TEXT_RST " undeclared in this scope \n", len_name, name);
				}

				*flag_error = 1;
				return -1;
			}


			// Check recursion
			SymbolEnv_Scope *scp_ptr = SymbolEnv_scope_get_current(env_ptr);
			while(1){

				if( SymbolEnv_Scope_entry_get_by_id(scp_ptr, name, len_name) != NULL ){
					break;
				}

				if( strncmp( SymbolEnv_Scope_get_name(scp_ptr), name, len_name) == 0 ){

					if(flag_print_errors == 1){
						int line_number = child_0->tkn_ptr->line;
						int column_number = child_0->tkn_ptr->column;

						printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
						printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

						printf("Function " TEXT_YLW "%*s" TEXT_RST " called recursively \n", len_name, name);
					}

					*flag_error = 1;
					return -1;
				}

				scp_ptr = SymbolEnv_Scope_get_parent(scp_ptr);
				if(scp_ptr == NULL){
					break;
				}
			}


			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;


			Type *type_def_ptr = SymbolEnv_Entry_get_type(etr_def_ptr);

			// printf("   call: ");
			// print_type(child_1->atr_ptr->type);
			// printf("\n");
			// printf("   call: ");
			// print_type(type_def_ptr->type_param_in_lst_ptr);
			// printf("\n");

			if( Type_check_compatibility(child_1->atr_ptr->type, type_def_ptr->type_param_in_lst_ptr) == -1 ){

				if(flag_print_errors == 1){
					int line_number = child_0->tkn_ptr->line;
					int column_number = child_0->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Function " TEXT_YLW "%*s" TEXT_RST " called with incorrect arguments\n", len_name, name);
				}

				*flag_error = 1;
				return -1;
			}

			Type *type_ptr = Type_new(TYPE_ENUM_FUNCTION_CALL);
			Type_set_function_param_in_lst(type_ptr, child_1->atr_ptr->type);
			child_1->atr_ptr->type = NULL;
			Type_set_function_param_out_lst(type_ptr, Type_clone(type_def_ptr->type_param_out_lst_ptr));

			node_ptr->atr_ptr->type = type_ptr;

			break;
		}


		case AST_OPERATOR_COND:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_get_child_by_node_index(node_ptr, 2);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			if(child_2 != NULL){
				status = Semantic_symbol_and_type_check(child_2, env_ptr, flag_error, flag_print_errors);
				if(status == -1)
					return status;
			}

			break;
		}

		case AST_OPERATOR_DECL:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;

			ParseTree_Node *id_node_ptr = child_1->child;
			while(id_node_ptr){
				char *id = id_node_ptr->tkn_ptr->data->string;
				int len_id = id_node_ptr->tkn_ptr->data->len_string;

				Type *type_id_ptr = Type_clone(type_ptr);

				SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_add(env_ptr, id, len_id, type_id_ptr);
				if(etr_ptr == NULL){

					if(flag_print_errors == 1){
						int line_number = id_node_ptr->tkn_ptr->line;
						int column_number = id_node_ptr->tkn_ptr->column;

						printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
						printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

						printf("Redeclaration of variable " TEXT_YLW "%*s" TEXT_RST " in this scope\n", len_id, id);

						// etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
						// printf( TEXT_BLD TEXT_BLU "note: " TEXT_RST);
						// printf("Variable " TEXT_YLW "%*s" TEXT_RST " was last declared at " TEXT_BLD "%d:%d" TEXT_RST "\n", len_id, id, line_number, column_number);
					}

					*flag_error = 1;
				}
				id_node_ptr->atr_ptr->entry = etr_ptr;

				id_node_ptr = id_node_ptr->sibling;
			}

			break;
		}

		case AST_OPERATOR_DEF:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_get_child_by_node_index(node_ptr, 2);
			ParseTree_Node *child_3 = ParseTree_Node_get_child_by_node_index(node_ptr, 3);

			char *name = child_1->tkn_ptr->data->string;
			int len_name = child_1->tkn_ptr->data->len_string;

			// printf("== def: %*s ==\n", len_name, name);

			Type *type_ptr = Type_new(TYPE_ENUM_FUNCTION_DEF);

			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_add(env_ptr, name, len_name, type_ptr);
			// printf("   def: %p %p\n", type_ptr, etr_ptr);
			if(etr_ptr == NULL){

				if(flag_print_errors == 1){
					int line_number = child_1->tkn_ptr->line;
					int column_number = child_1->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Redeclaration of function " TEXT_YLW "%*s" TEXT_RST " in this scope\n", len_name, name);

					// etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
					// printf( TEXT_BLD TEXT_BLU "note: " TEXT_RST);
					// printf("Variable " TEXT_YLW "%*s" TEXT_RST " was last declared at " TEXT_BLD "%d:%d" TEXT_RST "\n", len_id, id, line_number, column_number);
				}

				*flag_error = 1;
				return -1;
			}
			SymbolEnv_Entry_set_flag_initialized(etr_ptr);
			// printf("%s : %d\n", name, SymbolEnv_Entry_get_flag_initialized(etr_ptr));

			SymbolEnv_scope_add(env_ptr, name, len_name);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_2, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			// Set ids in param in list as initialized
			ParseTree_Node *param_node_ptr = child_2;
			while(param_node_ptr != NULL){
				ParseTree_Node *id_node_ptr = ParseTree_Node_get_child_by_node_index(param_node_ptr, 1);
				SymbolEnv_entry_set_flag_initialized_by_id(env_ptr, id_node_ptr->tkn_ptr->data->string, id_node_ptr->tkn_ptr->data->len_string);
				param_node_ptr = ParseTree_Node_get_child_by_node_index(param_node_ptr, 2);
			}

			Type_set_function_param_out_lst(type_ptr, child_0->atr_ptr->type);
			child_0->atr_ptr->type = NULL;
			Type_set_function_param_in_lst(type_ptr, child_2->atr_ptr->type);
			child_2->atr_ptr->type = NULL;

			// print_type(type_ptr);
			// printf("\n");

			status = Semantic_symbol_and_type_check(child_3, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;


			// Check if all variables declared in this scope have been initialized
			SymbolEnv_Scope *scp_ptr = SymbolEnv_scope_get_current(env_ptr);
			LinkedList *id_lst_ptr = SymbolEnv_Scope_get_id_lst(scp_ptr);

			LinkedListIterator *itr_ptr = LinkedListIterator_new(id_lst_ptr);
			LinkedListIterator_move_to_first(itr_ptr);
			char *id = LinkedListIterator_get_item(itr_ptr);

			while(id){
				if( SymbolEnv_entry_get_flag_initialized_by_id(env_ptr, id, strlen(id)) == 0 ){

					if(flag_print_errors == 1){
						// int line_number = child_1->tkn_ptr->line;
						// int column_number = child_1->tkn_ptr->column;

						// printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
						printf( TEXT_BLD TEXT_BLU "semantic warning: " TEXT_RST);

						printf("Variable " TEXT_YLW "%s" TEXT_RST " in scope " TEXT_YLW "%s" TEXT_RST " is not initialized\n", id, SymbolEnv_Scope_get_name(scp_ptr));

						// etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
						// printf( TEXT_BLD TEXT_BLU "note: " TEXT_RST);
						// printf("Variable " TEXT_YLW "%*s" TEXT_RST " was last declared at " TEXT_BLD "%d:%d" TEXT_RST "\n", len_id, id, line_number, column_number);
					}

					*flag_error = -1;
				}

				LinkedListIterator_move_to_next(itr_ptr);
				id = LinkedListIterator_get_item(itr_ptr);
			}

			LinkedListIterator_destroy(itr_ptr);


			scp_ptr = SymbolEnv_scope_exit(env_ptr);
			if(scp_ptr == NULL){
				// fprintf(stderr, "Error exiting scope %s\n", SymbolEnv_Scope_get_name( SymbolEnv_scope_get_current(env_ptr) ) );
				fprintf(stderr, "Semantic_symbol_and_type_check : Error exiting scope\n");
				*flag_error = 1;
				return -1;
			}

			break;
		}

		case AST_OPERATOR_ID_LIST:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			Type *type_ptr = Type_new(TYPE_ENUM_LIST);
			node_ptr->atr_ptr->type = type_ptr;

			ParseTree_Node *id_node_ptr = child_0;
			while(id_node_ptr != NULL){
				char *id = id_node_ptr->tkn_ptr->data->string;
				int len_id = id_node_ptr->tkn_ptr->data->len_string;

				SymbolEnv_Entry *id_etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
				if(id_etr_ptr == NULL){

					if(flag_print_errors == 1){
						int line_number = id_node_ptr->tkn_ptr->line;
						int column_number = id_node_ptr->tkn_ptr->column;

						printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
						printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

						printf("Variable " TEXT_YLW "%*s" TEXT_RST " undeclared in this scope \n", len_id, id);
					}

					*flag_error = 1;
					return -1;
				}

				Type *type_id_ptr = SymbolEnv_Entry_get_type(id_etr_ptr);
				Type_add_list_element(type_ptr, Type_clone(type_id_ptr));

				id_node_ptr->atr_ptr->entry = id_etr_ptr;
				id_node_ptr->atr_ptr->type = Type_clone(type_id_ptr);

				id_node_ptr = id_node_ptr->sibling;
			}

			break;
		}

		case AST_OPERATOR_MATRIX:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			int num_rows = 0;
			int num_columns = 0;

			ParseTree_Node *row_node_ptr = child_0;
			ParseTree_Node *column_node_ptr = row_node_ptr;
			ParseTree_Node *prev_column_node_ptr = NULL;

			// Find number of columns in first row for reference
			while(column_node_ptr != NULL){
				num_columns += 1;
				column_node_ptr = column_node_ptr->sibling;
			}

			while(row_node_ptr != NULL){
				column_node_ptr = row_node_ptr;

				int num_columns_cur = 0;
				while(column_node_ptr != NULL){
					num_columns_cur += 1;
					prev_column_node_ptr = column_node_ptr;
					column_node_ptr = column_node_ptr->sibling;
				}

				if(num_columns_cur != num_columns){

					if(flag_print_errors == 1){
						int line_number = prev_column_node_ptr->tkn_ptr->line;
						int column_number = prev_column_node_ptr->tkn_ptr->column;

						printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
						printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

						printf("Expected " TEXT_YLW "%d" TEXT_RST " numbers in each matrix row\n", num_columns);
					}

					*flag_error = 1;
					return -1;
				}

				num_rows += 1;
				row_node_ptr = row_node_ptr->child;
			}

			Type *type_ptr = Type_new(TYPE_ENUM_MATRIX);
			Type_set_matrix_len(type_ptr, num_rows, num_columns);

			node_ptr->atr_ptr->type = type_ptr;

			break;
		}

		case AST_OPERATOR_MATRIX_ELEMENT:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			char *id = node_ptr->tkn_ptr->data->string;
			int len_id = node_ptr->tkn_ptr->data->len_string;

			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
			if(etr_ptr == NULL){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Variable " TEXT_YLW "%*s" TEXT_RST " undeclared in this scope \n", len_id, id);
				}

				*flag_error = 1;
				return -1;
			}


			Type *type_ptr = SymbolEnv_Entry_get_type(etr_ptr);
			if(type_ptr->type_enum != TYPE_ENUM_MATRIX){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Variable " TEXT_YLW "%*s" TEXT_RST " of type " TEXT_YLW "%s" TEXT_RST " not subscriptable\n", len_id, id, type_to_name(type_ptr->type_enum) );
				}

				*flag_error = 1;
				return -1;
			}


			if(Type_check_completeness(type_ptr) == -1){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Cannot subscript uninitialized matrix " TEXT_YLW "%s" TEXT_RST "\n", id);
				}

				*flag_error = 1;
				return -1;
			}


			char* subscript_row_string = child_0->tkn_ptr->data->string;
			char* subscript_column_string = child_1->tkn_ptr->data->string;

			int subscript_row = strtol(subscript_row_string, NULL, 10);
			int subscript_column = strtol(subscript_column_string, NULL, 10);

			// Cvt 1 indexed to 0 indexed
			subscript_row--;
			subscript_column--;

			int num_rows = type_ptr->num_rows;
			int num_columns = type_ptr->num_columns;


			if(subscript_row >= num_rows || subscript_column >= num_columns){

				if(flag_print_errors == 1){
					int line_number = node_ptr->tkn_ptr->line;
					int column_number = node_ptr->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Subscript " TEXT_YLW "[%d,%d]" TEXT_RST " out of bound for matrix " TEXT_YLW "%s" TEXT_RST " of size " TEXT_YLW "[%d,%d]" TEXT_RST " \n", subscript_row, subscript_column, id, num_rows, num_columns);
				}

				*flag_error = 1;
				return -1;
			}


			node_ptr->atr_ptr->entry = etr_ptr;
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);

			break;
		}

		case AST_OPERATOR_DEF_PARAM_LIST:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_get_child_by_node_index(node_ptr, 2);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error, flag_print_errors);
			if(status == -1)
				return status;

			// Add symbol to symbol table
			char *id = child_1->tkn_ptr->data->string;
			int len_id = child_1->tkn_ptr->data->len_string;

			Type *type_id_ptr = Type_clone(child_0->atr_ptr->type);

			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_add(env_ptr, id, len_id, type_id_ptr);
			if(etr_ptr == NULL){

				if(flag_print_errors == 1){
					int line_number = child_1->tkn_ptr->line;
					int column_number = child_1->tkn_ptr->column;

					printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_RED "semantic error: " TEXT_RST);

					printf("Redeclaration of variable " TEXT_YLW "%*s" TEXT_RST " in this scope\n", len_id, id);

					// etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
					// printf( TEXT_BLD TEXT_BLU "note: " TEXT_RST);
					// printf("Variable " TEXT_YLW "%*s" TEXT_RST " was last declared at " TEXT_BLD "%d:%d" TEXT_RST "\n", len_id, id, line_number, column_number);
				}

				*flag_error = 1;
				return -1;
			}

			// Extend list of types
			if(child_2 == NULL){
				node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_LIST);

				Type_add_list_element_front(node_ptr->atr_ptr->type, child_0->atr_ptr->type);
				child_0->atr_ptr->type = NULL;
			}

			else{
				status = Semantic_symbol_and_type_check(child_2, env_ptr, flag_error, flag_print_errors);
				if(status == -1)
					return status;

				node_ptr->atr_ptr->type = child_2->atr_ptr->type;
				child_2->atr_ptr->type = NULL;

				Type_add_list_element_front(node_ptr->atr_ptr->type, child_0->atr_ptr->type);
				child_0->atr_ptr->type = NULL;
			}

			break;
		}

		case AST_OPERATOR_CALL_PARAM_LIST:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			// printf("== call_param_list: ==\n");

			Type *type_ptr = Type_new(TYPE_ENUM_LIST);
			node_ptr->atr_ptr->type = type_ptr;

			ParseTree_Node *val_node_ptr = child_0;
			while(val_node_ptr != NULL){

				status = Semantic_symbol_and_type_check(val_node_ptr, env_ptr, flag_error, flag_print_errors);
				if(status == -1)
					return status;

				Type *type_val_ptr = val_node_ptr->atr_ptr->type;
				Type_add_list_element(type_ptr, Type_clone(type_val_ptr));;

				// printf("   call_param_list: ");
				// print_type(val_node_ptr->atr_ptr->type);
				// printf("\t");
				// print_type(type_ptr);
				// printf("\n");

				val_node_ptr = val_node_ptr->sibling;
			}

			break;
		}

		case AST_OPERATOR_STMT_LIST:
		case AST_OPERATOR_STMT_OR_DEF_LIST:
		{
			ParseTree_Node *child = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			while(child){
				status = Semantic_symbol_and_type_check(child, env_ptr, flag_error, flag_print_errors);
				if(status == -1){
					// Continue to check for further errors
				}

				child = child->sibling;
			}

			break;
		}

		default:
		{
			fprintf(stderr, "Semantic_symbol_and_type_check : Unhandled case\n");
			return -1;

			break;
		}
	}

	return 0;
}

int Semantic_initialized_check(SymbolEnv *env_ptr, int flag_print_errors){
	int flag_initialized = 0;
	SymbolEnv_Scope *scp_ptr = SymbolEnv_scope_reset(env_ptr);

	while(1){
		SymbolEnv_scope_set_explicit(env_ptr, scp_ptr);
		// printf("%s\n", SymbolEnv_Scope_get_name(scp_ptr));

		LinkedList *id_lst_ptr = SymbolEnv_Scope_get_id_lst(scp_ptr);

		LinkedListIterator *itr_ptr = LinkedListIterator_new(id_lst_ptr);
		LinkedListIterator_move_to_first(itr_ptr);
		char *id = LinkedListIterator_get_item(itr_ptr);

		while(id){
			if( SymbolEnv_entry_get_flag_initialized_by_id(env_ptr, id, strlen(id)) == 0 ){

				if(flag_print_errors == 1){
					// int line_number = child_1->tkn_ptr->line;
					// int column_number = child_1->tkn_ptr->column;

					// printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
					printf( TEXT_BLD TEXT_BLU "semantic warning: " TEXT_RST);

					printf("Variable " TEXT_YLW "%s" TEXT_RST " in scope " TEXT_YLW "%s" TEXT_RST " never initialized\n", id, SymbolEnv_Scope_get_name(scp_ptr));

					// etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
					// printf( TEXT_BLD TEXT_BLU "note: " TEXT_RST);
					// printf("Variable " TEXT_YLW "%*s" TEXT_RST " was last declared at " TEXT_BLD "%d:%d" TEXT_RST "\n", len_id, id, line_number, column_number);
				}

				flag_initialized = -1;
			}

			LinkedListIterator_move_to_next(itr_ptr);
			id = LinkedListIterator_get_item(itr_ptr);
		}

		LinkedListIterator_destroy(itr_ptr);
		
		scp_ptr = SymbolEnv_Scope_get_inorder(scp_ptr);
		if(scp_ptr == NULL)
			break;
	}

	return flag_initialized;
}

int Semantic_root_scope_initialized_check(SymbolEnv *env_ptr, int flag_print_errors){
	int flag_initialized = 0;

	SymbolEnv_Scope *scp_ptr = SymbolEnv_scope_get_root(env_ptr);
	LinkedList *id_lst_ptr = SymbolEnv_Scope_get_id_lst(scp_ptr);

	LinkedListIterator *itr_ptr = LinkedListIterator_new(id_lst_ptr);
	LinkedListIterator_move_to_first(itr_ptr);
	char *id = LinkedListIterator_get_item(itr_ptr);

	while(id){
		if( SymbolEnv_entry_get_flag_initialized_by_id(env_ptr, id, strlen(id)) == 0 ){

			if(flag_print_errors == 1){
				// int line_number = child_1->tkn_ptr->line;
				// int column_number = child_1->tkn_ptr->column;

				// printf( TEXT_BLD "%d:%d: " TEXT_RST, line_number, column_number);
				printf( TEXT_BLD TEXT_BLU "semantic warning: " TEXT_RST);

				printf("Variable " TEXT_YLW "%s" TEXT_RST " in scope " TEXT_YLW "%s" TEXT_RST " never initialized\n", id, SymbolEnv_Scope_get_name(scp_ptr));

				// etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
				// printf( TEXT_BLD TEXT_BLU "note: " TEXT_RST);
				// printf("Variable " TEXT_YLW "%*s" TEXT_RST " was last declared at " TEXT_BLD "%d:%d" TEXT_RST "\n", len_id, id, line_number, column_number);
			}

			flag_initialized = -1;
		}

		LinkedListIterator_move_to_next(itr_ptr);
		id = LinkedListIterator_get_item(itr_ptr);
	}

	LinkedListIterator_destroy(itr_ptr);

	return flag_initialized;
}

void print_symbol_environment(SymbolEnv *env_ptr, FILE *file_ptr){
	int col[] = {5,20,25,5,25,13,5,10};

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"------------------------------");
	fprintf(file_ptr, "--\n");

	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"S No");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"id");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"Scope");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"Nesting level");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"Parent scope");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"Type");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"Width");
	fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"Offset");
	fprintf(file_ptr, " |\n");

	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"------------------------------");
	fprintf(file_ptr, "--\n");


	SymbolEnv_Scope *scp_ptr = SymbolEnv_scope_reset(env_ptr);

	int index = 0;

	while(1){
		SymbolEnv_scope_set_explicit(env_ptr, scp_ptr);
		// printf("%s\n", SymbolEnv_Scope_get_name(scp_ptr));

		LinkedList *id_lst_ptr = SymbolEnv_Scope_get_id_lst(scp_ptr);

		LinkedListIterator *itr_ptr = LinkedListIterator_new(id_lst_ptr);
		LinkedListIterator_move_to_first(itr_ptr);
		char *id = LinkedListIterator_get_item(itr_ptr);

		while(id){
			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, strlen(id));
			Type *type_ptr = SymbolEnv_Entry_get_type(etr_ptr);

			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",	col[0], 		++index);
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	id);
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "scope%-*.*s",	col[2]-5,col[2]-5,	SymbolEnv_Scope_get_name(scp_ptr));
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",	col[3],			SymbolEnv_Scope_get_nesting_level(scp_ptr)+1);

			if( SymbolEnv_Scope_get_parent(scp_ptr) != NULL ){
				fprintf(file_ptr, " | ");	fprintf(file_ptr, "scope%-*.*s",	col[4]-5,col[4]-5,	SymbolEnv_Scope_get_name(SymbolEnv_Scope_get_parent(scp_ptr)) );
			}
			else{
				fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"------------------------------");
			}

			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	type_to_name(type_ptr->type_enum) );
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",	col[6],			SymbolEnv_Entry_get_size(etr_ptr) );
			fprintf(file_ptr, " | ");	fprintf(file_ptr, "%*d",	col[7],			SymbolEnv_Entry_get_offset(etr_ptr) );
			fprintf(file_ptr, " |\n");

			LinkedListIterator_move_to_next(itr_ptr);
			id = LinkedListIterator_get_item(itr_ptr);
		}

		LinkedListIterator_destroy(itr_ptr);

		scp_ptr = SymbolEnv_Scope_get_inorder(scp_ptr);
		if(scp_ptr == NULL)
			break;
	}


	fprintf(file_ptr, " --");	fprintf(file_ptr, "%-*.*s",	col[0],col[0],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[1],col[1],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[2],col[2],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[3],col[3],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[4],col[4],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[5],col[5],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[6],col[6],	"------------------------------");
	fprintf(file_ptr, "---");	fprintf(file_ptr, "%-*.*s",	col[7],col[7],	"------------------------------");
	fprintf(file_ptr, "--\n");
}
