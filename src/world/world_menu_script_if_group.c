#include "fft/world.h"
#include "psx/types.h"

/* Menu script conditional: opcode byte 3 selects a predicate, byte 4 the
 * number of following commands, byte 5 the constant argument. When the
 * predicate holds the following commands are executed via the opcode table;
 * otherwise they are skipped by their length bytes. */
menu_instruction_t* world_menu_script_if_group(menu_instruction_t* script) {
    menu_instruction_t* instruction = script;
    s32 (*predicate)(s32) = g_world_menu_script_callbacks[instruction->conditional.predicate].value;
    s32 count = instruction->conditional.count;
    s32 arg;

    if (g_world_menu_use_scroll_position == 0) {
        arg = instruction->conditional.value;
    } else {
        arg = g_world_menu_scroll_offset + g_world_menu_scroll_row_offset;
        if (g_world_menu_scroll_pixel_offset < 0) {
            arg -= 1;
        }
    }
    instruction = menu_next_instruction(instruction);
    if (predicate(arg) == 0) {
        count -= 1;
        while (count != -1) {
            instruction = menu_next_instruction(instruction);
            count -= 1;
        }
    } else {
        count -= 1;
        while (count != -1) {
            instruction = g_world_menu_script_handlers[instruction->header.opcode](instruction);
            count -= 1;
        }
    }
    return instruction;
}
