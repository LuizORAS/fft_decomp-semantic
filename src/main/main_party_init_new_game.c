#include "fft/main.h"
#include "fft/world.h"

enum {
    NEW_GAME_GENERATE_DEFAULT_UNIT = 0,
    NEW_GAME_LOAD_EVENT_PARTY = 2,
    STARTING_EVENT_ID = 254,
    STARTING_WAR_FUNDS = 2000,
};

/* Start a new roster: empty it, then generate Ramza alone (mode 0) or load the
 * party of event 254 from its ENTD (mode 2); any other mode leaves it empty. The
 * inventory is reset to the new-game stock and the war funds to 2000. */
void main_party_init_new_game(int mode) {
    world_menu_load_text_1bd8();
    main_party_clear_all();

    if (mode == NEW_GAME_GENERATE_DEFAULT_UNIT) {
        main_party_generate_unit(MAIN_UNIT_TYPE_RAMZA);
    } else if (mode == NEW_GAME_LOAD_EVENT_PARTY) {
        main_entd_init_event_unit_data(STARTING_EVENT_ID);
    }

    main_item_init_new_game_inventory();
    world_script_set_variable(EVENT_SCRIPT_VAR_WAR_FUNDS, STARTING_WAR_FUNDS);
}
