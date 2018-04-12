#ifndef INCLUDE_GUARD_5700383F84D04A9BB07B687FDB28A81E
#define INCLUDE_GUARD_5700383F84D04A9BB07B687FDB28A81E


#include "LinkedList.h"


#include "Ast_init.h"


///////////
// Types //
///////////

typedef enum {
	// Primitive
	TYPE_ENUM_NUM = 1,
	TYPE_ENUM_RNUM,
	TYPE_ENUM_CHAR,

	// Constructed
	TYPE_ENUM_STR,
	TYPE_ENUM_MATRIX,
	TYPE_ENUM_LIST,
	TYPE_ENUM_FUNCTION_DEF,
	TYPE_ENUM_FUNCTION_CALL,

	TYPE_UNKOWN = -1,
} TypeEnum_type;


///////////////
// Constants //
///////////////

extern const int SIZE_NUM;
extern const int SIZE_RNUM;
extern const int SIZE_CHAR;


/////////////////////
// Data structures //
/////////////////////

typedef struct Type Type;

typedef struct Type
{
	TypeEnum_type type_enum;
	union{
		// STR
		struct{
			int len_string;
		};

		// MATRIX
		struct{
			int num_rows;
			int num_columns;
		};

		// LIST
		struct{
			LinkedList *lst_ptr;
		};

		// FUNCTION
		struct{
			Type *type_param_in_lst_ptr;
			Type *type_param_out_lst_ptr;
		};
	};
} Type;


//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

Type *Type_new(TypeEnum_type type_enum);

void Type_destroy(Type *type_ptr);

Type *Type_clone(Type *type_ptr);


///////////////
// Functions //
///////////////

void Type_set_string_len(Type *type_ptr, int len_string);

void Type_set_matrix_len(Type *type_ptr, int num_rows, int num_columns);

void Type_add_list_element(Type *type_ptr, Type *type_element_ptr);

void Type_add_function_param_in(Type *type_ptr, Type* type_param_in_ptr);

void Type_add_function_param_out(Type *type_ptr, Type* type_param_out_ptr);

/**
 * Returns the memory required in bytes to store a symbol of given type
 * @param  type_ptr Pointer to Type struct
 * @return          Memory required in bytes. 0 if type is function def or
 * uninitialized matrix or string
 */
int Type_get_size(Type *type_ptr);

/**
 * Check if two types are equal. If both types are string or are matrix, and one
 * of the them is not set (size = 0), they are equal
 * @param  type_1_ptr [description]
 * @param  type_2_ptr [description]
 * @return            0 if equal, -1 if not
 */
int Type_check_equal(Type *type_1_ptr, Type *type_2_ptr);

#endif
