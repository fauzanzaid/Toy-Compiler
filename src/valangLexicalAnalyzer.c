#include <stdio.h>
#include <stdlib.h>

#include "HashTable.h"
#include "Dfa.h"
#include "Lexer.h"
#include "Token.h"
#include "ParserLL1.h"
#include "SymbolEnv.h"

#include "Dfa_init.h"
#include "Lexer_init.h"
#include "Token_Data.h"
#include "Parser_init.h"
#include "Ast_init.h"
#include "Type_init.h"
#include "Semantic_init.h"
#include "Icg_init.h"
#include "CodeGen_init.h"

int main(int argc, char const *argv[])
{

	FILE *file_ptr;
	// file_ptr = fopen("bin/test_input.txt", "r");
	// file_ptr = fopen("bin/syntax_testcase1.txt", "r");
	// file_ptr = fopen("bin/semantics_testcase1.txt", "r");
	file_ptr = fopen("bin/codegen_c1.txt", "r");
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

	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Parser_init_add_rules(psr_ptr);
	Parser_init_add_rules_from_file(psr_ptr, "bin/grammar.txt");
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

		if(token_ignore(tkn_ptr) == 1){
			Token_destroy(tkn_ptr);
			continue;
		}

		// pretty_print_token(tkn_ptr);

		if(tkn_ptr->type == TOKEN_ERROR){
			printf("Lexing error\n");
			Token_destroy(tkn_ptr);
			continue;
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

	Lexer_print_errors(lxr_ptr);
	ParserLL1_print_errors(psr_ptr);
	print_parse_tree_preorder( ParserLL1_get_parse_tree(psr_ptr), stdout );

	ParseTree *tree_ptr = prune_parse_tree( ParserLL1_get_parse_tree(psr_ptr) );
	print_abstract_tree_preorder( tree_ptr, stdout );

	SymbolEnv *env_ptr = SymbolEnv_new("_main",5);
	int flag_semantic_error = 0;
	int flag_root_scope_initialized_error = 0;
	int flag_print_errors = 1;
	Semantic_symbol_and_type_check(tree_ptr, env_ptr, &flag_semantic_error, flag_print_errors);
	flag_root_scope_initialized_error = Semantic_root_scope_initialized_check(env_ptr, flag_print_errors);

	print_abstract_tree_preorder( tree_ptr, stdout );

	if(flag_semantic_error == 0 && flag_root_scope_initialized_error == 0){
		printf("Semantic check successful\n");
	}

	// if( Semantic_initialized_check(env_ptr, flag_print_errors) == -1 ){
	// 	printf("Uninitiaized values exist\n");
	// }

	// print_symbol_environment(env_ptr, stdout);

	SymbolEnv_layout_memory(env_ptr, (int(*)(void *))&Type_get_size);
	print_symbol_environment(env_ptr, stdout);

	LinkedList *quad_lst_ptr = Icg_generate_quads(tree_ptr, env_ptr);

	if(quad_lst_ptr != NULL){
		printf("Intermediate code generation successful\n");
		print_quad_list(quad_lst_ptr);

		SymbolEnv_layout_memory(env_ptr, (int(*)(void *))&Type_get_size);
		print_symbol_environment(env_ptr, stdout);


		FILE *asm_file_ptr = fopen("bin/test.asm", "w");
		CodeGen_generate_asm(env_ptr, quad_lst_ptr, asm_file_ptr);
		fclose(asm_file_ptr);


		quad_list_destroy(quad_lst_ptr);

	}

	SymbolEnv_destroy(env_ptr);
	ParseTree_destroy(tree_ptr);
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	fclose(file_ptr);

	return 0;
}
