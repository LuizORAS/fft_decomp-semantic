#include "fft/main.h"
#include "fft/memory.h"
#include "psx/types.h"

/* Rebuild the unit's active R/S/M bits (the eleven bytes from reaction_abilities) from its four
 * innate abilities and its equipped reaction, support and movement abilities (main_unit_set_rsm_flag).
 * An innate reaction is made the unit's reaction_ability before the equipped one is read, so a unit
 * with one (every monster, Byblos, Steel Giant and several boss jobs) keeps the innate reaction. */
void main_unit_enable_rsm_flags(battle_stats_t* unit) {
    s32 i;

    main_mem_zero(&unit->ability_flags, sizeof(unit->ability_flags));
    for (i = 0; i < 4; i++) {
        main_unit_set_rsm_flag(unit, unit->innate_abilities[i]);
    }
    main_unit_set_rsm_flag(unit, unit->abilities.reaction);
    main_unit_set_rsm_flag(unit, unit->abilities.support);
    main_unit_set_rsm_flag(unit, unit->abilities.movement);
}
