#include "fft/battle.h"
#include "psx/types.h"

/* The ability's element on the current target: Oil with fire doubles XA and is marked for removal,
 * Float (the mount's, when the target rides) nullifies earth, then the target's affinities
 * (battle_formula_apply_element_affinities). Every caller has already stored XA * YA as the damage, so
 * the Oil doubling changes nothing (QUIRKS.md). Weapon strikes use battle_formula_apply_weapon_element,
 * which skips Oil and Float. */
void battle_formula_apply_ability_element(void) {
    u8 element;
    battle_action_data_t* action;
    battle_stats_t* unit;
    s32 mount_info;

    element = g_current_ability.range_data.element;
    /* Oil doubles XA against fire, too late for the stored damage (see above), and is marked for removal. */
    if ((g_battle_action_target->status_sets.current[2] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_OIL))
        && (element & BATTLE_ELEMENT_FIRE)) {
        action = g_battle_action_target_data;
        g_current_ability.xa = (s16)g_current_ability.xa * 2;
        action->status_removal[2] |= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_OIL);
        if (battle_status_modify_inflictions(0) != 0) {
            g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
    }
    unit = g_battle_action_target;
    mount_info = unit->mount_info;
    if (mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) {
        unit = &g_battle_unit_stats[mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK];
    }
    /* Float (status 3 bit 0x40) nullifies earth (element bit 0x08). */
    if ((unit->status_sets.current[2] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT))
        && (element & BATTLE_ELEMENT_EARTH)) {
        /* The target loads mount_info into a0 although the callee takes none. */
        ((void (*)(s32))battle_formula_nullify_action)(mount_info);
        g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_FORCED_FAILURE;
    }
    battle_formula_apply_element_affinities(element);
}
