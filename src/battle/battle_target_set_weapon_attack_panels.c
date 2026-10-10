#include "fft/battle.h"
#include "psx/types.h"

/* Build the targeting panels for a weapon attack: the weapon's range around the
 * acting unit, without the unit's own tile. Returns the number of tiles in range. */
s32 battle_target_set_weapon_attack_panels(battle_ai_command_action_t* source) {
    u8 action[20];
    battle_stats_t* unit;
    s32 idx;

    main_util_copy_action_data(source, action);
    unit = &g_battle_unit_stats[action[0]];
    idx = unit->position.bits.y * g_battle_map_max_x + unit->x;
    battle_target_clear_panel_data();
    battle_target_calculate_weapon_range(unit);
    {
        battle_target_panel_t* p1;
        battle_target_panel_t* p2;
        p1 = &g_battle_target_panel_data[idx];
        p2 = &g_battle_target_panel_data[idx + 0x100];
        p1->remaining_range = 0;
        p2->remaining_range = 0;
        return battle_target_set_all_panels_targeted_if_targetable();
    }
}
