#include "fft/battle.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Declared as in the BATTLE callers. */
/* Run battle_unit_generate_crystal_or_treasure for the unit on the main thread
 * and return the treasure item when the result is 4, else -1. */
s32 world_menu_get_dead_unit_selection(s32 battle_unit_index) {
    battle_stats_t* unit = battle_unit_get_stats_from_battle_id(battle_unit_index);

    g_thread_call_target = (void (*)(void))battle_unit_generate_crystal_or_treasure;
    g_world_thread_inner_subroutine_result = (crystal_pickup_result_t*)thread_call_on_main_stack(unit);
    if (g_world_thread_inner_subroutine_result->result == -1 || g_world_thread_inner_subroutine_result->result == 0) {
        main_system_report_stub(0x11, 7);
    }
    if (g_world_thread_inner_subroutine_result->result == 4) {
        return g_world_thread_inner_subroutine_result->treasure_item;
    }
    return -1;
}
