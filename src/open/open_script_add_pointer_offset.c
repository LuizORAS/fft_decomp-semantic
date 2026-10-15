#include "psx/types.h"

/* Return `left` + `right`: the dispatcher and the text opcode turn a byte offset into the script
 * data into a pointer with it, the address held in an s32. */
s32 open_script_add_pointer_offset(s32 left, s32 right) {
    return left + right;
}
