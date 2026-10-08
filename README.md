## Simulated 8086 CPU

This project is implemented from the [October 1979 Intel 8086 Family User’s Manual](https://edge.edx.org/c4x/BITSPilani/EEE231/asset/8086_family_Users_Manual_1_.pdf). 
The project simulates arithmetic and bit manipulation instructions, data movement instructions, conditional instructions, and program transfer instructions. 
It also simulates, registers, flags, and main memory.

To simulate a program, assemble the program into 8086 machine code from an assembly file using a tool like [nasm](https://www.nasm.us/). Then, run:

```
./sim86 <path to machine code file>
```

The program takes the binary code stream and decodes each instruction, executes its logic, and prints reassembled assembly corresponding to the instruction.


To compile the program run:

```
cc -o sim86 sim86.c
```
