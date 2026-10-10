#include "fft/battle.h"
#include "psx/types.h"

/* Start loading the effect file of g_battle_ability_effect_id; an effect whose
 * file has no size is replaced by the Cure effect. Nothing in the game calls it:
 * battle_effect_init_data does the same in its first state. */
void battle_effect_load_data(void) {
    s32 lba;
    s32 byte_length;

    battle_effect_load_lba_and_size(g_battle_ability_effect_id, &lba, &byte_length);
    if (byte_length == 0) {
        g_battle_ability_effect_id = BATTLE_EFFECT_FILE_CURE;
        battle_effect_load_lba_and_size(BATTLE_EFFECT_FILE_CURE, &lba, &byte_length);
    }
    main_file_call_build_header(lba, byte_length, 0);
}
