#include <stdlib.h>
#include <stdio.h>


#include "SymbolEnv.h"


#include "Type_init.h"


///////////////
// Constants //
///////////////

const int SIZE_NUM = 2;
const int SIZE_RNUM = 4;
const int SIZE_CHAR = 1;

char *type_names[] = {
	"",
	"NUM",
	"RNUM",
	"CHAR",
	"STR",
	"MATRIX",
	"LIST",
	"FUNCTION_DEF",
	"FUNCTION_CALL",
	"LABEL",
	"UNKOWN",
};

int len_type_names = 11;


//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

Type *Type_new(TypeEnum_type type_enum){
	Type *type_ptr = malloc( sizeof(Type) );
	type_ptr->type_enum = type_enum;

	if(type_enum == TYPE_ENUM_STR){
		type_ptr->len_string = -1;
	}

	else if(type_enum == TYPE_ENUM_MATRIX){
		type_ptr->num_rows = -1;
		type_ptr->num_columns = -1;
	}

	else if(type_enum == TYPE_ENUM_LIST){
		type_ptr->lst_ptr = LinkedList_new();
	}

	else if(type_ptr->type_enum == TYPE_ENUM_FUNCTION_DEF || type_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL){
		type_ptr->type_param_in_lst_ptr = Type_new(TYPE_ENUM_LIST);
		type_ptr->type_param_out_lst_ptr = Type_new(TYPE_ENUM_LIST);
	}

	return type_ptr;
}

void Type_destroy(Type *type_ptr){
	if(type_ptr->type_enum == TYPE_ENUM_LIST){
		LinkedList *lst_ptr = type_ptr->lst_ptr;

		while( LinkedList_peek(lst_ptr) ){
			Type *type_element_ptr = LinkedList_pop(lst_ptr);
			Type_destroy(type_element_ptr);
		}
		LinkedList_destroy(lst_ptr);
	}

	else if(type_ptr->type_enum == TYPE_ENUM_FUNCTION_DEF || type_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL){
		Type_destroy(type_ptr->type_param_in_lst_ptr);
		Type_destroy(type_ptr->type_param_out_lst_ptr);
	}

	free(type_ptr);
}

void SymbolEnv_Type_destroy(void *type_ptr){
	Type_destroy(type_ptr);
}

Type *Type_clone(Type *type_ptr){
	Type *clone_ptr = Type_new(type_ptr->type_enum);

	if(type_ptr->type_enum == TYPE_ENUM_STR){
		clone_ptr->len_string = type_ptr->len_string;
	}

	else if(type_ptr->type_enum == TYPE_ENUM_MATRIX){
		clone_ptr->num_rows = type_ptr->num_rows;
		clone_ptr->num_columns = type_ptr->num_columns;
	}

	else if(type_ptr->type_enum == TYPE_ENUM_LIST){
		LinkedListIterator *itr_ptr = LinkedListIterator_new(type_ptr->lst_ptr);
		LinkedListIterator_move_to_first(itr_ptr);
		while(1){
			Type *type_element_ptr = LinkedListIterator_get_item(itr_ptr);
			if(type_element_ptr == NULL)
				break;

			Type_add_list_element(clone_ptr, Type_clone(type_element_ptr));

			LinkedListIterator_move_to_next(itr_ptr);
		}
		LinkedListIterator_destroy(itr_ptr);
	}

	else if(type_ptr->type_enum == TYPE_ENUM_FUNCTION_DEF || type_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL){
		clone_ptr->type_param_in_lst_ptr = Type_clone(type_ptr->type_param_in_lst_ptr);
		clone_ptr->type_param_out_lst_ptr = Type_clone(type_ptr->type_param_out_lst_ptr);
	}

	return clone_ptr;
}


///////////////
// Functions //
///////////////

void Type_set_string_len(Type *type_ptr, int len_string){
	if(type_ptr->type_enum != TYPE_ENUM_STR){
		fprintf(stderr, "Type_set_string_len : %p not STR type\n", type_ptr);
		return;
	}

	type_ptr->len_string = len_string;
}

void Type_set_matrix_len(Type *type_ptr, int num_rows, int num_columns){
	if(type_ptr->type_enum != TYPE_ENUM_MATRIX){
		fprintf(stderr, "Type_set_matrix_len : %p not MATRIX type\n", type_ptr);
		return;
	}

	type_ptr->num_rows = num_rows;
	type_ptr->num_columns = num_columns;
}

void Type_add_list_element(Type *type_ptr, Type *type_element_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_LIST){
		fprintf(stderr, "Type_add_list_element : %p not LIST type\n", type_ptr);
		return;
	}

	LinkedList_pushback(type_ptr->lst_ptr, type_element_ptr);
}

