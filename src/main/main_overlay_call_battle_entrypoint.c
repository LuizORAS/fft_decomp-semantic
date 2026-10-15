#include "fft/battle.h"
#include "fft/main.h"

/* Call BATTLE.BIN's entry point, main_overlay_battle_entry_stub: MAIN's 4 KB of nops at 0x80010ba8, which
 * return at once (QUIRKS.md). main_system_run_game_loop runs battle_state_run_game_loop after it. */
void main_overlay_call_battle_entrypoint(void) {
    main_overlay_battle_entry_stub();
}
