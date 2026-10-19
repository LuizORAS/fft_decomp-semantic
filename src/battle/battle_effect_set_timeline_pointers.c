#include "fft/battle_effect.h"
#include "psx/types.h"

/* Point g_battle_effect_timeline at the timeline section of effect file effect_id (the parent timeline
 * and the camera tracks) and g_battle_effect_child_timeline 8 bytes into it (the child timeline's duration
 * and channels). */
void battle_effect_set_timeline_pointers(s16 effect_id) {
    effect_file_header_t* header;
    effect_timeline_t* timeline;

    header = g_battle_effect_file_headers[effect_id];
    timeline = (effect_timeline_t*)((u8*)header + header->timeline);
    g_battle_effect_timeline = timeline;
    g_battle_effect_child_timeline = &timeline->child;
}
