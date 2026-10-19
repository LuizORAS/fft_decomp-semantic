#include "fft/event_attack.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"

/* LoadAttackGraphics (EVENT_OPCODE_LOAD_ATTACK_GRAPHICS), skipped during a replay
 * (EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS): open ATTACK.OUT (BATTLE_COMPANION_OVERLAY_ATTACK) in
 * its preloaded entry mode, then run its scenario conditionals, formation sprites and portraits
 * loads. */
void script_load_attack_graphics_event_instruction(void) {
    if (script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) == 0) {
        g_script_attack_entry_mode = SCRIPT_ATTACK_ENTRY_PRELOADED;
        menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_ATTACK);
        thread_yield();
        attack_scenario_load_conditionals();
        attack_gfx_build_formation_sprites();
        attack_gfx_load_portraits();
    }
}
