#include <stdio.h>
#include <string.h>


#include "LinkedList.h"
#include "SymbolEnv.h"
#include "Quad.h"


#include "CodeGen_init.h"


///////////////
// Constants //
///////////////

static const char VAR_LABEL[] = "vars";


////////////////////
// Pvt Prototypes //
////////////////////

static void CodeGen_generate_text(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr);

static void CodeGen_generate_bss(SymbolEnv *env_ptr, FILE *output_file_ptr);


///////////////
// Functions //
///////////////


void CodeGen_generate_asm(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr){
	CodeGen_generate_text(env_ptr, quad_lst_ptr, output_file_ptr);
	CodeGen_generate_bss(env_ptr, output_file_ptr);
}

static void CodeGen_generate_text(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr){

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
