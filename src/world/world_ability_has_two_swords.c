#include "fft/world.h"

/* Return nonzero when formation unit unit_id has Two Swords active; the slot check, the loadout
 * cleanup after an ability change and the best-fit equipment then allow a second weapon. */
s32 world_ability_has_two_swords(s16 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->support_abilities[2] & BATTLE_SUPPORT_SET_3_TWO_SWORDS;
}
