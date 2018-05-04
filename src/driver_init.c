#include <stdio.h>

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

#include "driver_init.h"


void run_lexer(const char *filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Print lexical errors immediatey
	Lexer_set_immediate_print_error(lxr_ptr, 1);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;

	while(1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			Token_destroy(tkn_ptr);
			break;
		}

		// Print token info
		pretty_print_token(tkn_ptr);
		Token_destroy(tkn_ptr);
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Free all resources
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}

void run_parser(const char *filename, const char *grammar_filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Print lexical errors immediatey
	Lexer_set_immediate_print_error(lxr_ptr, 1);
	// Create Parser that will perform parsing and construct parse tree
	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Print parsing errors immediately
	ParserLL1_set_immediate_print_error(psr_ptr, 1);

	// Add grammar rules to parser
	if(grammar_filename == NULL){
		Parser_init_add_rules(psr_ptr);
	}
	else{
		Parser_init_add_rules_from_file(psr_ptr, grammar_filename);
	}
	
	// Calculate first and follow sets and create parsing table
	ParserLL1_initialize_rules(psr_ptr);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;
	// Get the status of parser
	int parser_status;
	// Set flag if error
	int flag_parser_error = 0;
	// Clear flag if EOT is reached
	int flag_run = 1;

	while(flag_run == 1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			flag_run = 0;
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			flag_run = 0;
		}


		// Token can be used by parser
		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if (parser_status == PARSER_STEP_RESULT_SUCCESS){
			// Parsing completed successfully
			printf("   Parsing successfully completed\n");
			flag_run = 0;
			break;
		}

		if (parser_status == PARSER_STEP_RESULT_FAIL){
			// Error encountered. Parser enters panic mode
			flag_parser_error = 1;
		}

		if (parser_status == PARSER_STEP_RESULT_HALTED){
			// Parsing completed unsuccessfully
			flag_parser_error = 1;
			flag_run = 0;
			break;
		}
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Print any errors recorded by parser
	// if(flag_parser_error == 1){
	// 	ParserLL1_print_errors(psr_ptr);
	// }


	// Free all resources
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}

void run_parse_tree_printer(const char *filename, const char *grammar_filename, const char *tree_filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Do not print lexical errors
	Lexer_set_immediate_print_error(lxr_ptr, 0);
	// Create Parser that will perform parsing and construct parse tree
	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Do not print parsing errors
	ParserLL1_set_immediate_print_error(psr_ptr, 0);

	// Add grammar rules to parser
	if(grammar_filename == NULL){
		Parser_init_add_rules(psr_ptr);
	}
	else{
		Parser_init_add_rules_from_file(psr_ptr, grammar_filename);
	}

	// Calculate first and follow sets and create parsing table
	ParserLL1_initialize_rules(psr_ptr);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;
	// Get the status of parser
	int parser_status;
	// Set flag if error
	int flag_parser_error = 0;
	// Clear flag if EOT is reached
	int flag_run = 1;

	while(flag_run == 1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			flag_run = 0;
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			flag_run = 0;
		}


		// Token can be used by parser
		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if (parser_status == PARSER_STEP_RESULT_SUCCESS){
			// Parsing completed successfully
			flag_run = 0;
			break;
		}

		if (parser_status == PARSER_STEP_RESULT_FAIL){
			// Error encountered. Parser enters panic mode
			flag_parser_error = 1;
		}

		if (parser_status == PARSER_STEP_RESULT_HALTED){
			// Parsing completed unsuccessfully
			flag_parser_error = 1;
			flag_run = 0;
			break;
		}
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Print any errors recorded by parser
	// if(flag_parser_error == 1){
	// 	ParserLL1_print_errors(psr_ptr);
	// }


	FILE *tree_file_ptr = NULL;
	if(tree_filename != NULL){
		tree_file_ptr = fopen(tree_filename, "w");
		if(tree_file_ptr == NULL){
			printf("Unable to open file %s. Using stdout\n", tree_filename);
		}
	}
	if(tree_file_ptr == NULL){
		tree_file_ptr = stdout;
	}

	// Print parse tree
	ParseTree *tree_ptr = ParserLL1_get_parse_tree(psr_ptr);
	print_parse_tree( tree_ptr, tree_file_ptr );
	printf("   Parse tree successfully printed\n");

	// Skip if parsing or lexing error
	if(flag_lexer_error == 1 || flag_parser_error == 1){
		printf("   Syntactic errors exist. Parse tree may be incorrect\n");
	}

	// Free all resources
	if(tree_file_ptr != stdout)	fclose(tree_file_ptr);
	ParseTree_destroy(tree_ptr);
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}

