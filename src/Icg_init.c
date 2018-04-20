#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Token.h"
#include "ParseTree.h"
#include "SymbolEnv.h"
#include "Quad.h"


#include "Token_Data.h"
#include "Ast_init.h"
#include "Type_init.h"


#include "Icg_init.h"


///////////////
// Constants //
///////////////

const int MAX_LABEL_LEN = 20;
const int MAX_TEMP_LEN = 20;
const char LABEL_PREFIX[] = "__label_";
const char TEMP_PREFIX[] = "__";

const int REG_0 = 0;
const int REG_1 = 1;

char *quad_operator_names[] ={
	"",
	"identity",
	"negate",
	"not",
	"add",
	"subtract",
	"multiply",
	"divide",
	"and",
	"or",
	"copy",
	"copy_indexed_r",
	"copy_indexed_l",
	"copy_address_r",
	"copy_derefed_r",
	"copy_derefed_l",
	"label",
	"goto",
	"goto_lt",
	"goto_le",
	"goto_gt",
	"goto_ge",
	"goto_eq",
	"goto_ne",
	"call",
	"read_int",
	"read_char",
	"write_int",
	"write_char",
	"unknown",
};

int len_quad_operator_names = 30;


////////////////////
// Pvt Prototypes //
////////////////////

static int Icg_generate_quads_recursive(ParseTree_Node *node_ptr, SymbolEnv *env_ptr, int *num_temp, int *num_label);

static char *label_num_to_string(long long num);

static char *tmp_num_to_string(long long num);

static char *quad_operator_to_name(int op);

static SymbolEnv_Entry *get_label(SymbolEnv *env_ptr, int *num_label);


///////////////
// Functions //
///////////////

LinkedList *Icg_generate_quads(ParseTree_Node *node_ptr, SymbolEnv *env_ptr){
	int num_temp = 0;
	int num_label = 0;

	if( Icg_generate_quads_recursive(node_ptr, env_ptr, &num_temp, &num_label) == 0){
		LinkedList *quad_lst_ptr = node_ptr->atr_ptr->code;
		node_ptr->atr_ptr->code = NULL;
		return quad_lst_ptr;
	}

	return NULL;
}

