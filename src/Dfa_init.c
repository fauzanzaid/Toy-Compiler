#include "Dfa_init.h"
#include "Dfa.h"


///////////////
// Constants //
///////////////


const int states[] = {
	STATE_N_START,
	STATE_F_COMMA,
	STATE_F_SEMICOLON,
	STATE_F_OP,
	STATE_F_CL,
	STATE_F_SQO,
	STATE_F_SQC,
	STATE_F_PLUS,
	STATE_F_MINUS,
	STATE_F_MUL,
	STATE_F_DIV,
	STATE_F_SIZE,
	STATE_F_WHITESPACE,
	STATE_N_A_COMMENT,
	STATE_F_COMMENT,
	STATE_F_ID_OR_KW,
	STATE_F_ID,
	STATE_N_A_FUNID_OR_KW,
	STATE_F_FUNID_OR_KW,
	STATE_F_NUM,
	STATE_N_A_RNUM,
	STATE_N_B_RNUM,
	STATE_F_RNUM,
	STATE_N_A_STR,
	STATE_N_B_STR,
	STATE_F_STR,
	STATE_N_LOGICAL,
	STATE_N_A_AND,
	STATE_N_B_AND,
	STATE_N_C_AND,
	STATE_F_AND,
	STATE_N_A_OR,
	STATE_N_B_OR,
	STATE_F_OR,
	STATE_N_A_NOT,
	STATE_N_B_NOT,
	STATE_N_C_NOT,
	STATE_F_NOT,
	STATE_F_LT,
	STATE_F_LE,
	STATE_F_GT,
	STATE_F_GE,
	STATE_F_ASSIGNOP,
	STATE_F_EQ,
	STATE_N_A_NE,
	STATE_F_NE,
};

const int len_states = 46;

const char symbols[] = {
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'.', '_', '#', '\"',
	',', ';', '(', ')', '[', ']', '+', '-', '*', '/', '@',
	' ', '\n', '\t',
	0x04
};

const int len_symbols = 82;

const int start_state = STATE_N_START;

const int final_states[] = {
	STATE_F_COMMA,
	STATE_F_SEMICOLON,
	STATE_F_OP,
	STATE_F_CL,
	STATE_F_SQO,
	STATE_F_SQC,
	STATE_F_PLUS,
	STATE_F_MINUS,
	STATE_F_MUL,
	STATE_F_DIV,
	STATE_F_SIZE,
	STATE_F_WHITESPACE,
	STATE_F_COMMENT,
	STATE_F_ID_OR_KW,
	STATE_F_ID,
	STATE_F_FUNID_OR_KW,
	STATE_F_NUM,
	STATE_F_RNUM,
	STATE_F_STR,
	STATE_F_AND,
	STATE_F_OR,
	STATE_F_NOT,
	STATE_F_LT,
	STATE_F_LE,
	STATE_F_GT,
	STATE_F_GE,
	STATE_F_ASSIGNOP,
	STATE_F_EQ,
	STATE_F_NE,
};

const int len_start_states = 29;



///////////////
// Functions //
///////////////

