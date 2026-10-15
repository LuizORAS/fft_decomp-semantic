#include "fft/world.h"
#include "psx/libc.h"

/* Make the selected formation unit current: latch its index, fill its HP and MP, copy its status
 * summary, identity and stat details into the selected-unit display records and the unit into the
 * preview record (world_formation_copy_unit_to_temp), and ask every unit panel to redraw. An egg
 * hides the status banner; a unit staged while it is hidden brings it back nine frames later
 * (world_formation_step_unit_selection_slide). The shops stage their units the same way.
 *
 * The three display-data groups are copied separately before the full unit
 * snapshot. O1 preserves the target's load/store order and direct array
 * reference in the final branch; O2 emits 364 bytes instead of 388. */
void world_formation_stage_selected_unit(void) {
    /* Preserve the target's unused 128-byte stack area. */
    u8 unused[128];
    world_formation_unit_t** units;
    s32 index;

    g_world_formation_selected_unit_index_latch = g_world_formation_selected_unit_index;
    index = g_world_formation_selected_unit_index;
    units = g_world_formation_unit_pointers;
    units[index]->hp = units[index]->max_hp;
    units[index]->mp = units[index]->max_mp;
    bcopy(units[index], &g_world_selected_unit_stat_summary, 0x22);
    bcopy(&units[g_world_formation_selected_unit_index]->name_index, &g_world_selected_unit_identity, 0xe);
    bcopy(&units[g_world_formation_selected_unit_index]->move, &g_world_selected_unit_stat_detail, 0x40);
    world_formation_copy_unit_to_temp(g_world_formation_selected_unit_index, WORLD_FORMATION_PREVIEW_RECORD);
    if (g_world_status_display_thread_params.flags == MENU_STATUS_PANEL_HIDDEN) {
        g_world_formation_unit_slide_pending
            = ((units[g_world_formation_selected_unit_index]->gender_flags >> 2) ^ 1) & 1;
    } else {
        g_world_status_display_thread_params.flags
            = (g_world_formation_unit_pointers[g_world_formation_selected_unit_index]->gender_flags << 5) & 0x80;
    }
    world_menu_request_unit_panel_redraw();
}