int Icg_generate_quads_recursive(ParseTree_Node *node_ptr, SymbolEnv *env_ptr, int *num_temp, int *num_label){
	int status;

	switch(node_ptr->atr_ptr->op){
		case AST_OPERATOR_PLUS:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;


			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *child_0_quad_lst_ptr = child_0->atr_ptr->code;
			child_0->atr_ptr->code = NULL;

			LinkedList *child_1_quad_lst_ptr = child_1->atr_ptr->code;
			child_1->atr_ptr->code = NULL;

			if(child_0_quad_lst_ptr != NULL){
				LinkedList_append(quad_lst_ptr, child_0_quad_lst_ptr);
				LinkedList_destroy(child_0_quad_lst_ptr);
			}

			if(child_1_quad_lst_ptr != NULL){
				LinkedList_append(quad_lst_ptr, child_1_quad_lst_ptr);
				LinkedList_destroy(child_1_quad_lst_ptr);
			}


			Type *type_ptr = node_ptr->atr_ptr->type;

			char *tmp_id = tmp_num_to_string((*num_temp)++);
			int len_tmp_id = strlen(tmp_id);

			SymbolEnv_Entry *etr_tmp_ptr = SymbolEnv_entry_add(env_ptr, tmp_id, len_tmp_id, Type_clone(type_ptr));
			node_ptr->atr_ptr->result_entry = etr_tmp_ptr;
			free(tmp_id);

			if(type_ptr->type_enum == TYPE_ENUM_NUM){
				
				Quad_Node *quad_ptr = Quad_Node_new();
				LinkedList_pushback(quad_lst_ptr, quad_ptr);

				quad_ptr->op = QUAD_OP_ADD;
				quad_ptr->result.name = etr_tmp_ptr;
				quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;

				if(child_0->atr_ptr->result_value != NULL){
					quad_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_ptr->arg_1.constant = *(int *)(child_0->atr_ptr->result_value);
				}
				else{
					quad_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->arg_1.name = child_0->atr_ptr->result_entry;
				}

				if(child_1->atr_ptr->result_value != NULL){
					quad_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_ptr->arg_2.constant = *(int *)(child_1->atr_ptr->result_value);
				}
				else{
					quad_ptr->arg_2_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->arg_2.name = child_1->atr_ptr->result_entry;
				}
			}

			else if(type_ptr->type_enum == TYPE_ENUM_STR){

				Type *type_0_ptr = child_0->atr_ptr->type;
				Type *type_1_ptr = child_1->atr_ptr->type;

				int len_string_0 = type_0_ptr->len_string;
				int len_string_1 = type_1_ptr->len_string;
				int len_string = type_ptr->len_string;

				int idx = 0;


				for (int idx_0 = 0; idx_0 < len_string_0; ++idx_0, ++idx){
					Quad_Node *quad_a_ptr = Quad_Node_new();
					Quad_Node *quad_b_ptr = Quad_Node_new();

					LinkedList_pushback(quad_lst_ptr, quad_a_ptr);
					LinkedList_pushback(quad_lst_ptr, quad_b_ptr);

					quad_a_ptr->result_type = QUAD_ADDR_TYPE_REG;
					quad_a_ptr->result.reg = REG_0;

					if(child_0->atr_ptr->result_value != NULL){
						quad_a_ptr->op = QUAD_OP_COPY;

						quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_a_ptr->arg_1.constant =  ((char *)(child_0->atr_ptr->result_value))[idx_0];
					}
					else{
						quad_a_ptr->op = QUAD_OP_COPY_INDEXED_R;

						quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
						quad_a_ptr->arg_1.name = child_0->atr_ptr->result_entry;
						
						quad_a_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_a_ptr->arg_2.constant = idx_0;
					}

					quad_b_ptr->op = QUAD_OP_COPY_INDEXED_L;
					quad_b_ptr->result_type = QUAD_ADDR_TYPE_NAME;
					quad_b_ptr->result.name = etr_tmp_ptr;
					quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
					quad_b_ptr->arg_1.reg = REG_0;
					quad_b_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_b_ptr->arg_2.constant = idx;
				}

				for (int idx_1 = 0; idx_1 < len_string_1; ++idx_1, ++idx){
					Quad_Node *quad_a_ptr = Quad_Node_new();	// reg0 = chx[i]
					Quad_Node *quad_b_ptr = Quad_Node_new();	// tmp[j] = chx[i]

					LinkedList_pushback(quad_lst_ptr, quad_a_ptr);
					LinkedList_pushback(quad_lst_ptr, quad_b_ptr);

					quad_a_ptr->result_type = QUAD_ADDR_TYPE_REG;
					quad_a_ptr->result.reg = REG_0;

					if(child_1->atr_ptr->result_value != NULL){
						quad_a_ptr->op = QUAD_OP_COPY;

						quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_a_ptr->arg_1.constant =  ((char *)(child_1->atr_ptr->result_value))[idx_1];
					}
					else{
						quad_a_ptr->op = QUAD_OP_COPY_INDEXED_R;

						quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
						quad_a_ptr->arg_1.name = child_1->atr_ptr->result_entry;
						
						quad_a_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_a_ptr->arg_2.constant = idx_1;
					}

					quad_b_ptr->op = QUAD_OP_COPY_INDEXED_L;
					quad_b_ptr->result_type = QUAD_ADDR_TYPE_NAME;
					quad_b_ptr->result.name = etr_tmp_ptr;
					quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
					quad_b_ptr->arg_1.reg = REG_0;
					quad_b_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_b_ptr->arg_2.constant = idx;
				}
			}

			else if(type_ptr->type_enum == TYPE_ENUM_MATRIX){
				int num_rows = type_ptr->num_rows;
				int num_columns = type_ptr->num_columns;

				for (int i = 0; i < num_rows; ++i){
					for (int j = 0; j < num_columns; ++j){
						int idx = i*num_columns + j;

						Quad_Node *quad_a_ptr = Quad_Node_new();	// reg0 = ch0[i]
						Quad_Node *quad_b_ptr = Quad_Node_new();	// reg1 = ch1[i]
						Quad_Node *quad_c_ptr = Quad_Node_new();	// reg0 = reg0 + reg1
						Quad_Node *quad_d_ptr = Quad_Node_new();	// tmp[i] = reg0

						LinkedList_pushback(quad_lst_ptr, quad_a_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_b_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_c_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_d_ptr);


						quad_a_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_a_ptr->result.reg = REG_0;

						if(child_0->atr_ptr->result_value != NULL){
							quad_a_ptr->op = QUAD_OP_COPY;

							quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_a_ptr->arg_1.constant = ((int *)(child_0->atr_ptr->result_value))[idx];
						}
						else{
							quad_a_ptr->op = QUAD_OP_COPY_INDEXED_R;

							quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
							quad_a_ptr->arg_1.name = child_0->atr_ptr->result_entry;

							quad_a_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_a_ptr->arg_2.constant = idx;
						}


						quad_b_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_b_ptr->result.reg = REG_1;

						if(child_1->atr_ptr->result_value != NULL){
							quad_b_ptr->op = QUAD_OP_COPY;

							quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_b_ptr->arg_1.constant = ((int *)(child_1->atr_ptr->result_value))[idx];
						}
						else{
							quad_b_ptr->op = QUAD_OP_COPY_INDEXED_R;

							quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
							quad_b_ptr->arg_1.name = child_1->atr_ptr->result_entry;

							quad_b_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_b_ptr->arg_2.constant = idx;
						}


						quad_c_ptr->op = QUAD_OP_ADD;
						quad_c_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_c_ptr->result.reg = REG_0;
						quad_c_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
						quad_c_ptr->arg_1.reg = REG_0;
						quad_c_ptr->arg_2_type = QUAD_ADDR_TYPE_REG;
						quad_c_ptr->arg_2.reg = REG_1;


						quad_d_ptr->op = QUAD_OP_COPY_INDEXED_L;
						quad_d_ptr->result_type = QUAD_ADDR_TYPE_NAME;
						quad_d_ptr->result.name = etr_tmp_ptr;
						quad_d_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
						quad_d_ptr->arg_1.reg = REG_0;
						quad_d_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_d_ptr->arg_2.constant = idx;
					}
				}
			}

			else{
				fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case\n");
				return -1;
			}

			break;
		}

		case AST_OPERATOR_MINUS:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;


			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *child_0_quad_lst_ptr = child_0->atr_ptr->code;
			child_0->atr_ptr->code = NULL;

			LinkedList *child_1_quad_lst_ptr = child_1->atr_ptr->code;
			child_1->atr_ptr->code = NULL;

			if(child_0_quad_lst_ptr != NULL){
				LinkedList_append(quad_lst_ptr, child_0_quad_lst_ptr);
				LinkedList_destroy(child_0_quad_lst_ptr);
			}

			if(child_1_quad_lst_ptr != NULL){
				LinkedList_append(quad_lst_ptr, child_1_quad_lst_ptr);
				LinkedList_destroy(child_1_quad_lst_ptr);
			}


			Type *type_ptr = node_ptr->atr_ptr->type;

			char *tmp_id = tmp_num_to_string((*num_temp)++);
			int len_tmp_id = strlen(tmp_id);

			SymbolEnv_Entry *etr_tmp_ptr = SymbolEnv_entry_add(env_ptr, tmp_id, len_tmp_id, Type_clone(type_ptr));
			node_ptr->atr_ptr->result_entry = etr_tmp_ptr;
			free(tmp_id);

			if(type_ptr->type_enum == TYPE_ENUM_NUM){
				
				Quad_Node *quad_ptr = Quad_Node_new();
				LinkedList_pushback(quad_lst_ptr, quad_ptr);

				quad_ptr->op = QUAD_OP_SUBTRACT;
				quad_ptr->result.name = etr_tmp_ptr;
				quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;

				if(child_0->atr_ptr->result_value != NULL){
					quad_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_ptr->arg_1.constant = *(int *)(child_0->atr_ptr->result_value);
				}
				else{
					quad_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->arg_1.name = child_0->atr_ptr->result_entry;
				}

				if(child_1->atr_ptr->result_value != NULL){
					quad_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_ptr->arg_2.constant = *(int *)(child_1->atr_ptr->result_value);
				}
				else{
					quad_ptr->arg_2_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->arg_2.name = child_1->atr_ptr->result_entry;
				}
			}

			else if(type_ptr->type_enum == TYPE_ENUM_MATRIX){
				int num_rows = type_ptr->num_rows;
				int num_columns = type_ptr->num_columns;

				for (int i = 0; i < num_rows; ++i){
					for (int j = 0; j < num_columns; ++j){
						int idx = i*num_columns + j;

						Quad_Node *quad_a_ptr = Quad_Node_new();	// reg0 = ch0[i]
						Quad_Node *quad_b_ptr = Quad_Node_new();	// reg1 = ch1[i]
						Quad_Node *quad_c_ptr = Quad_Node_new();	// reg0 = reg0 + reg1
						Quad_Node *quad_d_ptr = Quad_Node_new();	// tmp[i] = reg0

						LinkedList_pushback(quad_lst_ptr, quad_a_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_b_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_c_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_d_ptr);


						quad_a_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_a_ptr->result.reg = REG_0;

						if(child_0->atr_ptr->result_value != NULL){
							quad_a_ptr->op = QUAD_OP_COPY;

							quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_a_ptr->arg_1.constant = ((int *)(child_0->atr_ptr->result_value))[idx];
						}
						else{
							quad_a_ptr->op = QUAD_OP_COPY_INDEXED_L;

							quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
							quad_a_ptr->arg_1.name = child_0->atr_ptr->result_entry;

							quad_a_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_a_ptr->arg_2.constant = idx;
						}


						quad_b_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_b_ptr->result.reg = REG_1;

						if(child_1->atr_ptr->result_value != NULL){
							quad_b_ptr->op = QUAD_OP_COPY;

							quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_b_ptr->arg_1.constant = ((int *)(child_1->atr_ptr->result_value))[idx];
						}
						else{
							quad_b_ptr->op = QUAD_OP_COPY_INDEXED_L;

							quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
							quad_b_ptr->arg_1.name = child_1->atr_ptr->result_entry;

							quad_b_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_b_ptr->arg_2.constant = idx;
						}


						quad_c_ptr->op = QUAD_OP_SUBTRACT;
						quad_c_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_c_ptr->result.reg = REG_0;
						quad_c_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
						quad_c_ptr->arg_1.reg = REG_0;
						quad_c_ptr->arg_2_type = QUAD_ADDR_TYPE_REG;
						quad_c_ptr->arg_2.reg = REG_1;


						quad_d_ptr->op = QUAD_OP_COPY_INDEXED_L;
						quad_d_ptr->result_type = QUAD_ADDR_TYPE_NAME;
						quad_d_ptr->result.name = etr_tmp_ptr;
						quad_d_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
						quad_d_ptr->arg_1.reg = REG_0;
						quad_d_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_d_ptr->arg_2.constant = idx;
					}
				}
			}

			else{
				fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case\n");
				return -1;
			}

			break;
		}

		case AST_OPERATOR_MUL:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;


			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *child_0_quad_lst_ptr = child_0->atr_ptr->code;
			child_0->atr_ptr->code = NULL;

			LinkedList *child_1_quad_lst_ptr = child_1->atr_ptr->code;
			child_1->atr_ptr->code = NULL;

			if(child_0_quad_lst_ptr != NULL){
				LinkedList_append(quad_lst_ptr, child_0_quad_lst_ptr);
				LinkedList_destroy(child_0_quad_lst_ptr);
			}

			if(child_1_quad_lst_ptr != NULL){
				LinkedList_append(quad_lst_ptr, child_1_quad_lst_ptr);
				LinkedList_destroy(child_1_quad_lst_ptr);
			}


			Type *type_ptr = node_ptr->atr_ptr->type;

			char *tmp_id = tmp_num_to_string((*num_temp)++);
			int len_tmp_id = strlen(tmp_id);

			SymbolEnv_Entry *etr_tmp_ptr = SymbolEnv_entry_add(env_ptr, tmp_id, len_tmp_id, Type_clone(type_ptr));
			node_ptr->atr_ptr->result_entry = etr_tmp_ptr;
			free(tmp_id);

			if(type_ptr->type_enum == TYPE_ENUM_NUM){
				
				Quad_Node *quad_ptr = Quad_Node_new();
				LinkedList_pushback(quad_lst_ptr, quad_ptr);

				quad_ptr->op = QUAD_OP_MULTIPLY;
				quad_ptr->result.name = etr_tmp_ptr;
				quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;

				if(child_0->atr_ptr->result_value != NULL){
					quad_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_ptr->arg_1.constant = *(int *)(child_0->atr_ptr->result_value);
				}
				else{
					quad_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->arg_1.name = child_0->atr_ptr->result_entry;
				}

				if(child_1->atr_ptr->result_value != NULL){
					quad_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_ptr->arg_2.constant = *(int *)(child_1->atr_ptr->result_value);
				}
				else{
					quad_ptr->arg_2_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->arg_2.name = child_1->atr_ptr->result_entry;
				}
			}

			else{
				fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case\n");
				return -1;
			}

			break;
		}

		case AST_OPERATOR_DIV:
		{
			fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case\n");
			return -1;

			break;
		}

		case AST_OPERATOR_SIZE:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			Type *type_0_ptr = child_0->atr_ptr->type;

			if(type_0_ptr->type_enum == TYPE_ENUM_STR){

				int *value_ptr = malloc(sizeof(int));
				*value_ptr = type_0_ptr->len_string;

				node_ptr->atr_ptr->result_value = value_ptr;
			}

			else if(type_0_ptr->type_enum == TYPE_ENUM_MATRIX){

				int *value_1_ptr = malloc(sizeof(int));
				int *value_2_ptr = malloc(sizeof(int));
				*value_1_ptr = type_0_ptr->num_rows;
				*value_2_ptr = type_0_ptr->num_columns;

				node_ptr->atr_ptr->result_value = value_1_ptr;
				node_ptr->atr_ptr->result_2_value = value_2_ptr;
			}

			break;
		}

		case AST_OPERATOR_ID:
		{
			node_ptr->atr_ptr->result_entry = node_ptr->atr_ptr->entry;

			break;
		}

		case AST_OPERATOR_FUNID:
		{
			// No code
			break;
		}

		case AST_OPERATOR_KW_INT:
		{
			// No code
			break;
		}

		case AST_OPERATOR_KW_REAL:
		{
			// No code
			break;
		}

		case AST_OPERATOR_KW_STRING:
		{
			// No code
			break;
		}

		case AST_OPERATOR_KW_MATRIX:
		{

			break;
		}

		case AST_OPERATOR_KW_READ:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;

			if(type_ptr->type_enum != TYPE_ENUM_NUM){
				fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case: Cannot read non NUM\n");
				return -1;
			}


			Quad_Node *quad_ptr = Quad_Node_new();

			quad_ptr->op = QUAD_OP_READ_INT;
			quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_ptr->result.name = child_0->atr_ptr->entry;


			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList_pushback(quad_lst_ptr, quad_ptr);

			break;
		}

		case AST_OPERATOR_KW_PRINT:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			Type *type_ptr = child_0->atr_ptr->type;
			SymbolEnv_Entry *etr_ptr = child_0->atr_ptr->entry;


			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;


			if(type_ptr->type_enum == TYPE_ENUM_NUM){
				Quad_Node *quad_1_ptr = Quad_Node_new();	// copy num to reg
				quad_1_ptr->op = QUAD_OP_COPY;
				quad_1_ptr->result_type = QUAD_ADDR_TYPE_REG;
				quad_1_ptr->result.reg = REG_0;
				quad_1_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
				quad_1_ptr->arg_1.name = etr_ptr;

				Quad_Node *quad_2_ptr = Quad_Node_new();	// write reg value as int
				quad_2_ptr->op = QUAD_OP_WRITE_INT;
				quad_2_ptr->result_type = QUAD_ADDR_TYPE_REG;
				quad_2_ptr->result.reg = REG_0;

				Quad_Node *quad_3_ptr = Quad_Node_new();	// copy \n to reg
				quad_3_ptr->op = QUAD_OP_COPY;
				quad_3_ptr->result_type = QUAD_ADDR_TYPE_REG;
				quad_3_ptr->result.reg = REG_0;
				quad_3_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
				quad_3_ptr->arg_1.constant = '\n';

				Quad_Node *quad_4_ptr = Quad_Node_new();	// write reg value as char
				quad_4_ptr->op = QUAD_OP_WRITE_CHAR;
				quad_4_ptr->result_type = QUAD_ADDR_TYPE_REG;
				quad_4_ptr->result.reg = REG_0;

				LinkedList_pushback(quad_lst_ptr, quad_1_ptr);
				LinkedList_pushback(quad_lst_ptr, quad_2_ptr);
				LinkedList_pushback(quad_lst_ptr, quad_3_ptr);
				LinkedList_pushback(quad_lst_ptr, quad_4_ptr);
			}

			else if(type_ptr->type_enum == TYPE_ENUM_STR){
				for (int i = 0; i < type_ptr->len_string; ++i){
					
					Quad_Node *quad_1_ptr = Quad_Node_new();	// copy char to reg
					quad_1_ptr->op = QUAD_OP_COPY_INDEXED_R;
					quad_1_ptr->result_type = QUAD_ADDR_TYPE_REG;
					quad_1_ptr->result.reg = REG_0;
					quad_1_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
					quad_1_ptr->arg_1.name = etr_ptr;
					quad_1_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_1_ptr->arg_2.constant = i;

					Quad_Node *quad_2_ptr = Quad_Node_new();	// write reg value as char
					quad_2_ptr->op = QUAD_OP_WRITE_CHAR;
					quad_2_ptr->result_type = QUAD_ADDR_TYPE_REG;
					quad_2_ptr->result.reg = REG_0;

					LinkedList_pushback(quad_lst_ptr, quad_1_ptr);
					LinkedList_pushback(quad_lst_ptr, quad_2_ptr);
				}

				Quad_Node *quad_3_ptr = Quad_Node_new();	// copy \n to reg
				quad_3_ptr->op = QUAD_OP_COPY;
				quad_3_ptr->result_type = QUAD_ADDR_TYPE_REG;
				quad_3_ptr->result.reg = REG_0;
				quad_3_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
				quad_3_ptr->arg_1.constant = '\n';

				Quad_Node *quad_4_ptr = Quad_Node_new();	// write reg value as char
				quad_4_ptr->op = QUAD_OP_WRITE_CHAR;
				quad_4_ptr->result_type = QUAD_ADDR_TYPE_REG;
				quad_4_ptr->result.reg = REG_0;

				LinkedList_pushback(quad_lst_ptr, quad_3_ptr);
				LinkedList_pushback(quad_lst_ptr, quad_4_ptr);
			}

			else if(type_ptr->type_enum == TYPE_ENUM_MATRIX){
				for (int i = 0; i < type_ptr->num_rows; ++i){
					for (int j = 0; j < type_ptr->num_columns; ++j){
						
						Quad_Node *quad_1_ptr = Quad_Node_new();	// copy num to reg
						quad_1_ptr->op = QUAD_OP_COPY_INDEXED_R;
						quad_1_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_1_ptr->result.reg = REG_0;
						quad_1_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
						quad_1_ptr->arg_1.name = etr_ptr;
						quad_1_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_1_ptr->arg_2.constant = i*type_ptr->num_columns + j;

						Quad_Node *quad_2_ptr = Quad_Node_new();	// write reg value as int
						quad_2_ptr->op = QUAD_OP_WRITE_INT;
						quad_2_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_2_ptr->result.reg = REG_0;

						Quad_Node *quad_3_ptr = Quad_Node_new();	// copy \n to reg
						quad_3_ptr->op = QUAD_OP_COPY;
						quad_3_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_3_ptr->result.reg = REG_0;
						quad_3_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_3_ptr->arg_1.constant = '\n';

						Quad_Node *quad_4_ptr = Quad_Node_new();	// write reg value as char
						quad_4_ptr->op = QUAD_OP_WRITE_CHAR;
						quad_4_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_4_ptr->result.reg = REG_0;

						LinkedList_pushback(quad_lst_ptr, quad_1_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_2_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_3_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_4_ptr);
					}
				}
			}

			break;
		}

		case AST_OPERATOR_NUM:
		{
			int *value_ptr = malloc(sizeof(int));
			*value_ptr = strtol(node_ptr->tkn_ptr->data->string, NULL, 10);

			node_ptr->atr_ptr->result_value = value_ptr;

			break;
		}

		case AST_OPERATOR_RNUM:
		{
			fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case: RNUM\n");
			return -1;

			break;
		}

		case AST_OPERATOR_STR:
		{
			char *string = node_ptr->tkn_ptr->data->string;
			int len_string = node_ptr->tkn_ptr->data->len_string;

			char *value_ptr = malloc( sizeof(char)*len_string );
			memcpy(value_ptr, string, len_string);

			node_ptr->atr_ptr->result_value = value_ptr;

			break;
		}

		case AST_OPERATOR_AND:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *quad_0_lst_ptr = NULL;
			LinkedList *quad_1_lst_ptr = NULL;


			// Labels
			SymbolEnv_Entry *etr_label_true_ptr = node_ptr->atr_ptr->label_true;
			SymbolEnv_Entry *etr_label_false_ptr = node_ptr->atr_ptr->label_false;
			SymbolEnv_Entry *etr_label_middle_ptr = get_label(env_ptr, num_label);


			// Pass labels
			child_0->atr_ptr->label_true = etr_label_middle_ptr;
			child_0->atr_ptr->label_false = etr_label_false_ptr;
			child_1->atr_ptr->label_true = etr_label_true_ptr;
			child_1->atr_ptr->label_false = etr_label_false_ptr;


			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			quad_0_lst_ptr = child_0->atr_ptr->code;
			quad_1_lst_ptr = child_1->atr_ptr->code;

			child_0->atr_ptr->code = NULL;
			child_1->atr_ptr->code = NULL;


			// Quads
			Quad_Node *quad_label_middle_ptr = NULL;
			quad_label_middle_ptr = Quad_Node_new();
			quad_label_middle_ptr->op = QUAD_OP_LABEL;
			quad_label_middle_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_label_middle_ptr->result.name = etr_label_middle_ptr;


			// Quad List
			LinkedList_append(quad_lst_ptr, quad_0_lst_ptr);
			LinkedList_destroy(quad_0_lst_ptr);

			LinkedList_pushback(quad_lst_ptr, quad_label_middle_ptr);

			LinkedList_append(quad_lst_ptr, quad_1_lst_ptr);
			LinkedList_destroy(quad_1_lst_ptr);

			break;
		}

		case AST_OPERATOR_OR:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *quad_0_lst_ptr = NULL;
			LinkedList *quad_1_lst_ptr = NULL;


			// Labels
			SymbolEnv_Entry *etr_label_true_ptr = node_ptr->atr_ptr->label_true;
			SymbolEnv_Entry *etr_label_false_ptr = node_ptr->atr_ptr->label_false;
			SymbolEnv_Entry *etr_label_middle_ptr = get_label(env_ptr, num_label);


			// Pass labels
			child_0->atr_ptr->label_true = etr_label_true_ptr;
			child_0->atr_ptr->label_false = etr_label_middle_ptr;
			child_1->atr_ptr->label_true = etr_label_true_ptr;
			child_1->atr_ptr->label_false = etr_label_false_ptr;


			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			quad_0_lst_ptr = child_0->atr_ptr->code;
			quad_1_lst_ptr = child_1->atr_ptr->code;

			child_0->atr_ptr->code = NULL;
			child_1->atr_ptr->code = NULL;


			// Quads
			Quad_Node *quad_label_middle_ptr = NULL;
			quad_label_middle_ptr = Quad_Node_new();
			quad_label_middle_ptr->op = QUAD_OP_LABEL;
			quad_label_middle_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_label_middle_ptr->result.name = etr_label_middle_ptr;


			// Quad List
			LinkedList_append(quad_lst_ptr, quad_0_lst_ptr);
			LinkedList_destroy(quad_0_lst_ptr);

			LinkedList_pushback(quad_lst_ptr, quad_label_middle_ptr);

			LinkedList_append(quad_lst_ptr, quad_1_lst_ptr);
			LinkedList_destroy(quad_1_lst_ptr);

			break;
		}

		case AST_OPERATOR_NOT:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *quad_0_lst_ptr = NULL;

			// Labels
			SymbolEnv_Entry *etr_label_true_ptr = node_ptr->atr_ptr->label_true;
			SymbolEnv_Entry *etr_label_false_ptr = node_ptr->atr_ptr->label_false;

			// Pass labels
			child_0->atr_ptr->label_true = etr_label_false_ptr;
			child_0->atr_ptr->label_false = etr_label_true_ptr;

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			quad_0_lst_ptr = child_0->atr_ptr->code;
			child_0->atr_ptr->code = NULL;

			// Quad List
			LinkedList_append(quad_lst_ptr, quad_0_lst_ptr);
			LinkedList_destroy(quad_0_lst_ptr);

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

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			// Labels
			SymbolEnv_Entry *etr_label_true_ptr = node_ptr->atr_ptr->label_true;
			SymbolEnv_Entry *etr_label_false_ptr = node_ptr->atr_ptr->label_false;


			// Quads
			Quad_Node *quad_goto_true_ptr = Quad_Node_new();

			switch(node_ptr->atr_ptr->op){
				case AST_OPERATOR_LT:	quad_goto_true_ptr->op = QUAD_OP_GOTO_LT;	break;
				case AST_OPERATOR_LE:	quad_goto_true_ptr->op = QUAD_OP_GOTO_LE;	break;
				case AST_OPERATOR_GT:	quad_goto_true_ptr->op = QUAD_OP_GOTO_GT;	break;
				case AST_OPERATOR_GE:	quad_goto_true_ptr->op = QUAD_OP_GOTO_GE;	break;
				case AST_OPERATOR_EQ:	quad_goto_true_ptr->op = QUAD_OP_GOTO_EQ;	break;
				case AST_OPERATOR_NE:	quad_goto_true_ptr->op = QUAD_OP_GOTO_NE;	break;
			}

			quad_goto_true_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_goto_true_ptr->result.name = etr_label_true_ptr;

			if(child_0->atr_ptr->result_value != NULL){
				quad_goto_true_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
				quad_goto_true_ptr->arg_1.constant = *(int *)(child_0->atr_ptr->result_value);
			}
			else{
				quad_goto_true_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
				quad_goto_true_ptr->arg_1.name = child_0->atr_ptr->result_entry;
			}

			if(child_1->atr_ptr->result_value != NULL){
				quad_goto_true_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
				quad_goto_true_ptr->arg_2.constant = *(int *)(child_1->atr_ptr->result_value);
			}
			else{
				quad_goto_true_ptr->arg_2_type = QUAD_ADDR_TYPE_NAME;
				quad_goto_true_ptr->arg_2.name = child_1->atr_ptr->result_entry;
			}

			Quad_Node *quad_goto_false_ptr = Quad_Node_new();
			quad_goto_false_ptr->op = QUAD_OP_GOTO;
			quad_goto_false_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_goto_false_ptr->result.name = etr_label_false_ptr;


			// Quad list
			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList_pushback(quad_lst_ptr, quad_goto_true_ptr);
			LinkedList_pushback(quad_lst_ptr, quad_goto_false_ptr);

			break;
		}

		case AST_OPERATOR_ASSIGN:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;


			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *child_1_quad_lst_ptr = child_1->atr_ptr->code;
			child_1->atr_ptr->code = NULL;

			// !!!
			if(child_1_quad_lst_ptr != NULL){
				LinkedList_append(quad_lst_ptr, child_1_quad_lst_ptr);
				LinkedList_destroy(child_1_quad_lst_ptr);
			}


			Type *type_0_ptr = child_0->atr_ptr->type;


			if(type_0_ptr->type_enum != TYPE_ENUM_LIST){
				SymbolEnv_Entry *etr_ptr = child_0->atr_ptr->entry;

				if(type_0_ptr->type_enum == TYPE_ENUM_NUM){
					Quad_Node *quad_ptr = Quad_Node_new();
					LinkedList_pushback(quad_lst_ptr, quad_ptr);

					quad_ptr->op = QUAD_OP_COPY;
					quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->result.name = etr_ptr;

					if(child_1->atr_ptr->result_value != NULL){
						quad_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_ptr->arg_1.constant =  *(int *)(child_1->atr_ptr->result_value);
					}
					else{
						quad_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
						quad_ptr->arg_1.name = child_1->atr_ptr->result_entry;
					}
				}

				else if(type_0_ptr->type_enum == TYPE_ENUM_STR){
					int len_string = type_0_ptr->len_string;

					for (int idx = 0; idx < len_string; ++idx){
						Quad_Node *quad_a_ptr = Quad_Node_new();
						Quad_Node *quad_b_ptr = Quad_Node_new();

						LinkedList_pushback(quad_lst_ptr, quad_a_ptr);
						LinkedList_pushback(quad_lst_ptr, quad_b_ptr);

						quad_a_ptr->result_type = QUAD_ADDR_TYPE_REG;
						quad_a_ptr->result.reg = REG_0;

						if(child_1->atr_ptr->result_value != NULL){
							quad_a_ptr->op = QUAD_OP_COPY;

							quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_a_ptr->arg_1.constant =  ((char *)(child_1->atr_ptr->result_value))[idx];
						}
						else{
							quad_a_ptr->op = QUAD_OP_COPY_INDEXED_R;

							quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
							quad_a_ptr->arg_1.name = child_1->atr_ptr->result_entry;
							
							quad_a_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_a_ptr->arg_2.constant = idx;
						}

						quad_b_ptr->op = QUAD_OP_COPY_INDEXED_L;
						quad_b_ptr->result_type = QUAD_ADDR_TYPE_NAME;
						quad_b_ptr->result.name = etr_ptr;
						quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
						quad_b_ptr->arg_1.reg = REG_0;
						quad_b_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_b_ptr->arg_2.constant = idx;
					}
				}

				else if(type_0_ptr->type_enum == TYPE_ENUM_MATRIX){
					int num_rows = type_0_ptr->num_rows;
					int num_columns = type_0_ptr->num_columns;

					for (int i = 0; i < num_rows; ++i){
						for (int j = 0; j < num_columns; ++j){
							int idx = i*num_columns + j;

							Quad_Node *quad_a_ptr = Quad_Node_new();	// reg0 = ch1[i]
							Quad_Node *quad_b_ptr = Quad_Node_new();	// ch0[i] = reg0

							LinkedList_pushback(quad_lst_ptr, quad_a_ptr);
							LinkedList_pushback(quad_lst_ptr, quad_b_ptr);


							quad_a_ptr->result_type = QUAD_ADDR_TYPE_REG;
							quad_a_ptr->result.reg = REG_0;

							if(child_1->atr_ptr->result_value != NULL){
								quad_a_ptr->op = QUAD_OP_COPY;

								quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
								quad_a_ptr->arg_1.constant = ((int *)(child_1->atr_ptr->result_value))[idx];
							}
							else{
								quad_a_ptr->op = QUAD_OP_COPY_INDEXED_R;

								quad_a_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
								quad_a_ptr->arg_1.name = child_1->atr_ptr->result_entry;

								quad_a_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
								quad_a_ptr->arg_2.constant = idx;
							}


							quad_b_ptr->op = QUAD_OP_COPY_INDEXED_L;
							quad_b_ptr->result_type = QUAD_ADDR_TYPE_NAME;
							quad_b_ptr->result.name = etr_ptr;
							quad_b_ptr->arg_1_type = QUAD_ADDR_TYPE_REG;
							quad_b_ptr->arg_1.reg = REG_0;
							quad_b_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
							quad_b_ptr->arg_2.constant = idx;
						}
					}
				}
			}

			else{
				// List

				SymbolEnv_Entry *etr_0_ptr = child_0->child->atr_ptr->entry;
				Type *type_0_ptr = child_0->child->atr_ptr->type;

				if(type_0_ptr->type_enum != TYPE_ENUM_NUM){
					fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case: Not a NUM is list\n");
					return -1;
				}

				Quad_Node *quad_ptr = Quad_Node_new();
				LinkedList_pushback(quad_lst_ptr, quad_ptr);

				quad_ptr->op = QUAD_OP_COPY;
				quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;
				quad_ptr->result.name = etr_0_ptr;

				if(child_1->atr_ptr->result_value != NULL){
					quad_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
					quad_ptr->arg_1.constant =  *(int *)(child_1->atr_ptr->result_value);
				}
				else{
					fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case: Name in list\n");
					return -1;
				}


				if(child_0->child->sibling != NULL){
					SymbolEnv_Entry *etr_1_ptr = child_0->child->sibling->atr_ptr->entry;
					Type *type_1_ptr = child_0->child->sibling->atr_ptr->type;

					if(type_1_ptr->type_enum != TYPE_ENUM_NUM){
						fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case: Not a NUM is list\n");
						return -1;
					}

					Quad_Node *quad_ptr = Quad_Node_new();
					LinkedList_pushback(quad_lst_ptr, quad_ptr);

					quad_ptr->op = QUAD_OP_COPY;
					quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;
					quad_ptr->result.name = etr_1_ptr;

					if(child_1->atr_ptr->result_value != NULL){
						quad_ptr->arg_1_type = QUAD_ADDR_TYPE_CONSTANT;
						quad_ptr->arg_1.constant =  *(int *)(child_1->atr_ptr->result_2_value);
					}
					else{
						fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case: Name in list\n");
						return -1;
					}


					if(child_0->child->sibling->sibling != NULL){
						fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case: List too long\n");
						return -1;
					}
				}

			}

			break;
		}

		case AST_OPERATOR_CALL:
		{
			// No code
			break;
		}

		case AST_OPERATOR_COND:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);
			ParseTree_Node *child_2 = ParseTree_Node_get_child_by_node_index(node_ptr, 2);


			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			LinkedList *quad_0_lst_ptr = NULL;
			LinkedList *quad_1_lst_ptr = NULL;
			LinkedList *quad_2_lst_ptr = NULL;


			// Generate labels
			SymbolEnv_Entry *etr_label_true_ptr = get_label(env_ptr, num_label);
			SymbolEnv_Entry *etr_label_false_ptr = get_label(env_ptr, num_label);
			SymbolEnv_Entry *etr_label_next_ptr = NULL;

			if(child_2 != NULL){
				etr_label_next_ptr = get_label(env_ptr, num_label);
			}


			// Pass label to first child
			child_0->atr_ptr->label_true = etr_label_true_ptr;
			child_0->atr_ptr->label_false = etr_label_false_ptr;

			status = Icg_generate_quads_recursive(child_0, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			status = Icg_generate_quads_recursive(child_1, env_ptr, num_temp, num_label);
			if(status == -1)
				return status;

			quad_0_lst_ptr = child_0->atr_ptr->code;
			quad_1_lst_ptr = child_1->atr_ptr->code;

			child_0->atr_ptr->code = NULL;
			child_1->atr_ptr->code = NULL;

			if(child_2 != NULL){
				status = Icg_generate_quads_recursive(child_2, env_ptr, num_temp, num_label);
				if(status == -1)
					return status;

				quad_2_lst_ptr = child_2->atr_ptr->code;
				child_2->atr_ptr->code = NULL;
			}


			// Create quads for labels
			Quad_Node *quad_label_true_ptr = NULL;
			Quad_Node *quad_label_false_ptr = NULL;
			Quad_Node *quad_label_next_ptr = NULL;
			Quad_Node *quad_goto_next_ptr = NULL;

			quad_label_true_ptr = Quad_Node_new();
			quad_label_true_ptr->op = QUAD_OP_LABEL;
			quad_label_true_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_label_true_ptr->result.name = etr_label_true_ptr;

			quad_label_false_ptr = Quad_Node_new();
			quad_label_false_ptr->op = QUAD_OP_LABEL;
			quad_label_false_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_label_false_ptr->result.name = etr_label_false_ptr;

			if(child_2 != NULL){
				quad_label_next_ptr = Quad_Node_new();
				quad_label_next_ptr->op = QUAD_OP_LABEL;
				quad_label_next_ptr->result_type = QUAD_ADDR_TYPE_NAME;
				quad_label_next_ptr->result.name = etr_label_next_ptr;

				quad_goto_next_ptr = Quad_Node_new();
				quad_goto_next_ptr->op = QUAD_OP_GOTO;
				quad_goto_next_ptr->result_type = QUAD_ADDR_TYPE_NAME;
				quad_goto_next_ptr->result.name = etr_label_next_ptr;
			}


			// Build up quad list

			LinkedList_append(quad_lst_ptr, quad_0_lst_ptr);
			LinkedList_destroy(quad_0_lst_ptr);

			LinkedList_pushback(quad_lst_ptr, quad_label_true_ptr);

			LinkedList_append(quad_lst_ptr, quad_1_lst_ptr);
			LinkedList_destroy(quad_1_lst_ptr);

			if(child_2 != NULL){
				LinkedList_pushback(quad_lst_ptr, quad_goto_next_ptr);
			}

			LinkedList_pushback(quad_lst_ptr, quad_label_false_ptr);

			if(child_2 != NULL){
				LinkedList_append(quad_lst_ptr, quad_2_lst_ptr);
				LinkedList_destroy(quad_2_lst_ptr);

				LinkedList_pushback(quad_lst_ptr, quad_label_next_ptr);
			}


			// if(quad_0_lst_ptr)	quad_list_destroy(quad_0_lst_ptr);
			// if(quad_1_lst_ptr)	quad_list_destroy(quad_1_lst_ptr);
			// if(quad_2_lst_ptr)	quad_list_destroy(quad_2_lst_ptr);


			break;
		}

		case AST_OPERATOR_DECL:
		{
			// No code
			break;
		}

		case AST_OPERATOR_DEF:
		{
			fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case\n");
			return -1;

			break;
		}

		case AST_OPERATOR_ID_LIST:
		{
			// No code
			break;
		}

		case AST_OPERATOR_MATRIX:
		{
			Type *type_ptr = node_ptr->atr_ptr->type;

			int num_rows = type_ptr->num_rows;
			int num_columns = type_ptr->num_columns;

			int *value_ptr = malloc( sizeof(int) * num_rows * num_columns );

			ParseTree_Node *row_node_ptr = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *column_node_ptr;

			for (int i = 0; i < num_rows; ++i){
				column_node_ptr = row_node_ptr;

				for (int j = 0; j < num_columns; ++j){

					value_ptr[i*num_columns + j] = strtol(column_node_ptr->tkn_ptr->data->string, NULL, 10);
					column_node_ptr = column_node_ptr->sibling;
				}

				row_node_ptr = row_node_ptr->child;
			}

			node_ptr->atr_ptr->result_value = value_ptr;

			break;
		}

		case AST_OPERATOR_MATRIX_ELEMENT:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			SymbolEnv_Entry *etr_matrix_ptr = node_ptr->atr_ptr->entry;

			Type *type_matrix_ptr = SymbolEnv_Entry_get_type(etr_matrix_ptr);
			int num_columns = type_matrix_ptr->num_columns;

			int idx_row = strtol(child_0->tkn_ptr->data->string, NULL, 10);
			int idx_column = strtol(child_1->tkn_ptr->data->string, NULL, 10);
			int idx = idx_row*num_columns + idx_column;


			char *tmp_id = tmp_num_to_string((*num_temp)++);
			int len_tmp_id = strlen(tmp_id);
			Type *type_ptr = Type_new(TYPE_ENUM_NUM);

			SymbolEnv_Entry *etr_tmp_ptr = SymbolEnv_entry_add(env_ptr, tmp_id, len_tmp_id, type_ptr);
			node_ptr->atr_ptr->result_entry = etr_tmp_ptr;


			LinkedList* quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			Quad_Node *quad_ptr = Quad_Node_new();
			LinkedList_pushback(quad_lst_ptr, quad_ptr);

			quad_ptr->op = QUAD_OP_COPY_INDEXED_R;

			quad_ptr->arg_1_type = QUAD_ADDR_TYPE_NAME;
			quad_ptr->arg_1.name = etr_matrix_ptr;
			
			quad_ptr->arg_2_type = QUAD_ADDR_TYPE_CONSTANT;
			quad_ptr->arg_2.constant = idx;
			
			quad_ptr->result_type = QUAD_ADDR_TYPE_NAME;
			quad_ptr->result.name = etr_tmp_ptr;

			free(tmp_id);

			break;
		}

		case AST_OPERATOR_DEF_PARAM_LIST:
		{
			// No code
			break;
		}

		case AST_OPERATOR_CALL_PARAM_LIST:
		{
			// No code
			break;
		}

		case AST_OPERATOR_STMT_LIST:
		case AST_OPERATOR_STMT_OR_DEF_LIST:
		{
			ParseTree_Node *child = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			LinkedList *quad_lst_ptr = LinkedList_new();
			node_ptr->atr_ptr->code = quad_lst_ptr;

			while(child){
				status = Icg_generate_quads_recursive(child, env_ptr, num_temp, num_label);
				if(status == -1)
					return status;

				LinkedList *child_quad_lst_ptr = child->atr_ptr->code;
				child->atr_ptr->code = NULL;
				
				if(child_quad_lst_ptr != NULL){
					LinkedList_append(quad_lst_ptr, child_quad_lst_ptr);
					LinkedList_destroy(child_quad_lst_ptr);
				}

				child = child->sibling;
			}

			break;
		}// No code

		default:
		{
			fprintf(stderr, "Icg_generate_quads_recursive : Unhandled case\n");
			return -1;

			break;
		}
	}

	return 0;
}


