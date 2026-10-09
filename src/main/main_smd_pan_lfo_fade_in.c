#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xEB: modulator 2 fade_level and its
 * reset value become 0x100 / (byte + 1). Twin of main_smd_pitch_shift_fade_in. */
u8* main_smd_pan_lfo_fade_in(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    s16 divisor;

    divisor = (note_data[0] + 1) & 0xff;
    note_data++;
    if (divisor != 0) {
        divisor = 0x100 / divisor;
        channel->modulators[2].fade_step = divisor;
        channel->modulators[2].fade_level = divisor;
    }
    return note_data;
}
