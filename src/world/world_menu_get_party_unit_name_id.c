#include "fft/world.h"

/* Returns the selected party unit's name id from party_unit_t.name_id: its name
 * class in the high byte and its index in the low byte. */
s32 world_menu_get_party_unit_name_id(void) {
    const unit_name_id_t* name_id = &main_party_get_unit(g_world_selected_unit_identity.roster_slot)->name_id;

    return (name_id->fields.name_class << 8) | name_id->fields.index;
}
