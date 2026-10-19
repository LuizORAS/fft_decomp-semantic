#include "fft/main.h"
#include "psx/types.h"

/* Build the party member an ENTD record names by its sprite set (main_party_find_slot_by_sprite_set,
 * main_unit_init_from_party_slot), then take the record's team and its join-after-event and
 * save-formation flags. Returns -1 when no party member has that sprite set, else 0. */
s32 main_unit_init_from_entd_party_member(battle_stats_t* unit, entd_unit_t* entd_unit) {
    s32 slot;

    slot = main_party_find_slot_by_sprite_set(entd_unit->sprite_set);
    if (slot != -1) {
        main_unit_init_from_party_slot(unit, slot, g_main_unit_name_uses_world_text);
        /* Keep the party unit's kind flags and take the two persistence
         * controls from the ENTD definition. */
        unit->unit_flags &= ~(UNIT_FLAG_JOIN_AFTER_EVENT | UNIT_FLAG_SAVE_FORMATION);
        unit->unit_flags |= entd_unit->unit_flags & (UNIT_FLAG_JOIN_AFTER_EVENT | UNIT_FLAG_SAVE_FORMATION);
        unit->team_flags = entd_unit->team_flags;
        unit->initial_team_flags = entd_unit->team_flags;
        return 0;
    }
    return -1;
}
