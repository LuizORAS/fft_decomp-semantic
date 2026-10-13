#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/wldcore.h"
#include "fft/world.h"
#include "psx/types.h"

/* TeleportOut event instruction: play the teleport effect and sound on the
 * unit, flash its palette to white and then to black over 60 frames each,
 * and with `remove_unit` remove the unit from existence. */
void world_script_teleportout_event_instruction(s32 unit_id, s32 remove_unit) {
    s32 misc_id;
    s32 unit_index;

    misc_id = script_get_misc_id(unit_id);
    if (misc_id != EVENT_RUNTIME_ID_NONE) {
        g_world_sound_pending_sfx_id = MAIN_SFX_TELEPORT_OUT;
        g_thread_call_target = effect_set_secondary_teleport_by_misc_id;
        thread_call_on_main_stack(misc_id);
        unit_index = wldcore_unit_get_id_from_misc_id(misc_id);
        world_unit_set_animation(misc_id, BATTLE_UNIT_ANIMATION_RAISE_HANDS);
        battle_gfx_init_misc_unit_palette_modulation(misc_id, 2, 0x1f, 0x1f, 0x1f);
        world_thread_wait_frames(0x3c);
        battle_gfx_init_misc_unit_palette_modulation(misc_id, 2, -0x1f, -0x1f, -0x1f);
        world_thread_wait_frames(0x3c);
        if (remove_unit != 0) {
            battle_unit_set_cannot_exist(unit_index);
        }
    }
}
