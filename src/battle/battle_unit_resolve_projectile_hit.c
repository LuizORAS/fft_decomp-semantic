#include "fft/battle.h"

/* Lands a projectile strike on its target: the weapon hit sound, the target's
 * reaction animation, its post-action display and the evade and weapon-element
 * effect data. Does nothing when either unit is missing. */
void battle_unit_resolve_projectile_hit(battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target) {
    if ((attacker != 0) && (target != 0)) {
        battle_sound_select_weapon_hit(attacker, 1);
        battle_unit_set_target_animation_from_attack_type(attacker, target);
        battle_gfx_prepare_post_action_display_by_misc_id(target->unit_id);
        battle_effect_start_action_weapon_hit_secondary(attacker, 1);
    }
}
