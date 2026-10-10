#include "fft/battle.h"

/* Build the AI command for unit_id's turn (battle_state_handle_ai_command_state): MOVE to a tile, ACT with
 * the chosen action, or END_TURN. The turn ends at once when the chosen action is spent and the unit has
 * neither moved nor acted since the last command, and when the unit or any present unit has no AI slot
 * (more than 16 units). Otherwise, unless an action is already chosen and the unit has not acted, it
 * plans again: it enters AI simulation (battle_ai_begin_simulation), refreshes the decision flags, marks
 * the usable abilities, builds the movement panels, sets the team targeting flags and the target, and
 * chooses the action (battle_ai_select_action). Then the unit moves first, unless the action is
 * done from its own tile (act_before_move) or it has already acted; it ends the turn after acting or
 * with no action. A stage that runs out of the frame's time returns -1 (here VSync(1) above 440 at
 * entry) and the next call resumes it; a command returns 0 and is kept in g_battle_ai_command_history. */
s32 battle_ai_build_command(s32 unit_id, battle_ai_command_t* command) {
    battle_ai_data_t* ai;
    battle_stats_t* unit;
    battle_ai_command_t* last_command;
    battle_ai_command_t* history;
    s32 i;
    s32 ticks;
    u8 moved;

    ticks = VSync(1);
    ai = &g_battle_ai_data_base;
    if (ticks > 440)
        return -1;
    if (g_battle_ai_data_base.decision_state) {
        /* Resume: re-enter the suspended decision stage. */
        switch (g_battle_ai_data_base.main_ai_state) {
        case BATTLE_AI_COMMAND_BUILD_REFRESH:
            goto refresh;
        case BATTLE_AI_COMMAND_BUILD_MOVEMENT:
            goto movement;
        case BATTLE_AI_COMMAND_BUILD_CHOOSE_ACTION:
            goto choose;
        }
    }
    if (ai->selected_action.rank_byte == 0xff && ai->acting_unit->movement_taken == ai->movement_taken
        && ai->acting_unit->action_taken == ai->action_taken) {
        command->kind = BATTLE_AI_COMMAND_END_TURN;
        return 0;
    }
    ai->acting_unit_id = unit_id;
    if (ai->unit_battle_ids[unit_id] >= 16) {
        command->kind = BATTLE_AI_COMMAND_END_TURN;
        return 0;
    }
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (g_battle_unit_stats[i].entd_slot != BATTLE_ENTD_SLOT_NONE && ai->unit_battle_ids[i] >= 16) {
            command->kind = BATTLE_AI_COMMAND_END_TURN;
            return 0;
        }
    }
    battle_ai_init_acting_unit_data();
    if (ai->acting_unit->charged_ability_ct == 0xff)
        ai->acting_unit_decision->targeting_flags_1 &= ~BATTLE_AI_TARGETING_FLAG_1_REFLECTED_ACTION;
    if (!ai->selected_action.rank_byte || ai->acting_unit->action_taken) {
        battle_ai_begin_simulation();
    refresh:
        if (battle_ai_refresh_unit_decision_flags() == -1) {
            ai->main_ai_state = BATTLE_AI_COMMAND_BUILD_REFRESH;
            ai->decision_state = 1;
            return -1;
        }
        battle_ai_set_ability_considerations(ai->acting_unit->action_taken);
    movement:
        if (battle_ai_build_reachable_tiles(ai->acting_unit->movement_taken) == -1) {
            ai->main_ai_state = BATTLE_AI_COMMAND_BUILD_MOVEMENT;
            ai->decision_state = 1;
            return -1;
        }
        ai->decision_state = 0;
        battle_ai_update_team_targeting_flags();
        battle_ai_init_target_consideration();
    choose:
        if (battle_ai_select_action() == -1) {
            ai->main_ai_state = BATTLE_AI_COMMAND_BUILD_CHOOSE_ACTION;
            ai->decision_state = 1;
            return -1;
        }
        ai->decision_state = 0;
        ai->selected_action.rank_byte = 0xff;
        if (ai->selected_action.reflected_action)
            ai->acting_unit_decision->targeting_flags_1 |= BATTLE_AI_TARGETING_FLAG_1_REFLECTED_ACTION;
        battle_ai_end_simulation();
    }
    if (!ai->acting_unit->movement_taken && (ai->acting_unit->action_taken || !ai->selected_action.act_before_move)) {
        ai->selected_action.coords.bytes.zero = 0;
        if (ai->selected_action.coords.word != ai->acting_unit_coords.word) {
            command->kind = BATTLE_AI_COMMAND_MOVE;
            command->data.move.x = ai->selected_action.coords.bytes.x;
            command->data.move.y = ai->selected_action.coords.bytes.y;
            command->data.move.elevation = ai->selected_action.coords.bytes.elevation;
            goto preserve;
        }
    }
    if (ai->acting_unit->action_taken || !ai->selected_action.skillset) {
        command->kind = BATTLE_AI_COMMAND_END_TURN;
    } else {
        command->kind = BATTLE_AI_COMMAND_ACT;
        battle_ai_transfer_halfword_values(command->data.halfwords, (u16*)&ai->selected_action.unit_id, 20);
    }
preserve:
    moved = ai->acting_unit->movement_taken;
    ai->movement_taken = moved;
    unit = ai->acting_unit;
    ai->action_taken = unit->action_taken;
    battle_ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    history = &g_battle_ai_command_history[0];
    last_command = history + 1;
    battle_ai_transfer_halfword_values((u16*)history, (u16*)last_command, 24);
    battle_ai_transfer_halfword_values((u16*)last_command, (u16*)command, 24);
    return 0;
}
