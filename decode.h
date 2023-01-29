u32 offset = 0;
extern struct arm7tdmi cpu;

void dec_inst(u32 instruction){
    printf("decodeing...\n");
    if((instruction & Branch_eigen) == 0xa000000){
        offset = Arm_B_and_BL(ArmBranchDec(instruction));
    }
    if((instruction & DataProcess_eigen) >> 25 == 0 || (instruction & DataProcess_eigen) >> 25 == 1){
        printf("Data Process Metch\n");
        if(((instruction >> 20) & 0x1) == 0){
            if((((instruction >> 21) & 0xf) < 12) && (((instruction >> 21) & 0xf) > 7)){
                printf("PSR\n");
                Arm_PSRTransfer(ArmPSRTransfDec(instruction));
            }
            else{Arm_DataProc(ArmDataProcDec(instruction));}
        }
        else{Arm_DataProc(ArmDataProcDec(instruction));}

    }
    if((instruction & Multiply_eigen) == 0x90){
        //MUL, MLA
        Arm_Mul(ArmMultiDec(instruction));
    }
    if((instruction & MultiLong_inst) == 0x800090){
        //NULL,MLAL
        Arm_MulLong(ArmMultLongiDec(instruction));
    }
    /*
    else if(instruction & MutiplyLong_eigen == 0x800090)Arm_mutiplylong(instruction);
    else if(instruction & SingleDataSwap_eigen == 0x1000090)Arm_singledataswap(instruction);
    else if(instruction & BranchAndExchange_eigen == 0x12fff10)Arm_branchandexchange(instruction);
    else if(instruction & HalfwordDataTransferRO_eigen == 0x90)Arm_halfworddatatransferRO(instruction);
    else if(instruction & HalfwordDataTransferIO_eigen == 0x400090)Arm_halfworddatatransferIO(instruction);
    else if(instruction & SingleDataTransfer_eigen == 0x4000000)Arm_singlefatatransfer(instruction);
    else if(instruction & Undefined_eigen == 0x6000010)Arm_undefined(instruction);
    else if(instruction & BlockDataTransfer_eigen == 0x8000000)Arm_blockdatatransfer(instruction);
    else if(instruction & Branch_eigen == 0xa000000)Arm_branch(instruction);
    else if(instruction & CoprocessorDataT_eigen == 0xc000000)Arm_coprocessordataT(instruction);
    else if(instruction & CoprocessorDataO_eigen == 0xe000000)Arm_coprocessordataO(instruction);
    else if(instruction & CoprocessorRegisterT_eigen == 0xe000010)Arm_coprocessorregisterT(instruction);
    else if(instruction & SoftwareInterrupt_eigen == 0xf000000)Arm_softwareinterrupt(instruction);
    */
   printf("end decode...\n");
}
/*
void exe_inst(u32 instruction){
    
}

*/



u8 chk_cond(u8 cond){
    u8 N = ((cpu.CPSR >> 31) & 0x1);
    u8 Z = ((cpu.CPSR >> 30) & 0x1);
    u8 C = ((cpu.CPSR >> 29) & 0x1);
    u8 V = ((cpu.CPSR >> 28) & 0x1);
    switch(cond){
        case 0:
            if(Z == 1)return 1;
            return 0;
        case 1:
            if(Z == 0)return 1;
            return 0;
        case 2:
            if(C == 1)return 1;
            return 0;
        case 3:
            if(C == 0)return 1;
            return 0;
        case 4:
            if(N == 1)return 1;
            return 0;
        case 5:
            if(N == 0)return 1;
            return 0;
        case 6:
            if(V == 1)return 1;
            return 0;
        case 7:
            if(V == 0)return 1;
            return 0;
        case 8:
            if(C == 1 && N == 0)return 1;
            return 0;
        case 9:
            if(C == 0 || Z == 1)return 1;
            return 0;
        case 10:
            if(N == V)return 1;
            return 0;
        case 11:
            if(N != V)return 1;
            return 0;
        case 12:
            if((Z == 0) & (N == V))return 1;
            return 0;
        case 13:
            if((Z == 1) | (N != V))return 1;
            return 0;
        case 14:
            return 1;
    }
}