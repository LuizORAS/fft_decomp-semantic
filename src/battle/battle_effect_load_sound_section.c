#include "fft/battle.h"
#include "psx/types.h"

/* Replace the effect sounds: unregister the sound resource of the previous effect
 * file, then register this file's sound section and take its id as the base for
 * the effect's sound ids. A file without a sound section clears both globals. */
void battle_effect_load_sound_section(s16 index) {
    effect_file_header_t* base;
    s32 offset;

    if (g_battle_effect_sound_section_ptr != 0) {
        main_sound_remove_vfx_resource(g_battle_effect_sound_section_ptr);
    }

    base = g_battle_effect_file_headers[index];
    offset = base->sound_section;
    if (offset != 0) {
        g_battle_effect_sound_section_ptr = (main_sound_resource_t*)((u8*)base + offset);
        SuzukiAppendVFXSMD((main_sound_resource_t*)((u8*)base + offset));
        g_battle_effect_sound_data_base = MAIN_SOUND_ID(g_battle_effect_sound_section_ptr->id, 0);
    } else {
        g_battle_effect_sound_section_ptr = 0;
        g_battle_effect_sound_data_base = 0;
    }
}
