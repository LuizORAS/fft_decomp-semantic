#include "fft/battle.h"
#include "psx/types.h"

/* Bows and crossbows lose a quarter of the base hit at night and another quarter in a storm, which
 * raises the target's evades by a third each. It reads the weather variable itself, not
 * battle_map_get_effective_weather, so a snowstorm counts too and the map's ignore-weather flag is
 * not checked (QUIRKS.md). */
void battle_formula_apply_weather_effects_on_bows(void) {
    u8* base_hit;
    s32 weather;
    s32 time_of_day;

    if ((u32)(g_main_item_primary_data[g_current_ability.weapon_id].type - ITEM_TYPE_CROSSBOW) < 2) {
        weather = battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER);
        time_of_day = battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY);
        if (time_of_day == 1) { /* night */
            g_current_ability.base_hit = g_current_ability.base_hit * 3 / 4;
        }
        /* storm or strong storm; snow is not folded in here */
        if ((u32)(weather - BATTLE_WEATHER_STORM) < 2) {
            base_hit = &g_current_ability.base_hit;
            *base_hit = *base_hit * 3 / 4;
        }
    }
}
