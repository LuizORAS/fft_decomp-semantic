#include "fft/battle.h"
#include "fft/world.h"

battle_stats_t* world_unit_get_by_unit_id(s32 unit_id) {
    s32 index;
    battle_stats_t* stats;

    find_unit_by_id(unit_id, &index);
    if (index < 0) {
        if (unit_id < CHARACTER_IDENTITY_RAMZA_CHAPTER_1 || unit_id >= CHARACTER_IDENTITY_GENERIC_FIRST) {
            return (battle_stats_t*)-1;
        }
        index = battle_unit_find_slot_by_character_identity(unit_id);
        if (index >= 0) {
            stats = battle_unit_get_stats_from_battle_id(index);
            if (stats->unit_id >= EVENT_UNIT_ID_DEPLOYED_FIRST && stats->unit_id < EVENT_UNIT_ID_DEPLOYED_END) {
                return stats;
            }
        }
        return (battle_stats_t*)-1;
    }
    if (battle_unit_get_misc_id_by_battle_id(index) >= 0) {
        return battle_unit_get_stats_from_battle_id(index);
    }
    return (battle_stats_t*)-1;
}