void run_ast_printer(const char *filename, const char *grammar_filename, const char *tree_filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Do not print lexical errors
	Lexer_set_immediate_print_error(lxr_ptr, 0);
	// Create Parser that will perform parsing and construct parse tree
	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Do not print parsing errors
	ParserLL1_set_immediate_print_error(psr_ptr, 0);

	// Add grammar rules to parser
	if(grammar_filename == NULL){
		Parser_init_add_rules(psr_ptr);
	}
	else{
		Parser_init_add_rules_from_file(psr_ptr, grammar_filename);
	}

	// Calculate first and follow sets and create parsing table
	ParserLL1_initialize_rules(psr_ptr);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;
	// Get the status of parser
	int parser_status;
	// Set flag if error
	int flag_parser_error = 0;
	// Clear flag if EOT is reached
	int flag_run = 1;

	while(flag_run == 1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			flag_run = 0;
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			flag_run = 0;
		}


		// Token can be used by parser
		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if (parser_status == PARSER_STEP_RESULT_SUCCESS){
			// Parsing completed successfully
			flag_run = 0;
			break;
		}

		if (parser_status == PARSER_STEP_RESULT_FAIL){
			// Error encountered. Parser enters panic mode
			flag_parser_error = 1;
		}

		if (parser_status == PARSER_STEP_RESULT_HALTED){
			// Parsing completed unsuccessfully
			flag_parser_error = 1;
			flag_run = 0;
			break;
		}
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Print any errors recorded by parser
	// if(flag_parser_error == 1){
	// 	ParserLL1_print_errors(psr_ptr);
	// }

	// Skip if parsing or lexing error
	if(flag_lexer_error == 1 || flag_parser_error == 1){
		printf("   Cannot construct AST. Input file is syntactically incorrect\n");

		// Free resources before return
		ParserLL1_destroy(psr_ptr);
		Lexer_destroy(lxr_ptr);
		keyword_table_free();
		Dfa_destroy(dfa_ptr);
		if(filename)	fclose(file_ptr);

		return;
	}


	FILE *tree_file_ptr = NULL;
	if(tree_filename != NULL){
		tree_file_ptr = fopen(tree_filename, "w");
		if(tree_file_ptr == NULL){
			printf("Unable to open file %s. Using stdout\n", tree_filename);
		}
	}
	if(tree_file_ptr == NULL){
		tree_file_ptr = stdout;
	}

	// Print ast
	ParseTree *tree_ptr = prune_parse_tree(ParserLL1_get_parse_tree(psr_ptr));
	print_abstract_tree_preorder(tree_ptr, tree_file_ptr);
	printf("   Abstract syntax tree successfully printed\n");

	// Free all resources
	if(tree_filename)	fclose(tree_file_ptr);
	ParseTree_destroy(tree_ptr);
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}

