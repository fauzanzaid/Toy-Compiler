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
	// file_ptr = fopen("bin/test_input.txt", "r");
	file_ptr = fopen("bin/testcase5.txt", "r");
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

	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, token_to_symbol);
	Parser_init_add_rules(psr_ptr);
	ParserLL1_initialize_rules(psr_ptr);

	Token *tkn_ptr;
	int parser_status;

	int flag = 1;

	// for(int i=0; i<100; ++i)
	while(flag == 1)
	{
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			printf("NULL token received\n");
			break;
		}

		if(token_ignore(tkn_ptr) == 1)
			continue;

		printf("sym=%d\t\t\t", token_to_symbol(tkn_ptr));
		pretty_print_token(tkn_ptr);

		if(tkn_ptr->type == TOKEN_ERROR){
			printf("ERROR token received\n");
			Token_destroy(tkn_ptr);
			break;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			flag = 0;
		}

		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if(parser_status<0)
			printf("Parsing error %d\n", parser_status);
		else if(parser_status == PARSER_STEP_RESULT_SUCCESS)
			printf("Parsing successful\n");

		// printf("\n");
	}

	fclose(file_ptr);

	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	Dfa_destroy(dfa_ptr);

	return 0;
}
