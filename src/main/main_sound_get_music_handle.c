#include "fft/main.h"

/* Return the music record of music slot `slot`. No retail code calls it. */
suzuki_music_t* main_sound_get_music_handle(int slot) {
    return g_main_sound_music.slots.handles.music[slot];
}
