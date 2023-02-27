#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "cpu.h"
#include "cart.h"
#include "arm_decode.h"
#include "arm_instruction.h"
#include "decode.h"
#include "ppu.h"

struct arm7tdmi cpu;
struct gba_rom *gba_rom;
u32 *nptr;

int main(int argc, char *argv[]){
    cpu_init(cpu);
    cpu.reg[0] = 8;
    cpu.reg[1] = 5;
    cpu.reg[2] = 16;
    cpu.reg[3] = 8;
    cpu.reg[4] = 5;
    cpu.reg[5] = 9;
    cpu.CPSR = 18;
    cpu.Mem[8] = 0x1234d678;
    //cpu.reg[15] = 8;
    /*for(int i=0;i < 16;i++){
        printf("cpu.reg[%d] = %d\n", i, cpu.reg[i]);
    }*/
    /*gba_rom = malloc(sizeof(struct gba_rom));
    gba_rom = read_rom("./pok_g_386.gba");
    
    nptr = gba_rom->gba_rom_buf;
    cpu_prefetch(cpu, nptr);
    if(offset){
        printf("nptr : 0x%08x\n", nptr);
        nptr = (u32 *)((u8 *)gba_rom->gba_rom_buf + offset);
        printf(" after nptr : 0x%08x\n", nptr);
    }*/
    //u32 code = 0xE0810002;
    //u32 code = 0xA0D19FE5;
    //little endian
    u32 code = 0xE1D0B0F0;
    ArmInstDec(code);
    for(int i=0;i < 16;i++){
        printf("cpu.reg[%d] = %x\n", i, cpu.reg[i]);
    }

    return 0;
}