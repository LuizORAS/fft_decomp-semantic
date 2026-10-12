#include "fft/battle_action.h"
#include "fft/battle_event.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Bows and crossbows lose a quarter of the base hit at night and another
 * quarter when the weather variable is 3 or 4 (a storm, or a snowstorm on a
 * snow map). The variable is read as it is, not through
 * battle_map_get_effective_weather. */
void battle_formula_apply_weather_effects_on_bows(void) {
    u8* base_hit;
    s32 weather;
    s32 time_of_day;

    if (g_main_item_primary_data[g_action_current_ability.weapon_id].type == ITEM_TYPE_CROSSBOW
        || g_main_item_primary_data[g_action_current_ability.weapon_id].type == ITEM_TYPE_BOW) {
        weather = battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER);
        time_of_day = battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY);
        if (time_of_day == 1) { /* night */
            g_action_current_ability.base_hit = g_action_current_ability.base_hit * 3 / 4;
        }
        /* The raw weather variable, not battle_map_get_effective_weather: a
         * storm strength counts on a snow map and on a map that ignores weather. */
        if (weather == BATTLE_WEATHER_STORM || weather == BATTLE_WEATHER_STRONG_STORM) {
            base_hit = &g_action_current_ability.base_hit;
            *base_hit = *base_hit * 3 / 4;
        }
    }
}
