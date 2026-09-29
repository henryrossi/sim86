#ifndef DECODE_H
#define DECODE_H

#include "sim86.h"

enum instruction_bits_type {
        bits_literal,
        bits_mod,
        bits_reg,
        bits_rm,
        bits_disp,
        bits_data,

        bits_d,
        bits_s,
        bits_w,

        bits_rel_jmp_disp,

        bits_count,
};

typedef struct instruction_bits {
        enum instruction_bits_type type;
        uint8_t bit_count;
        uint8_t bits;
} instruction_bits;

typedef struct instruction_format {
        enum operation_type op;
        instruction_bits bits[16];
} instruction_format;

instruction decode(memory *mem, uint32_t ip);

#endif // DECODE_H
