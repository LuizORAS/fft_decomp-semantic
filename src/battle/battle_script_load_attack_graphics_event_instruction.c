#include "fft/battle.h"
#include "fft/event_attack.h"
#include "psx/types.h"

/* LoadAttackGraphics (EVENT_OPCODE_LOAD_ATTACK_GRAPHICS), skipped during a replay
 * (EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS): open ATTACK.OUT (BATTLE_COMPANION_OVERLAY_ATTACK) in
 * its preloaded entry mode, then run its scenario conditionals, formation sprites and portraits
 * loads. */
void battle_script_load_attack_graphics_event_instruction(void) {
    if (battle_script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) == 0) {
        g_battle_script_attack_entry_mode = SCRIPT_ATTACK_ENTRY_PRELOADED;
        battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_ATTACK);
        battle_thread_yield();
        attack_load_scenario_conditionals();
        attack_gfx_build_formation_sprites();
        attack_gfx_load_portraits();
    }
}
