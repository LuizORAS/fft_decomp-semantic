#include "fft/world.h"

/* Return the command after `instruction` and the commands it counts: an if group (MENU_SCRIPT_OP_IF_GROUP)
 * counts byte 4 children, an if-else group (MENU_SCRIPT_OP_IF_ELSE_GROUP) bytes 4 and 5. The scrollable list
 * uses it to skip a row's commands; the overlays' *_menu_script_skip_group are its twins. */
menu_instruction_t* world_menu_script_skip_group(menu_instruction_t* instruction) {
    s32 count;

    if (instruction->header.opcode == MENU_SCRIPT_OP_IF_GROUP) {
        count = instruction->conditional.count;
        count += 1;
    } else if (instruction->header.opcode == MENU_SCRIPT_OP_IF_ELSE_GROUP) {
        count = instruction->if_else.count_true;
        count += instruction->if_else.count_false;
        count += 1;
    } else {
        count = 1;
    }
    count -= 1;
    while (count != -1) {
        instruction = menu_next_instruction(instruction);
        count -= 1;
    }
    return instruction;
}
