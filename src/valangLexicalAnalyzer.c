#include <stdio.h>

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
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, stdin, 1, success_evaluate_function, error_evaluate_function);



	Token *tkn_ptr = Lexer_get_next_token(lxr_ptr);

	// for(int i=0; i<5; ++i)
	while(1)
	{
		// printf("lin:%d\tcol:%d\tpos:%d\ttyp:%d\n\n", tkn_ptr->line, tkn_ptr->column, tkn_ptr->position, tkn_ptr->type);
		pretty_print_token(tkn_ptr);

		if(tkn_ptr->type == TOKEN_EOT)	break;
		if(tkn_ptr->type == TOKEN_ERROR)	break;

		tkn_ptr = Lexer_get_next_token(lxr_ptr);
		if(tkn_ptr == NULL)	break;
	}


	Lexer_destroy(lxr_ptr);
	Dfa_destroy(dfa_ptr);

	return 0;
}