void Dfa_init_add_transitions(Dfa *dfa_ptr){

	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_COMMA, ',');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_SEMICOLON, ';');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_OP, '(');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_CL, ')');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_SQO, '[');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_SQC, ']');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_PLUS, '+');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_MINUS, '-');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_MUL, '*');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_DIV, '/');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_SIZE, '@');
	// Dfa_add_transition_many(dfa_ptr, STATE_N_START, STATE_F_WHITESPACE, {' ', '\n', '\t'}, 3);
	Dfa_add_transition_many(dfa_ptr, STATE_N_START, STATE_F_WHITESPACE, " \n\t", 3);


	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_N_A_COMMENT, '#');
	Dfa_add_transition_single_invert(dfa_ptr, STATE_N_A_COMMENT, STATE_F_COMMENT, '\n');


	Dfa_add_transition_range(dfa_ptr, STATE_N_START, STATE_F_ID_OR_KW, 'a', 'z');
	Dfa_add_transition_range(dfa_ptr, STATE_N_START, STATE_F_ID_OR_KW, 'A', 'Z');
	Dfa_add_transition_range(dfa_ptr, STATE_F_ID_OR_KW, STATE_F_ID_OR_KW, 'a', 'z');
	Dfa_add_transition_range(dfa_ptr, STATE_F_ID_OR_KW, STATE_F_ID_OR_KW, 'A', 'Z');
	Dfa_add_transition_range(dfa_ptr, STATE_F_ID_OR_KW, STATE_F_ID, '0', '9');


	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_N_A_FUNID_OR_KW, '_');
	Dfa_add_transition_range(dfa_ptr, STATE_N_A_FUNID_OR_KW, STATE_F_FUNID_OR_KW, 'a', 'z');
	Dfa_add_transition_range(dfa_ptr, STATE_N_A_FUNID_OR_KW, STATE_F_FUNID_OR_KW, 'A', 'Z');
	Dfa_add_transition_range(dfa_ptr, STATE_F_FUNID_OR_KW, STATE_F_FUNID_OR_KW, 'a', 'z');
	Dfa_add_transition_range(dfa_ptr, STATE_F_FUNID_OR_KW, STATE_F_FUNID_OR_KW, 'A', 'Z');
	Dfa_add_transition_range(dfa_ptr, STATE_F_FUNID_OR_KW, STATE_F_FUNID_OR_KW, '0', '9');


	Dfa_add_transition_range(dfa_ptr, STATE_N_START, STATE_F_NUM, '0', '9');
	Dfa_add_transition_range(dfa_ptr, STATE_F_NUM, STATE_F_NUM, '0', '9');
	Dfa_add_transition_single(dfa_ptr, STATE_F_NUM, STATE_N_A_RNUM, '.');
	Dfa_add_transition_range(dfa_ptr, STATE_N_A_RNUM, STATE_N_B_RNUM, '0', '9');
	Dfa_add_transition_range(dfa_ptr, STATE_N_B_RNUM, STATE_F_RNUM, '0', '9');


	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_N_A_STR, '\"');
	Dfa_add_transition_range(dfa_ptr, STATE_N_A_STR, STATE_N_B_STR, 'a', 'z');
	Dfa_add_transition_range(dfa_ptr, STATE_N_B_STR, STATE_N_B_STR, 'a', 'z');
	Dfa_add_transition_single(dfa_ptr, STATE_N_B_STR, STATE_F_STR, '\"');


	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_N_LOGICAL, '.');

	Dfa_add_transition_single(dfa_ptr, STATE_N_LOGICAL, STATE_N_A_AND, 'a');
	Dfa_add_transition_single(dfa_ptr, STATE_N_A_AND, STATE_N_B_AND, 'n');
	Dfa_add_transition_single(dfa_ptr, STATE_N_B_AND, STATE_N_C_AND, 'd');
	Dfa_add_transition_single(dfa_ptr, STATE_N_C_AND, STATE_F_AND, '.');

	Dfa_add_transition_single(dfa_ptr, STATE_N_LOGICAL, STATE_N_A_OR, 'o');
	Dfa_add_transition_single(dfa_ptr, STATE_N_A_OR, STATE_N_B_OR, 'r');
	Dfa_add_transition_single(dfa_ptr, STATE_N_B_OR, STATE_F_OR, '.');

	Dfa_add_transition_single(dfa_ptr, STATE_N_LOGICAL, STATE_N_A_NOT, 'n');
	Dfa_add_transition_single(dfa_ptr, STATE_N_A_NOT, STATE_N_B_NOT, 'o');
	Dfa_add_transition_single(dfa_ptr, STATE_N_B_NOT, STATE_N_C_NOT, 't');
	Dfa_add_transition_single(dfa_ptr, STATE_N_C_NOT, STATE_F_NOT, '.');


	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_LT, '<');
	Dfa_add_transition_single(dfa_ptr, STATE_F_LT, STATE_F_LE, '=');
	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_GT, '>');
	Dfa_add_transition_single(dfa_ptr, STATE_F_GT, STATE_F_GE, '=');

	Dfa_add_transition_single(dfa_ptr, STATE_N_START, STATE_F_ASSIGNOP, '=');
	Dfa_add_transition_single(dfa_ptr, STATE_F_ASSIGNOP, STATE_F_EQ, '=');
	Dfa_add_transition_single(dfa_ptr, STATE_F_ASSIGNOP, STATE_N_A_NE, '/');
	Dfa_add_transition_single(dfa_ptr, STATE_N_A_NE, STATE_F_NE, '=');
}
