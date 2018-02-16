#include <stdio.h>
#include <stdlib.h>

#include "HashTable.h"
#include "Dfa.h"
#include "Lexer.h"
#include "Token.h"
#include "ParserLL1.h"

#include "Dfa_init.h"
#include "Lexer_init.h"
#include "Token_Data.h"
#include "Parser_init.h"

int main(int argc, char const *argv[])
{

	FILE *file_ptr;
	file_ptr = fopen("bin/test_input.txt", "r");
	if(file_ptr == NULL){
		printf("Unable to open file\n");
		return -1;
	}

	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	Dfa_init_add_transitions(dfa_ptr);

	// Initializes  hashtable
	keyword_table_init();
	// Lexer *lxr_ptr = Lexer_new(dfa_ptr, stdin, 1, success_evaluate_function, error_evaluate_function);
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, 1, success_evaluate_function, error_evaluate_function);

	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol);
	Parser_init_add_rules(psr_ptr);
	ParserLL1_initialize_rules(psr_ptr);

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
			Token_destroy(tkn_ptr);
			break;
		}

		else if(tkn_ptr->type == TOKEN_ERROR){
			printf("ERROR token received\n");
			Token_destroy(tkn_ptr);
			break;
		}

		Token_destroy(tkn_ptr);
	}

	fclose(file_ptr);

	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	Dfa_destroy(dfa_ptr);

	return 0;
}
