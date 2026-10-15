#include "fft/main.h"
#include "psx/types.h"

/* Return 1 while music record `music` plays (SUZUKI_MUSIC_STATUS_PLAYING). main_sound_update_tunes waits
 * on it for a tune to end, and OPEN's music test for a track. */
u32 SuzukiGetMusicPlaying(suzuki_music_t* music) {
    return (music->status & SUZUKI_MUSIC_STATUS_PLAYING) != 0;
}
