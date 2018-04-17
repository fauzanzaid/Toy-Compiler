#ifndef INCLUDE_GUARD_05B114D39E434BA39D3BD0510239B6ED
#define INCLUDE_GUARD_05B114D39E434BA39D3BD0510239B6ED


#include "ParseTree.h"
#include "SymbolEnv.h"


///////////////
// Functions //
///////////////


int Semantic_symbol_and_type_check(ParseTree_Node *node_ptr, SymbolEnv *env_ptr, int *flag_error, int flag_print_errors);

int Semantic_initialized_check(SymbolEnv *env_ptr, int flag_print_errors);

int Semantic_root_scope_initialized_check(SymbolEnv *env_ptr, int flag_print_errors);

void print_symbol_environment(SymbolEnv *env_ptr, FILE *file_ptr);

#endif
