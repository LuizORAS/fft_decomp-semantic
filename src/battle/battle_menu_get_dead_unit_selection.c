#include "fft/battle.h"
#include "psx/types.h"

/* Run battle_unit_generate_crystal_or_treasure for battle unit battle_id on the main stack and
 * publish the record as g_dead_unit_context, reporting error (0x11, 7) when it is -1 or 0. Returns the
 * chest's item for a chest, else -1. The WORLD twin is world_menu_get_dead_unit_selection. */
s32 battle_menu_get_dead_unit_selection(s32 battle_id) {
    battle_stats_t* stats;
    main_crystal_pickup_result_t* result;

    stats = battle_unit_get_stats_from_battle_id(battle_id);
    g_battle_thread_call_target = (void (*)(void))battle_unit_generate_crystal_or_treasure;
    result = (main_crystal_pickup_result_t*)battle_thread_call_on_main_stack(stats);
    g_dead_unit_context = result;
    if ((u32)(result->result + 1) < 2) {
        main_system_report_error_2(0x11, 7);
    }
    /* Reload the published context after the call; reusing result changes the frame and registers. */
    if (g_dead_unit_context->result != 4) {
        return -1;
    }
    return g_dead_unit_context->treasure_item;
}
