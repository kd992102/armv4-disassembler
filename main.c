#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "cpu.h"
#include "cart.h"
#include "arm_decode.h"
#include "arm_instruction.h"
#include "decode.h"

struct arm7tdmi cpu;
struct gba_rom *gba_rom;

int main(int argc, char *argv[]){
    /*cpu_init(cpu);
    cpu.reg[0] = 8;
    cpu.reg[1] = 5;
    cpu.reg[2] = 16;
    cpu.reg[3] = 8;
    cpu.reg[4] = 5;
    cpu.reg[5] = 9;
    cpu.CPSR = 18;
    cpu.Mem[8] = 0x1234d678;
    u32 code = 0xE1D0B0F0;
    ArmInstDec(code);
    for(int i=0;i < 16;i++){
        printf("cpu.reg[%d] = %x\n", i, cpu.reg[i]);
    }*/
    printf("Hello\n");
    return 0;
}