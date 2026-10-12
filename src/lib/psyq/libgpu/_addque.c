/* LIBGPU 80026454-80026478. */
#include "psx/libgpu.h"
int _addque(psyq_gpu_operation_t operation, void* source, u32 argument) {
    return _addque2(operation, source, 0, argument);
}
