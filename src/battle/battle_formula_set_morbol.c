#include "fft/battle_action.h"
#include "fft/data.h"
#include "fft/unit.h"

/* Formula 0x58 "Set Morbol: Hit(MA+X)%". */

/* Generic character identities are 0x80 male, 0x81 female, 0x82 monster;
 * Morbol-type jobs are 0x82 Morbol, 0x83 Ochu, 0x84 Great Morbol. */
void battle_formula_set_morbol(void) {
    action_data_t* action;
    battle_stats_t* target;

    battle_formula_store_ma_and_x();
    battle_formula_apply_elemental_strengthen();
    battle_formula_apply_magic_attack_up();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_xa_plus_ya_status_damage();
    battle_formula_use_hp_damage_as_action_hit_percent();
    action = g_action_target_data;
    if (action->hit == 0)
        return;
    target = g_action_target;
    if (!(target->team_flags & BATTLE_TEAM_FLAG_IMMORTAL) && target->mount_info == 0
        && target->character_identity >= CHARACTER_IDENTITY_GENERIC_MALE
        && target->character_identity <= CHARACTER_IDENTITY_MONSTER
        && (target->job_id < JOB_ID_MORBOL || target->job_id > JOB_ID_MORBOL_FAMILY_LAST)) {
        /* The 16-bit store clears all other special-effect flags. */
        action->special_effect = ACTION_SPECIAL_EFFECT_MORBOL;
        action->attack_type = ACTION_TYPE_PSEUDO_STATUS;
        return;
    }
    battle_formula_force_attack_miss();
}
