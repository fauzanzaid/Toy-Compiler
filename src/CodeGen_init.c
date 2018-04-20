#include <stdio.h>
#include <string.h>


#include "LinkedList.h"
#include "SymbolEnv.h"
#include "Quad.h"


#include "Type_init.h"
#include "CodeGen_init.h"


///////////////
// Constants //
///////////////

static char START_LABEL[] = "_start";
static char VAR_LABEL[] = "vars";

static char BYTE_DIR[] = "byte";
static char WORD_DIR[] = "word";

static char REG_AL[] = "al";
static char REG_BL[] = "bl";
static char REG_CL[] = "cl";
static char REG_AX[] = "ax";
static char REG_BX[] = "bx";
static char REG_CX[] = "cx";
static char REG_R8B[] = "r8b";
static char REG_R9B[] = "r9b";
static char REG_R8W[] = "r8w";
static char REG_R9W[] = "r9w";


////////////////////
// Pvt Prototypes //
////////////////////

static void CodeGen_generate_text(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr);

static void CodeGen_generate_bss(SymbolEnv *env_ptr, FILE *output_file_ptr);

// static void CodeGen_generate_instr_mov()

///////////////
// Functions //
///////////////


void CodeGen_generate_asm(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr){
	CodeGen_generate_text(env_ptr, quad_lst_ptr, output_file_ptr);
	fprintf(output_file_ptr, "\n");
	CodeGen_generate_bss(env_ptr, output_file_ptr);
}