void quad_list_destroy(LinkedList *quad_lst_ptr){
	while( LinkedList_peek(quad_lst_ptr) != NULL ){
		Quad_Node_destroy( LinkedList_pop(quad_lst_ptr) );
	}
	LinkedList_destroy(quad_lst_ptr);
}

void print_quad_list(LinkedList *quad_lst_ptr){
	LinkedListIterator *itr_ptr = LinkedListIterator_new(quad_lst_ptr);
	LinkedListIterator_move_to_first(itr_ptr);

	Quad_Node *quad_ptr = LinkedListIterator_get_item(itr_ptr);

	while(quad_ptr){


		if(quad_ptr->result_type == QUAD_ADDR_TYPE_EMPTY)
			printf("%*s   ", 15, "");
		else if(quad_ptr->result_type == QUAD_ADDR_TYPE_NAME)
			printf("%*s (mem)   ", 10, SymbolEnv_Entry_get_id(quad_ptr->result.name));
		else if(quad_ptr->result_type == QUAD_ADDR_TYPE_REG)
			printf("%*d (reg)   ", 10, quad_ptr->result.reg);

		printf("%*s   ", 15, quad_operator_to_name(quad_ptr->op) );

		if(quad_ptr->arg_1_type == QUAD_ADDR_TYPE_EMPTY)
			printf("%*s   ", 15, "");
		else if(quad_ptr->arg_1_type == QUAD_ADDR_TYPE_NAME)
			printf("%*s (mem)   ", 10, SymbolEnv_Entry_get_id(quad_ptr->arg_1.name));
		else if(quad_ptr->arg_1_type == QUAD_ADDR_TYPE_CONSTANT)
			printf("%*lld (cns)   ", 10, quad_ptr->arg_1.constant);
		else if(quad_ptr->arg_1_type == QUAD_ADDR_TYPE_REG)
			printf("%*d (reg)   ", 10, quad_ptr->arg_1.reg);

		if(quad_ptr->arg_2_type == QUAD_ADDR_TYPE_EMPTY)
			printf("%*s   ", 15, "");
		else if(quad_ptr->arg_2_type == QUAD_ADDR_TYPE_NAME)
			printf("%*s (mem)   ", 10, SymbolEnv_Entry_get_id(quad_ptr->arg_2.name));
		else if(quad_ptr->arg_2_type == QUAD_ADDR_TYPE_CONSTANT)
			printf("%*lld (cns)   ", 10, quad_ptr->arg_2.constant);
		else if(quad_ptr->arg_2_type == QUAD_ADDR_TYPE_REG)
			printf("%*d (reg)   ", 10, quad_ptr->arg_2.reg);

		printf("\n");

		printf("---------------------------------------------------------------------------\n");

		LinkedListIterator_move_to_next(itr_ptr);
		quad_ptr = LinkedListIterator_get_item(itr_ptr);
	}

	LinkedListIterator_destroy(itr_ptr);
}

