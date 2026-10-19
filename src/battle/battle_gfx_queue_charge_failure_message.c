#include "fft/battle_action.h"
#include "fft/battle_gfx.h"

/* Queues the failure message for a charged action that cannot start: mode 1 no
 * target, 2 silenced, 3 not enough MP (the result of
 * battle_action_begin); any other mode queues none.
 *
 * The three numeric displays are pointed at the FRAME.BIN page, and the first
 * display is built at once unless one is already running. */
void battle_gfx_queue_charge_failure_message(battle_unit_t* unit, s32 mode) {
    battle_gfx_sprite_display_data_t* display_0 = unit->numeric_display.displays[0];
    battle_gfx_sprite_display_data_t* display_1 = unit->numeric_display.displays[1];
    battle_gfx_sprite_display_data_t* display_2 = unit->numeric_display.displays[2];

    display_2->tpage = BATTLE_GFX_TPAGE_FRAME;
    display_1->tpage = BATTLE_GFX_TPAGE_FRAME;
    display_0->tpage = BATTLE_GFX_TPAGE_FRAME;
    switch (mode) {
    case 1:
        unit->action_display_flags |= BATTLE_ACTION_DISPLAY_FLAG_NO_TARGET;
        break;
    case 2:
        unit->action_display_flags |= BATTLE_ACTION_DISPLAY_FLAG_SILENCED;
        break;
    case 3:
        unit->action_display_flags |= BATTLE_ACTION_DISPLAY_FLAG_NO_MP;
        break;
    }
    if (unit->numeric_display.active == 0) {
        unit->numeric_display.progress = 0;
        battle_gfx_build_next_special_action_result_display(unit);
    }
}
