#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0x8F "Nop": a separate copy of main_smd_no_instruction, with
 * length 1 in g_main_smd_opcode_lengths. */
u8* main_smd_no_instruction_4(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    return note_data;
}
