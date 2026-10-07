#include "fft/battle.h"
#include "psx/types.h"

/* Mark the tiles in range of the unit's weapon (its weapon_data_t): striking and lunging weapons
 * their fixed shapes (battle_target_calculate_strike_lunge_range), direct weapons their range
 * spread out less the tiles within 2 steps, the others the arc range
 * (battle_target_calculate_arc_range); untargetable tiles are then dropped.
 *
 * Frog uses the bare-hand entry; an empty right hand falls back to the left hand, and non-weapon
 * items also use entry 0. */
void battle_target_calculate_weapon_range(battle_stats_t* unit) {
    s32 y;
    s32 x;
    s32 index;
    u8 weapon;
    u8 range;
    u8 flags;
    weapon_data_t* data;
    battle_target_panel_t* panel;
    battle_target_panel_t* upper;

    y = unit->position.bits.y;
    weapon = unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON];
    x = unit->x;
    index = x + y * g_battle_map_max_x;
    if (unit->status_sets.current[2] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)) {
        weapon = ITEM_ID_NOTHING;
    }
    if (weapon == ITEM_ID_NONE) {
        weapon = unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON];
        if (weapon == ITEM_ID_NONE) {
            weapon = ITEM_ID_NOTHING;
        }
    }
    if (weapon >= ITEM_ID_SHIELD_FIRST) {
        weapon = ITEM_ID_NOTHING;
    }
    data = &g_main_item_weapon_data[weapon];
    range = data->range;
    flags = data->flags;
    panel = &g_battle_target_panels[index];
    upper = &g_battle_target_panels[index + 0x100];
    panel->remaining_range = range + 1;
    upper->remaining_range = range + 1;
    panel->mark = 1;
    if (flags & (WEAPON_FLAG_STRIKING | WEAPON_FLAG_LUNGING)) {
        battle_target_calculate_strike_lunge_range(unit, flags);
    } else if (flags & WEAPON_FLAG_DIRECT) {
        battle_target_spread_panels(range, 0);
        battle_target_remove_close_range(x, y, 2);
    } else {
        battle_target_calculate_arc_range(unit, range, flags);
    }
    battle_target_clear_range_on_untargetable_tiles();
}
