#include "fft/main.h"
#include "psx/libspu.h"
#include "psx/types.h"

/* Sets the SPU reverb mode and parameters.
 *
 * Modes 0-9 are Psy-Q reverb types: the reverb is switched off while a
 * changed mode (or mode 0) is programmed with the work-area clear flag
 * 0x100; any mode from 10 up only records depth, delay and feedback. A
 * negative mode reloads the stored mode, which still carries 0x100 (LIBSPU
 * strips the flag from its own copy only): it never equals the SPU's mode,
 * so there is no early return, and it is then treated as 10 or more. */
void main_sound_set_reverb_mode(s32 mode, s16 depth, s32 delay, s32 feedback) {
    SpuReverbAttr current;

    SpuGetReverbModeParam(&current);
    if (mode < 0) {
        if (current.mode == g_main_sound_reverb_attr.mode) {
            return;
        }
        mode = g_main_sound_reverb_attr.mode;
    }
    g_main_sound_spu_state.reverb_depth = depth;
    g_main_sound_reverb_attr.delay = delay;
    g_main_sound_reverb_attr.feedback = feedback;
    if (mode < 10) {
        if (current.mode != mode || mode == 0) {
            SpuSetReverb(0);
            g_main_sound_reverb_attr.mode = mode | 0x100;
            g_main_sound_reverb_attr.mask |= 1;
            SpuSetReverbModeParam(&g_main_sound_reverb_attr);
            g_main_sound_reverb_attr.mask = 0;
            SpuSetReverb(1);
        }
        main_sound_commit_volume_change();
        SpuSetReverbDepth(&g_main_sound_reverb_attr);
        g_main_sound_reverb_attr.mask = 0;
    }
}
