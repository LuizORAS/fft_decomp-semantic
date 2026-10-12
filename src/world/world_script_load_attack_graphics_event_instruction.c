#include "fft/event_attack.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_load_attack_graphics_event_instruction (LoadAttackGraphics): unless
 * a replay suppresses it (EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS), open ATTACK.OUT and run its
 * scenario conditionals, formation sprites and portraits loads. */
void world_script_load_attack_graphics_event_instruction(void) {
    if (world_script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) == 0) {
        g_world_script_attack_entry_mode = SCRIPT_ATTACK_ENTRY_PRELOADED;
        world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_ATTACK);
        world_thread_yield();
        attack_load_scenario_conditionals();
        attack_gfx_build_formation_sprites();
        attack_gfx_load_portraits();
    }
}
