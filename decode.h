u32 offset = 0;
extern struct arm7tdmi cpu;

void dec_inst(u32 instruction){
    printf("decodeing...\n");
    if((instruction & Branch_eigen) == 0xa000000){
        offset = Arm_B_and_BL(ArmBranchDec(instruction));
    }
    else if((instruction & Multiply_eigen) == 0x90){
        //MUL, MLA
        printf("Multiple Dec\n");
        Arm_Mul(ArmMultiDec(instruction));
    }

    else if((instruction & MutiplyLong_eigen) == 0x800090){
        //NULL,MLAL
        printf("Multiple Long Dec\n");
        Arm_MulLong(ArmMultLongiDec(instruction));
    }
    else if((instruction & SingleDataTransfer_eigen) == 0x4000000){
        printf("SDT Dec\n");
        Arm_SDT(SingleDataTransDec(instruction));
    }
    else if((instruction & HalfwordDataTransfer_eigen) == 0x90){
        printf("SDTS Dec\n");
        Arm_SDTS(SingleDataTransSignedDec(instruction));
    }
    else if((instruction & DataProcess_eigen) == 0x0){
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
}
/*
void exe_inst(u32 instruction){
    
}

*/