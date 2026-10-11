#include "fft/battle.h"
#include "psx/types.h"

/* Enter the action-execute state for the casting unit, point the cursor at its
 * action's target and release the ability's effect. The secondary-effect state
 * calls it every frame.
 *
 * A plain attack (ability 0), the knockback strike (ABILITY_ID_KNOCKBACK) and an
 * ability without an effect file play nothing. Accumulate and the Charge
 * abilities play the effect at animation speed 1 unless it is an item effect.
 * Every other ability plays at animation speed 2 with the tracked sound effects
 * paused; an item effect is held back until the unit shows its hold-item
 * animation. The AT-list-active flag is cleared on every path. */
void battle_action_set_item_throw_stone_ability_display(void) {
    battle_unit_misc_data_t* unit;
    s32 ability;

    g_battle_game_state = BATTLE_GAME_STATE_ACTION_EXECUTE;
    unit = battle_unit_get_casting_misc_data();
    ability = unit->used_ability_id;
    unit->state_frame_counter = 0;
    battle_target_gather_x_y_data_for_attacks(unit);

    if ((ability == 0) || (ability == ABILITY_ID_KNOCKBACK)) {
        g_main_system_animation_speed = 1;
        battle_action_clear_at_list_active();
        return;
    }
    if (battle_effect_load_ability(ability) != 0) {
        g_main_system_animation_speed = 1;
        battle_action_clear_at_list_active();
        return;
    }
    if ((ability == ABILITY_ID_BASIC_SKILL_ACCUMULATE)
        || (ability >= ABILITY_ID_CHARGE_FIRST && ability < ABILITY_ID_MATH_FIRST)) {
        if (battle_effect_is_item_ability(ability) != 0) {
            g_main_system_animation_speed = 1;
            battle_action_clear_at_list_active();
            return;
        }
        battle_effect_play();
        g_main_system_animation_speed = 1;
        battle_action_clear_at_list_active();
        return;
    }
    if ((battle_effect_is_item_ability(ability) == 0)
        || ((unit->animation.sequence_id >> 1) == BATTLE_UNIT_ANIMATION_HOLD_ITEM)) {
        battle_effect_play();
    }
    main_sound_stop_current_weather_sfx();
    g_main_system_animation_speed = 2;
    battle_action_clear_at_list_active();
}
