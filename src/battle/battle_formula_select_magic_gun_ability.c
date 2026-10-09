#include "fft/battle.h"
#include "psx/types.h"

/* Selects a Fire, Lightning, or Ice spell and installs its ability data. */
void battle_formula_select_magic_gun_ability(void) {
    u8 element;
    u8 pct;
    u16 id;
    s32 roll;

    element = g_battle_action_current_ability.weapon_data.element;
    roll = battle_formula_get_random_0_7fff();
    pct = (roll * 100) / 0x8000;
    if (element & BATTLE_ELEMENT_FIRE) {
        if (pct < 10) {
            id = 0x12;
        } else {
            id = 0x10;
            if (pct < 40) {
                id = 0x11;
            }
        }
    } else if (element & BATTLE_ELEMENT_LIGHTNING) {
        if (pct < 10) {
            id = 0x16;
        } else {
            id = 0x14;
            if (pct < 40) {
                id = 0x15;
            }
        }
    } else {
        if (pct < 10) {
            id = 0x1A;
        } else {
            id = 0x18;
            if (pct < 40) {
                id = 0x19;
            }
        }
    }
    g_battle_action_current_ability.ability_id = id;
    g_battle_action_attacker->last_ability_id = id;
    main_util_copy_byte_data(&g_main_ability_range_data[id], &g_battle_action_current_ability.range_data,
        sizeof(g_battle_action_current_ability.range_data));
}
