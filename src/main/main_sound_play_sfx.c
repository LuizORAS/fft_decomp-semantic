#include "fft/main.h"

/* Plays a sound on SFX channels 6-7 at the highest priority, replacing
 * whatever those channels play. */
void main_sound_play_sfx(int sound_id) {
    SuzukiPlaySound1(sound_id);
}
