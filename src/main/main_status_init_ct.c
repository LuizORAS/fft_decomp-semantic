#include "fft/main.h"
#include "fft/memory.h"

/* Clear the unit's status counts and set its death counter: 3, or 0xff (never runs out) for an
 * immortal unit or a formation unit. */
void main_status_init_ct(battle_stats_t* unit) {
    main_mem_zero(unit->status_ct, sizeof(unit->status_ct));
    if ((unit->team_flags & BATTLE_TEAM_FLAG_IMMORTAL)
        || (unit->unit_flags & (UNIT_FLAG_SAVE_FORMATION | UNIT_FLAG_LOAD_FORMATION))) {
        unit->death_counter = 0xff;
    } else {
        unit->death_counter = 3;
    }
}
