#include "fft/main.h"
#include "psx/types.h"

/* Pauses a music handle: clears the playing bit, sets the pause bit that
 * main_smd_resume_music clears again, and queues the voices of its active
 * channels for key-off. */
void main_smd_pause_music(suzuki_music_t* music) {
    if (music != 0) {
        music->status = (music->status & ~SUZUKI_MUSIC_STATUS_PLAYING) | SUZUKI_MUSIC_STATUS_PAUSED;
        g_main_sound_music_key_off_voices |= SuzukiGetActiveChannels(music);
    }
}
