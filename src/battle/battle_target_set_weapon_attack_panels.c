#include "fft/battle.h"
#include "psx/types.h"

/* Attack and Charge: mark the tiles in range of the acting unit's weapon
 * (battle_target_calculate_weapon_range), without its own tile, and flag the targetable ones;
 * returns their count. */
s32 battle_target_set_weapon_attack_panels(battle_ai_command_action_t* source) {
    u8 action[20];
    battle_stats_t* unit;
    s32 idx;

    main_util_copy_action_data((const u8*)source, action);
    unit = &g_battle_unit_stats[action[0]];
    idx = unit->position.bits.y * g_battle_map_max_x + unit->x;
    battle_target_clear_panel_data();
    battle_target_calculate_weapon_range(unit);
    {
        battle_target_panel_t* p1;
        battle_target_panel_t* p2;
        p1 = &g_battle_target_panels[idx];
        p2 = &g_battle_target_panels[idx + 0x100];
        p1->remaining_range = 0;
        p2->remaining_range = 0;
        /* The target passes p1 to the argument-less callee and returns its
         * count of targetable panels. */
        return ((s32 (*)(battle_target_panel_t*))battle_target_set_ability_range_flags)(p1);
    }
}
