#include "fft/world.h"

/* Walks past the records that follow the header, returning the next one. */
world_menu_instruction_t* world_menu_skip_instruction_group(world_menu_instruction_t* instruction) {
    s32 count;

    if (instruction->header.opcode == 1) {
        count = instruction->conditional.count;
        count += 1;
    } else if (instruction->header.opcode == 2) {
        count = instruction->if_else.count_true;
        count += instruction->if_else.count_false;
        count += 1;
    } else {
        count = 1;
    }
    count -= 1;
    while (count != -1) {
        instruction = world_menu_next_instruction(instruction);
        count -= 1;
    }
    return instruction;
}
