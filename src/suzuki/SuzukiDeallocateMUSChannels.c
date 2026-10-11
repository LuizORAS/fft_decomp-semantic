#include "psx/suzuki.h"
#include "psx/types.h"

/* Stops a music record: clears its playing bit and queues the voices of its
 * channels for key-off. The record stays queued and loaded. */
void SuzukiDeallocateMUSChannels(suzuki_music_t* music) {
    if (music != 0) {
        music->status &= ~SUZUKI_MUSIC_STATUS_PLAYING;
        g_main_sound_music_key_off_voices |= SuzukiGetActiveChannels(music);
    }
}
