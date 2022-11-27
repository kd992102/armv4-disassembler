u8 IsBranch = 0;

void cpu_init(struct arm7tdmi cpu){
    cpu.reg[0] = 0;
    cpu.reg[1] = 0;
    cpu.reg[2] = 0;
    cpu.reg[3] = 0;
    cpu.reg[4] = 0;
    cpu.reg[5] = 0;
    cpu.reg[6] = 0;
    cpu.reg[7] = 0;
    cpu.reg[8] = 0;
    cpu.reg[9] = 0;
    cpu.reg[10] = 0;
    cpu.reg[11] = 0;
    cpu.reg[12] = 0;
    cpu.reg[13] = 0;
    cpu.reg[14] = 0;
    cpu.reg[15] = 0;
    cpu.CPSR = 0;
    cpu.SPSR = 0;
    cpu.cache1 = 0;
    cpu.cache2 = 0;
}

//read 8 bytes and decode it
void cpu_prefetch(struct arm7tdmi cpu, u32 *gbarom_buf){
    if(IsBranch){
        cpu.cache1 = 0;
        cpu.cache2 = 0;
    }
    cpu.cache2 = *((u32 *)((u8 *)gbarom_buf + cpu.reg[15] - 8));
    
    dec_inst(cpu.cache2);
    cpu.cache1 = cpu.cache2;
    cpu.cache2 = *((u32 *)((u8 *)gbarom_buf + cpu.reg[15] - 4));
    //printf("cache1 : 0x%08x, cache2 : 0x%08x, pc : %d\n", cpu.cache1, cpu.cache2, cpu.pc);
    dec_inst(cpu.cache2);
    //exe_inst(cpu.cache1);
}
/*
struct Mem_Map{
    unsigned int bios[0x1000];
    unsigned int space1[0x7FF000];
    unsigned int OnBoardWRAM[0x10000]; //for interrupt
    unsigned int space2[0x3F0000];
    unsigned int OnChipWRAM[0x2000];
    unsigned int space3[0x3FE000];
    unsigned int IORegisters[0x100];
    unsigned int space4[0x3FFF00];
};*/