#ifndef INCLUDE_GUARD_5DF93AC3D06A4C99B1646134D8B425AF
#define INCLUDE_GUARD_5DF93AC3D06A4C99B1646134D8B425AF


#include "LinkedList.h"
#include "ParseTree.h"
#include "SymbolEnv.h"


///////////////
// Functions //
///////////////

LinkedList *Icg_generate_quads(ParseTree_Node *node_ptr, SymbolEnv *env_ptr);

void quad_list_destroy(LinkedList *quad_lst_ptr);

void print_quad_list(LinkedList *quad_lst_ptr);


#endif
