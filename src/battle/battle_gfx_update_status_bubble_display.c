#include "fft/battle.h"
#include "psx/types.h"

/* Draws the unit's status bubble when the unit is on screen and has one.
 *
 * Nothing is drawn unless the camera-relative position lies within x 0x61..0x19f
 * and y -0x1f..0x10f, the ordering-table depth within 1..0x17f,
 * sprite_graphic_trigger is set, the unit is not a mount carrying its rider and
 * status_bubble_active is set. */
void battle_gfx_update_status_bubble_display(battle_unit_misc_data_t* unit) {
    u16 screen_coords[2];
    u16 screen_x;
    s16 screen_y;
    s32 screen_x_in_range;

    /* The camera-relative position is read unsigned (lhu) here. */
    screen_x = unit->camera_relative_x;
    screen_coords[0] = screen_x;
    screen_x_in_range = screen_x >= 0x61 && screen_x <= 0x19f;
    screen_y = unit->camera_relative_y;
    screen_coords[1] = screen_y;
    if (!screen_x_in_range) {
        return;
    }
    if (screen_y < -0x1f || screen_y > 0x10f) {
        return;
    }
    if ((unit->otag_depth_index - 1) >= 0x17f) {
        return;
    }
    if (unit->sprite_graphic_trigger == 0) {
        return;
    }
    if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_MOUNT) {
        return;
    }
    if (unit->status_bubble_active == 0) {
        return;
    }
    battle_gfx_draw_status_bubble(unit, screen_coords);
}
