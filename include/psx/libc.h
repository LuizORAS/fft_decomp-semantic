#ifndef PSX_LIBC_H
#define PSX_LIBC_H

#include "psx/types.h"

extern void srand(unsigned int);
extern int rand(void);
extern int sprintf(char* destination, const char* format, ...);
void printf(const char* fmt, ...);
extern char* strcat(char* destination, const char* source);
extern char* strcpy(char* destination, const char* source);
extern int strlen(const char* source);
extern void* memchr(const void* source, int value, unsigned int size);
extern void* memset(void* destination, int value, unsigned int size);
extern void* memcpy(void* destination, const void* source, unsigned int size);
s32 strcmp(const char* left, const char* right);
extern void bcopy(const void* source, void* destination, int size);
void* bzero(void* destination, s32 byte_length);
extern int abs(int value);

s32 puts(const char* text);

s32 setjmp(void* jump_buffer);

void* memmove(void* destination, const void* source, s32 count);

/* driver internals */

typedef enum {
    PSYQ_PRINTF_LEFT = 1,
    PSYQ_PRINTF_PLUS = 2,
    PSYQ_PRINTF_ALTERNATE = 4,
    PSYQ_PRINTF_ZERO = 8,
    PSYQ_PRINTF_PRECISION = 16,
    PSYQ_PRINTF_SHORT = 32,
    PSYQ_PRINTF_LONG = 64,
    PSYQ_PRINTF_CAPITAL_L = 128
} psyq_printf_flag_e;

/* The formatter changes flags as a word and the sign through byte one. */
typedef struct {
    union {
        u32 bits;
        struct {
            u8 flags;
            u8 sign;
            u8 _unknown_02[2];
        } bytes;
    } flags;
    s32 width;
    s32 precision;
} psyq_printf_format_t;

extern const psyq_printf_format_t g_psyq_libc_default_format;
extern const char g_psyq_libc_uppercase_digits[];
extern const char g_psyq_libc_lowercase_digits[];

#endif