void Type_add_list_element_front(Type *type_ptr, Type *type_element_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_LIST){
		fprintf(stderr, "Type_add_list_element : %p not LIST type\n", type_ptr);
		return;
	}

	LinkedList_push(type_ptr->lst_ptr, type_element_ptr);
}

void Type_add_function_param_in_single(Type *type_ptr, Type* type_param_in_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_FUNCTION_CALL && type_ptr->type_enum != TYPE_ENUM_FUNCTION_DEF){
		fprintf(stderr, "Type_add_function_param_in_single : %p not FUNCTION_CALL or FUNCTION_DEF type\n", type_ptr);
		return;
	}

	Type_add_list_element(type_ptr->type_param_in_lst_ptr, type_param_in_ptr);
}

void Type_add_function_param_out_single(Type *type_ptr, Type* type_param_out_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_FUNCTION_CALL && type_ptr->type_enum != TYPE_ENUM_FUNCTION_DEF){
		fprintf(stderr, "Type_add_function_param_out_single : %p not FUNCTION_CALL or FUNCTION_DEF type\n", type_ptr);
		return;
	}

	Type_add_list_element(type_ptr->type_param_out_lst_ptr, type_param_out_ptr);
}

void Type_set_function_param_in_lst(Type *type_ptr, Type* type_param_in_lst_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_FUNCTION_CALL && type_ptr->type_enum != TYPE_ENUM_FUNCTION_DEF){
		fprintf(stderr, "Type_set_function_param_in_lst : %p not FUNCTION_CALL or FUNCTION_DEF type\n", type_ptr);
		return;
	}

	Type_destroy(type_ptr->type_param_in_lst_ptr);
	type_ptr->type_param_in_lst_ptr = type_param_in_lst_ptr;
}

void Type_set_function_param_out_lst(Type *type_ptr, Type* type_param_out_lst_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_FUNCTION_CALL && type_ptr->type_enum != TYPE_ENUM_FUNCTION_DEF){
		fprintf(stderr, "Type_set_function_param_out_lst : %p not FUNCTION_CALL or FUNCTION_DEF type\n", type_ptr);
		return;
	}

	Type_destroy(type_ptr->type_param_out_lst_ptr);
	type_ptr->type_param_out_lst_ptr = type_param_out_lst_ptr;
}


int Type_get_size(Type *type_ptr){
	int size = -1;

	switch(type_ptr->type_enum){

		case TYPE_ENUM_NUM:
		{
			size = SIZE_NUM;
			break;
		}

		case TYPE_ENUM_RNUM:
		{
			size = SIZE_RNUM;
			break;
		}

		case TYPE_ENUM_CHAR:
		{
			size = SIZE_CHAR;
			break;
		}

		case TYPE_ENUM_STR:
		{
			if(type_ptr->len_string < 0)
				size = -1;
			else{
				// For storing elements
				size = type_ptr->len_string * SIZE_CHAR;
			}

			break;
		}

		case TYPE_ENUM_MATRIX:
		{
			if(type_ptr->num_rows < 0 && type_ptr->num_columns < 0)
				size = -1;
			else{
				// For storing elements
				size = type_ptr->num_rows * type_ptr->num_columns * SIZE_NUM;
			}

			break;
		}

		case TYPE_ENUM_LIST:
		{
			LinkedListIterator *itr_ptr = LinkedListIterator_new(type_ptr->lst_ptr);
			LinkedListIterator_move_to_first(itr_ptr);
			while(1){
				Type *type_element_ptr = LinkedListIterator_get_item(itr_ptr);
				if(type_element_ptr == NULL)
					break;

				int size_element = Type_get_size(type_element_ptr);
				if(size_element < 0){
					size = -1;
					break;
				}

				size += size_element;

				LinkedListIterator_move_to_next(itr_ptr);
			}
			LinkedListIterator_destroy(itr_ptr);
			break;
		}

		case TYPE_ENUM_FUNCTION_CALL:
		case TYPE_ENUM_FUNCTION_DEF:
		{
			// int size_element;

			// size_element = Type_get_size(type_ptr->type_param_in_lst_ptr);
			// if(size_element < 0){
			// 	size = -1;
			// 	break;
			// }
			// size += size_element;

			// size_element = Type_get_size(type_ptr->type_param_out_lst_ptr);
			// if(size_element < 0){
			// 	size = -1;
			// 	break;
			// }
			// size += size_element;

			size = 0;

			break;
		}

		case TYPE_ENUM_LABEL:
		{
			size = 0;
			break;
		}

		default:{
			fprintf(stderr, "Type_get_size : Type object (%p) of unknown type %d\n", type_ptr, type_ptr->type_enum);
			size = -1;

			break;
		}
	}

	return size;
}

