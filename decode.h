void ArmInstDec(u32 instruction){
    if(((instruction >> 26) & 0x3) == 0){
        if((instruction & BranchAndExchange_eigen) == 0x12fff10){
            ArmBX(ArmBXDec(instruction));
            //puts("BX");
        }
        else if((instruction & SingleDataSwap_eigen) == 0x1b00090){
            ArmSWP(ArmSWPDec(instruction));
            //puts("SWP");
        }
        else if((instruction & Multiply_eigen) == 0x90){
            ArmMUL(ArmMULDec(instruction));
            //puts("MUL");
        }
        else if((instruction & MultiplyLong_eigen) == 0x800090){
            ArmMULL(ArmMULLDec(instruction));
            //puts("MULL");
        }
        else if((instruction & HalfwordDataTransfer_eigen) == 0x90){
            ArmSDTS(ArmSDTSDec(instruction));
            //puts("SDTS");
        }
        else{
            if((instruction & 0x1900000) == 0x1000000)ArmPSRT(ArmDataProcDec(instruction));
            else{ArmDataProc(ArmDataProcDec(instruction));}
            //puts("DataProc");
        }
    }
    else if(((instruction >> 26) & 0x3) == 1){
        if((instruction & SingleDataTransfer_eigen) == 0x4000000){
            ArmSDT(ArmSDTDec(instruction));
            //puts("SDT");
        }
        else{
            ArmUDF(ArmUDFDec(instruction));
            //puts("UDF");
        }
    }
    else if(((instruction >> 26) & 0x3) == 2){
        if(((instruction >> 25) & 0x1) == 1){
            ArmBranch(ArmBranchDec(instruction));
            //puts("B");
        }
        else{
            ArmBDT(ArmBDTDec(instruction));
            //puts("BDT");
        }
    }
    else if(((instruction >> 26) & 0x3) == 3){
        if(((instruction >> 24) & 0x3) == 3){
            ArmSWI(ArmSWIDec(instruction));
            //puts("SWI");
        }
        else{
            puts("Coprocessor Instruction.");
        }
    }
    else{
        puts("Exception");
    }
}
/*
void exe_inst(u32 instruction){
    
}

*/