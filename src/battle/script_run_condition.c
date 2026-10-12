#include "fft/battle_event.h"

/* Comparison opcodes 0xa0..0xa5: replace the comparison-result variable (32-bit script
 * variable 0) with the truth value of comparing it against the operand variable (variable
 * 1), using <=, >=, ==, !=, < and > in opcode order. Any other opcode changes
 * nothing. */
void script_run_condition(event_opcode_e opcode) {
    if (opcode == EVENT_CONDITION_SECOND_GTE_FIRST) {
        g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT] = g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT]
            <= g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_OPERAND];
        return;
    }
    if (opcode == EVENT_CONDITION_FIRST_GTE_SECOND) {
        g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT] = g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT]
            >= g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_OPERAND];
        return;
    }
    if (opcode == EVENT_CONDITION_EQUAL) {
        g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT] = g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT]
            == g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_OPERAND];
        return;
    }
    if (opcode == EVENT_CONDITION_NOT_EQUAL) {
        g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT] = g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT]
            != g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_OPERAND];
        return;
    }
    if (opcode == EVENT_CONDITION_FIRST_LT_SECOND) {
        g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT] = g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT]
            < g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_OPERAND];
        return;
    }
    if (opcode == EVENT_CONDITION_SECOND_LT_FIRST) {
        g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT] = g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT]
            > g_script_variables[EVENT_SCRIPT_VAR_COMPARISON_OPERAND];
        return;
    }
}
