#ifndef INCLUDE_GUARD_9ED8D2045C1D46BC814C9CB19F2B9E8E
#define INCLUDE_GUARD_9ED8D2045C1D46BC814C9CB19F2B9E8E

void run_lexer(const char *filename);

void run_parser(const char *input_filename, const char *grammar_filename);

void run_parse_tree_printer(const char *filename, const char *grammar_filename, const char *tree_filename);

void run_ast_printer(const char *filename, const char *grammar_filename, const char *tree_filename);

void run_memory_checker(const char *filename, const char *grammar_filename);

void run_symbol_table_printer(const char *filename, const char *grammar_filename);

void run_front_end_checker(const char *filename, const char *grammar_filename);

void run_code_generator(const char *filename, const char *grammar_filename, const char *assembly_filename);

#endif
