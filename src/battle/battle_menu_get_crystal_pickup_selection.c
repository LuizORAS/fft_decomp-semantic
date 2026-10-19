#include "fft/battle.h"
#include "fft/battle_menu.h"
#include "fft/battle_unit.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Run battle_unit_generate_crystal_or_treasure for battle unit battle_id on the main stack and keep
 * its record in g_battle_crystal_pickup_context, reporting error (0x11, 7) when the result is -1 or 0.
 * Returns the chest's item when the result is 4 (a treasure chest), else -1. The WORLD twin is
 * world_menu_get_crystal_pickup_selection. */
s32 battle_menu_get_crystal_pickup_selection(s32 battle_id) {
    battle_stats_t* stats;
    main_crystal_pickup_result_t* result;

    stats = battle_unit_get_stats_from_battle_id(battle_id);
    g_thread_call_target = (void (*)(void))battle_unit_generate_crystal_or_treasure;
    result = (main_crystal_pickup_result_t*)thread_call_on_main_stack(stats);
    g_battle_crystal_pickup_context = result;
    if (result->result == -1 || result->result == 0) {
        main_system_report_error_2(0x11, 7);
    }
    /* Reload the published context after the call; reusing result changes the frame and registers. */
    if (g_battle_crystal_pickup_context->result != 4) {
        return -1;
    }
    return g_battle_crystal_pickup_context->treasure_item;
}
