#ifndef INCLUDE_GUARD_F1333A6825DF4E99A2C3FDF02EFE639F
#define INCLUDE_GUARD_F1333A6825DF4E99A2C3FDF02EFE639F

#include "Token.h"
#include "HashTable.h"


///////////////
// Hashtable //
///////////////

extern HashTable *keyword_table;
void keyword_table_init();


///////////////
// Functions //
///////////////

void success_evaluate_function(Token *tkn_ptr, int state, char *string, int len_string);
void error_evaluate_function(Token *tkn_ptr);

#endif
