#include "fft/open.h"

/* Runtime flag bits: 0x80 world overlay loaded, 0x100 party chosen,
 * 0x4000 << party_mode selects the party (three bits, 0x1C000). */
#define OPEN_FLAG_WORLD_OVERLAY_LOADED 0x80
#define OPEN_FLAG_PARTY_CHOSEN         0x100
#define OPEN_PARTY_MODE_FLAG_BASE      0x4000
#define OPEN_PARTY_MODE_FLAG_MASK      0x1C000

/* Load WORLD if needed, then initialize the selected new-game party. */
void open_title_init_new_game_party(s32 party_mode, s32 world_load_mode) {
    u32 flags;
    u32 party_mode_flag;

    if ((g_open_system_runtime_flags & OPEN_FLAG_WORLD_OVERLAY_LOADED) == 0) {
        g_open_system_runtime_flags |= OPEN_FLAG_WORLD_OVERLAY_LOADED;
        main_overlay_open_world_bin(world_load_mode);
    }

    party_mode_flag = OPEN_PARTY_MODE_FLAG_BASE << party_mode;
    flags = g_open_system_runtime_flags;
    if ((flags & OPEN_FLAG_PARTY_CHOSEN) == 0 || (flags & party_mode_flag) == 0) {
        g_open_system_runtime_flags = ((flags | OPEN_FLAG_PARTY_CHOSEN) & ~OPEN_PARTY_MODE_FLAG_MASK) | party_mode_flag;
        main_party_init_new_game(party_mode);
        world_text_generate_formation_unit_name_string();
    }
}
