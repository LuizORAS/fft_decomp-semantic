#include "fft/battle.h"

struct battle_gfx_misc_data_header;
extern void battle_gfx_invalidate_sp2_vram_slot(struct battle_gfx_misc_data_header*);

/* RESUME_ATTACK_PHASE: show the queued effect messages one by one; when none is left and the
 * caster's numbers are gone, strike again for a continued attack, or free the SP2 buffer and
 * VRAM slot, store the acting unit's data and set the animations, then go on to the next action
 * phase (up to the reacting-unit phase) and announce the next due action
 * (battle_state_announce_next_ability). After the pre-action phase (Hamedo), or when no action
 * is due, go on to the action's EXP and JP (battle_action_grant_rewards). For the acting unit's
 * own action the facing goes to battle_action_post_action_facing_stub, which is empty. */
void battle_state_handle_resume_attack_phase_state(void) {
    s32 facing;
    battle_stats_t* battle_data;
    s32 command;
    battle_unit_misc_data_t* unit;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = *battle_menu_get_selected_command_address();
    if (command >= 7 && (command < 9 || command == 0xff)) {
        g_battle_action_post_action = 1;
    }
    unit = battle_unit_get_casting_misc_data();
    if (g_battle_action_post_action != 0 && battle_action_show_next_effect_message() == 0
        && unit->numeric_display_active == 0) {
        if (unit->continue_attack != 0) {
            unit->continue_attack_count += 1;
            battle_action_start_strike();
            return;
        }
        if (g_battle_gfx_sp2_data != 0) {
            main_heap_free(g_battle_gfx_sp2_data);
            g_battle_gfx_sp2_data = 0;
        }
        battle_gfx_invalidate_sp2_vram_slot((struct battle_gfx_misc_data_header*)unit);
        if (g_battle_turn_event == BATTLE_TURN_EVENT_UNIT_READY
            && g_battle_action_phase == BATTLE_ACTION_PHASE_ACTING_UNIT) {
            facing = unit->facing;
            if (facing < 0) {
                facing += 0x3ff;
            }
            facing = (u32)facing >> 10;
            battle_data = unit->battle_data;
            battle_action_post_action_facing_stub(battle_data, facing & 0xff);
        }
        battle_action_store_acting_unit_data(unit->battle_data);
        battle_unit_call_set_animation_based_on_status(unit);
        battle_unit_update_anim_display_for_all_targets(unit);
        battle_unit_get_source_misc_data();
        if (g_battle_action_phase != BATTLE_ACTION_PHASE_PRE_ACTION) {
            if (g_battle_action_phase != BATTLE_ACTION_PHASE_REACTING_UNIT) {
                g_battle_action_phase += 1;
            }
            if (battle_state_announce_next_ability() != 0) {
                return;
            }
        }
        battle_action_grant_rewards();
    }
}
