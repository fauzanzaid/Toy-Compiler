#include <stdlib.h>


#include "LinkedList.h"
#include "SymbolEnv.h"
#include "Quad.h"


#include "CodeGen_init.h"


////////////////////
// Pvt Prototypes //
////////////////////

static void CodeGen_generate_text(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr);

static void CodeGen_generate_bss(SymbolEnv *env_ptr, FILE *output_file_ptr);


///////////////
// Functions //
///////////////


void CodeGen_generate_asm(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr){

}

static void CodeGen_generate_text(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr){

}

static void CodeGen_generate_bss(SymbolEnv *env_ptr, FILE *output_file_ptr){

}
