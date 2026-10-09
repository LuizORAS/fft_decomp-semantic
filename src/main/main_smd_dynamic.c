#include "fft/main.h"
#include "psx/types.h"

/* SMD opcode 0xE0 "Dynamic": absolute channel volume. An absolute
 * volume ends any crescendo/decrescendo ramp (ramp_flags bit 3). */
u8* main_smd_dynamic(u8* note_data, suzuki_music_t* music, suzuki_music_channel_t* channel) {
    channel->volume = note_data[0] << 24;
    channel->voice_update_flags |= 0x100;
    channel->ramp_flags &= ~8;
    return note_data + 1;
}
