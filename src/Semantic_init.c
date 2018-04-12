#include "ParseTree.h"
#include "SymbolEnv.h"


#include "Ast_init.h"
#include "Type_init.h"
#include "Semantic_init.h"


///////////////
// Functions //
///////////////


int Semantic_symbol_and_type_check(ParseTree_Node *node_ptr, SymbolEnv *env_ptr){

	switch(node_ptr->atr_ptr->op)
	{
		case OPERATOR_PLUS:
		{
			ParseTree_Node *child_0 = ParseTree_Node_get_child_by_node_index(node_ptr, 0);
			ParseTree_Node *child_1 = ParseTree_Node_get_child_by_node_index(node_ptr, 1);

			break;
		}

		case OPERATOR_MINUS:
		{

			break;
		}

		case OPERATOR_MUL:
		{

			break;
		}

		case OPERATOR_DIV:
		{

			break;
		}

		case OPERATOR_SIZE:
		{

			break;
		}

		case OPERATOR_ID:
		{

			break;
		}

		case OPERATOR_FUNID:
		{

			break;
		}

		case OPERATOR_KW_INT:
		{

			break;
		}

		case OPERATOR_KW_REAL:
		{

			break;
		}

		case OPERATOR_KW_STRING:
		{

			break;
		}

		case OPERATOR_KW_MATRIX:
		{

			break;
		}

		case OPERATOR_KW_READ:
		{

			break;
		}

		case OPERATOR_KW_PRINT:
		{

			break;
		}

		case OPERATOR_NUM:
		{

			break;
		}

		case OPERATOR_RNUM:
		{

			break;
		}

		case OPERATOR_STR:
		{

			break;
		}

		case OPERATOR_AND:
		{

			break;
		}

		case OPERATOR_OR:
		{

			break;
		}

		case OPERATOR_NOT:
		{

			break;
		}

		case OPERATOR_LT:
		{

			break;
		}

		case OPERATOR_LE:
		{

			break;
		}

		case OPERATOR_GT:
		{

			break;
		}

		case OPERATOR_GE:
		{

			break;
		}

		case OPERATOR_ASSIGN:
		{

			break;
		}

		case OPERATOR_EQ:
		{

			break;
		}

		case OPERATOR_NE:
		{

			break;
		}

		case OPERATOR_CALL:
		{

			break;
		}

		case OPERATOR_COND:
		{

			break;
		}

		case OPERATOR_DECL:
		{

			break;
		}

		case OPERATOR_DEF:
		{

			break;
		}

		case OPERATOR_ID_LIST:
		{

			break;
		}

		case OPERATOR_MATRIX:
		{

			break;
		}

		case OPERATOR_MATRIX_ELEMENT:
		{

			break;
		}

		case OPERATOR_PARAM_LIST:
		{

			break;
		}

		case OPERATOR_STMT_LIST:
		{

			break;
		}

		case OPERATOR_STMT_OR_DEF_LIST:
		{
			ParseTree_Node *child = ParseTree_Node_get_child_by_node_index(node_ptr, 0);

			break;
		}

		default:
		{

			break;
		}
	}


}
