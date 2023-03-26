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

void ThumbInstDec(u16 instruction){
    if((instruction >> 15) & 0x1){
        if((instruction >> 14) & 0x1){
            //11
            if((instruction & MulLS_eigen) == 0xc000){
                ThumbMulLS(ThumbMulLSDec(instruction));
            }
            else if((instruction & CondB_eigen) == 0xd000){
                ThumbCondB(ThumbCondBDec(instruction));
            }
            else if((instruction & SWI_eigen) == 0xdf00){
                ThumbSWI(ThumbSWIDec(instruction));
            }
            else if((instruction & Uncond_eigen) == 0xe000){
                ThumbUcond(ThumbUcondBDec(instruction));
            }
            else{
                ThumbLongBL(ThumbLongBLDec(instruction));
            }
        }
        else{
            //10
            if((instruction & LSH_eigen) == 0x8000){
                ThumbLSH(ThumbLSHDec(instruction));
            }
            else if((instruction & SPLS_eigen) == 0x9000){
                ThumbSPLS(ThumbSPLSDec(instruction));
            }
            else if((instruction & LAddr_eigen) == 0xa000){
                ThumbLAddr(ThumbLAddrDec(instruction));
            }
            else if((instruction & AddSP_eigen) == 0xb000){
                ThumbAddSP(ThumbAddSPDec(instruction));
            }
            else{
                ThumbPPReg(ThumbPPRegDec(instruction));
            }
        }
    }
    else{
        if((instruction >> 14) & 0x1){
            //01
            if((instruction & ALU_eigen) == 0x4000){
                ThumbALU(ThumbALUDec(instruction));
            }
            else if((instruction & BranchEX_eigen) == 0x4400){
                ThumbBX(ThumbBXDec(instruction));
            }
            else if((instruction & PCLoad_eigen) == 0x4800){
                ThumbPCLoad(ThumbPCloadDec(instruction));
            }
            else if((instruction & LSReg_eigen) == 0x5000){
                ThumbLSReg(ThumbLSRegDec(instruction));
            }
            else if((instruction & LSImm_eigen) == 0x5200){
                ThumbLSBH(ThumbLSBHDec(instruction));
            }
            else{
                ThumbLSImm(ThumbLSImmDec(instruction));
            }
        }
        else{
            //00
            if((instruction & AddSub_eigen) == 0x1800){
                ThumbAS(ThumbASDec(instruction));
            }
            else if((instruction & MoveShiftedReg_eigen) == 0x0){
                ThumbMVReg(ThumbMVRegDec(instruction));
            }
            else{
                ThumbImm(ThumbImmDec(instruction));
            }
        }
    }
}