#include "fft/battle_action.h"
#include "fft/memory.h"

/* Reset an action result record for a new strike: zero the damage, healing, gil, special effect,
 * stat changes, status lists and EXP/JP changes, then set hit 1, accuracy 100 and attack type 0.
 * reaction_id and last_received_attack are kept. */
void battle_action_clear_current_data(battle_action_data_t* action) {
    main_mem_zero(action, 0xE);
    main_mem_zero(&action->special_effect, 0x16);
    main_mem_zero(&action->exp_change, 2);
    /* The target stores the accuracy as a 16-bit value (0x2a..0x2b). */
    action->attack_accuracy = 100;
    action->attack_type = 0;
    action->hit = 1;
    battle_action_clear_status_changes(action);
}
