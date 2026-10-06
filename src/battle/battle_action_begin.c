#include "fft/battle.h"
#include "psx/types.h"

/* Begin the unit's action in the primary context. An absent unit returns -1, an action that a
 * First Strike (Hamedo) cancelled returns 6, and a refusal from battle_action_check_and_consume_mp
 * returns its code (2 Silence, 3 not enough MP, 5 Frog, 7 and up cannot act); in those two cases the
 * actor's position or command is saved (battle_action_store_acting_unit_data). Otherwise the unit
 * becomes the acting unit with its ability, every unit's reaction id and outcome is cleared, the
 * strike data is set up and the command is recorded for Mimes; returns 0.
 * battle_state_announce_next_ability shows codes 2 and 3 over the unit; the AI simulation runs it
 * too. */
s32 battle_action_begin(s32 id) {
    battle_stats_t* unit;
    s32 i;
    battle_stats_t* base;
    s32 mp_result;
    u16 ability;

    g_battle_acting_unit_id = -1;
    g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
    if (id >= BATTLE_UNIT_SLOT_COUNT
        || (unit = &g_battle_unit_stats[id], base = g_battle_unit_stats, unit->entd_slot == BATTLE_ENTD_SLOT_NONE)) {
        return -1;
    }
    if (g_current_ability_hamedo_flag != 0) {
        battle_action_store_acting_unit_data(unit);
        return 6;
    }
    mp_result = battle_action_check_and_consume_mp(unit);
    if (mp_result != 0) {
        battle_action_store_acting_unit_data(unit);
        return mp_result;
    }
    /* The target loads the s16 last_ability_id unsigned (lhu). */
    ability = *(u16*)&unit->last_ability_id;
    g_battle_acting_unit_id = id;
    g_battle_acting_unit_used_ability_id = ability;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        base[i].action.reaction_id = 0;
        base[i].ability_outcome = 0;
    }
    battle_action_init_current_ability_strike_data(unit);
    battle_action_set_current_attacker_data(unit);
    return 0;
}
