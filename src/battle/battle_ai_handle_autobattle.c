#include "fft/battle.h"

/* Run the acting unit's Auto-Battle mode: Fight for life (battle_ai_select_fight_for_life_action),
 * Protect allies, Save fading life, which turns into Protect allies when an ally is in peril, or Run
 * like a rabbit (battle_ai_select_retreat_action). Returns 1 when a mode chose, 0 for Manual, -1 to
 * suspend. Protect allies would fall back to Run like a rabbit on -2, which it never returns
 * (QUIRKS.md). */
s32 battle_ai_handle_autobattle(void) {
    s32 result;
    /* Pin: unpinned, GCC routes the result through $a0 and loses the $v0 delay-slot fills (4 bytes larger). */
    register s32 direct_result __asm__("$2");

    /* A -2 result falls back to another policy's case; those jumps stay gotos. */
    switch (g_battle_ai_data_base.autobattle_setting) {
    case BATTLE_AUTO_BATTLE_FIGHT_FOR_LIFE:
        direct_result = battle_ai_select_fight_for_life_action();
        if (direct_result != -1)
            return 1;
        return -1;
    case BATTLE_AUTO_BATTLE_PROTECT_ALLIES:
    protect_allies:
        result = battle_ai_select_protect_allies_action();
        if (result == -1) {
            return -1;
        }
        if (result == -2) {
            goto run_like_a_rabbit;
        }
        return 1;
    case BATTLE_AUTO_BATTLE_SAVE_FADING_LIFE:
        result = battle_ai_select_save_fading_life_action();
        if (result == -1) {
            return -1;
        }
        if (result == -2) {
            goto protect_allies;
        }
        return 1;
    case BATTLE_AUTO_BATTLE_RUN_LIKE_A_RABBIT:
    run_like_a_rabbit:
        direct_result = battle_ai_select_retreat_action();
        if (direct_result != -1)
            return 1;
        return -1;
    default:
        return 0;
    }
}
