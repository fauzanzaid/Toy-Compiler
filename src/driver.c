#include <stdio.h>


#include "driver_init.h"


// ASCII text codes
#define TEXT_RED	"\x1B[31m"
#define TEXT_GRN	"\x1B[32m"
#define TEXT_YLW	"\x1B[33m"
#define TEXT_BLD	"\x1B[1m"
#define TEXT_RST	"\x1B[0m"


int main(int argc, char const *argv[])
{
	printf("\n");
	printf("   This is a toy compiler.\n");
	printf("   Refer to examples to check language syntax and features.\n");
	printf("   Some features have not been implemented beyond syntax analysis.\n");
	printf("\n");


	const char *input_filename = NULL;
	const char *tree_filename = NULL;
	const char *grammar_filename = NULL;
	const char *assembly_filename = NULL;

	if(argc < 2)
		printf("   stdin will be used as input. Press Ctrl + D to signal end of input\n   To use input file, specify name as first command line argument.\n\n");
	else
		input_filename = argv[1];

	if(argc < 3)
		printf("   stdout will be used as output for assembly\n   Specify a filename as second command line argument to override.\n\n");
	else
		assembly_filename = argv[2];


	int wid = 80;

	while(1){

		printf(TEXT_YLW);

		printf("\n");

		printf("%s", " --");
		printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
		printf("%s", "--\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, "Press a key to take corresponding action:");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (0) Exit");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (1) Print token list");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (2) Perform parsing and display parse tree, in preorder traversal");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (3) Print abstract syntax tree, in preorder traversal");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (4) Print parse tree and abstract syntax tree memory information");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (5) Print symbol table");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (6) Check syntactic and semantic correctness");
		printf("%s", " |\n");

		printf("%s", " | ");
		printf("%-*.*s", wid, wid, " (7) Output assembly program");
		printf("%s", " |\n");

		printf("%s", " --");
		printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
		printf("%s", "--\n");

		printf("\n");
		printf("   Your choice > ");
		printf(TEXT_RST);
		
		fflush(stdout);
		
		char input;
		scanf(" %c", &input);
		printf("\n");


		if(input == '0'){
			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "Exit");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			break;
		}

		else if(input == '1'){
			printf(TEXT_YLW);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "List of Tokens");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			printf("\n");
			run_lexer(input_filename);
			printf("\n");

			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);
		}

		else if(input == '2'){
			printf(TEXT_YLW);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "Parse Tree");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			printf("\n");
			run_parse_tree_printer(input_filename, NULL, NULL);
			printf("\n");

			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);
		}

		else if(input == '3'){
			printf(TEXT_YLW);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "Abstract Syntax Tree");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			printf("\n");
			run_ast_printer(input_filename, NULL, NULL);
			printf("\n");

			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);
		}

		else if(input == '4'){
			printf(TEXT_YLW);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "Memory information");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			printf("\n");
			run_memory_checker(input_filename, NULL);
			printf("\n");

			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);
		}

		else if(input == '5'){
			printf(TEXT_YLW);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "Symbol Table");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			printf("\n");
			run_symbol_table_printer(input_filename, NULL);
			printf("\n");

			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);
		}

		else if(input == '6'){
			printf(TEXT_YLW);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "Syntactic and Semantic check");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			printf("\n");
			run_front_end_checker(input_filename, NULL);
			printf("\n");

			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);
		}

		else if(input == '7'){
			printf(TEXT_YLW);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf("%s", " | ");
			printf("%-*.*s", wid, wid, "Generate assembly");
			printf("%s", " |\n");

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);

			printf("\n");
			run_code_generator(input_filename, NULL, assembly_filename);
			printf("\n");

			printf(TEXT_RED);

			printf("%s", " --");
			printf("%-*.*s", wid, wid, "----------------------------------------------------------------------------------------------------");
			printf("%s", "--\n");

			printf(TEXT_RST);
		}


		else{
			printf("   Try again!\n");
		}
	}

	return 0;
}