void run_memory_checker(const char *filename, const char *grammar_filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Do not print lexical errors
	Lexer_set_immediate_print_error(lxr_ptr, 0);
	// Create Parser that will perform parsing and construct parse tree
	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Do not print parsing errors
	ParserLL1_set_immediate_print_error(psr_ptr, 0);

	// Add grammar rules to parser
	if(grammar_filename == NULL){
		Parser_init_add_rules(psr_ptr);
	}
	else{
		Parser_init_add_rules_from_file(psr_ptr, grammar_filename);
	}

	// Calculate first and follow sets and create parsing table
	ParserLL1_initialize_rules(psr_ptr);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;
	// Get the status of parser
	int parser_status;
	// Set flag if error
	int flag_parser_error = 0;
	// Clear flag if EOT is reached
	int flag_run = 1;

	while(flag_run == 1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			flag_run = 0;
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			flag_run = 0;
		}


		// Token can be used by parser
		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if (parser_status == PARSER_STEP_RESULT_SUCCESS){
			// Parsing completed successfully
			flag_run = 0;
			break;
		}

		if (parser_status == PARSER_STEP_RESULT_FAIL){
			// Error encountered. Parser enters panic mode
			flag_parser_error = 1;
		}

		if (parser_status == PARSER_STEP_RESULT_HALTED){
			// Parsing completed unsuccessfully
			flag_parser_error = 1;
			flag_run = 0;
			break;
		}
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Print any errors recorded by parser
	// if(flag_parser_error == 1){
	// 	ParserLL1_print_errors(psr_ptr);
	// }


	// Print memory information
	
	ParseTree *tree_ptr = ParserLL1_get_parse_tree(psr_ptr);
	int node_count_parse_tree = ParseTree_get_node_count(tree_ptr);
	int size_parse_tree = node_count_parse_tree*sizeof(tree_ptr);

	printf("   Parse tree:   Number of nodes = %*d      Allocated memory = %*d bytes\n", 5, node_count_parse_tree,	6, size_parse_tree);

	// Skip if parsing or lexing error
	if(flag_lexer_error == 1 || flag_parser_error == 1){
		printf("\n   Cannot construct AST. Input file is syntactically incorrect\n");

		// Free resources before return
		ParserLL1_destroy(psr_ptr);
		ParseTree_destroy(tree_ptr);
		Lexer_destroy(lxr_ptr);
		keyword_table_free();
		Dfa_destroy(dfa_ptr);
		if(filename)	fclose(file_ptr);

		return;
	}

	tree_ptr = prune_parse_tree(tree_ptr);
	int node_count_ast = ParseTree_get_node_count(tree_ptr);
	int size_ast = node_count_ast*sizeof(tree_ptr);

	double compression = (size_parse_tree - size_ast) / (0.01 * size_parse_tree);

	printf("   AST:          Number of nodes = %*d      Allocated memory = %*d bytes\n", 5, node_count_ast,			6, size_ast);
	printf("\n   Compression percentage = %2.2f%%\n", compression);

	// Free all resources
	ParseTree_destroy(tree_ptr);
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}

void run_symbol_table_printer(const char *filename, const char *grammar_filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Do not print lexical errors
	Lexer_set_immediate_print_error(lxr_ptr, 0);
	// Create Parser that will perform parsing and construct parse tree
	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Do not print parsing errors
	ParserLL1_set_immediate_print_error(psr_ptr, 0);

	// Add grammar rules to parser
	if(grammar_filename == NULL){
		Parser_init_add_rules(psr_ptr);
	}
	else{
		Parser_init_add_rules_from_file(psr_ptr, grammar_filename);
	}

	// Calculate first and follow sets and create parsing table
	ParserLL1_initialize_rules(psr_ptr);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;
	// Get the status of parser
	int parser_status;
	// Set flag if error
	int flag_parser_error = 0;
	// Clear flag if EOT is reached
	int flag_run = 1;

	while(flag_run == 1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			flag_run = 0;
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			flag_run = 0;
		}


		// Token can be used by parser
		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if (parser_status == PARSER_STEP_RESULT_SUCCESS){
			// Parsing completed successfully
			flag_run = 0;
			break;
		}

		if (parser_status == PARSER_STEP_RESULT_FAIL){
			// Error encountered. Parser enters panic mode
			flag_parser_error = 1;
		}

		if (parser_status == PARSER_STEP_RESULT_HALTED){
			// Parsing completed unsuccessfully
			flag_parser_error = 1;
			flag_run = 0;
			break;
		}
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Print any errors recorded by parser
	// if(flag_parser_error == 1){
	// 	ParserLL1_print_errors(psr_ptr);
	// }


	// Skip if parsing or lexing error
	if(flag_lexer_error == 1 || flag_parser_error == 1){
		printf("   Cannot construct symbol table. Input file is syntactically incorrect\n");

		// Free resources before return
		ParserLL1_destroy(psr_ptr);
		Lexer_destroy(lxr_ptr);
		keyword_table_free();
		Dfa_destroy(dfa_ptr);
		if(filename)	fclose(file_ptr);

		return;
	}

	
	// Construct ast
	ParseTree *tree_ptr = prune_parse_tree(ParserLL1_get_parse_tree(psr_ptr));
	// Create Symbol table
	SymbolEnv *env_ptr = SymbolEnv_new("_main",5);

	// Set flag if semantic error
	int flag_semantic_error = 0;
	// Set flag to print semantic errors to stdout
	int flag_print_semantic_error = 0;
	// Set flag if root scope is uninitialized
	int flag_root_scope_initialized_error = 0;

	// Populate symbol table and perform type checking
	Semantic_symbol_and_type_check(tree_ptr, env_ptr, &flag_semantic_error, flag_print_semantic_error);
	// Check root scope for initialzation
	if( Semantic_root_scope_initialized_check(env_ptr, flag_print_semantic_error) == -1 )
		flag_root_scope_initialized_error = 1;

	// Layout memory
	SymbolEnv_layout_memory(env_ptr, (int(*)(void *))&Type_get_size);

	// Print symbol table
	print_symbol_environment(env_ptr, stdout);
	printf("   Symbol table successfully printed\n");

	if(flag_semantic_error || flag_root_scope_initialized_error)
		printf("   Semantic errors exist. Symbol table may be incorrect\n");

	// Free all resources
	SymbolEnv_destroy(env_ptr);
	ParseTree_destroy(tree_ptr);
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}