static char *quad_operator_to_name(int op){
	if(op > 0 && op < len_quad_operator_names ){
		return quad_operator_names[op];
	}
	return quad_operator_names[len_quad_operator_names-1];
}


static char *label_num_to_string(long long num){
	char *buf = malloc( sizeof(char) * (MAX_LABEL_LEN+1) );
	int size = snprintf(buf, MAX_LABEL_LEN+1, "%s%lld", LABEL_PREFIX, num);

	if(size > MAX_LABEL_LEN){
		fprintf(stderr, "label_num_to_string : Label too long\n");
		free(buf);
		return NULL;
	}

	return buf;
}


static char *tmp_num_to_string(long long num){
	char *buf = malloc( sizeof(char) * (MAX_LABEL_LEN+1) );
	int size = snprintf(buf, MAX_LABEL_LEN+1, "%s%lld", TEMP_PREFIX, num);

	if(size > MAX_LABEL_LEN){
		fprintf(stderr, "label_num_to_string : Label too long\n");
		free(buf);
		return NULL;
	}

	return buf;
}

static SymbolEnv_Entry *get_label(SymbolEnv *env_ptr, int *num_label){
	char *label_id = label_num_to_string((*num_label)++);
	int len_label_id = strlen(label_id);
	SymbolEnv_Entry *etr_label_ptr = SymbolEnv_entry_add(env_ptr, label_id, len_label_id, Type_new(TYPE_ENUM_LABEL));
	free(label_id);

	return etr_label_ptr;
}
