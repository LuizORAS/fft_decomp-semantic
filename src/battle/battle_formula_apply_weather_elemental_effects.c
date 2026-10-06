#include "fft/battle.h"
#include "psx/types.h"

/* The weather on the ability's element, as HP damage: in a storm (weather 3 or 4) fire * 3 / 4 and
 * lightning * 5 / 4; in a snowstorm (6 or 7) ice * 5 / 4. Rain (2), snow (5) and maps that ignore
 * the weather (battle_map_get_effective_weather) change nothing. */
void battle_formula_apply_weather_elemental_effects(void) {
    u8 element;
    s32 weather;

    element = g_current_ability.range_data.element;
    weather = battle_map_get_effective_weather();
    if (weather == BATTLE_WEATHER_STORM || weather == BATTLE_WEATHER_STRONG_STORM) {
        if (element & BATTLE_ELEMENT_FIRE) {
            g_battle_action_target_data->hp_damage = g_battle_action_target_data->hp_damage * 3 / 4;
        }
        if (element & BATTLE_ELEMENT_LIGHTNING) {
            g_battle_action_target_data->hp_damage = g_battle_action_target_data->hp_damage * 5 / 4;
        }
    }
    if (weather == BATTLE_WEATHER_SNOWSTORM || weather == BATTLE_WEATHER_STRONG_SNOWSTORM) {
        if (element & BATTLE_ELEMENT_ICE) {
            g_battle_action_target_data->hp_damage = g_battle_action_target_data->hp_damage * 5 / 4;
        }
    }
}
