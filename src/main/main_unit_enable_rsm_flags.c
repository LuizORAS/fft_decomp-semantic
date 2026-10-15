#include "fft/main.h"
#include "psx/types.h"

/* Rebuild the unit's active R/S/M bits (the eleven bytes from reaction_abilities) from its four
 * innate abilities and its equipped reaction, support and movement abilities (main_unit_set_rsm_flag).
 * An innate reaction is made the unit's reaction_ability before the equipped one is read, so a unit
 * with one (every monster, Byblos, Steel Giant and several boss jobs) keeps the innate reaction. */
void main_unit_enable_rsm_flags(battle_stats_t* unit) {
    s32 i;
    battle_stats_t* walk;

    main_util_clear_byte_data(unit->reaction_abilities, 0xB);
    i = 0;
    /* Keep the base at the unit start and advance it by one ability width;
     * loading through the field preserves the target's +0x0a displacement. */
    walk = unit;
    do {
        main_unit_set_rsm_flag(unit, walk->innate_abilities[0]);
        walk = (battle_stats_t*)((u8*)walk + sizeof(u16));
        i += 1;
    } while (i < 4);
    main_unit_set_rsm_flag(unit, unit->reaction_ability);
    main_unit_set_rsm_flag(unit, unit->support_ability);
    main_unit_set_rsm_flag(unit, unit->movement_ability);
}
