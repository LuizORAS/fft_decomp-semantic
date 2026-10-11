#include "fft/battle.h"
#include "psx/types.h"

/* Enter ACTION_EXECUTE and play the casting unit's ability effect, with the cursor moved to its
 * target. A plain attack, a knockback or an ability without an effect file plays nothing at 60 fps.
 * Accumulate and the Charge abilities play theirs unless they are item abilities; any other ability
 * plays it unless it is an item ability (one whose unit animation is 0x39 plays anyway), pauses the
 * tracked sound effects and runs at 30 fps. The map cursor hides. */
void battle_action_play_ability_effect(void) {
    battle_unit_misc_data_t* unit;
    s32 ability;

    g_battle_game_state = BATTLE_GAME_STATE_ACTION_EXECUTE;
    unit = battle_unit_get_casting_misc_data();
    ability = unit->used_ability_id;
    unit->state_frame_counter = 0;
    battle_target_move_cursor_to_action_target(unit);

    if ((ability == 0) || (ability == ABILITY_ID_KNOCKBACK)) {
        g_main_system_vsync_interval = 1;
        battle_target_hide_cursor();
        return;
    }
    if (battle_effect_load_ability(ability) != 0) {
        g_main_system_vsync_interval = 1;
        battle_target_hide_cursor();
        return;
    }
    if ((ability == ABILITY_ID_BASIC_SKILL_ACCUMULATE)
        || (ability >= ABILITY_ID_CHARGE_FIRST && ability < ABILITY_ID_MATH_FIRST)) {
        if (battle_effect_is_item_ability(ability) != 0) {
            g_main_system_vsync_interval = 1;
            battle_target_hide_cursor();
            return;
        }
        battle_effect_play();
        g_main_system_vsync_interval = 1;
        battle_target_hide_cursor();
        return;
    }
    if ((battle_effect_is_item_ability(ability) == 0)
        || ((unit->sequence_id >> 1) == BATTLE_UNIT_ANIMATION_HOLD_ITEM)) {
        battle_effect_play();
    }
    main_sound_pause_tracked_sfx();
    g_main_system_vsync_interval = 2;
    battle_target_hide_cursor();
}
