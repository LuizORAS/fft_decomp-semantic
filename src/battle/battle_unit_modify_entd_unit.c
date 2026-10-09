#include "fft/battle.h"

/* Transform a unit into a Morbol, keeping its name, position and facing. */
void battle_unit_modify_entd_unit(battle_stats_t* unit) {
    u8 saved_name[0x10];
    u8 palette[8];
    u8* name;
    u8 unit_id;
    u8 formation_index;
    u8 x;
    u8 y;
    u8 higher_elevation;
    u8 facing;

    /* Preserve the unit's encoded name across the reset. */
    name = unit->name;
    unit_id = unit->unit_id;
    unit->job_id = JOB_ID_MORBOL;
    unit->character_identity = CHARACTER_IDENTITY_MONSTER;
    main_util_copy_byte_data(name, saved_name, 0x10);
    formation_index = unit->formation_index;
    x = unit->x;
    y = unit->position.bits.y;
    higher_elevation = unit->position.bits.higher_elevation;
    facing = unit->position.bits.facing;
    unit->formation_index = 0xFE;
    main_unit_init_for_battle(unit, 0, 0, 0x82);
    unit->x = x;
    unit->position.bits.y = y;
    unit->formation_index = formation_index;
    unit->position.bits.higher_elevation = higher_elevation;
    unit->position.bits.facing = facing;
    main_util_copy_byte_data(saved_name, name, 0x10);
    unit->unit_id = unit_id;
    main_unit_calculate_palette_spritesheet(unit, palette);
}
