#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "cart.h"
#include "instruction.h"
#include "decode.h"
#include "cpu.h"
#include "ppu.h"

struct arm7tdmi cpu;
struct gba_rom *gba_rom;
u32 *nptr;

int main(int argc, char *argv[]){
    cpu_init(cpu);
    cpu.reg[1] = 5;
    cpu.reg[2] = 3;
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
    u32 code = 0xE0810002;
    dec_inst(code);
    for(int i=0;i < 16;i++){
        printf("cpu.reg[%d] = %d\n", i, cpu.reg[i]);
    }

    return 0;
}