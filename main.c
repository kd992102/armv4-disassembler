#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "arm_decode.h"
#include "arm_instruction.h"
#include "thumb_decode.h"
#include "thumb_instruction.h"
#include "decode.h"

FILE *bios_bin;
int main(int argc, char *argv[]){
    if(strncmp(argv[1],"-h",2) == 0){
        puts("armv4-disassembler");
        puts("------------------------------------------------------------------------------------------------------");
        puts("armv4disass [filename/-h] [-t/-a]");
        puts("-h   help   for listing information.");
        puts("-t   thumb  for switching the disassembler decode in thumb mode(2 bytes each insstruction).");
        puts("-a   arm    for switching the disassembler decode in  arm mode(4 bytes each insstruction).");
        puts("The disassembler is default decode in arm mode\n");
        puts("If you want to create an output file, you can add this syntax [>/>>] [output filename] at the end of the command.");
        return 0;
    }
    size_t bios_size;
    bios_bin = fopen(argv[1],"rb");
    if(bios_bin == NULL){
        printf("file open failed\n");
        return 1;
    }
    fseek(bios_bin, 0, SEEK_END);
    bios_size = ftell(bios_bin);
    printf("File size:%d\n", bios_size);
    fseek(bios_bin, 0, SEEK_SET);
    u8 arm_inst_buf[4];
    u8 thumb_inst_buf[2];
    if(argv[2] == NULL){
        //arm mode
        cpu_mode = 1;
        cpu_read_offset = 4;
    }
    else if(strncmp(argv[2], "-t", 2) == 0){
        //thumb mode
        cpu_mode = 0;
        cpu_read_offset = 2;
    }
    else if(strncmp(argv[2], "-a", 2) == 0){
        //arm mode
        cpu_mode = 1;
        cpu_read_offset = 4;
    }
    for(addr=0;addr<0x580;addr+=cpu_read_offset){
        if(cpu_mode){
            fread(arm_inst_buf, sizeof(char), 4, bios_bin);
            printf("0x%08x   %08X   ", addr, *((u32 *)(&arm_inst_buf)));
            ArmInstDec(*((u32 *)(&arm_inst_buf)));
        }
        else{
            fread(thumb_inst_buf, sizeof(char), 2, bios_bin);
            printf("0x%04x   %04X   ", addr, *((u16 *)(&thumb_inst_buf)));
            ThumbInstDec(*((u16 *)(&thumb_inst_buf)));
        }
    }
    fclose(bios_bin);
    bios_bin = NULL;
    return 0;
}