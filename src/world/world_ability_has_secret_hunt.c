#include "fft/world.h"

/* Return nonzero when formation unit unit_id has Secret Hunt active; the fur shop
 * (world_shop_run_fur_menu_step) opens only when a unit does. */
s32 world_ability_has_secret_hunt(s16 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->support_abilities[2] & BATTLE_SUPPORT_SET_3_SECRET_HUNT;
}
