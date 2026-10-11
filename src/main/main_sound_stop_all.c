#include "fft/main.h"
#include "psx/types.h"

/* Stops every SMD music record and silences every SFX channel. */
void main_sound_stop_all(void) {
    main_smd_stop_marked_music();
    SuzukiTurnOffAllMusic();
}
