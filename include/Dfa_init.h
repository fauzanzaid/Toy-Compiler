#ifndef INCLUDE_GUARD_592C10D803FA4F978FE6E10DD63C455D
#define INCLUDE_GUARD_592C10D803FA4F978FE6E10DD63C455D

#include "Dfa.h"


///////////////
// Constants //
///////////////

typedef enum {
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
}State_type;

extern const int states[];
extern const int len_states;
extern const char symbols[];
extern const int len_symbols;
extern const int start_state;
extern const int final_states[];
extern const int len_start_states;


///////////////
// Functions //
///////////////

void Dfa_init_add_transitions(Dfa *dfa_prt);

#endif
