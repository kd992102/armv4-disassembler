#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "arm_decode.h"
#include "arm_instruction.h"
#include "decode.h"

//struct gba_rom *gba_rom;
FILE *bios_bin;
int main(int argc, char *argv[]){
    //printf("%x\n", cpu.reg[13]);
    size_t bios_size;
    bios_bin = fopen("gba_bios.bin","rb");
    if(bios_bin == NULL){
        printf("file open failed\n");
        return 1;
    }
    fseek(bios_bin, 0, SEEK_END);
    bios_size = ftell(bios_bin);
    printf("File size:%d\n", bios_size);
    fseek(bios_bin, 0, SEEK_SET);
    u8 inst_buf[4];
    for(addr=0;addr<0x580;addr+=4){
        fread(inst_buf, sizeof(char), 4, bios_bin);
        printf("0x%08x %08X ", addr, *((u32 *)(&inst_buf)));
        ArmInstDec(*((u32 *)(&inst_buf)));
    }
    fclose(bios_bin);
    bios_bin = NULL;
    return 0;
}