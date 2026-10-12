#include "fft/battle_action.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Steps a unit's fade-out removal; each phase waits for the unit's palette
 * animation to end.
 *
 * FLASH starts the fade to black and moves to DARKEN. DARKEN then disables the
 * battle unit slot, removes the misc record and returns 1; for the source unit it
 * only resets the graphic trigger. Every other path returns 0. */
s32 battle_unit_update_fade_out_removal(battle_unit_misc_data_t* unit) {
    u8 phase;

    phase = unit->fade_out_phase;
    if (phase == BATTLE_MISC_FADE_OUT_NONE) {
        return 0;
    }
    if (g_battle_map_palette_state.clut_rows[MAP_CLUT_ROW_UNITS].animations[unit->unit_id].active != 0) {
        return 0;
    }
    switch (phase) {
    case BATTLE_MISC_FADE_OUT_FLASH:
        unit->fade_out_phase = BATTLE_MISC_FADE_OUT_DARKEN;
        battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_BASE, 4, unit->unit_id, -0x1f, -0x1f, -0x1f);
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
