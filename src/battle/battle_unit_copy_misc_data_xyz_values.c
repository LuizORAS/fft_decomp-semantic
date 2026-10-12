#include "fft/battle_unit.h"

/* The unit's drawn position: its screen position plus the event and SEQ offsets,
 * with the Float bob added to the height. */
void battle_unit_copy_misc_data_xyz_values(SVECTOR* destination, const battle_unit_misc_data_t* misc_data) {
    /* float_bob_offset is read signed (lh) here. */
    main_util_set_svector(destination, misc_data->screen.vx + misc_data->event_offset.vx + misc_data->seq_offset.vx,
        misc_data->screen.vy + misc_data->event_offset.vy + misc_data->seq_offset.vy
            + *(s16*)&misc_data->float_bob_offset,
        misc_data->screen.vz + misc_data->event_offset.vz + misc_data->seq_offset.vz);
}
