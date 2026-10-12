#include "fft/battle.h"
#include "fft/battle_action.h"
#include "psx/types.h"

/* Element step, run after the HP damage is stored: Oil against fire, Float
 * against earth (a rider uses its mount's Float), then the target's affinities
 * to the ability's elements. */
void battle_formula_apply_elemental(void) {
    u8 element;
    action_data_t* action;
    battle_stats_t* unit;
    s32 mount_info;

    element = g_action_current_ability.range_data.element;
    /* Oil (status byte 2 bit 0x80) is removed by fire (element bit 0x80). XA is
     * doubled, but every caller has already stored the HP damage and nothing
     * reads XA afterwards, so the damage does not change. */
    if ((g_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_OIL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_OIL))
        && (element & BATTLE_ELEMENT_FIRE)) {
        action = g_action_target_data;
        g_action_current_ability.xa = g_action_current_ability.xa * 2;
        action->status_changes.removal[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_OIL)]
            |= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_OIL);
        if (battle_status_modify_inflictions(0) != 0) {
            g_action_target_data->attack_type = ACTION_TYPE_STATUS_CHANGE;
        }
    }
    unit = g_action_target;
    mount_info = unit->mount_info;
    if (mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) {
        unit = &g_battle_unit_stats[mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK];
    }
    /* Float (status byte 2 bit 0x40) nullifies earth (element bit 0x08). */
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FLOAT)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT))
        && (element & BATTLE_ELEMENT_EARTH)) {
        battle_formula_nullify_action();
        g_action_target_data->miss_type = ACTION_MISS_TYPE_FORCED_FAILURE;
    }
    battle_formula_modify_damage_for_element(element);
}
