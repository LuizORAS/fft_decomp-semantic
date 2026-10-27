#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "fft/main.h"
#include "psx/types.h"

/*
 * Effect-file load state machine: state 0 starts loading the ability's effect
 * file (falling back to effect 1 when its size is zero) at the effect window's
 * start in the companion window, state 1 polls the load, state 2 waits out the
 * countdown (60 frames when a secondary effect is active), finalizes the
 * secondary effect and fades the music when the effect header flag 0x10 is
 * set, and state 3 waits for the secondary effect phase to end. Returns
 * nonzero while work remains. A load state outside 0-3 leaves `result` unset:
 * the test then reads whatever the caller left in $a0.
 */
s32 battle_effect_update_file_load(void) {
    s32 result;
    s32 lba;
    s32 byte_length;
    u8* destination;

    destination = g_main_heap_companion_overlay_load_address + RAM_WINDOW_START_EFFECT_FROM_COMPANION;
    if (g_battle_ability_effect_id >= 0) {
        switch (g_battle_effect_load_state) {
        case BATTLE_EFFECT_LOAD_START:
            if (g_battle_effect_charge_secondary_id != 0) {
                g_battle_effect_load_countdown = 60;
            } else {
                g_battle_effect_load_countdown = 0;
            }
            battle_effect_get_file_extent(g_battle_ability_effect_id, &lba, &byte_length);
            if (byte_length == 0) {
                g_battle_ability_effect_id = BATTLE_EFFECT_FILE_CURE;
                battle_effect_get_file_extent(BATTLE_EFFECT_FILE_CURE, &lba, &byte_length);
            }
            result = main_file_request_read_bytes(lba, byte_length, destination);
            if (result == 0) {
                g_battle_effect_load_state = BATTLE_EFFECT_LOAD_READING;
                result = 1;
            }
            break;
        case BATTLE_EFFECT_LOAD_READING:
            result = main_file_is_still_loading();
            if (result == 0) {
                g_battle_effect_load_state = BATTLE_EFFECT_LOAD_COUNTDOWN;
                result = 1;
            }
            break;
        case BATTLE_EFFECT_LOAD_COUNTDOWN:
            if (g_battle_effect_load_countdown != 0) {
                result = 1;
                break;
            }
            if (g_battle_effect_charge_secondary_id != 0) {
                battle_effect_finalize_secondary(g_battle_effect_charge_secondary_id);
            }
            g_battle_effect_load_state = BATTLE_EFFECT_LOAD_ENDING_SECONDARY;
            battle_effect_set_flags_section_pointer(g_battle_ability_effect_id);
            /* An `lw` of the section's leading 32-bit value, not a byte test. */
            if (g_battle_effect_flags_section->flags & EFFECT_FLAG_QUIET_MUSIC) {
                main_sound_set_current_music_volume(0x40, 0xF0);
            }
            result = 1;
            break;
        case BATTLE_EFFECT_LOAD_ENDING_SECONDARY:
            result = battle_effect_get_secondary_phase(g_battle_effect_charge_secondary_id)
                != BATTLE_EFFECT_SECONDARY_PHASE_ENDED;
            break;
        }
        if (g_battle_effect_load_countdown > 0) {
            g_battle_effect_load_countdown--;
        }
        return result != 0;
    }
    return 0;
}
