#include "fft/battle.h"
#include "psx/types.h"

void battle_action_set_item_throw_stone_ability_display(void) {
    battle_unit_misc_data_t* unit;
    s32 ability;

    g_battle_game_state = BATTLE_GAME_STATE_ACTION_EXECUTE;
    unit = battle_unit_get_casting_misc_data();
    ability = unit->used_ability_id;
    unit->state_frame_counter = 0;
    battle_target_gather_x_y_data_for_attacks(unit);

    if ((ability == 0) || (ability == ABILITY_ID_KNOCKBACK)) {
        g_battle_state_vsync_interval = 1;
        battle_target_hide_cursor();
        return;
    }
    if (battle_effect_load_ability(ability) != 0) {
        g_battle_state_vsync_interval = 1;
        battle_target_hide_cursor();
        return;
    }
    if ((ability == ABILITY_ID_BASIC_SKILL_ACCUMULATE)
        || ((u32)(ability - ABILITY_ID_CHARGE_FIRST) < (ABILITY_ID_MATH_FIRST - ABILITY_ID_CHARGE_FIRST))) {
        if (battle_effect_is_item_ability(ability) != 0) {
            g_battle_state_vsync_interval = 1;
            battle_target_hide_cursor();
            return;
        }
        battle_effect_play();
        g_battle_state_vsync_interval = 1;
        battle_target_hide_cursor();
        return;
    }
    if ((battle_effect_is_item_ability(ability) == 0) || ((unit->encoded_animation >> 1) == 0x39)) {
        battle_effect_play();
    }
    main_sound_pause_tracked_sfx();
    g_battle_state_vsync_interval = 2;
    battle_target_hide_cursor();
}
