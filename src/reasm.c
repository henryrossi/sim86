#include "reasm.h"
#include "sim86.h"

const char *opcode_mnemonics[] = {
    "none", "mov", "add", "sub",  "cmp",   "je",     "jl",   "jle", "jb",
    "jbe",  "jp",  "jo",  "js",   "jne",   "jnl",    "jg",   "jnb", "ja",
    "jnp",  "jno", "jns", "loop", "loopz", "loopnz", "jcxz",
};

static const char *get_reg_name(reg_access reg) {
        const char *reg_names[][3] = {
            {"", "", ""},       {"al", "ah", "ax"}, {"bl", "bh", "bx"},
            {"cl", "ch", "cx"}, {"dl", "dh", "dx"}, {"sp", "sp", "sp"},
            {"bp", "bp", "bp"}, {"si", "si", "si"}, {"di", "di", "di"},
            {"es", "es", "es"}, {"cs", "cs", "cs"}, {"ss", "ss", "ss"},
            {"ds", "ds", "ds"}, {"ip", "ip", "ip"},
        };

        return reg_names[reg.index][(reg.count == 2) ? 2 : reg.offset & 1];
}

static const char *get_effective_addr_expr(effective_addr_expr addr) {
        const char *rm_base[] = {
            "", "bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx",
        };

        return rm_base[addr.base];
}

void print_instruction(instruction instr, FILE *dest) {
        fprintf(dest, "%s ", opcode_mnemonics[instr.op]);

        const char *sep = "";
        uint32_t operand_count =
            sizeof(instr.operands) / sizeof(instr.operands[0]);
        for (uint32_t operand_i = 0; operand_i < operand_count; operand_i++) {
                instruction_operand operand = instr.operands[operand_i];

                fprintf(dest, "%s", sep);
                sep = ", ";

                switch (operand.type) {
                case operand_none:
                        break;
                case operand_register:
                        fprintf(dest, "%s", get_reg_name(operand.reg));
                        break;
                case operand_memory:
                        if (instr.operands[0].type != operand_register) {
                                fprintf(dest, "%s ", instr.w ? "word" : "byte");
                        }

                        fprintf(dest, "[%s",
                                get_effective_addr_expr(operand.address));
                        if (operand.address.disp != 0) {
                                fprintf(dest, "%+d", operand.address.disp);
                        }
                        fprintf(dest, "]");
                        break;
                case operand_immediate:
                        fprintf(dest, "%d", operand.s_immediate);
                        break;
                case operand_relative_immediate:
                        fprintf(dest, "$%+d", operand.s_immediate);
                        break;
                }
        }
        fprintf(dest, "\n");
}
