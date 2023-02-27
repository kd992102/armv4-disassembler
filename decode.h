u32 offset = 0;
extern struct arm7tdmi cpu;

void ArmInstDec(u32 instruction){
    if(((instruction >> 26) & 0x3) == 0){
        if((instruction & BranchAndExchange_eigen) == 0x12fff10){
            ArmBX(ArmBXDec(instruction));
        }
        else if((instruction & SingleDataSwap_eigen) == 0x1b00090){
            ArmSWP(ArmSWPDec(instruction));
        }
        else if((instruction & Multiply_eigen) == 0x90){
            ArmMUL(ArmMULDec(instruction));
        }
        else if((instruction & MultiplyLong_eigen) == 0x800090){
            ArmMULL(ArmMULLDec(instruction));
        }
        else if((instruction & HalfwordDataTransfer_eigen) == 0x90){
            ArmSDTS(ArmSDTSDec(instruction));
        }
        else{
            ArmDataProc(ArmDataProcDec(instruction));
        }
    }
    else if(((instruction >> 26) & 0x3) == 1){
        if((instruction & SingleDataTransfer_eigen) == 0x4000000){
            ArmSDT(ArmSDTDec(instruction));
        }
        else{
            ArmUDF(ArmUDFDec(instruction));
        }
    }
    else if(((instruction >> 26) & 0x3) == 2){
        if(((instruction >> 25) & 0x1) == 1){
            ArmBranch(ArmBranchDec(instruction));
        }
        else{
            ArmBDT(ArmBDTDec(instruction));
        }
    }
    else if(((instruction >> 26) & 0x3) == 3){
        if(((instruction >> 24) & 0x3) == 3){
            ArmSWI(ArmSWIDec(instruction));
        }
        else{
            puts("Coprocessor Instruction.\n");
        }
    }
}
/*
void exe_inst(u32 instruction){
    
}

*/