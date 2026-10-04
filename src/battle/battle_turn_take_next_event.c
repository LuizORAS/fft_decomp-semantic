#include "fft/battle.h"
#include "psx/types.h"

/* Take the next event from the turn clock: store its type in g_battle_turn_event and its unit
 * as the casting unit. A turn or a due ability animates the unit and sets its enemy level data;
 * an action result applies the staged result, relocating the unit when that returns -1
 * (battle_unit_set_map_coords_after_death_dismount). Then clear the source unit's
 * CT-resolution flags and move the cursor to it. The clock never returns event 0x400
 * (QUIRKS.md). */
void battle_turn_take_next_event(void) {
    battle_unit_misc_data_t* unit;
    s32 action;
    s32 misc_id;

    action = battle_turn_run_clock(0);
    misc_id = action & 0xff;
    g_battle_turn_event = action & 0xff00;
    if (g_battle_turn_event == BATTLE_TURN_EVENT_NONE) {
        return;
    }
    unit = battle_unit_get_misc_data_by_battle_id(misc_id);
    if (unit != 0) {
        g_battle_casting_misc_id = unit->unit_id;
    } else {
        main_system_handle_pointer_exception(12);
    }
    switch (g_battle_turn_event) {
    case BATTLE_TURN_EVENT_UNIT_READY:
    case BATTLE_TURN_EVENT_ABILITY_READY:
        battle_unit_animate_and_set_enemy_level_data_by_misc_id(unit->unit_id);
        break;
    case BATTLE_TURN_EVENT_ACTION_RESULT:
        unit->pending_attack_result = battle_action_finalize_attack_and_flag_reactions(misc_id);
        if (unit->pending_attack_result == -1) {
            battle_unit_find_relocation_tile(misc_id, &unit->dismount);
            battle_unit_set_map_coords_after_death_dismount(unit);
        }
        break;
    case BATTLE_TURN_EVENT_UNKNOWN_0400:
        unit->pending_attack_result = battle_action_finalize_attack_and_flag_reactions(misc_id);
        if (unit->pending_attack_result == -1) {
            battle_unit_find_relocation_tile(misc_id, &unit->dismount);
            battle_unit_set_map_coords_after_death_dismount(unit);
        }
        battle_gfx_prepare_post_action_display_by_misc_id(unit->unit_id);
        battle_unit_update_display_by_misc_id(unit->unit_id);
        break;
    case BATTLE_TURN_EVENT_MIME:
        break;
    case 0x600: /* a label above 0x500 on the error path makes GCC split the upper
                   tree at 0x500 as the target does; nothing compares it, so the
                   original value is not recoverable */
    default:
        main_system_handle_pointer_exception(20);
        break;
    }
    unit = battle_unit_get_source_misc_data();
    if (unit != 0) {
        unit->ability_ct_resolved = 0;
        if (g_battle_turn_event != BATTLE_TURN_EVENT_UNKNOWN_0400) {
            battle_target_move_cursor_to_unit(unit);
        }
    }
}
