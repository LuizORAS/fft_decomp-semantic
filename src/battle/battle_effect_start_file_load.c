#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "fft/main.h"
#include "psx/types.h"

/* Start loading the effect file of g_battle_ability_effect_id; an effect whose
 * file has no size is replaced by the Cure effect. Nothing in the game calls it:
 * battle_effect_update_file_load does the same in its first state. */
void battle_effect_start_file_load(void) {
    s32 lba;
    s32 byte_length;

    battle_effect_get_file_extent(g_battle_ability_effect_id, &lba, &byte_length);
    if (byte_length == 0) {
        g_battle_ability_effect_id = BATTLE_EFFECT_FILE_CURE;
        battle_effect_get_file_extent(BATTLE_EFFECT_FILE_CURE, &lba, &byte_length);
    }
    main_file_request_read_bytes(lba, byte_length, 0);
}