void run_front_end_checker(const char *filename, const char *grammar_filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Print lexical errors
	Lexer_set_immediate_print_error(lxr_ptr, 1);
	// Create Parser that will perform parsing and construct parse tree
	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Print parsing errors
	ParserLL1_set_immediate_print_error(psr_ptr, 1);

	// Add grammar rules to parser
	if(grammar_filename == NULL){
		Parser_init_add_rules(psr_ptr);
	}
	else{
		Parser_init_add_rules_from_file(psr_ptr, grammar_filename);
	}

	// Calculate first and follow sets and create parsing table
	ParserLL1_initialize_rules(psr_ptr);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;
	// Get the status of parser
	int parser_status;
	// Set flag if error
	int flag_parser_error = 0;
	// Clear flag if EOT is reached
	int flag_run = 1;

	while(flag_run == 1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			flag_run = 0;
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			flag_run = 0;
		}


		// Token can be used by parser
		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if (parser_status == PARSER_STEP_RESULT_SUCCESS){
			// Parsing completed successfully
			flag_run = 0;
			break;
		}

		if (parser_status == PARSER_STEP_RESULT_FAIL){
			// Error encountered. Parser enters panic mode
			flag_parser_error = 1;
		}

		if (parser_status == PARSER_STEP_RESULT_HALTED){
			// Parsing completed unsuccessfully
			flag_parser_error = 1;
			flag_run = 0;
			break;
		}
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Print any errors recorded by parser
	// if(flag_parser_error == 1){
	// 	ParserLL1_print_errors(psr_ptr);
	// }


	// Skip if parsing or lexing error
	if(flag_lexer_error == 1 || flag_parser_error == 1){
		printf("\n   Cannot proceed to semnatic check. Input file is syntactically incorrect\n");

		// Free resources before return
		ParserLL1_destroy(psr_ptr);
		Lexer_destroy(lxr_ptr);
		keyword_table_free();
		Dfa_destroy(dfa_ptr);
		if(filename)	fclose(file_ptr);

		return;
	}


	// Construct ast
	ParseTree *tree_ptr = prune_parse_tree(ParserLL1_get_parse_tree(psr_ptr));
	// Create Symbol table
	SymbolEnv *env_ptr = SymbolEnv_new("_main",5);

	// Set flag if semantic error
	int flag_semantic_error = 0;
	// Print semantic errors to stdout
	int flag_print_semantic_error = 1;
	// Set flag if root scope is uninitialized
	int flag_root_scope_initialized_error = 0;

	// Populate symbol table and perform type checking
	Semantic_symbol_and_type_check(tree_ptr, env_ptr, &flag_semantic_error, flag_print_semantic_error);
	// Check root scope for initialzation
	if( Semantic_root_scope_initialized_check(env_ptr, flag_print_semantic_error) == -1 )
		flag_root_scope_initialized_error = 1;

	if(flag_semantic_error == 0 && flag_root_scope_initialized_error == 0){
		printf("   Input file is semantically and syntactically correct\n");
	}

	// Free all resources
	SymbolEnv_destroy(env_ptr);
	ParseTree_destroy(tree_ptr);
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}

