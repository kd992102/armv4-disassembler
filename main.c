#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "cpu.h"
#include "cart.h"
#include "arm_decode.h"
#include "arm_instruction.h"
#include "decode.h"

struct arm7tdmi cpu;
//struct gba_rom *gba_rom;
FILE *bios_bin;

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
    size_t bios_size, read_stat;
    cpu_init(cpu);
    bios_bin = fopen("gba_bios.bin","rb");
    if(bios_bin == NULL){
        printf("file open failed\n");
        return 1;
    }
    
    fseek(bios_bin, 0, SEEK_END);
    bios_size = ftell(bios_bin);
    printf("File size:%d\n", bios_size);
    fseek(bios_bin, 0, SEEK_SET);

    read_stat = fread(cpu.Mem, sizeof(char), bios_size, bios_bin);
    if(read_stat)printf("Read file success.\n");
    fclose(bios_bin);
    bios_bin = NULL;
    for(int addr=0;addr<64;addr+=4){
        printf("0x%08x ", *((u32 *)(&cpu.Mem[addr])));
        cpu.reg[15] = addr + 0x8;
        ArmInstDec(*((u32 *)(&cpu.Mem[addr])));
    }
    return 0;
}