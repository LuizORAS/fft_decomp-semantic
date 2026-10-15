#include "psx/libgte.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_lerp_fixed12. */
s32 world_menu_lerp_fixed12(s32 from, s32 to, s32 t) {
    return ((to - from) * t) / ONE + from;
}
