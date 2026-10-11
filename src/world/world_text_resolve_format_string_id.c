#include "fft/world.h"
#include "psx/types.h"

/* Convert a text-format substitution opcode and its value into the id of the
 * string to insert. */
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
