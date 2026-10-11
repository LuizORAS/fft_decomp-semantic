#include "fft/world.h"
#include "psx/types.h"

/* Menu-script opcode handler: byte 3 of the instruction selects the
   semi-transparency flag used by the window/text primitives. Returns the
   address of the next instruction (byte 1 is the instruction length). */
world_menu_instruction_t* world_menu_script_set_semi_trans(world_menu_instruction_t* instruction) {
    g_world_menu_semi_trans = instruction->draw_state.value;
    return world_menu_next_instruction(instruction);
}
