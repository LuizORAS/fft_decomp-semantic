#include "fft/battle.h"
#include "psx/types.h"

/* Start the target's reaction to the attacker's action and its post-action
 * display. Both parameters are misc records; byte 4 is the unit id. */
void battle_unit_start_post_attack_animation_display(s32 attacker, u8* target) {
    if (attacker != 0 && target != 0) {
        battle_unit_set_target_animation_from_attack_type(
            (battle_unit_misc_data_t*)attacker, (battle_unit_misc_data_t*)target);
        battle_gfx_prepare_post_action_display_by_misc_id(target[4]);
    }
}