static void CodeGen_generate_text(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr){

	fprintf(output_file_ptr, "global %s\n", START_LABEL);
	fprintf(output_file_ptr, "\tsection .text\n");
	fprintf(output_file_ptr, "%s:\n", START_LABEL);

	LinkedListIterator *itr_ptr = LinkedListIterator_new(quad_lst_ptr);
	LinkedListIterator_move_to_first(itr_ptr);

	Quad_Node *quad_ptr = LinkedListIterator_get_item(itr_ptr);

	while(quad_ptr){

		int				quad_result_addr_type = 0,		quad_arg_1_addr_type = 0,		quad_arg_2_addr_type = 0;
		SymbolEnv_Entry	*quad_result_etr_ptr = NULL,	*quad_arg_1_etr_ptr = NULL,		*quad_arg_2_etr_ptr = NULL;
		Type			*quad_result_type_ptr = NULL,	*quad_arg_1_type_ptr = NULL,	*quad_arg_2_type_ptr = NULL;
		char			*quad_result_id = NULL,			*quad_arg_1_id = NULL,			*quad_arg_2_id = NULL;
		int				quad_result_offset = 0,			quad_arg_1_offset = 0,			quad_arg_2_offset = 0;
		int				quad_result_reg = 0,			quad_arg_1_reg = 0,				quad_arg_2_reg = 0;
		int				quad_result_constant = 0,		quad_arg_1_constant = 0,		quad_arg_2_constant = 0;

		quad_result_addr_type = quad_ptr->result_type;
		quad_arg_1_addr_type = quad_ptr->arg_1_type;
		quad_arg_2_addr_type = quad_ptr->arg_2_type;

		if(quad_result_addr_type == QUAD_ADDR_TYPE_NAME){
			quad_result_etr_ptr = quad_ptr->result.name;
			quad_result_type_ptr = SymbolEnv_Entry_get_type(quad_result_etr_ptr);
			quad_result_id = SymbolEnv_Entry_get_id(quad_result_etr_ptr);
			quad_result_offset = SymbolEnv_Entry_get_offset(quad_result_etr_ptr);
		}
		else if(quad_result_addr_type == QUAD_ADDR_TYPE_REG){
			quad_result_reg = quad_ptr->result.reg;
		}
		else if(quad_result_addr_type == QUAD_ADDR_TYPE_CONSTANT){
			quad_result_constant = quad_ptr->result.constant;
		}

		if(quad_arg_1_addr_type == QUAD_ADDR_TYPE_NAME){
			quad_arg_1_etr_ptr = quad_ptr->arg_1.name;
			quad_arg_1_type_ptr = SymbolEnv_Entry_get_type(quad_arg_1_etr_ptr);
			quad_arg_1_id = SymbolEnv_Entry_get_id(quad_arg_1_etr_ptr);
			quad_arg_1_offset = SymbolEnv_Entry_get_offset(quad_arg_1_etr_ptr);
		}
		else if(quad_arg_1_addr_type == QUAD_ADDR_TYPE_REG){
			quad_arg_1_reg = quad_ptr->arg_1.reg;
		}
		else if(quad_arg_1_addr_type == QUAD_ADDR_TYPE_CONSTANT){
			quad_arg_1_constant = quad_ptr->arg_1.constant;
		}

		if(quad_arg_2_addr_type == QUAD_ADDR_TYPE_NAME){
			quad_arg_2_etr_ptr = quad_ptr->arg_2.name;
			quad_arg_2_type_ptr = SymbolEnv_Entry_get_type(quad_arg_2_etr_ptr);
			quad_arg_2_id = SymbolEnv_Entry_get_id(quad_arg_2_etr_ptr);
			quad_arg_2_offset = SymbolEnv_Entry_get_offset(quad_arg_2_etr_ptr);
		}
		else if(quad_arg_2_addr_type == QUAD_ADDR_TYPE_REG){
			quad_arg_2_reg = quad_ptr->arg_2.reg;
		}
		else if(quad_arg_2_addr_type == QUAD_ADDR_TYPE_CONSTANT){
			quad_arg_2_constant = quad_ptr->arg_2.constant;
		}


		char *SIZE_DIR = NULL;
		int SIZE_BYTES = 0;

		Type *tmp_type_ptr = NULL;
		if(quad_result_type_ptr != NULL)	tmp_type_ptr = quad_result_type_ptr;
		else if(quad_arg_1_type_ptr!= NULL)	tmp_type_ptr = quad_arg_1_type_ptr;
		else if(quad_arg_2_type_ptr!= NULL)	tmp_type_ptr = quad_arg_2_type_ptr;

		if(tmp_type_ptr == NULL){
			SIZE_DIR = WORD_DIR;
			SIZE_BYTES = 2;
		}
		else{
			if(tmp_type_ptr->type_enum == TYPE_ENUM_STR){
				SIZE_DIR = BYTE_DIR;
				SIZE_BYTES = 1;
			}
			else{
				SIZE_DIR = WORD_DIR;
				SIZE_BYTES = 2;
			}
		}


		char *REG_A = NULL;
		char *REG_B = NULL;
		char *REG_C = NULL;

		char *REG_U0 = NULL;
		char *REG_U1 = NULL;

		if(SIZE_DIR == BYTE_DIR){
			REG_A = REG_AL;
			REG_B = REG_BL;
			REG_C = REG_CL;
			REG_U0 = REG_R8B;
			REG_U1 = REG_R9B;
		}
		else{
			REG_A = REG_AX;
			REG_B = REG_BX;
			REG_C = REG_CX;
			REG_U0 = REG_R8W;
			REG_U1 = REG_R9W;
		}


		switch(quad_ptr->op){
			case QUAD_OP_IDENTITY:
			case QUAD_OP_NEGATE:
			case QUAD_OP_NOT:
			{
				// Not implemented
				break;
			}

			case QUAD_OP_ADD:
			{

				break;
			}

			case QUAD_OP_SUBTRACT:
			{

				break;
			}

			case QUAD_OP_MULTIPLY:
			{

				break;
			}

			case QUAD_OP_DIVIDE:
			{

				break;
			}

			case QUAD_OP_AND:
			{
				// Not implemented
				break;
			}

			case QUAD_OP_OR:
			{

				break;
			}

			case QUAD_OP_COPY:
			{
				if(quad_result_addr_type == QUAD_ADDR_TYPE_NAME && quad_arg_1_addr_type == QUAD_ADDR_TYPE_NAME){
					fprintf(output_file_ptr, "\tmov" "\t%s"		"\t%s"			", [%s+%d]"	"\t;" "\n", SIZE_DIR, REG_A, VAR_LABEL, quad_arg_1_offset);
					fprintf(output_file_ptr, "\tmov" "\t%s"		"\t[%s+%d]"		", %s"		"\t;" "\n", SIZE_DIR, VAR_LABEL, quad_result_offset, REG_A);
				}

				else{
					fprintf(output_file_ptr, "\tmov" "\t%s", SIZE_DIR);

					switch(quad_result_addr_type){
						case QUAD_ADDR_TYPE_NAME:{
							fprintf(output_file_ptr, "\t[%s+%d]", VAR_LABEL, quad_result_offset);
							break;
						}
						case QUAD_ADDR_TYPE_REG:{
							if(quad_result_reg == 0)
								fprintf(output_file_ptr, "\t%s", REG_U0);
							else if(quad_result_reg == 1)
								fprintf(output_file_ptr, "\t%s", REG_U1);
							break;
						}
					}

					switch(quad_arg_1_addr_type){
						case QUAD_ADDR_TYPE_NAME:{
							fprintf(output_file_ptr, ", [%s+%d]", VAR_LABEL, quad_arg_1_offset);
							break;
						}
						case QUAD_ADDR_TYPE_REG:{
							if(quad_arg_1_reg == 0)
								fprintf(output_file_ptr, ", %s", REG_U0);
							else if(quad_arg_1_reg == 1)
								fprintf(output_file_ptr, ", %s", REG_U1);
							break;
						}
						case QUAD_ADDR_TYPE_CONSTANT:{
							fprintf(output_file_ptr, ", %d", quad_arg_1_constant);
							break;
						}
					}

					printf("\t;");
					printf("\n");
				}

				break;
			}

			case QUAD_OP_COPY_INDEXED_R:
			{
				// Assumption that index (arg_2) is immediate
				int index_offset = quad_arg_2_constant * SIZE_BYTES;

				if(quad_result_addr_type == QUAD_ADDR_TYPE_NAME && quad_arg_1_addr_type == QUAD_ADDR_TYPE_NAME){
					fprintf(output_file_ptr, "\tmov" "\t%s"		"\t%s"			", [%s+%d]"	"\t;" "\n", SIZE_DIR, REG_A, VAR_LABEL, quad_arg_1_offset + index_offset);
					fprintf(output_file_ptr, "\tmov" "\t%s"		"\t[%s+%d]"		", %s"		"\t;" "\n", SIZE_DIR, VAR_LABEL, quad_result_offset, REG_A);
				}

				else{
					fprintf(output_file_ptr, "\tmov" "\t%s", SIZE_DIR);

					switch(quad_result_addr_type){
						case QUAD_ADDR_TYPE_NAME:{
							fprintf(output_file_ptr, "\t[%s+%d]", VAR_LABEL, quad_result_offset);
							break;
						}
						case QUAD_ADDR_TYPE_REG:{
							if(quad_result_reg == 0)
								fprintf(output_file_ptr, "\t%s", REG_U0);
							else if(quad_result_reg == 1)
								fprintf(output_file_ptr, "\t%s", REG_U1);
							break;
						}
					}

					switch(quad_arg_1_addr_type){
						case QUAD_ADDR_TYPE_NAME:{
							fprintf(output_file_ptr, ", [%s+%d]", VAR_LABEL, quad_arg_1_offset + index_offset);
							break;
						}
					}

					printf("\t;");
					printf("\n");
				}

				break;
			}

			case QUAD_OP_COPY_INDEXED_L:
			{
				// Assumption that index (arg_2) is immediate
				int index_offset = quad_arg_2_constant * SIZE_BYTES;

				if(quad_result_addr_type == QUAD_ADDR_TYPE_NAME && quad_arg_1_addr_type == QUAD_ADDR_TYPE_NAME){
					fprintf(output_file_ptr, "\tmov" "\t%s"		"\t%s"			", [%s+%d]"	"\t;" "\n", SIZE_DIR, REG_A, VAR_LABEL, quad_arg_1_offset);
					fprintf(output_file_ptr, "\tmov" "\t%s"		"\t[%s+%d]"		", %s"		"\t;" "\n", SIZE_DIR, VAR_LABEL, quad_result_offset + index_offset, REG_A);
				}

				else{
					fprintf(output_file_ptr, "\tmov" "\t%s", SIZE_DIR);

					switch(quad_result_addr_type){
						case QUAD_ADDR_TYPE_NAME:{
							fprintf(output_file_ptr, "\t[%s+%d]", VAR_LABEL, quad_result_offset + index_offset);
							break;
						}
					}

					switch(quad_arg_1_addr_type){
						case QUAD_ADDR_TYPE_NAME:{
							fprintf(output_file_ptr, ", [%s+%d]", VAR_LABEL, quad_arg_1_offset);
							break;
						}
						case QUAD_ADDR_TYPE_REG:{
							if(quad_arg_1_reg == 0)
								fprintf(output_file_ptr, ", %s", REG_U0);
							else if(quad_arg_1_reg == 1)
								fprintf(output_file_ptr, ", %s", REG_U1);
							break;
						}
						case QUAD_ADDR_TYPE_CONSTANT:{
							fprintf(output_file_ptr, ", %d", quad_arg_1_constant);
							break;
						}
					}

					printf("\t;");
					printf("\n");
				}

				break;
			}

			case QUAD_OP_COPY_ADDRESS_R:
			{
				// Not implemented
				break;
			}

			case QUAD_OP_COPY_DEREFED_R:
			{
				// Not implemented
				break;
			}

			case QUAD_OP_COPY_DEREFED_L:
			{
				// Not implemented
				break;
			}

			case QUAD_OP_LABEL:
			{

				break;
			}

			case QUAD_OP_GOTO:
			{

				break;
			}

			case QUAD_OP_GOTO_LT:
			{

				break;
			}

			case QUAD_OP_GOTO_LE:
			{

				break;
			}

			case QUAD_OP_GOTO_GT:
			{

				break;
			}

			case QUAD_OP_GOTO_GE:
			{

				break;
			}

			case QUAD_OP_GOTO_EQ:
			{

				break;
			}

			case QUAD_OP_GOTO_NE:
			{

				break;
			}

			case QUAD_OP_CALL:
			{
				// Not implemented
				break;
			}

			case QUAD_OP_READ_INT:
			{

				break;
			}

			case QUAD_OP_READ_CHAR:
			{

				break;
			}

			case QUAD_OP_WRITE_INT:
			{

				break;
			}

			case QUAD_OP_WRITE_CHAR:
			{

				break;
			}

			default:
			{
				fprintf(stderr, "CodeGen_generate_text : Unhandle op\n");
				return;

				break;
			}

		}

		LinkedListIterator_move_to_next(itr_ptr);
		quad_ptr = LinkedListIterator_get_item(itr_ptr);
	}

	LinkedListIterator_destroy(itr_ptr);
}

