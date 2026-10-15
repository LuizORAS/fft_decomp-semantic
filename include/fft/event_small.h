#ifndef EVENT_SMALL_H
#define EVENT_SMALL_H

/* EVENT/SMALL.OUT: the unit and job names of a battle. BATTLE loads it while the battle map starts
 * (battle_map_step_init_sequence, battle_menu_poll_small_load) and binds its text sections. */

#include "fft/battle.h"

/* text */
/* The name data and the routine that binds it (roster unit names, job names) over the battle text sections. */

extern u8 g_small_text_name_data[];
void small_text_init_battle_pointers(void);

#endif
