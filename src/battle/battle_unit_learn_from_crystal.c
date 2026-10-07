#include "fft/battle.h"
#include "psx/types.h"

/* Apply the choice made at a crystal or chest (flags, from the CRYSTAL_LEARN state and the
 * post-move events): 4 takes the chest's item into the party (battle_action_add_party_item, whose
 * result it returns); 2 restores all HP and MP as the unit's action result; 1 adds the abilities the
 * crystal offered (g_main_crystal_pickup_result.learned) to the unit's learned abilities. The unit's
 * action record is cleared first; returns 0 unless a chest's item was taken. */
s32 battle_unit_learn_from_crystal(battle_stats_t* unit, s32 flags) {
    s32 i;
    u8* dst;
    u8* src;
    u8* learned;
    u8* crystal_bits;
    u8* end;

    battle_action_clear_current_data(&unit->action);
    unit->action.miss_type = BATTLE_ACTION_MISS_TYPE_CANCELLED;
    unit->action.hit = 0;
    if (flags & 4) {
        return battle_action_add_party_item(unit, g_main_crystal_pickup_result.treasure_item);
    }
    if (flags & 2) {
        unit->action.hit = 1;
        unit->action.attack_type = BATTLE_ACTION_TYPE_HP_HEALING | BATTLE_ACTION_TYPE_MP_HEALING;
        unit->action.miss_type = BATTLE_ACTION_MISS_TYPE_HIT;
        unit->action.hp_healing = unit->max_hp - unit->hp;
        unit->action.mp_healing = unit->max_mp - unit->mp;
    }
    if (flags & 1) {
        /* OR the crystal's ability bits into learned_abilities, three bytes
         * per job for 19 jobs; walked as raw bytes to keep the biased pointers. */
        i = 0;
        src = g_main_crystal_pickup_result.learned[0];
        dst = (u8*)unit;
        do {
            learned = dst + 0x99;
            crystal_bits = src;
            end = dst + 0x9C;
            do {
                *learned |= *crystal_bits;
                learned++;
                crystal_bits++;
                /* The signed compare is byte-required (slt, not sltu). */
            } while ((s32)learned < (s32)end);
            src += 3;
            i++;
            dst += 3;
        } while (i < UNIT_LEARNED_ABILITY_JOB_COUNT);
    }
    return 0;
}
