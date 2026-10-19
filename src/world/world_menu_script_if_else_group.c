#include "fft/world.h"
#include "psx/types.h"

/* Menu script if/else: opcode byte 3 selects a predicate, byte 4 the number
 * of commands in the "then" branch, byte 5 the number in the "else" branch,
 * byte 6 the constant argument. The taken branch is executed via the opcode
 * table; the other is skipped by its length bytes. */
menu_instruction_t* world_menu_script_if_else_group(menu_instruction_t* instruction) {
    s32 (*predicate)(s32) = g_world_menu_script_callbacks[instruction->if_else.predicate].value;
    s32 count_true = instruction->if_else.count_true;
    s32 count_false = instruction->if_else.count_false;
    s32 arg;

    if (g_world_menu_use_scroll_position == 0) {
        arg = instruction->if_else.value;
    } else {
        arg = g_world_menu_scroll_offset + g_world_menu_scroll_row_offset;
        if (g_world_menu_scroll_pixel_offset < 0) {
            arg -= 1;
        }
    }
    instruction = menu_next_instruction(instruction);
    if (predicate(arg) != 0) {
        count_true -= 1;
        while (count_true != -1) {
            instruction = g_world_menu_script_handlers[instruction->header.opcode](instruction);
            count_true -= 1;
        }
        count_false -= 1;
        while (count_false != -1) {
            instruction = menu_next_instruction(instruction);
            count_false -= 1;
        }
    } else {
        count_true -= 1;
        while (count_true != -1) {
            instruction = menu_next_instruction(instruction);
            count_true -= 1;
        }
        count_false -= 1;
        while (count_false != -1) {
            instruction = g_world_menu_script_handlers[instruction->header.opcode](instruction);
            count_false -= 1;
        }
    }
    return instruction;
}
