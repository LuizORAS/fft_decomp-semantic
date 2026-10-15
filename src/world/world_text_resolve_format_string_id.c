#include "fft/world.h"
#include "psx/types.h"

/* Same as battle_text_resolve_format_string_id: the text id substitution code `event_code` stands for
 * with `value`, or -1 for a code that names no text. */
s32 world_text_resolve_format_string_id(s32 event_code, s32 value) {
    if (event_code == TEXT_FORMAT_RAMZA_NAME) {
        return TEXT_ID_UNIT_NAME_SPECIAL_BASE;
    }
    if (event_code == TEXT_FORMAT_UNIT_NAME) {
        return value + TEXT_ID_UNIT_NAME_SPECIAL_BASE;
    }
    if (event_code == TEXT_FORMAT_LOCATION_NAME) {
        return value + TEXT_ID_LOCATION_NAME_BASE;
    }
    if (event_code == TEXT_FORMAT_ITEM_NAME) {
        return value + TEXT_ID_ITEM_NAME_BASE;
    }
    if (event_code == TEXT_FORMAT_ABILITY_NAME) {
        return value + TEXT_ID_ABILITY_NAME_BASE;
    }
    if (event_code == TEXT_FORMAT_EXPLICIT_ID) {
        return value;
    }
    return -1;
}
