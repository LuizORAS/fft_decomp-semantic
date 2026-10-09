#include "fft/battle.h"
#include "psx/types.h"

s32 battle_unit_update_fade_out_removal(battle_unit_misc_data_t* unit) {
    u8 phase;

    phase = unit->fade_out_phase;
    if (phase == BATTLE_MISC_FADE_OUT_NONE) {
        return 0;
    }
    if (g_battle_map_palette_state.banks[3].animations[unit->unit_id].active != 0) {
        return 0;
    }
    switch (phase) {
    case BATTLE_MISC_FADE_OUT_FLASH:
        unit->fade_out_phase = BATTLE_MISC_FADE_OUT_DARKEN;
        battle_gfx_start_misc_unit_palette_modulation(4, 4, unit->unit_id, -0x1f, -0x1f, -0x1f);
        return 0;
    case BATTLE_MISC_FADE_OUT_DARKEN:
        if (unit->unit_id != g_battle_unit_source_misc_id) {
            unit->fade_out_phase = BATTLE_MISC_FADE_OUT_REMOVED;
            if (unit->battle_data != 0) {
                battle_unit_disable_remove(unit->battle_data->misc_unit_id);
            }
            battle_unit_remove_misc(unit);
            return 1;
        }
        battle_gfx_reset_unit_graphic_trigger(unit->unit_id);
        return 0;
    }
    return 0;
}
