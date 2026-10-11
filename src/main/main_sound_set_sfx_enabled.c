#include "fft/main.h"
#include "psx/types.h"

/* Sets or clears MAIN_SOUND_DRIVER_SFX_ENABLED; disabling also silences
 * every SFX channel first. The Play Sound entry points test the flag. */
void main_sound_set_sfx_enabled(s32 enabled) {
    if (enabled != 0) {
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_SFX_ENABLED;
    } else {
        SuzukiTurnOffAllMusic();
        g_main_sound_driver_flags &= ~MAIN_SOUND_DRIVER_SFX_ENABLED;
    }
}
