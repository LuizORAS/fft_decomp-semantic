#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_set_timeline_pointers(s16 effect_id) {
    effect_file_header_t* header;
    u8* timeline;

    header = g_battle_effect_data_ptrs[effect_id];
    timeline = (u8*)header + header->timeline;
    g_battle_effect_timeline = (effect_timeline_t*)timeline;
    g_battle_effect_timing_channels = (battle_effect_tick_channels_t*)(timeline + 8);
}