int Type_check_completeness(Type *type_ptr){
	switch(type_ptr->type_enum){
		case TYPE_ENUM_NUM:
		case TYPE_ENUM_RNUM:
		case TYPE_ENUM_CHAR:
		case TYPE_ENUM_LABEL:
		{
			break;
		}

		case TYPE_ENUM_STR:
		{
			if(type_ptr->len_string < 0)
				return -1;

			break;
		}

		case TYPE_ENUM_MATRIX:
		{
			if(type_ptr->num_rows < 0 || type_ptr->num_columns < 0)
				return -1;

			break;
		}

		case TYPE_ENUM_LIST:
		{
			LinkedListIterator *itr_ptr = LinkedListIterator_new(type_ptr->lst_ptr);
			LinkedListIterator_move_to_first(itr_ptr);
			while(1){
				Type *type_element_ptr = LinkedListIterator_get_item(itr_ptr);

				if(type_element_ptr == NULL)
					break;

				if( Type_check_completeness(type_element_ptr) == -1 ){
					LinkedListIterator_destroy(itr_ptr);
					return -1;
				}

				LinkedListIterator_move_to_next(itr_ptr);
			}
			LinkedListIterator_destroy(itr_ptr);

			break;
		}

		case TYPE_ENUM_FUNCTION_DEF:
		case TYPE_ENUM_FUNCTION_CALL:
		{
			if( Type_check_completeness(type_ptr->type_param_in_lst_ptr) == -1 )
				return -1;
			if( Type_check_completeness(type_ptr->type_param_out_lst_ptr) == -1 )
				return -1;

			break;
		}

		default:
		{
			fprintf(stderr, "Type_check_completeness : Type object (%p) of unknown type %d\n", type_ptr, type_ptr->type_enum);
			return -1;
		}
	}

	return 0;
}

int Type_check_compatibility(Type *type_1_ptr, Type *type_2_ptr){
	if( type_1_ptr->type_enum == type_2_ptr->type_enum ){

		if( type_1_ptr->type_enum == TYPE_ENUM_NUM || type_1_ptr->type_enum == TYPE_ENUM_RNUM || type_1_ptr->type_enum == TYPE_ENUM_CHAR || type_1_ptr->type_enum == TYPE_ENUM_LABEL )
			return 0;

		else if( type_1_ptr->type_enum == TYPE_ENUM_STR ){

			if( Type_check_completeness(type_1_ptr) == -1 || Type_check_completeness(type_2_ptr) == -1 )
				return 0;

			else if( type_1_ptr->len_string == type_2_ptr->len_string )
				return 0;

			else
				return -1;
		}

		else if( type_1_ptr->type_enum == TYPE_ENUM_MATRIX ){

			if( Type_check_completeness(type_1_ptr) == -1 || Type_check_completeness(type_2_ptr) == -1 )
				return 0;

			else if( type_1_ptr->num_rows == type_2_ptr->num_rows && type_1_ptr->num_columns == type_2_ptr->num_columns )
				return 0;

			else
				return -1;
		}

		else if( type_1_ptr->type_enum == TYPE_ENUM_LIST ){

			LinkedListIterator *itr_1_ptr = LinkedListIterator_new(type_1_ptr->lst_ptr);
			LinkedListIterator *itr_2_ptr = LinkedListIterator_new(type_2_ptr->lst_ptr);

			LinkedListIterator_move_to_first(itr_1_ptr);
			LinkedListIterator_move_to_first(itr_2_ptr);

			while(1){
				Type *type_element_1_ptr = LinkedListIterator_get_item(itr_1_ptr);
				Type *type_element_2_ptr = LinkedListIterator_get_item(itr_2_ptr);

				if(type_element_1_ptr == NULL && type_element_2_ptr == NULL)
					break;

				else if(
					(type_element_1_ptr == NULL && type_element_2_ptr != NULL) ||
					(type_element_1_ptr != NULL && type_element_2_ptr == NULL)
				){
					LinkedListIterator_destroy(itr_1_ptr);
					LinkedListIterator_destroy(itr_2_ptr);
					return -1;
				}

				else if( Type_check_compatibility(type_element_1_ptr, type_element_2_ptr) == -1 ){
					LinkedListIterator_destroy(itr_1_ptr);
					LinkedListIterator_destroy(itr_2_ptr);
					return -1;
				}

				LinkedListIterator_move_to_next(itr_1_ptr);
				LinkedListIterator_move_to_next(itr_2_ptr);
			}

			LinkedListIterator_destroy(itr_1_ptr);
			LinkedListIterator_destroy(itr_2_ptr);

			return 0;
		}

		else if( type_1_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL || type_1_ptr->type_enum == TYPE_ENUM_FUNCTION_DEF ){

			if( Type_check_compatibility(type_1_ptr->type_param_in_lst_ptr, type_2_ptr->type_param_in_lst_ptr) == -1)
				return -1;

			if( Type_check_compatibility(type_1_ptr->type_param_out_lst_ptr, type_2_ptr->type_param_out_lst_ptr) == -1)
				return -1;

			return 0;
		}
	}

	else{

		for (int i = 0; i < 2; ++i)
		{
			if(i == 1){
				// switch type_ptr
				Type *temp = type_1_ptr;
				type_1_ptr = type_2_ptr;
				type_2_ptr = temp;
			}

			if(
				type_1_ptr->type_enum == TYPE_ENUM_FUNCTION_DEF &&
				type_2_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL
			){
				if(
					( Type_check_compatibility(type_1_ptr->type_param_in_lst_ptr, type_2_ptr->type_param_in_lst_ptr) == 0 ) &&
					( Type_check_compatibility(type_1_ptr->type_param_out_lst_ptr, type_2_ptr->type_param_out_lst_ptr) == 0 )
				){
					return 0;
				}
			}

			else if( type_1_ptr->type_enum == TYPE_ENUM_LIST ){
				if(
					type_2_ptr->type_enum == TYPE_ENUM_NUM ||
					type_2_ptr->type_enum == TYPE_ENUM_RNUM ||
					type_2_ptr->type_enum == TYPE_ENUM_CHAR ||
					type_2_ptr->type_enum == TYPE_ENUM_STR ||
					type_2_ptr->type_enum == TYPE_ENUM_MATRIX
				){
					if( LinkedList_get_size(type_1_ptr->lst_ptr) == 1 ){
						if( Type_check_compatibility( LinkedList_peek(type_1_ptr->lst_ptr), type_2_ptr) == 0 ){
							return 0;
						}
					}

					// else{
					// 	return -1;
					// }
				}

				else if( type_2_ptr->type_enum == TYPE_ENUM_FUNCTION_DEF || type_2_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL ){
					if( Type_check_compatibility(type_1_ptr, type_2_ptr->type_param_out_lst_ptr) == 0 ){
						return 0;
					}
				}
			}

			else if(
				type_1_ptr->type_enum == TYPE_ENUM_NUM ||
				type_1_ptr->type_enum == TYPE_ENUM_RNUM ||
				type_1_ptr->type_enum == TYPE_ENUM_CHAR ||
				type_1_ptr->type_enum == TYPE_ENUM_STR ||
				type_1_ptr->type_enum == TYPE_ENUM_MATRIX
			){
				if( type_2_ptr->type_enum == TYPE_ENUM_FUNCTION_DEF || type_2_ptr->type_enum == TYPE_ENUM_FUNCTION_CALL ){
					if( Type_check_compatibility(type_1_ptr, type_2_ptr->type_param_out_lst_ptr) == 0 ){
						return 0;
					}
				}
			}
		}

		return -1;
	}

	return -1;
}

