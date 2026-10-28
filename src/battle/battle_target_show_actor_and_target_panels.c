#include "fft/battle.h"
#include "fft/battle_target.h"

/* Show the acting unit, and the unit under the map cursor as its target (panel mode 5), or the
 * acting unit alone (mode 2). */
void battle_target_show_actor_and_target_panels(void) {
    battle_unit_t* target;
    battle_unit_t* source;

    target = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    source = battle_unit_get_source();
    if (target != 0) {
        battle_unit_start_view_thread(
            UNIT_VIEW_MODE_ATTACK_PREVIEW, source->stats->battle_id, target->stats->battle_id);
    } else {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_HOVERED_UNIT, source->stats->battle_id, 0xFF);
    }
}
