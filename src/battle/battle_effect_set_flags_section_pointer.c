#include "fft/battle.h"
#include "psx/types.h"

/* Point g_battle_effect_flags_section at the flags section of effect file effect_id: the effect's flags
 * and its sound channels. battle_effect_update_file_load binds it before the effect plays to test
 * EFFECT_FLAG_QUIET_MUSIC. */
void battle_effect_set_flags_section_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_file_headers[effect_id];
    g_battle_effect_flags_section = (effect_flags_section_t*)((u8*)header + header->flags_section);
}