void run_code_generator(const char *filename, const char *grammar_filename, const char *assembly_filename){

	FILE *file_ptr;
	int buffer_size;
	if(filename == NULL){
		file_ptr = stdin;
		buffer_size = 1;
	}
	else{
		file_ptr = fopen(filename, "r");
		buffer_size = 4096;
	}

	if(file_ptr == NULL){
		printf("Unable to open file \"%s\"\n", filename);
		return;
	}

	// Create DFA
	Dfa *dfa_ptr = Dfa_new(states, len_states, symbols, len_symbols, start_state, final_states, len_final_states);
	// Add transition rules to DFA
	Dfa_init_add_transitions(dfa_ptr);
	// Initializes hashtable to differentiate keywords
	keyword_table_init();
	// Create Lexer that will perform lexing
	Lexer *lxr_ptr = Lexer_new(dfa_ptr, file_ptr, buffer_size, success_evaluate_function, error_evaluate_function);
	// Do not print lexical errors
	Lexer_set_immediate_print_error(lxr_ptr, 0);
	// Create Parser that will perform parsing and construct parse tree
	ParserLL1 *psr_ptr = ParserLL1_new(variable_symbols, len_variable_symbols, terminal_symbols, len_terminal_symbols, start_symbol, empty_symbol, end_symbol, forget_terminal_symbols, len_forget_terminal_symbols, token_to_symbol, symbol_to_string, token_to_value);
	// Do not print parsing errors
	ParserLL1_set_immediate_print_error(psr_ptr, 0);

	// Add grammar rules to parser
	if(grammar_filename == NULL){
		Parser_init_add_rules(psr_ptr);
	}
	else{
		Parser_init_add_rules_from_file(psr_ptr, grammar_filename);
	}

	// Calculate first and follow sets and create parsing table
	ParserLL1_initialize_rules(psr_ptr);

	// The pointer to token created by the lexer will be stored here
	Token *tkn_ptr;
	// Set flag if lexing error
	int flag_lexer_error = 0;
	// Get the status of parser
	int parser_status;
	// Set flag if error
	int flag_parser_error = 0;
	// Clear flag if EOT is reached
	int flag_run = 1;

	while(flag_run == 1){
		// Get the next token
		tkn_ptr = Lexer_get_next_token(lxr_ptr);

		if(tkn_ptr == NULL){
			// Error reading
			flag_run = 0;
			break;
		}

		if(token_ignore(tkn_ptr) == 1){
			// Token is whitespace or comment
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_ERROR){
			// Lexical error in input
			flag_lexer_error = 1;
			Token_destroy(tkn_ptr);
			continue;
		}

		if(tkn_ptr->type == TOKEN_EOT){
			// End of file reached
			flag_run = 0;
		}


		// Token can be used by parser
		parser_status = ParserLL1_step(psr_ptr, tkn_ptr);

		if (parser_status == PARSER_STEP_RESULT_SUCCESS){
			// Parsing completed successfully
			flag_run = 0;
			break;
		}

		if (parser_status == PARSER_STEP_RESULT_FAIL){
			// Error encountered. Parser enters panic mode
			flag_parser_error = 1;
		}

		if (parser_status == PARSER_STEP_RESULT_HALTED){
			// Parsing completed unsuccessfully
			flag_parser_error = 1;
			flag_run = 0;
			break;
		}
	}

	// Print any errors recorded by lexer
	// if(flag_lexer_error == 1)
	// 	Lexer_print_errors(lxr_ptr);

	// Print any errors recorded by parser
	// if(flag_parser_error == 1){
	// 	ParserLL1_print_errors(psr_ptr);
	// }


	// Skip if parsing or lexing error
	if(flag_lexer_error == 1 || flag_parser_error == 1){
		printf("   Cannot generate assembly program. Input file is syntactically incorrect\n");

		// Free resources before return
		ParserLL1_destroy(psr_ptr);
		Lexer_destroy(lxr_ptr);
		keyword_table_free();
		Dfa_destroy(dfa_ptr);
		if(filename)	fclose(file_ptr);

		return;
	}


	// Construct ast
	ParseTree *tree_ptr = prune_parse_tree(ParserLL1_get_parse_tree(psr_ptr));
	// Create Symbol table
	SymbolEnv *env_ptr = SymbolEnv_new("_main",5);

	// Set flag if semantic error
	int flag_semantic_error = 0;
	// Do not print semantic errors to stdout
	int flag_print_semantic_error = 0;
	// Set flag if root scope is uninitialized
	int flag_root_scope_initialized_error = 0;

	// Populate symbol table and perform type checking
	Semantic_symbol_and_type_check(tree_ptr, env_ptr, &flag_semantic_error, flag_print_semantic_error);
	// Check root scope for initialzation
	if( Semantic_root_scope_initialized_check(env_ptr, flag_print_semantic_error) == -1 )
		flag_root_scope_initialized_error = 1;


	// Skip if semantic error
	if(flag_semantic_error == 1 || flag_root_scope_initialized_error == 1){
		printf("   Cannot generate assembly program. Input file is semantically incorrect\n");

		// Free resources before return
		SymbolEnv_destroy(env_ptr);
		ParseTree_destroy(tree_ptr);
		ParserLL1_destroy(psr_ptr);
		Lexer_destroy(lxr_ptr);
		keyword_table_free();
		Dfa_destroy(dfa_ptr);
		if(filename)	fclose(file_ptr);

		return;
	}

	// Generate quadruples
	LinkedList *quad_lst_ptr = Icg_generate_quads(tree_ptr, env_ptr);

	// Skip if unsupported constructs
	if(quad_lst_ptr == NULL){
		printf("\n   Cannot generate assembly program. Unimplemented features in input file\n");

		// Free resources before return
		SymbolEnv_destroy(env_ptr);
		ParseTree_destroy(tree_ptr);
		ParserLL1_destroy(psr_ptr);
		Lexer_destroy(lxr_ptr);
		keyword_table_free();
		Dfa_destroy(dfa_ptr);
		if(filename)	fclose(file_ptr);

		return;
	}

	// Layout memory
	SymbolEnv_layout_memory(env_ptr, (int(*)(void *))&Type_get_size);


	FILE *asm_file_ptr = NULL;
	if(assembly_filename != NULL){
		asm_file_ptr = fopen(assembly_filename, "w");
		if(asm_file_ptr == NULL){
			printf("   Unable to open file %s. Using stdout\n", assembly_filename);
		}
	}
	if(asm_file_ptr == NULL){
		asm_file_ptr = stdout;
	}

	// Set flag if code genration error
	int flag_codegen_error = 0;
	
	// Generate assembly program
	if ( CodeGen_generate_asm(env_ptr, quad_lst_ptr, asm_file_ptr) == -1)
		flag_codegen_error = 1;

	if(flag_codegen_error == 1){
		printf("\n   Cannot generate assembly program. Unimplemented features in input file\n");
	}
	else{
		if(asm_file_ptr == stdout)
			printf("\n");
		printf("   Assembly program succesfully generated\n");
		if(asm_file_ptr != stdout){
			printf("\n   To compile and execute assembly program, execute on terminal:\n");
			printf("   nasm -felf64 %s -o nasm.o && gcc nasm.o -o nasm && ./nasm\n", assembly_filename);
		}
	}


	// Free all resources
	if(asm_file_ptr != stdout)	fclose(asm_file_ptr);
	if(quad_lst_ptr)	quad_list_destroy(quad_lst_ptr);
	SymbolEnv_destroy(env_ptr);
	ParseTree_destroy(tree_ptr);
	ParserLL1_destroy(psr_ptr);
	Lexer_destroy(lxr_ptr);
	keyword_table_free();
	Dfa_destroy(dfa_ptr);
	if(filename)	fclose(file_ptr);
}
