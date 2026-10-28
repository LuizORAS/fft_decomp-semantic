#include "fft/wldcore.h"

/* Same as main_sound_play_sfx: WLDCORE's menus, lists and the bar play their sounds through it. */
void wldcore_sound_play_effect(s32 sound_id) {
    main_sound_play_sfx(sound_id);
}
