#include "psx/suzuki.h"
#include "psx/types.h"

/* Return 1 while music record `music` plays (SUZUKI_MUSIC_STATUS_PLAYING). main_sound_update_tunes waits
 * on it for a tune to end, and OPEN's music test for a track. */
u32 suzuki_smd_is_music_playing(suzuki_music_t* music) {
    return (music->status & SUZUKI_MUSIC_STATUS_PLAYING) != 0;
}
