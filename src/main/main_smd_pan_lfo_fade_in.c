#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xEB "Pan LFO Fade In": modulator 2 fade_level and its reset
 * value become 0x100 / (byte + 1). Twin of main_smd_pitch_shift_fade_in;
 * byte 0xff leaves both values unchanged. */
u8* main_smd_pan_lfo_fade_in(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    u8 duration;
    s16 step;

    duration = note_data[0] + 1;
    note_data++;
    if (duration != 0) {
        step = 0x100 / duration;
        channel->modulators[2].fade_step = step;
        channel->modulators[2].fade_level = step;
    }
    return note_data;
}
