#include "psx/suzuki.h"
#include "psx/types.h"

/* SMD opcode 0xAE "Nop": consumes no parameter. */
u8* main_smd_opcode_ae(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    return note_data;
}
