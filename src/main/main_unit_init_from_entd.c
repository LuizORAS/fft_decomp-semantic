#include "fft/main.h"
#include "psx/types.h"

/* Build a unit from its ENTD record: identity (main_unit_calculate_entd_data), jobs and skillsets,
 * abilities, R/S/M flags, raw stats rolled and grown to its level, equippable types, the record's
 * equipment for a unit outside the roster, equipment bonuses, Move, Jump, names, and Ramza's name and
 * birthday from the roster. A record that loads a party member is built from the roster by the
 * identity step, which ends it (-1 when the member is missing). use_world_text picks WORLD's text for
 * the names. transform_job JOB_ID_MORBOL is battle_unit_transform_into_morbol rebuilding a unit with
 * no ENTD record: the reset, the identity step and the ENTD equipment are skipped. Returns 0 on
 * success.
 *
 * A temporary formation index makes main_unit_calculate_actual_stats grow a generated unit's stats to
 * its level. */
s32 main_unit_init_from_entd(battle_stats_t* unit, entd_unit_t* entd, u32 use_world_text, s32 transform_job) {
    g_main_unit_name_uses_world_text = use_world_text;
    if (transform_job != JOB_ID_MORBOL) {
        main_unit_reset_battle_state(unit);
        if (main_unit_calculate_entd_data(unit, entd) != 0) {
            return -1;
        }
    }
    if (transform_job == 0) {
        if ((entd->unit_flags & UNIT_FLAG_LOAD_FORMATION) != 0) {
            return 0;
        }
        if (entd->sprite_set < CHARACTER_IDENTITY_RAMZA_END) {
            if (entd->birthday[0] == 0) {
                return 0;
            }
        }
    }
    main_unit_calculate_jobs_and_skillsets_from_entd(unit, entd);
    main_unit_calculate_abilities(unit, entd);
    main_unit_enable_rsm_flags(unit);
    main_unit_generate_raw_stats(unit);
    main_unit_copy_job_growths_and_multipliers(unit);
    if (unit->formation_index == BATTLE_FORMATION_INDEX_NONE) {
        unit->formation_index = BATTLE_FORMATION_INDEX_GENERATED_UNIT;
    }
    main_unit_calculate_actual_stats(unit, 0);
    if (unit->formation_index == BATTLE_FORMATION_INDEX_GENERATED_UNIT) {
        unit->formation_index = BATTLE_FORMATION_INDEX_NONE;
    }
    main_unit_set_equippable_items(unit);
    if (transform_job != JOB_ID_MORBOL) {
        if (unit->formation_index >= PARTY_ROSTER_SLOT_COUNT) {
            main_unit_calculate_entd_equipment(unit, entd);
        }
    }
    main_unit_apply_equipment_move_jump_and_name(unit);
    main_unit_store_ramza_name_birthday_zodiac(unit);
    return 0;
}
