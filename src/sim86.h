#ifndef SIM86_H
#define SIM86_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum operation_type {
        op_none,
        op_mov,
        op_add,
        op_sub,
        op_cmp,
        op_je,
        op_jl,
        op_jle,
        op_jb,
        op_jbe,
        op_jp,
        op_jo,
        op_js,
        op_jne,
        op_jnl,
        op_jg,
        op_jnb,
        op_ja,
        op_jnp,
        op_jno,
        op_jns,
        op_loop,
        op_loopz,
        op_loopnz,
        op_jcxz,

};

enum operand_type {
        operand_none,
        operand_register,
        operand_memory,
        operand_immediate,
        operand_relative_immediate,
};

enum reg_index {
        register_none,

        register_a,
        register_b,
        register_c,
        register_d,
        register_sp,
        register_bp,
        register_si,
        register_di,
        register_es,
        register_cs,
        register_ss,
        register_ds,
        register_ip,
        register_flags,

        register_count,
};

typedef struct reg_access {
        enum reg_index index;
        uint8_t offset;
        uint8_t count;
} reg_access;

enum effective_addr_base {
        effective_addr_direct,

        effective_addr_bx_si,
        effective_addr_bx_di,
        effective_addr_bp_si,
        effective_addr_bp_di,
        effective_addr_si,
        effective_addr_di,
        effective_addr_bp,
        effective_addr_bx,

        effective_addr_count,
};

typedef struct effective_addr_expr {
        enum effective_addr_base base;
        int32_t disp;
} effective_addr_expr;

typedef struct instruction_operand {
        enum operand_type type;
        union {
                effective_addr_expr address;
                reg_access reg;
                uint32_t u_immediate;
                int32_t s_immediate;
        };
} instruction_operand;

typedef struct instruction {
        enum operation_type op;
        instruction_operand operands[2];
        uint32_t size;
        uint32_t w;
} instruction;

typedef struct memory {
        uint8_t bytes[1024 * 1024];
} memory;

#endif // SIM86_H
