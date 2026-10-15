#include "fft/battle.h"

/* Show the level-up effect on a unit: battle_action_report_level_up and
 * battle_action_report_job_level_up start it with their tunes. */
void battle_effect_set_secondary_level_up(battle_unit_misc_data_t* unit) {
    battle_effect_set_secondary(unit, BATTLE_EFFECT_SECONDARY_ANIMATION_LEVEL_UP);
}
