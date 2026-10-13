#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "fft/memory.h"

/*
 * Prepare the next AI movement, action or end-turn command.
 *
 * Resume suspended setup stages before resolving the selected action. Return
 * -1 while suspended, or 0 after writing a command. Early end-turn paths skip
 * coordinate restoration and command-history updates.
 */
s32 ai_build_command(s32 unit_id, ai_command_t* command) {
    ai_data_t* ai;
    unit_stats_t* unit;
    ai_command_t* last_command;
    ai_command_t* history;
    s32 i;
    s32 ticks;
    u8 moved;

    ticks = VSync(1);
    ai = &g_ai_data_base;
    if (ticks >= AI_DECISION_SCANLINE_LIMIT)
        return -1;
    if (g_ai_data_base.resume_pending) {
        /* Resume: re-enter the suspended decision stage. */
        switch (g_ai_data_base.main_ai_state) {
        case AI_COMMAND_BUILD_REFRESH:
            goto refresh;
        case AI_COMMAND_BUILD_MOVEMENT:
            goto movement;
        case AI_COMMAND_BUILD_CHOOSE_ACTION:
            goto choose;
        }
    }
    if (ai->selected_action.rank.decided == AI_ACTION_DECIDED && ai->acting_unit->movement_taken == ai->movement_taken
        && ai->acting_unit->action_taken == ai->action_taken) {
        command->kind = AI_COMMAND_END_TURN;
        return 0;
    }
    ai->acting_unit_id = unit_id;
    if (ai->unit_compact_indices[unit_id] >= AI_COMPACT_UNIT_COUNT) {
        command->kind = AI_COMMAND_END_TURN;
        return 0;
    }
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (g_battle_unit_stats[i].entd_slot != BATTLE_ENTD_SLOT_NONE
            && ai->unit_compact_indices[i] >= AI_COMPACT_UNIT_COUNT) {
            command->kind = AI_COMMAND_END_TURN;
            return 0;
        }
    }
    ai_init_acting_unit_data();
    if (ai->acting_unit->charged_ability_ct == 0xff)
        ai->acting_unit_decision->targeting_flags_1 &= ~AI_TARGETING_FLAG_1_REFLECTED_ACTION;
    if (!ai->selected_action.rank.decided || ai->acting_unit->action_taken) {
        ai_begin_action_simulation();
    refresh:
        if (ai_refresh_unit_decision_flags() == -1) {
            ai->main_ai_state = AI_COMMAND_BUILD_REFRESH;
            ai->resume_pending = 1;
            return -1;
        }
        ai_set_ability_considerations(ai->acting_unit->action_taken);
    movement:
        if (ai_set_movement_panel_data(ai->acting_unit->movement_taken) == -1) {
            ai->main_ai_state = AI_COMMAND_BUILD_MOVEMENT;
            ai->resume_pending = 1;
            return -1;
        }
        ai->resume_pending = 0;
        ai_update_team_targeting_flags();
        ai_init_target_consideration();
    choose:
        if (ai_select_action_initial() == -1) {
            ai->main_ai_state = AI_COMMAND_BUILD_CHOOSE_ACTION;
            ai->resume_pending = 1;
            return -1;
        }
        ai->resume_pending = 0;
        ai->selected_action.rank.decided = AI_ACTION_DECIDED;
        if (ai->selected_action.reflected_action)
            ai->acting_unit_decision->targeting_flags_1 |= AI_TARGETING_FLAG_1_REFLECTED_ACTION;
        ai_end_action_simulation();
    }
    if (!ai->acting_unit->movement_taken && (ai->acting_unit->action_taken || !ai->selected_action.act_before_move)) {
        ai->selected_action.coords.fields.zero = 0;
        if (ai->selected_action.coords.word != ai->acting_unit_coords.word) {
            command->kind = AI_COMMAND_MOVE;
            command->data.move.x = ai->selected_action.coords.fields.x;
            command->data.move.y = ai->selected_action.coords.fields.y;
            command->data.move.layer = ai->selected_action.coords.fields.elevation;
            goto preserve;
        }
    }
    if (ai->acting_unit->action_taken || !ai->selected_action.command.skillset_id) {
        command->kind = AI_COMMAND_END_TURN;
    } else {
        command->kind = AI_COMMAND_ACT;
        mem_copy_u16_values(&command->data.action, &ai->selected_action.command, sizeof(command->data.action));
    }
preserve:
    moved = ai->acting_unit->movement_taken;
    ai->movement_taken = moved;
    unit = ai->acting_unit;
    ai->action_taken = unit->action_taken;
    ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    history = &g_ai_command_history[0];
    last_command = history + 1;
    mem_copy_u16_values(history, last_command, sizeof(*history));
    mem_copy_u16_values(last_command, command, sizeof(*last_command));
    return 0;
}
