#ifndef INCLUDE_GUARD_46914D4B75094B9C9FD63ED2AAEF9A2A
#define INCLUDE_GUARD_46914D4B75094B9C9FD63ED2AAEF9A2A


#include <stdio.h>


#include "LinkedList.h"
#include "SymbolEnv.h"
#include "Quad.h"


///////////////
// Functions //
///////////////


void CodeGen_generate_asm(SymbolEnv *env_ptr, LinkedList *quad_lst_ptr, FILE *output_file_ptr);


#endif
