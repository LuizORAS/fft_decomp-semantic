#include "fft/world.h"

/* Why a formation unit cannot use a menu, as a WORLD_FORMATION_UNIT_LOCK_*
 * value: away on a proposition, a monster, an egg or a Mime, tested in that
 * order; WORLD_FORMATION_UNIT_UNLOCKED otherwise. */
s32 world_formation_get_unit_lock(s32 unit_id) {
    world_formation_unit_t* unit;
    world_formation_unit_t* current_unit;

    unit = g_world_formation_unit_pointers[unit_id];
    if (unit->proposition_status != 0)
        return WORLD_FORMATION_UNIT_LOCK_PROPOSITION;
    if (unit->uses_monster_skillset != 0)
        return WORLD_FORMATION_UNIT_LOCK_MONSTER;
    if (world_job_is_special_monster(unit->identity.job_id) != 0)
        return WORLD_FORMATION_UNIT_LOCK_MONSTER;
    current_unit = g_world_formation_unit_pointers[unit_id];
    if (current_unit->gender_flags & UNIT_FLAG_EGG)
        return WORLD_FORMATION_UNIT_LOCK_EGG;
    if (current_unit->identity.job_id == JOB_ID_MIME)
        return WORLD_FORMATION_UNIT_LOCK_MIME;
    return WORLD_FORMATION_UNIT_UNLOCKED;
}
