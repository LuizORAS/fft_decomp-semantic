#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/memory.h"
#include "psx/types.h"

/* volatile view: the target reloads this global at every use. */
extern battle_stats_t* volatile g_battle_action_attacker;

/* Magic Gun (formula 0x04): pick the spell from the gun's element, fire (Fire), lightning (Bolt) or
 * else ice (Ice): level 3 on a 0-99 roll below 10, level 2 below 40, else level 1 (10%, 30%, 60%;
 * an estimate's middle roll picks level 1). The spell's data becomes the current ability and the
 * attacker's last ability. */
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
            id = ABILITY_ID_BLACK_MAGIC_FIRE_3;
        } else {
            id = ABILITY_ID_BLACK_MAGIC_FIRE;
            if (pct < 40) {
                id = ABILITY_ID_BLACK_MAGIC_FIRE_2;
            }
        }
    } else if (element & BATTLE_ELEMENT_LIGHTNING) {
        if (pct < 10) {
            id = ABILITY_ID_BLACK_MAGIC_BOLT_3;
        } else {
            id = ABILITY_ID_BLACK_MAGIC_BOLT;
            if (pct < 40) {
                id = ABILITY_ID_BLACK_MAGIC_BOLT_2;
            }
        }
    } else {
        if (pct < 10) {
            id = ABILITY_ID_BLACK_MAGIC_ICE_3;
        } else {
            id = ABILITY_ID_BLACK_MAGIC_ICE;
            if (pct < 40) {
                id = ABILITY_ID_BLACK_MAGIC_ICE_2;
            }
        }
    }
    g_battle_action_current_ability.ability_id = id;
    g_battle_action_attacker->action_command.ability_id = id;
    main_mem_copy_byte_data_source_first(&g_main_ability_range_data[id], &g_battle_action_current_ability.range_data,
        sizeof(g_battle_action_current_ability.range_data));
}
