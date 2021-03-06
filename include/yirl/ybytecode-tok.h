/*
 * yirl bytecode instruction tokens
 */

DEF(YB_ADD, "+", string)
DEF(YB_SUB, "-", string)
DEF(YB_DIV, "/", string)
DEF(YB_MULT, "*", string)
DEF(YB_EQUAL, "==", string)
DEF(YB_YG_GET_PUSH, "yg_get_push", separated_string)
DEF(YB_YG_GET_PUSH_INT, "yg_get_push_int", separated_string)
DEF(YB_GET_AT_IDX, "get_at_idx", separated_string)
DEF(YB_GET_AT_STR, "get_at_str", separated_string)
DEF(YB_STRING_ADD_CH_ENT, "string_add_ch_ent", separated_string)
DEF(YB_STRING_ADD_CH, "string_add_ch", separated_string)
DEF(YB_CALL, "call_entity", separated_string)
DEF(YB_INCR, "incr", separated_string)
DEF(YB_LEAVE, "leave", separated_string)
DEF(YB_STACK_POP, "stack_pop", separated_string)
DEF(YB_CREATE_INT, "int", separated_string)
DEF(YB_CREATE_ARRAY, "array", separated_string)
DEF(YB_CREATE_FUNC, "func", separated_string)
DEF(YB_PUSH_BACK, "push_back", separated_string)
DEF(YB_CREATE_STRING, "string", separated_string)
DEF(YB_SET_INT, "set_int", separated_string)
DEF(YB_PRINT_ENTITY, "print_entity", separated_string)
DEF(YB_PRINT_POS, "print_pos", separated_string)
DEF(YB_PRINT_IRET, "print_iret", separated_string)
DEF(YB_EQUAL_NBR, "equal_nbr", separated_string)
DEF(YB_NOT_EQUAL_NBR, "not_equal_nbr", separated_string)
DEF(YB_JMP, "jmp", separated_string)
DEF(YB_REGISTRE_WIDGET_SUBTYPE, "registre_widget_subtype", separated_string)
DEF(YB_NEW_WIDGET, "new_widget", separated_string)
DEF(YB_RETURN, "return", separated_string)
DEF(YB_JMP_IF_0, "is_iret_0", separated_string)
DEF(YB_INF_COMP_NBR, "is_nbr_inf", separated_string)
DEF(YB_SUP_COMP_NBR, "is_nbr_sup", separated_string)
DEF(YB_END_FUNC, "}", string)
