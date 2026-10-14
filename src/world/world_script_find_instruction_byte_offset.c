#include "fft/script.h"
#include "fft/world.h"

/* Returns the byte offset of the next `instruction` opcode at or after
 * `offset`, or 0 when EventEnd comes first. */
s32 world_script_find_instruction_byte_offset(s32 offset, s32 instruction) {
    const u8* event = (u8*)g_script_event_block;

    do {
        u8 opcode = event[offset];

        if (opcode == EVENT_OPCODE_END) {
            return 0;
        }
        if (opcode == instruction) {
            return offset;
        }
        offset += g_world_event_instruction_sizes[opcode] + 1;
    } while (1);
}