char *type_to_name(int op){
	if(op>0 && op<len_type_names)
		return type_names[op];
	else
		return type_names[len_type_names-1];
}


//////////////
// Printing //
//////////////

void print_type(Type *type_ptr){

	printf("%s", type_to_name(type_ptr->type_enum));

	switch(type_ptr->type_enum){
		case TYPE_ENUM_NUM:
		case TYPE_ENUM_RNUM:
		case TYPE_ENUM_CHAR:
		case TYPE_ENUM_LABEL:
		{
			break;
		}

		case TYPE_ENUM_STR:
		{
			printf("(%d)", type_ptr->len_string);
			break;
		}

		case TYPE_ENUM_MATRIX:
		{
			printf("(%d,%d)", type_ptr->num_rows, type_ptr->num_columns);
			break;
		}

		case TYPE_ENUM_LIST:
		{
			printf("[ ");

			LinkedListIterator *itr_ptr = LinkedListIterator_new(type_ptr->lst_ptr);
			LinkedListIterator_move_to_first(itr_ptr);
			while(1){
				Type *type_element_ptr = LinkedListIterator_get_item(itr_ptr);
				if(type_element_ptr == NULL)
					break;

				print_type(type_element_ptr);
				printf(", ");

				LinkedListIterator_move_to_next(itr_ptr);
			}
			LinkedListIterator_destroy(itr_ptr);

			printf("]");
			break;
		}

		case TYPE_ENUM_FUNCTION_DEF:
		case TYPE_ENUM_FUNCTION_CALL:
		{
			printf("( ");
			print_type(type_ptr->type_param_in_lst_ptr);
			printf(", ");
			print_type(type_ptr->type_param_out_lst_ptr);
			printf(")");
			break;
		}

		default:
		{
			printf("?");
			break;
		}
	}
}
