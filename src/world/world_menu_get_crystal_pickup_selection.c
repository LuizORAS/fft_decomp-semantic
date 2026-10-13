#include "fft/battle.h"
#include "fft/world.h"

extern main_crystal_pickup_result_t* battle_unit_generate_crystal_or_treasure(battle_stats_t* mover);

/* WORLD twin of battle_menu_get_crystal_pickup_selection. */
s32 world_menu_get_crystal_pickup_selection(s32 battle_unit_index) {
    battle_stats_t* unit = battle_unit_get_stats_from_battle_id(battle_unit_index);

    g_world_thread_call_target = (void (*)(void))battle_unit_generate_crystal_or_treasure;
    g_world_crystal_pickup_context = (main_crystal_pickup_result_t*)world_thread_call_on_main_stack(unit);
    if (g_world_crystal_pickup_context->result == -1 || g_world_crystal_pickup_context->result == 0) {
        main_system_report_error_2(0x11, 7);
    }
    if (g_world_crystal_pickup_context->result == 4) {
        return g_world_crystal_pickup_context->treasure_item;
    }
    return -1;
}
