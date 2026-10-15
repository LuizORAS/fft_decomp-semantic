#include "fft/wldcore.h"
#include "psx/types.h"

/* Sort the event transition's screen sprite (g_wldcore_transition_screen_sprite) into `ordering_table`
 * while the top level is a spinning event transition (not WLDCORE_EVENT_TRANSITION_MODE_FADE).
 *
 * The level is spelled g_wldcore_menu_stack_records_next[current - 1]; the
 * equivalent g_wldcore_menu_stack_records[current] changes the address
 * arithmetic. */
void wldcore_gfx_draw_transition_screen(GsOT* ordering_table) {
    s32 current;

    current = g_wldcore_menu_stack_depth;
    if (g_wldcore_menu_stack_types[current] == WLDCORE_MENU_LEVEL_EVENT_TRANSITION
        && g_wldcore_menu_stack_records_next[current - 1].event_transition.mode != WLDCORE_EVENT_TRANSITION_MODE_FADE) {
        GsSortSprite(&g_wldcore_transition_screen_sprite, ordering_table, 2);
    }
}
