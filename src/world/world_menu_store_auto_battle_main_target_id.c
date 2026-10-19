#include "fft/unit.h"
#include "fft/world.h"

/* WORLD twin of battle_menu_store_auto_battle_main_target_id. */
void world_menu_store_auto_battle_main_target_id(u8 target, s32 unit_id) {
    battle_stats_t* unit = battle_unit_get_stats_from_battle_id(unit_id);

    unit->auto_battle_target = target;
    unit->auto_battle_setting = g_world_menu_resolved_command;
}
