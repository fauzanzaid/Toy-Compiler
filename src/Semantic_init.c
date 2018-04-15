#include <stdio.h>


#include "Token.h"
#include "ParseTree.h"
#include "SymbolEnv.h"


#include "Token_Data.h"
#include "Ast_init.h"
#include "Type_init.h"
#include "Semantic_init.h"


///////////////
// Functions //
///////////////


int Semantic_symbol_and_type_check(ParseTree_Node *node_ptr, SymbolEnv *env_ptr, int *flag_error){

	int status;

	switch(node_ptr->atr_ptr->op)
	{
		case OPERATOR_PLUS:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			// printf("== + ==\n");

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
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
				fprintf(stderr, "Incompatible operand types for +\n");
				*flag_error = 1;
				return -1;
			}

			if( Type_check_completeness(type_0_ptr) == -1 || Type_check_completeness(type_1_ptr) == -1 ){
				fprintf(stderr, "Incomplete operand types for +\n");
				*flag_error = 1;
				return -1;
			}

			Type *type_ptr = Type_clone(type_0_ptr);

			if(type_ptr->type_enum == TYPE_ENUM_STR){
				type_ptr->len_string = type_0_ptr->len_string + type_1_ptr->len_string;
			}

			// printf("   +    ");
			// print_type(type_ptr);
			// printf("\n");

			node_ptr->atr_ptr->type = type_ptr;

			break;
		}

		case OPERATOR_MINUS:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
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
				fprintf(stderr, "Incompatible operand types for -\n");
				*flag_error = 1;
				return -1;
			}

			if( Type_check_completeness(type_0_ptr) == -1 || Type_check_completeness(type_1_ptr) == -1 ){
				fprintf(stderr, "Incomplete operand types for -\n");
				*flag_error = 1;
				return -1;
			}

			node_ptr->atr_ptr->type = Type_clone(type_0_ptr);

			break;
		}

		case OPERATOR_MUL:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
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
				fprintf(stderr, "Incompatible operand types for *\n");
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

		case OPERATOR_DIV:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
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
				fprintf(stderr, "Incompatible operand types for /\n");
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

		case OPERATOR_SIZE:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;
			child_0->atr_ptr->type = NULL;

			if(type_ptr->type_enum == TYPE_ENUM_STR){
				node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);

				Type_destroy(type_ptr);
			}

			else if(type_ptr->type_enum == TYPE_ENUM_MATRIX){
				node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_LIST);
				Type_add_list_element(node_ptr->atr_ptr->type, Type_new(TYPE_ENUM_NUM));
				Type_add_list_element(node_ptr->atr_ptr->type, Type_new(TYPE_ENUM_NUM));

				Type_destroy(type_ptr);
			}

			else{
				fprintf(stderr, "Incompatible type for @\n");
				Type_destroy(type_ptr);

				*flag_error = 1;
				return -1;
			}

			break;
		}

		case OPERATOR_ID:
		{
			char *id = node_ptr->tkn_ptr->data->string;
			int len_id = node_ptr->tkn_ptr->data->len_string;

			// printf("== id: %*s ==\n", len_id, id);

			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
			if(etr_ptr == NULL){
				fprintf(stderr, "Symbol %*s not declared in this scope\n", len_id, id);
				*flag_error = 1;
				return -1;
			}

			node_ptr->atr_ptr->entry = etr_ptr;
			node_ptr->atr_ptr->type = Type_clone( SymbolEnv_Entry_get_type(etr_ptr) );

			break;
		}

		case OPERATOR_FUNID:
		{

			break;
		}

		case OPERATOR_KW_INT:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);
			break;
		}

		case OPERATOR_KW_REAL:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_RNUM);
			break;
		}

		case OPERATOR_KW_STRING:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_STR);
			break;
		}

		case OPERATOR_KW_MATRIX:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_MATRIX);
			break;
		}

		case OPERATOR_KW_READ:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;
			child_0->atr_ptr->type = NULL;

			if(
				type_ptr->type_enum != TYPE_ENUM_NUM &&
				type_ptr->type_enum != TYPE_ENUM_RNUM
			){
				fprintf(stderr, "Incompatible type for read()\n");
				Type_destroy(type_ptr);

				*flag_error = 1;
				return -1;
			}

			Type_destroy(type_ptr);

			break;
		}

		case OPERATOR_KW_PRINT:
		{
			// No check required
			break;
		}

		case OPERATOR_NUM:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);
			break;
		}

		case OPERATOR_RNUM:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_RNUM);
			break;
		}

		case OPERATOR_STR:
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

		case OPERATOR_ASSIGN:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			// printf("== assign ==\n");

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
			if(status == -1)
				return status;

			// printf("   assign    ");
			// print_type(child_0->atr_ptr->type);
			// printf("\n");
			// printf("   assign    ");
			// print_type(child_1->atr_ptr->type);
			// printf("\n");

			Type *type_0_ptr = child_0->atr_ptr->type;
			// child_0->atr_ptr->type = NULL;
			Type *type_1_ptr = child_1->atr_ptr->type;
			// child_1->atr_ptr->type = NULL;

			if( Type_check_compatibility(type_0_ptr, type_1_ptr) == -1 ){
				fprintf(stderr, "Incompatible operand types for =\n");
				*flag_error = 1;
				return -1;
			}

			if( Type_check_completeness(type_1_ptr) == -1 ){
				fprintf(stderr, "Incomplete type of rval for =\n");
				*flag_error = 1;
				return -1;
			}

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

					else if(type_0_ptr->type_enum == TYPE_ENUM_STR){
						type_0_ptr->num_rows = type_1_element_ptr->num_rows;
						type_0_in_etr_ptr->num_rows = type_1_element_ptr->num_rows;
						type_0_ptr->num_columns = type_1_element_ptr->num_columns;
						type_0_in_etr_ptr->num_columns = type_1_element_ptr->num_columns;
					}
				}

				else if(type_0_ptr->type_enum == TYPE_ENUM_LIST){

					LinkedListIterator *itr_0_ptr = LinkedListIterator_new(type_0_ptr->lst_ptr);
					LinkedListIterator_move_to_first(itr_0_ptr);
					// Type struct of OPERATOR_ID_LIST
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

			break;
		}

		case OPERATOR_AND:
		case OPERATOR_OR:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
			if(status == -1)
				return status;

			break;
		}

		case OPERATOR_NOT:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			break;
		}

		case OPERATOR_LT:
		case OPERATOR_LE:
		case OPERATOR_GT:
		case OPERATOR_GE:
		case OPERATOR_EQ:
		case OPERATOR_NE:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
			if(status == -1)
				return status;

			Type *type_0_ptr = child_0->atr_ptr->type;
			Type *type_1_ptr = child_1->atr_ptr->type;

			if(
				( type_0_ptr->type_enum != TYPE_ENUM_NUM && type_0_ptr->type_enum != TYPE_ENUM_RNUM ) ||
				( type_1_ptr->type_enum != TYPE_ENUM_NUM && type_1_ptr->type_enum != TYPE_ENUM_RNUM )
			){
				fprintf(stderr, "Incompatible types for boolean operators\n");
				*flag_error = 1;
				return -1;
			}

			break;
		}

		case OPERATOR_CALL:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			char *name = child_0->tkn_ptr->data->string;
			int len_name = child_0->tkn_ptr->data->len_string;

			// printf("== call: %*s ==\n", len_name, name);

			SymbolEnv_Entry *etr_def_ptr = SymbolEnv_entry_get_by_id(env_ptr, name, len_name);
			if(etr_def_ptr == NULL){
				fprintf(stderr, "Function %*s not declared in this scope\n", len_name, name);
				*flag_error = 1;
				return -1;
			}

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
			if(status == -1)
				return status;

			Type *type_def_ptr = SymbolEnv_Entry_get_type(etr_def_ptr);

			// print_type(child_1->atr_ptr->type);
			// printf("\n");
			// print_type(type_def_ptr->type_param_in_lst_ptr);
			// printf("\n");

			if( Type_check_compatibility(child_1->atr_ptr->type, type_def_ptr->type_param_in_lst_ptr) == -1 ){
				fprintf(stderr, "Function %*s called with incorrect arguments\n", len_name, name);
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


		case OPERATOR_COND:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_get_child_by_node_index(node_ptr, 2);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_1, env_ptr, flag_error);
			if(status == -1)
				return status;

			if(child_2 != NULL){
				status = Semantic_symbol_and_type_check(child_2, env_ptr, flag_error);
				if(status == -1)
					return status;
			}

			break;
		}

		case OPERATOR_DECL:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;

			ParseTree_Node *id_node_ptr = child_1->child;
			while(id_node_ptr){
				char *id = id_node_ptr->tkn_ptr->data->string;
				int len_id = id_node_ptr->tkn_ptr->data->len_string;

				Type *type_id_ptr = Type_clone(type_ptr);

				SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_add(env_ptr, id, len_id, 0, type_id_ptr);
				if(etr_ptr == NULL){
					fprintf(stderr, "Symbol %*s already declared\n", len_id, id);
					*flag_error = 1;
				}
				id_node_ptr->atr_ptr->entry = etr_ptr;

				id_node_ptr = id_node_ptr->sibling;
			}

			break;
		}

		case OPERATOR_DEF:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_get_child_by_node_index(node_ptr, 2);
			ParseTree_Node *child_3 = ParseTree_Node_get_child_by_node_index(node_ptr, 3);

			char *name = child_1->tkn_ptr->data->string;
			int len_name = child_1->tkn_ptr->data->len_string;

			// printf("== def: %*s ==\n", len_name, name);

			Type *type_ptr = Type_new(TYPE_ENUM_FUNCTION_DEF);

			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_add(env_ptr, name, len_name, 0, type_ptr);
			if(etr_ptr == NULL){
				fprintf(stderr, "Symbol %*s already declared\n", len_name, name);
				*flag_error = 1;
				return -1;
			}

			SymbolEnv_scope_add(env_ptr, name, len_name);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			status = Semantic_symbol_and_type_check(child_2, env_ptr, flag_error);
			if(status == -1)
				return status;

			Type_set_function_param_out_lst(type_ptr, child_0->atr_ptr->type);
			child_0->atr_ptr->type = NULL;
			Type_set_function_param_in_lst(type_ptr, child_2->atr_ptr->type);
			child_2->atr_ptr->type = NULL;

			// print_type(type_ptr);
			// printf("\n");

			status = Semantic_symbol_and_type_check(child_3, env_ptr, flag_error);
			if(status == -1)
				return status;

			SymbolEnv_Scope *scp_ptr = SymbolEnv_scope_exit(env_ptr);
			if(scp_ptr == NULL){
				// fprintf(stderr, "Error exiting scope %s\n", SymbolEnv_Scope_get_name( SymbolEnv_scope_get_current(env_ptr) ) );
				fprintf(stderr, "Error exiting scope\n");
				*flag_error = 1;
				return -1;
			}

			break;
		}

		case OPERATOR_ID_LIST:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			Type *type_ptr = Type_new(TYPE_ENUM_LIST);

			ParseTree_Node *id_node_ptr = child_0;
			while(id_node_ptr != NULL){
				char *id = id_node_ptr->tkn_ptr->data->string;
				int len_id = id_node_ptr->tkn_ptr->data->len_string;

				SymbolEnv_Entry *id_etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, len_id);
				if(id_etr_ptr == NULL){
					fprintf(stderr, "Symbol %*s not declared in this scope\n", len_id, id);
					*flag_error = 1;
					return -1;
				}

				Type *type_id_ptr = SymbolEnv_Entry_get_type(id_etr_ptr);
				Type_add_list_element(type_ptr, Type_clone(type_id_ptr));

				id_node_ptr->atr_ptr->entry = id_etr_ptr;
				id_node_ptr->atr_ptr->type = Type_clone(type_id_ptr);

				id_node_ptr = id_node_ptr->sibling;
			}

			node_ptr->atr_ptr->type = type_ptr;

			break;
		}

		case OPERATOR_MATRIX:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			int num_rows = 0;
			int num_columns = 0;

			ParseTree_Node *row_node_ptr = child_0;
			ParseTree_Node *column_node_ptr = row_node_ptr;

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
					column_node_ptr = column_node_ptr->sibling;
				}

				if(num_columns_cur != num_columns){
					fprintf(stderr, "Inconsistent number of elements in matrix\n");
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

		case OPERATOR_MATRIX_ELEMENT:
		{
			node_ptr->atr_ptr->type = Type_new(TYPE_ENUM_NUM);
			break;
		}

		case OPERATOR_PARAM_LIST:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_get_child_by_node_index(node_ptr, 2);

			status = Semantic_symbol_and_type_check(child_0, env_ptr, flag_error);
			if(status == -1)
				return status;

			// Add symbol to symbol table
			char *id = child_1->tkn_ptr->data->string;
			int len_id = child_1->tkn_ptr->data->len_string;

			Type *type_id_ptr = Type_clone(child_0->atr_ptr->type);

			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_add(env_ptr, id, len_id, 0, type_id_ptr);
			if(etr_ptr == NULL){
				fprintf(stderr, "Symbol %*s already declared\n", len_id, id);
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
				status = Semantic_symbol_and_type_check(child_2, env_ptr, flag_error);
				if(status == -1)
					return status;

				node_ptr->atr_ptr->type = child_2->atr_ptr->type;
				child_2->atr_ptr->type = NULL;

				Type_add_list_element_front(node_ptr->atr_ptr->type, child_0->atr_ptr->type);
				child_0->atr_ptr->type = NULL;
			}

			break;
		}

		case OPERATOR_STMT_LIST:
		case OPERATOR_STMT_OR_DEF_LIST:
		{
			ParseTree_Node *child = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			while(child){
				status = Semantic_symbol_and_type_check(child, env_ptr, flag_error);
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
}
