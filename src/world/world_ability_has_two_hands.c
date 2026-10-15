#include "fft/world.h"

/* Return nonzero when formation unit unit_id has Two Hands active; the best-fit equipment
 * (world_unit_build_best_fit_equipment and the shop's fitting-room copy) then picks a weapon that two
 * hands can hold and leaves the other hand empty, even with Two Swords. */
s32 world_ability_has_two_hands(s16 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->support_abilities[2] & BATTLE_SUPPORT_SET_3_TWO_HANDS;
}
