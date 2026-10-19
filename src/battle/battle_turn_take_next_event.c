#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Take the next event from the turn clock, a 32-bit value whose second byte is the event type and whose low
 * byte is the unit (nothing happens when no event is queued): store its type in g_battle_turn_event and
 * its unit as the casting unit. A turn or a due ability animates the unit and sets its enemy level
 * data; an action result applies the staged result, relocating the unit when that returns -1
 * (battle_unit_set_map_coords_after_death_dismount). Then clear the source unit's CT-resolution flags
 * and move the cursor to it. The clock never returns event 0x400 (QUIRKS.md). */
void battle_turn_take_next_event(void) {
    battle_unit_t* unit;
    s32 action;
    s32 runtime_id;

    action = battle_turn_run_clock(BATTLE_BETWEEN_TURN_MODE_RUN);
    runtime_id = action & 0xff;
    g_battle_turn_event = action & 0xff00;
    if (g_battle_turn_event == BATTLE_TURN_EVENT_NONE) {
        return;
    }
    unit = battle_unit_get_by_battle_id(runtime_id);
    if (unit != 0) {
        g_battle_unit_source_runtime_id = unit->runtime_id;
    } else {
        main_system_handle_pointer_exception(12);
    }
    switch (g_battle_turn_event) {
    case BATTLE_TURN_EVENT_UNIT_READY:
    case BATTLE_TURN_EVENT_ABILITY_READY:
        battle_unit_draw_and_activate_by_runtime_id(unit->runtime_id);
        break;
    case BATTLE_TURN_EVENT_ACTION_RESULT:
        unit->pending_attack_result = battle_action_apply_target_result(runtime_id);
        if (unit->pending_attack_result == -1) {
            battle_unit_find_relocation_tile(runtime_id, &unit->dismount);
            battle_unit_set_map_coords_after_death_dismount(unit);
        }
        break;
    case BATTLE_TURN_EVENT_UNKNOWN_0400:
        unit->pending_attack_result = battle_action_apply_target_result(runtime_id);
        if (unit->pending_attack_result == -1) {
            battle_unit_find_relocation_tile(runtime_id, &unit->dismount);
            battle_unit_set_map_coords_after_death_dismount(unit);
        }
        battle_gfx_prepare_post_action_display_by_runtime_id(unit->runtime_id);
        battle_unit_update_display_by_runtime_id(unit->runtime_id);
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
    unit = battle_unit_get_source();
    if (unit != 0) {
        unit->turn_action_flags = 0;
        if (g_battle_turn_event != BATTLE_TURN_EVENT_UNKNOWN_0400) {
            battle_target_move_cursor_to_unit(unit);
        }
    }
}
