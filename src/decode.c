#include "decode.h"
#include "sim86.h"

instruction_format instruction_format_table[] = {
    {op_mov,
     {{bits_literal, 6, 0x22},
      {bits_d, 1},
      {bits_w, 1},
      {bits_mod, 2},
      {bits_reg, 3},
      {bits_rm, 3}}},
    {op_mov,
     {
         {bits_literal, 7, 0x63},
         {bits_w, 1},
         {bits_mod, 2},
         {bits_literal, 3, 0},
         {bits_rm, 3},
         {bits_data, 0, 1},
         {bits_d, 0, 0},
     }},
    {op_mov,
     {{bits_literal, 4, 0x0B},
      {bits_w, 1},
      {bits_reg, 3},
      {bits_data, 0, 1},
      {bits_d, 0, 1}}},
    {op_mov,
     {{bits_literal, 7, 0x50},
      {bits_w, 1},
      {bits_disp, 0, 1},
      {bits_reg, 0, 0},
      {bits_mod, 0, 0},
      {bits_rm, 0, 0x06},
      {bits_d, 0, 1}}},
    {op_mov,
     {{bits_literal, 7, 0x51},
      {bits_w, 1},
      {bits_disp, 0, 1},
      {bits_reg, 0, 0},
      {bits_mod, 0, 0},
      {bits_rm, 0, 0x06},
      {bits_d, 0, 0}}},
    {op_add,
     {
         {bits_literal, 6, 0x00},
         {bits_d, 1},
         {bits_w, 1},
         {bits_mod, 2},
         {bits_reg, 3},
         {bits_rm, 3},
     }},
    {op_add,
     {
         {bits_literal, 6, 0x20},
         {bits_s, 1},
         {bits_w, 1},
         {bits_mod, 2},
         {bits_literal, 3, 0x00},
         {bits_rm, 3},
         {bits_data, 0, 1},
     }},
    {op_add,
     {
         {bits_literal, 7, 0x02},
         {bits_w, 1},
         {bits_data, 0, 1},
         {bits_reg, 0, 0},
         {bits_d, 0, 1},
     }},
    {op_sub,
     {
         {bits_literal, 6, 0x0A},
         {bits_d, 1},
         {bits_w, 1},
         {bits_mod, 2},
         {bits_reg, 3},
         {bits_rm, 3},
     }},
    {op_sub,
     {
         {bits_literal, 6, 0x20},
         {bits_s, 1},
         {bits_w, 1},
         {bits_mod, 2},
         {bits_literal, 3, 0x05},
         {bits_rm, 3},
         {bits_data, 0, 1},
     }},
    {op_sub,
     {
         {bits_literal, 7, 0x16},
         {bits_w, 1},
         {bits_data, 0, 1},
         {bits_reg, 0, 0},
         {bits_d, 0, 1},
     }},
    {op_cmp,
     {
         {bits_literal, 6, 0x0E},
         {bits_d, 1},
         {bits_w, 1},
         {bits_mod, 2},
         {bits_reg, 3},
         {bits_rm, 3},
     }},
    {op_cmp,
     {
         {bits_literal, 6, 0x20},
         {bits_s, 1},
         {bits_w, 1},
         {bits_mod, 2},
         {bits_literal, 3, 0x07},
         {bits_rm, 3},
         {bits_data, 0, 1},
     }},
    {op_cmp,
     {
         {bits_literal, 7, 0x1E},
         {bits_w, 1},
         {bits_data, 0, 1},
         {bits_reg, 0, 0},
         {bits_d, 0, 1},
     }},
    {op_je,
     {
         {bits_literal, 8, 0x74},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jl,
     {
         {bits_literal, 8, 0x7C},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jle,
     {
         {bits_literal, 8, 0x7E},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jb,
     {
         {bits_literal, 8, 0x72},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jbe,
     {
         {bits_literal, 8, 0x76},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jp,
     {
         {bits_literal, 8, 0x7A},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jo,
     {
         {bits_literal, 8, 0x70},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_js,
     {
         {bits_literal, 8, 0x78},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jne,
     {
         {bits_literal, 8, 0x75},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jnl,
     {
         {bits_literal, 8, 0x7D},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jg,
     {
         {bits_literal, 8, 0x7F},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},

    {op_jnb,
     {
         {bits_literal, 8, 0x73},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_ja,
     {
         {bits_literal, 8, 0x77},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jnp,
     {
         {bits_literal, 8, 0x7B},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jno,
     {
         {bits_literal, 8, 0x71},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jns,
     {
         {bits_literal, 8, 0x79},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_loop,
     {
         {bits_literal, 8, 0xE2},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_loopz,
     {
         {bits_literal, 8, 0xE1},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_loopnz,
     {
         {bits_literal, 8, 0xE0},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
    {op_jcxz,
     {
         {bits_literal, 8, 0xE3},
         {bits_disp, 0, 1},
         {bits_rel_jmp_disp, 0, 1},
     }},
};

static instruction_operand get_reg_operand(uint32_t intel_reg_index,
                                           uint32_t wide) {
        reg_access reg_table[][2] = {
            {{register_a, 0, 1}, {register_a, 0, 2}},
            {{register_c, 0, 1}, {register_c, 0, 2}},
            {{register_d, 0, 1}, {register_d, 0, 2}},
            {{register_b, 0, 1}, {register_b, 0, 2}},
            {{register_a, 1, 1}, {register_sp, 0, 2}},
            {{register_c, 1, 1}, {register_bp, 0, 2}},
            {{register_d, 1, 1}, {register_si, 0, 2}},
            {{register_b, 1, 1}, {register_di, 0, 2}},
        };

        return (instruction_operand){
            .type = operand_register,
            .reg = reg_table[intel_reg_index & 0x07][wide != 0]};
}

static uint32_t parse_data_value(memory *mem, uint32_t *ip, uint32_t exists,
                                 uint32_t wide, uint32_t sign_extended) {
        uint32_t result = 0;
        if (!exists) {
                return result;
        }

        if (wide) {
                uint32_t b1 = mem->bytes[*ip];
                uint32_t b2 = mem->bytes[(*ip) + 1];
                result = (b2 << 8) | b1;
                *ip += 2;
        } else {
                result = mem->bytes[*ip];
                if (sign_extended) {
                        uint32_t sign = result & 0x80;
                        result |= sign;
                }
                *ip += 1;
        }

        return result;
}

static instruction try_decode(instruction_format *inst, memory *mem,
                              uint32_t ip) {
        instruction result = {0};
        uint8_t valid = 1;
        uint32_t bits[bits_count] = {0};
        uint32_t bits_found = 0;

        uint32_t starting_ip = ip;

        uint8_t carryover_count = 0;
        uint8_t carryover_bits = 0;
        uint32_t ifmt_bits_size = sizeof(inst->bits) / sizeof(inst->bits[0]);
        for (uint32_t i = 0; i < ifmt_bits_size && valid; i++) {
                instruction_bits test_bits = inst->bits[i];
                if ((test_bits.type == bits_literal) &&
                    (test_bits.bit_count == 0)) {
                        break;
                }

                uint8_t read_bits = test_bits.bits;
                if (test_bits.bit_count != 0) {
                        if (carryover_count == 0) {
                                carryover_count = 8;
                                carryover_bits = mem->bytes[ip];
                                ip++;
                        }

                        carryover_count -= test_bits.bit_count;
                        read_bits = carryover_bits;
                        read_bits >>= carryover_count;
                        read_bits &= ~(0xFF << test_bits.bit_count);
                }

                if (test_bits.type == bits_literal) {
                        valid = valid && (test_bits.bits == read_bits);
                } else {
                        bits[test_bits.type] = read_bits;
                        bits_found |= (1 << test_bits.type);
                }
        }

        if (!valid) {
                return result;
        }

        uint32_t mod = bits[bits_mod];
        uint32_t rm = bits[bits_rm];
        uint32_t w = bits[bits_w];
        uint32_t d = bits[bits_d];
        uint32_t s = bits[bits_s];

        uint32_t has_direct_address = (mod == 0) && (rm == 6);
        uint32_t has_disp = (mod == 1) || (mod == 2) || has_direct_address ||
                            bits[bits_rel_jmp_disp];
        uint32_t disp_is_w = (mod == 2) || has_direct_address;
        uint32_t data_is_w = !s && w;

        bits[bits_disp] =
            parse_data_value(mem, &ip, has_disp, disp_is_w, (!disp_is_w));
        bits[bits_data] =
            parse_data_value(mem, &ip, bits[bits_data], data_is_w, s);

        int16_t disp = (int16_t)bits[bits_disp];
        if (!disp_is_w) {
                int8_t temp = (int8_t)bits[bits_disp];
                disp = temp;
        }

        result.op = inst->op;
        result.size = ip - starting_ip;
        result.w = w;

        instruction_operand *reg_operand = &result.operands[d ? 0 : 1];
        instruction_operand *mod_operand = &result.operands[d ? 1 : 0];

        if (bits_found & (1 << bits_reg)) {
                *reg_operand = get_reg_operand(bits[bits_reg], w);
        }

        if (bits_found & (1 << bits_mod)) {
                if (mod == 3) {
                        *mod_operand = get_reg_operand(rm, w);
                } else {
                        mod_operand->type = operand_memory;
                        if (has_disp) {
                                mod_operand->address.disp = disp;
                        }

                        if ((mod == 0) & (rm == 6)) {
                                mod_operand->address.base =
                                    effective_addr_direct;
                        } else {
                                mod_operand->address.base = 1 + rm;
                        }
                }
        }

        instruction_operand *unused_operand = &result.operands[0];
        if (unused_operand->type) {
                unused_operand = &result.operands[1];
        }

        if (!unused_operand->type) {
                if (bits_found & (1 << bits_rel_jmp_disp)) {
                        unused_operand->type = operand_relative_immediate;
                        unused_operand->s_immediate = disp + result.size;
                }
                if (bits_found & (1 << bits_data)) {
                        unused_operand->type = operand_immediate;
                        unused_operand->u_immediate = bits[bits_data];
                }
        }

        return result;
}

instruction decode(memory *mem, uint32_t ip) {
        instruction result = {.op = op_none};

        uint32_t ifmt_table_size = sizeof(instruction_format_table) /
                                   sizeof(instruction_format_table[0]);
        for (uint32_t i = 0; i < ifmt_table_size; i++) {
                instruction_format inst = instruction_format_table[i];
                result = try_decode(&inst, mem, ip);
                if (result.op) {
                        break;
                }
        }
        return result;
}
