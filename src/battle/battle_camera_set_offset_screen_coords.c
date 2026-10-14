#include "fft/battle.h"

/* Set the screen offset (g_battle_offset_screen_coords). No retail code calls it. */
void battle_camera_set_offset_screen_coords(VECTOR* coords) {
    g_battle_offset_screen_coords.vx = coords->vx;
    g_battle_offset_screen_coords.vy = coords->vy;
    g_battle_offset_screen_coords.vz = coords->vz;
}
