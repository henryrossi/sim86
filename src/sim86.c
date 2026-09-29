#include "sim86.h"
#include "decode.h"
#include "execute.h"
#include "reasm.h"

#include "decode.c"
#include "execute.c"
#include "reasm.c"

int LoadFileIntoMemory(const char *filename, memory *mem, uint32_t offset) {
        int res = 0;

        FILE *fp = fopen(filename, "rb");
        if (!fp)
                return res;

        int mem_size = sizeof(mem->bytes) / sizeof(mem->bytes[0]);
        res = fread(mem + offset, 1, mem_size, fp);
        fclose(fp);

        return res;
}

void print_registers(void) {
        char *reg_names[8] = {"ax", "bx", "cx", "dx", "sp", "bp", "si", "di"};
        printf("\n; final registers\n");
        for (uint32_t i = 0; i < 8; i++) {
                printf(";    %s: 0x%04x (%d)\n", reg_names[i], registers[i + 1],
                       registers[i + 1]);
        }
        printf("\n;    ip: 0x%04x (%d)\n", registers[register_ip],
               registers[register_ip]);
}

void print_flags(void) {
        printf("; flags: ");
        if (flags.zero)
                printf("z");
        if (flags.sign)
                printf("s");
        printf("\n");
}

int main(int argc, char *argv[]) {
        if (argc != 2) {
                fprintf(stderr, "Please input binary file\n");
                return 1;
        }

        memory *mem = malloc(sizeof(memory));

        uint32_t inst_size = LoadFileIntoMemory(argv[1], mem, 0);
        if (inst_size == 0) {
                fprintf(stderr, "Couldn't read input file\n");
                return 1;
        }

        uint16_t *ip = &registers[register_ip];
        fprintf(stdout, "bits 16\n");
        while (*ip < inst_size) {
                instruction i = decode(mem, *ip);
                if (i.op) {
                        *ip += i.size;
                        print_instruction(i, stdout);
                        exec_instruction(mem, i);
                        print_flags();
                } else {
                        fprintf(stderr, "Unrecognized binary in the "
                                        "instruction stream\n");
                        *ip += 1;
                }
        }
        print_registers();

        return 0;
}
