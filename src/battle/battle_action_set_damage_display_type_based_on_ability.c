#include "fft/battle.h"
#include "fft/battle_action.h"
#include "psx/types.h"

/* Resolve the casting unit's next strike and start showing it.
 *
 * When a reaction replaced the ability, or the strike is a plain attack, the
 * knockback strike or an ability without an effect file, the attack animation
 * starts directly. Otherwise the game enters the effect-file-open state with the
 * targets and their result animations stored for the effect. */
void battle_action_set_damage_display_type_based_on_ability(void) {
    battle_unit_misc_data_t* misc;
    u16 raw;
    s32 ability;
    s32 target;

    g_main_system_animation_speed = 1;
    misc = battle_unit_get_casting_misc_data();
    battle_action_resolve_ability_strike(misc->battle_data->misc_unit_id, &misc->strike);
    raw = misc->strike.last_attack_id;
    /* The target reads 0x1a6 as a signed 16-bit value. */
    target = misc->strike.reaction_id;
    ability = raw & 0xffff;
    misc->used_ability_id = raw;
    battle_gfx_set_thrown_item_graphic_palette(misc->strike.used_weapon_id, misc);
    if (((target != 0) && (ability != target)) || (ability == ABILITY_ID_KNOCKBACK) || (ability == 0)
        || (battle_effect_load_ability(ability) != 0)) {
        battle_action_set_target_coords_and_attacker_anim();
        return;
    }
    g_battle_game_state = BATTLE_GAME_STATE_START_EFFECT_FILE_OPEN;
    battle_effect_store_targets_and_display_types(ability, misc);
}
