#include "fft/battle.h"
#include "psx/types.h"

/* volatile view: the target reloads this global at every use. */
extern battle_stats_t* volatile g_battle_action_attacker;

/* Magic Gun (formula 0x04): pick the spell from the gun's element, fire (Fire), lightning (Bolt) or
 * else ice (Ice): level 3 on a 0-99 roll below 10, level 2 below 40, else level 1 (an estimate's
 * middle roll picks level 1). The spell's data becomes the current ability and the attacker's last
 * ability. */
void battle_formula_select_magic_gun_ability(void) {
    u8 element;
    u8 pct;
    u16 id;
    volatile u16* current_ability;
    u8* src;
    battle_stats_t* attacker;
    s32 roll;

    element = g_current_ability.weapon_data.element;
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
    current_ability = &g_current_ability.ability_id;
    *current_ability = id;
    src = (u8*)&g_current_ability.range_data;
    attacker = g_battle_action_attacker;
    attacker->last_ability_id = id;
    main_util_copy_byte_data(&g_main_ability_range_data[id], src, 14);
}
