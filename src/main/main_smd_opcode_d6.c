#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xD6: stores the byte in portamento_ticks and
 * mirrors "non-zero" into bit 2 of ramp_flags. */
u8* main_smd_opcode_d6(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    u16 value;

    value = *note_data++;
    channel->portamento_ticks = value;
    if (value != 0)
        channel->ramp_flags |= SUZUKI_RAMP_PORTAMENTO;
    else
        channel->ramp_flags &= ~SUZUKI_RAMP_PORTAMENTO;
    return note_data;
}
