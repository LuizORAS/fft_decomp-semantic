#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_set_timeline_pointers(s16 effect_id) {
    effect_file_header_t* header;
    effect_timeline_t* timeline;

    header = g_battle_effect_data_ptrs[effect_id];
    timeline = (effect_timeline_t*)((u8*)header + header->timeline);
    g_battle_effect_timeline = timeline;
    g_battle_effect_timing_channels = &timeline->child;
}
