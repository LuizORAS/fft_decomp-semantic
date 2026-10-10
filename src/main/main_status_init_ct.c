#include "fft/main.h"
#include "psx/types.h"

/* Clear the timed-status CTs and start the death counter at 3, or at 0xff for
 * an immortal unit and for one with a save- or load-formation flag. */
void main_status_init_ct(battle_stats_t* unit) {
    main_util_clear_byte_data(unit->status_ct, BATTLE_TIMED_STATUS_COUNT);
    if ((unit->team_flags & BATTLE_TEAM_FLAG_IMMORTAL)
        || (unit->unit_flags & (UNIT_FLAG_SAVE_FORMATION | UNIT_FLAG_LOAD_FORMATION))) {
        unit->death_counter = 0xff;
    } else {
        unit->death_counter = 3;
    }
}