static void CodeGen_generate_bss(SymbolEnv *env_ptr, FILE *output_file_ptr){
	
	fprintf(output_file_ptr, "\tsection .bss\n");
	fprintf(output_file_ptr, "%s:\n", VAR_LABEL);

	SymbolEnv_Scope *scp_ptr = SymbolEnv_scope_reset(env_ptr);

	while(1){
		SymbolEnv_scope_set_explicit(env_ptr, scp_ptr);

		LinkedList *id_lst_ptr = SymbolEnv_Scope_get_id_lst(scp_ptr);

		LinkedListIterator *itr_ptr = LinkedListIterator_new(id_lst_ptr);
		LinkedListIterator_move_to_first(itr_ptr);
		char *id = LinkedListIterator_get_item(itr_ptr);

		while(id){
			SymbolEnv_Entry *etr_ptr = SymbolEnv_entry_get_by_id(env_ptr, id, strlen(id));
			int width = SymbolEnv_Entry_get_size(etr_ptr);
			int offset = SymbolEnv_Entry_get_offset(etr_ptr);

			fprintf(output_file_ptr, "\tresb\t%d\t; %d\t%s\n", width, offset, id);

			LinkedListIterator_move_to_next(itr_ptr);
			id = LinkedListIterator_get_item(itr_ptr);
		}

		LinkedListIterator_destroy(itr_ptr);

		scp_ptr = SymbolEnv_Scope_get_inorder(scp_ptr);
		if(scp_ptr == NULL)
			break;

	}
}
