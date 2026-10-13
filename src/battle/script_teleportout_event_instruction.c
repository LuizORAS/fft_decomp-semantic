#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/battle_script.h"
#include "fft/battle_sound.h"
#include "fft/battle_thread.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* TeleportOut: play the teleport sound and secondary effect on the unit, raise
 * its hands, modulate its palette by +0x1f per channel, wait 60 frames, modulate
 * it by -0x1f and wait 60 frames more. Then, when `remove` is set, mark its
 * battle unit as not existing. A unit id that does not resolve does nothing. The
 * interpreter always passes remove = 1. */
void script_teleportout_event_instruction(s32 unit_id, s32 remove) {
    s32 misc;
    s32 idx;

    misc = script_get_misc_id(unit_id);
    if (misc == EVENT_RUNTIME_ID_NONE) {
        return;
    }
    g_sound_pending_sfx_id = MAIN_SFX_TELEPORT_OUT;
    g_thread_call_target = (void (*)(void))effect_set_secondary_teleport_by_misc_id;
    thread_call_on_main_stack(misc);
    idx = battle_unit_get_battle_index_by_misc_id(misc);
    battle_unit_set_animation(misc, BATTLE_UNIT_ANIMATION_RAISE_HANDS);
    battle_gfx_init_misc_unit_palette_modulation(misc, 2, 0x1F, 0x1F, 0x1F);
    battle_thread_wait_frames(0x3C);
    battle_gfx_init_misc_unit_palette_modulation(misc, 2, -0x1F, -0x1F, -0x1F);
    battle_thread_wait_frames(0x3C);
    if (remove != 0) {
        battle_unit_set_cannot_exist(idx);
    }
}
