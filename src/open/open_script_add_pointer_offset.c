#include "fft/open.h"

/* Return `base` + `byte_offset`: the dispatcher and the text opcode turn a byte offset into the script
 * data into a pointer with it, the address held in an s32. */
u8* open_script_add_pointer_offset(u8* base, s32 byte_offset) {
    return base + byte_offset;
}
