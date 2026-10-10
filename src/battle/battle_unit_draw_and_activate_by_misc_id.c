#include "fft/battle.h"
#include "psx/types.h"

/* Makes a unit visible: enables its drawing, triggers its sprite graphic and,
 * when it has battle stats, runs battle_unit_set_enemy_level_data_by_battle_id.
 * Returns 0 when no record has the misc id. */
s32 battle_unit_draw_and_activate_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit;
    battle_stats_t* battle_data;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        battle_data = unit->battle_data;
        unit->draw_enabled = 1;
        unit->sprite_graphic_trigger = 1;
        if (battle_data != 0) {
            battle_unit_set_enemy_level_data_by_battle_id(battle_data->misc_unit_id);
        }
        return 1;
    }
    return 0;
}
