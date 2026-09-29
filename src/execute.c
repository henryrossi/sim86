#include "execute.h"
#include "sim86.h"

const char *opcode_mnemonics_t[] = {
    "none", "mov", "add", "sub",  "cmp",   "je",     "jl",   "jle", "jb",
    "jbe",  "jp",  "jo",  "js",   "jne",   "jnl",    "jg",   "jnb", "ja",
    "jnp",  "jno", "jns", "loop", "loopz", "loopnz", "jcxz",
};

uint16_t registers[register_count] = {0};

typedef struct flags_register {
        uint8_t zero;
        uint8_t sign;
} flags_register;

flags_register flags = {0};

void reset_flags(void) {
        flags.sign = 0;
        flags.zero = 0;
}

void arithmetic_update_flags(uint16_t val) {
        flags.zero = val ? 0 : 1;
        flags.sign = val & 0x8000 ? 1 : 0;
}

uint32_t calc_effective_address(effective_addr_expr expr) {
        uint32_t base = 0;
        switch (expr.base) {
        case effective_addr_bx_si:
                base = registers[register_b] + registers[register_si];
                break;
        case effective_addr_bx_di:
                base = registers[register_b] + registers[register_di];
                break;
        case effective_addr_bp_si:
                base = registers[register_bp] + registers[register_si];
                break;
        case effective_addr_bp_di:
                base = registers[register_bp] + registers[register_di];
                break;
        case effective_addr_si:
                base = registers[register_si];
                break;
        case effective_addr_di:
                base = registers[register_di];
                break;
        case effective_addr_bp:
                base = registers[register_bp];
                break;
        case effective_addr_bx:
                base = registers[register_b];
                break;
        default:
                break;
        }
        return base + expr.disp;
}

void exec_instruction(memory *mem, instruction instr) {
        uint16_t res = 0;

        switch (instr.op) {
        case op_mov:
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_immediate) {
                        reg_access reg = instr.operands[0].reg;
                        registers[reg.index] = instr.operands[1].u_immediate;
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_register) {
                        reg_access dest = instr.operands[0].reg;
                        reg_access src = instr.operands[1].reg;
                        registers[dest.index] = registers[src.index];
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_memory) {
                        reg_access reg = instr.operands[0].reg;
                        uint32_t addr =
                            calc_effective_address(instr.operands[1].address);
                        uint8_t a = mem->bytes[addr];
                        if (instr.w) {
                                uint8_t b = mem->bytes[addr + 1];
                                registers[reg.index] = (b << 8) + a;
                        } else {
                                registers[reg.index] = a;
                        }
                }
                if (instr.operands[0].type == operand_memory &&
                    instr.operands[1].type == operand_register) {
                        reg_access reg = instr.operands[1].reg;
                        uint32_t addr =
                            calc_effective_address(instr.operands[0].address);
                        if (instr.w) {
                                uint8_t a = 0x00FF & registers[reg.index];
                                uint8_t b = registers[reg.index] >> 8;
                                mem->bytes[addr] = a;
                                mem->bytes[addr + 1] = b;
                        } else {
                                mem->bytes[addr] = registers[reg.index];
                        }
                }
                if (instr.operands[0].type == operand_memory &&
                    instr.operands[1].type == operand_immediate) {
                        uint32_t addr =
                            calc_effective_address(instr.operands[0].address);
                        if (instr.w) {
                                uint8_t a =
                                    0x00FF & instr.operands[1].u_immediate;
                                uint8_t b = instr.operands[1].u_immediate >> 8;
                                mem->bytes[addr] = a;
                                mem->bytes[addr + 1] = b;

                        } else {
                                mem->bytes[addr] =
                                    instr.operands[1].u_immediate;
                        }
                }

                break;
        case op_add:
                reset_flags();
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_immediate) {
                        reg_access reg = instr.operands[0].reg;
                        registers[reg.index] += instr.operands[1].u_immediate;
                        arithmetic_update_flags(registers[reg.index]);
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_register) {
                        reg_access dest = instr.operands[0].reg;
                        reg_access src = instr.operands[1].reg;
                        registers[dest.index] += registers[src.index];
                        arithmetic_update_flags(registers[dest.index]);
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_memory) {
                        reg_access reg = instr.operands[0].reg;
                        uint32_t addr =
                            calc_effective_address(instr.operands[1].address);
                        uint8_t a = mem->bytes[addr];
                        if (instr.w) {
                                uint8_t b = mem->bytes[addr + 1];
                                registers[reg.index] += (b << 8) + a;
                        } else {
                                registers[reg.index] += a;
                        }
                }
                break;
        case op_sub:
                reset_flags();
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_immediate) {
                        reg_access reg = instr.operands[0].reg;
                        registers[reg.index] -= instr.operands[1].u_immediate;
                        arithmetic_update_flags(registers[reg.index]);
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_register) {
                        reg_access dest = instr.operands[0].reg;
                        reg_access src = instr.operands[1].reg;
                        registers[dest.index] -= registers[src.index];
                        arithmetic_update_flags(registers[dest.index]);
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_memory) {
                        reg_access reg = instr.operands[0].reg;
                        uint32_t addr =
                            calc_effective_address(instr.operands[1].address);
                        uint8_t a = mem->bytes[addr];
                        if (instr.w) {
                                uint8_t b = mem->bytes[addr + 1];
                                registers[reg.index] -= (b << 8) + a;
                        } else {
                                registers[reg.index] -= a;
                        }
                }
                break;
        case op_cmp:
                reset_flags();
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_immediate) {
                        reg_access reg = instr.operands[0].reg;
                        res = registers[reg.index] -
                              instr.operands[1].u_immediate;
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_register) {
                        reg_access dest = instr.operands[0].reg;
                        reg_access src = instr.operands[1].reg;
                        res = registers[dest.index] - registers[src.index];
                }
                if (instr.operands[0].type == operand_register &&
                    instr.operands[1].type == operand_memory) {
                        reg_access reg = instr.operands[0].reg;
                        uint32_t addr =
                            calc_effective_address(instr.operands[1].address);
                        uint8_t a = mem->bytes[addr];
                        if (instr.w) {
                                uint8_t b = mem->bytes[addr + 1];
                                res = registers[reg.index] - (b << 8) + a;
                        } else {
                                res = registers[reg.index] - a;
                        }
                }
                arithmetic_update_flags(res);
                break;
        case op_jne:
                if (!flags.zero) {
                        registers[register_ip] +=
                            instr.operands[0].s_immediate - instr.size;
                }
                break;
        default:
                fprintf(stderr,
                        "Execution not implemented for this opcode: %s\n",
                        opcode_mnemonics_t[instr.op]);
                break;
        }
}
