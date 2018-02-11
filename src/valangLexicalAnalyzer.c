#include <stdio.h>
#include <stdlib.h>

#include "HashTable.h"
#include "Dfa.h"
#include "Lexer.h"
#include "Token.h"

#include "Dfa_init.h"
#include "Lexer_init.h"
#include "Token_Data.h"

int main(int argc, char const *argv[])
{
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	Dfa_init_add_transitions(dfa_ptr);

	// Lexer *lxr_ptr = Lexer_new(dfa_ptr, stdin, 1, success_evaluate_function, error_evaluate_function);
	FILE *file_ptr;
	file_ptr = fopen("test_input.txt", "r");
	if(file_ptr == NULL){
		printf("Unable to open file\n");
		return -1;
	}
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, 1, success_evaluate_function, error_evaluate_function);


	// Imitializes  hashtable
	keyword_table_init();

	Token *tkn_ptr;

	// for(int i=0; i<5; ++i)
	while(1)
	{
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			printf("NULL token received\n");
			break;
		}

		pretty_print_token(tkn_ptr);

		if(tkn_ptr->type == TOKEN_EOT){
			printf("EOT token received\n");
			break;
		}

		else if(tkn_ptr->type == TOKEN_ERROR){
			printf("ERROR token received\n");
			break;
		}

		// Token_destroy(tkn_ptr);
	}

	// fclose(file_ptr);
	Lexer_destroy(lxr_ptr);
	Dfa_destroy(dfa_ptr);

	return 0;
}
