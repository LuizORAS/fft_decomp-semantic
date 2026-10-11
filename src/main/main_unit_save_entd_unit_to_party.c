#include "fft/battle.h"
#include "fft/main.h"

/* Build the unit in slot unit_id of an ENTD encounter (main_unit_init_from_entd, in a stack record)
 * and save it into the party roster; main_entd_init_event_unit_data fills the roster this way.
 * Returns -2 for an empty slot, else main_party_save_unit's result (0, or -1 when the roster is
 * full). */
s32 main_unit_save_entd_unit_to_party(entd_encounter_t* entd, s32 unit_id, s32 allow_guest, s32 use_world_text) {
    entd_unit_t* entd_unit = &entd->units[unit_id];

    g_main_entd_current_unit = entd_unit;
    if (entd_unit->sprite_set == CHARACTER_IDENTITY_ENTD_NONE) {
        return -2;
    }
    {
        battle_stats_t unit;
        battle_stats_t* unit_data = &unit;

        unit_data->misc_unit_id = 0xff;
        unit_data->entd_slot = 0x20;
        unit_data->existence = BATTLE_UNIT_EXISTENCE_ACTIVE;
        unit_data->formation_index = BATTLE_FORMATION_INDEX_NONE;
        main_unit_init_from_entd(unit_data, entd_unit, use_world_text, 0);
        return main_party_save_unit(unit_data, allow_guest);
    }
}
