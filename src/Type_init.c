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


//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

Type *Type_new(TypeEnum_type type_enum){
	Type *type_ptr = malloc( sizeof(Type) );
	type_ptr->type_enum = type_enum;

	if(type_enum == TYPE_ENUM_STR){
		type_ptr->len_string = 0;
	}

	else if(type_enum == TYPE_ENUM_MATRIX){
		type_ptr->num_rows = 0;
		type_ptr->num_columns = 0;
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
	Type *clone_ptr = Type_new(clone_ptr->type_enum);

	if(type_ptr->type_enum == TYPE_ENUM_STR){
		clone_ptr->len_string = type_ptr->len_string;
	}

	else if(type_ptr->type_enum == TYPE_ENUM_MATRIX){
		clone_ptr->num_rows = type_ptr->num_rows;
		clone_ptr->num_rows = type_ptr->num_rows;
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

	return type_ptr;
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

void Type_add_function_param_in(Type *type_ptr, Type* type_param_in_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_FUNCTION_CALL || type_ptr->type_enum != TYPE_ENUM_FUNCTION_DEF){
		fprintf(stderr, "Type_add_function_param_in : %p not FUNCTION_CALL or FUNCTION_DEF type\n", type_ptr);
		return;
	}

	Type_add_list_element(type_ptr->type_param_in_lst_ptr, type_param_in_ptr);
}

void Type_add_function_param_out(Type *type_ptr, Type* type_param_out_ptr){
	if(type_ptr->type_enum != TYPE_ENUM_FUNCTION_CALL || type_ptr->type_enum != TYPE_ENUM_FUNCTION_DEF){
		fprintf(stderr, "Type_add_function_param_out : %p not FUNCTION_CALL or FUNCTION_DEF type\n", type_ptr);
		return;
	}

	Type_add_list_element(type_ptr->type_param_out_lst_ptr, type_param_out_ptr);
}

int Type_get_size(Type *type_ptr){
	int size = 0;

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
			if(type_ptr->len_string == 0)
				size = 0;
			else{
				// For storing index
				size = SIZE_NUM;
				// For storing elements
				size += type_ptr->len_string * SIZE_CHAR;
			}

			break;
		}

		case TYPE_ENUM_MATRIX:
		{
			if(type_ptr->num_rows == 0 && type_ptr->num_columns == 0)
				size = 0;
			else{
				// For storing index
				size = 2 * SIZE_NUM;
				// For storing elements
				size += type_ptr->num_rows * type_ptr->num_columns * SIZE_NUM;
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

				size += Type_get_size(type_element_ptr);

				LinkedListIterator_move_to_next(itr_ptr);
			}
			LinkedListIterator_destroy(itr_ptr);
			break;
		}

		case TYPE_ENUM_FUNCTION_DEF:
		{
			size += Type_get_size(type_ptr->type_param_in_lst_ptr);
			size += Type_get_size(type_ptr->type_param_out_lst_ptr);

			break;
		}

		case TYPE_ENUM_FUNCTION_CALL:
		{
			size += Type_get_size(type_ptr->type_param_in_lst_ptr);
			size += Type_get_size(type_ptr->type_param_out_lst_ptr);

			break;
		}

		default:{
			fprintf(stderr, "Type_get_size : Type object (%p) of unknown type %d\n", type_ptr, type_ptr->type_enum);
			break;
		}
	}

	return size;
}

int Type_check_equal(Type *type_1_ptr, Type *type_2_ptr){

}
