int addr = 0;
//return shift amount

void ArmBX(struct Arm_BranchAndExchange inst){
    printf("BX%s R%d\n", cond[inst.cond], inst.Rn);
    //change state
}
//Modified
void ArmBranch(struct Arm_Branch inst){
    u32 offset;
    if((inst.offset >> 23))offset = (inst.offset << 2) | 0xff000000;
    else{offset = (u32)(inst.offset << 2);}
    if(inst.L)printf("BL%s 0x%x\n", cond[inst.cond], offset + addr + 0x8);
    else{printf("B%s 0x%x\n",cond[inst.cond], offset + addr + 0x8);}
}

void ArmPSRT(struct Arm_DataProcess inst){
    u32 opr;
    u8 shift;
    if(inst.I){
        shift = 2 * ((inst.Operand2 >> 8) & 0xf);
        if(shift < 8){
            switch(shift){
                case 2:
                    opr = ((u32)(inst.Operand2 & 0x3) << 30) | ((inst.Operand2 & 0xff) >> 2);
                    break;
                case 4:
                    opr = ((u32)(inst.Operand2 & 0xf) << 28) | ((inst.Operand2 & 0xff) >> 4);
                    break;
                case 6:
                    opr = ((u32)(inst.Operand2 & 0x3f) << 26) | ((inst.Operand2 & 0xff) >> 6);
                    break;
            }
        }
        else{
            opr = (u32)(inst.Operand2 & 0xff) << (32 - shift);
        }
    }
    if(inst.Opcode & 0x1){
        //MSR
        if(inst.Rn & 0x1){
            printf("MSR%s %s_all, R%d\n", cond[inst.cond], psr[(inst.Opcode >> 1) & 0x1], inst.Operand2 & 0xf);
        }
        else{
            if(inst.I)printf("MRS%s %s_flg, #0x%x\n", cond[inst.cond], psr[(inst.Opcode >> 1) & 0x1], opr);
            else{
                printf("MRS%s %s_flg, R%d\n", cond[inst.cond], psr[(inst.Opcode >> 1) & 0x1], inst.Operand2 & 0xf);
            }
        }
    }
    else{
        //MRS
        printf("MRS%s R%d, %s\n", cond[inst.cond], inst.Rd, psr[(inst.Opcode >> 1) & 0x1]);
    }
}

void ArmDataProc(struct Arm_DataProcess inst){
    char s = '\0';
    u32 opr;
    u8 shift;
    if(inst.S)s = 'S';
    if(inst.I){
        //shift amount
        shift = ((inst.Operand2 >> 8) & 0xf) << 1;
        opr = ((u32)(inst.Operand2 & 0xff)) << (32 - shift);
    }
    else{
        shift = (inst.Operand2 >> 5) & 0x3;
    }

    if(inst.Opcode >= 0x8 && inst.Opcode <= 0xb){
        if(inst.I)printf("%s%s R%d, #0x%x\n", opcode[inst.Opcode], cond[inst.cond], inst.Rn, opr);
        else{
            if((inst.Operand2 >> 4) & 0x1){
                printf("%s%s R%d, R%d, %s R%d\n", opcode[inst.Opcode], cond[inst.cond], inst.Rn, inst.Operand2 & 0xf, rottype[(inst.Operand2 >> 5) & 0x3], (inst.Operand2 >> 8) & 0xf);
            }
            else{
                printf("%s%s R%d, R%d, %s #%d\n", opcode[inst.Opcode], cond[inst.cond], inst.Rn, inst.Operand2 & 0xf, rottype[(inst.Operand2 >> 5) & 0x3], (inst.Operand2 >> 7) & 0x1f);
            }
        }
    }
    else if(inst.Opcode == 0xd || inst.Opcode == 0xf){
        if(inst.I)printf("%s%s%c R%d, #0x%x\n", opcode[inst.Opcode], cond[inst.cond], s, inst.Rd, opr);
        else{
            if((inst.Operand2 >> 4) & 0x1){
                printf("%s%s%c R%d, R%d, %s R%d\n", opcode[inst.Opcode], cond[inst.cond], s, inst.Rd, inst.Operand2 & 0xf, rottype[(inst.Operand2 >> 5) & 0x3], (inst.Operand2 >> 8) & 0xf);
            }
            else{
                printf("%s%s%c R%d, R%d, %s #%d\n", opcode[inst.Opcode], cond[inst.cond], s, inst.Rd, inst.Operand2 & 0xf, rottype[(inst.Operand2 >> 5) & 0x3], (inst.Operand2 >> 7) & 0x1f);
            }
        }
    }
    else{
        if(inst.I)printf("%s%s%c R%d, R%d, #0x%x\n", opcode[inst.Opcode], cond[inst.cond], s, inst.Rd, inst.Rn, opr);
        else{
            if((inst.Operand2 >> 4) & 0x1){
                printf("%s%s%c R%d, R%d, R%d, %s R%d\n", opcode[inst.Opcode], cond[inst.cond], s, inst.Rd, inst.Rn, inst.Operand2 & 0xf, rottype[(inst.Operand2 >> 5) & 0x3], (inst.Operand2 >> 8) & 0xf);
            }
            else{
                printf("%s%s%c R%d, R%d, R%d, %s #%d\n", opcode[inst.Opcode], cond[inst.cond], s, inst.Rd, inst.Rn, inst.Operand2 & 0xf, rottype[(inst.Operand2 >> 5) & 0x3], (inst.Operand2 >> 7) & 0x1f);
            }
        }
    }
}

//OK
void ArmMUL(struct Arm_Multiply inst){
    char s = '\0';
    if(inst.S)s = 'S';
    if(inst.A){
        //MLA
        printf("MLA%s%c R%d, R%d, R%d, R%d\n", cond[inst.cond], s, inst.Rd, inst.Rm, inst.Rs, inst.Rn);
    }
    else{
        //MUL
        printf("MUL%s%c R%d, R%d, R%d\n", cond[inst.cond], s, inst.Rd, inst.Rm, inst.Rs);
    }
}
//OK
void ArmMULL(struct Arm_MultiplyLong inst){
    char s = '\0';
    if(inst.S)s = 'S';
    if(inst.A){
        //MLA
        if(inst.U)printf("SMLAL%s%c R%d, R%d, R%d, R%d\n", cond[inst.cond], s, inst.RdLo, inst.RdHi, inst.Rm, inst.Rs);
        else{
            printf("UMLAL%s%c R%d, R%d, R%d, R%d\n", cond[inst.cond], s, inst.RdLo, inst.RdHi, inst.Rm, inst.Rs);
        }
    }
    else{
        //MUL
        if(inst.U)printf("SMULL%s%c R%d, R%d, R%d, R%d\n", cond[inst.cond], s, inst.RdLo, inst.RdHi, inst.Rm, inst.Rs);
        else{
            printf("UMULL%s%c R%d, R%d, R%d, R%d\n", cond[inst.cond], s, inst.RdLo, inst.RdHi, inst.Rm, inst.Rs);
        }
    }
}

//Single Data Transfer
void ArmSDT(struct Arm_SingleDataTransfer inst){
    char w = '\0';
    char b = '\0';
    char u = '\0';
    char shift;
    u32 opr;
    if(inst.B)b = 'B';
    if(inst.U)u = '-';
    if(inst.W)w = '!';
    if(inst.I == 0){
        //shift amount
        opr = inst.Offset;
    }
    else{
        shift = (inst.Offset >> 5) & 0x3;
    }

    if(inst.L){
        //LDR
        if(inst.I == 0){
            if(inst.P)printf("LDR%s%c R%d, [R%d, #0x%x]%c\n", cond[inst.cond], b, inst.Rd, inst.Rn, opr, w);
            else{if(inst.P)printf("LDR%s%c R%d, [R%d], #0x%x\n", cond[inst.cond], b, inst.Rd, inst.Rn, opr);}
        }
        else{
            if(inst.P){
                if((inst.Offset >> 4) & 0x1)printf("LDR%s%c R%d, [R%d, %cR%d, %s, R%d]%c\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 8) & 0xf, w);
                else{
                    printf("LDR%s%c R%d, [R%d, %cR%d, %s, #0x%x]%c\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 7) & 0x1f, w);
                }
            }
            else{
                if((inst.Offset >> 4) & 0x1)printf("LDR%s%c R%d, [R%d], %cR%d, %s, R%d\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 8) & 0xf);
                else{
                    printf("LDR%s%c R%d, [R%d], %cR%d, %s, #0x%x\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 7) & 0x1f);
                }
            }    
        }
    }
    else{
        //SDT
        if(inst.I == 0){
            if(inst.P)printf("STR%s%c R%d, [R%d, #0x%x]%c\n", cond[inst.cond], b, inst.Rd, inst.Rn, opr, w);
            else{if(inst.P)printf("STR%s%c R%d, [R%d], #0x%x\n", cond[inst.cond], b, inst.Rd, inst.Rn, opr);}
        }
        else{
            if(inst.P){
                if((inst.Offset >> 4) & 0x1)printf("STR%s%c R%d, [R%d, %cR%d, %s, R%d]%c\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 8) & 0xf, w);
                else{
                    printf("STR%s%c R%d, [R%d, %cR%d, %s, #0x%x]%c\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 7) & 0x1f, w);
                }
            }
            else{
                if((inst.Offset >> 4) & 0x1)printf("STR%s%c R%d, [R%d], %cR%d, %s, R%d\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 8) & 0xf);
                else{
                    printf("STR%s%c R%d, [R%d], %cR%d, %s, #0x%x\n", cond[inst.cond], b, inst.Rd, inst.Rn, u, inst.Offset & 0xf, rottype[(inst.Offset >> 5) & 0x3], (inst.Offset >> 7) & 0x1f);
                }
            }    
        }
    }
}

//signed
void ArmSDTS(struct Arm_HalfwordDataTransferOffset inst){
    char w = '\0';
    u8 sh = (inst.S << 1) | (inst.H);
    char u = '\0';
    u32 opr = (inst.Hi << 4) | inst.Rm;
    if(inst.U)u = '-';
    if(inst.W)w = '!';
    if(inst.L){
        //LDR
        if(inst.Hi){
            if(inst.P)printf("LDR%s%s R%d, [R%d, #%c0x%x]%c\n", cond[inst.cond], sh_label[sh], inst.Rd, inst.Rn, u, opr, w);
            else{printf("LDR%s%s R%d, [R%d], #%c0x%x\n", cond[inst.cond], sh_label[sh], inst.Rd, inst.Rn, u, opr);}
        }
        else{
            if(inst.P)printf("LDR%s%s R%d, [R%d, %cR%d]%c\n", cond[inst.cond], sh_label[sh], inst.Rd, inst.Rn, u, inst.Rm, w);
            else{printf("LDR%s%s R%d, [R%d], %cR%d\n", cond[inst.cond], sh_label[sh], inst.Rd, inst.Rn, u, inst.Rm);}    
        }
    }
    else{
        //SDT
        if(inst.Hi){
            if(inst.P)printf("STRH%s R%d, [R%d, #%c0x%x]%c\n", cond[inst.cond], inst.Rd, inst.Rn, u, opr, w);
            else{printf("STRH%s R%d, [R%d], #%c0x%x\n", cond[inst.cond], inst.Rd, inst.Rn, u, opr);}
        }
        else{
            if(inst.P)printf("STRH%s R%d, [R%d, %cR%d]%c\n", cond[inst.cond], inst.Rd, inst.Rn, u, inst.Rm, w);
            else{printf("STRH%s R%d, [R%d], %cR%d\n", cond[inst.cond], inst.Rd, inst.Rn, u, inst.Rm);}    
        }
    }
}


void ArmBDT(struct Arm_BlockDataTransfer inst){
    char w = '\0';
    char u = '\0';
    char mode = (inst.L << 2) | (inst.P << 1) | inst.U;
    u8 c = 0;
    if(inst.U)u = '-';
    if(inst.W)w = '!';
    //get Reglist
    if(inst.L){
        //LDM
        if(inst.Rn == 13)printf("LDM%s%s R%d%c, {", cond[inst.cond], addr_mode_stack[mode], inst.Rn, w);
        else{printf("LDM%s%s R%d%c, {", cond[inst.cond], addr_mode[mode], inst.Rn, w);}
    }
    else{
        //STM
        if(inst.Rn == 13)printf("STM%s%s R%d%c, {", cond[inst.cond], addr_mode_stack[mode], inst.Rn, w);
        else{printf("STM%s%s R%d%c, {", cond[inst.cond], addr_mode[mode], inst.Rn, w);}
    }
    for(int i=0;i<16;i++){
        if(inst.RegisterList & (1 << i)){
            if(c!=0)printf(",R%d", i);
            else{printf("R%d", i);}
            c+=1;
        }
    }
    if(inst.S)printf("}%c\n", '^');
    else{printf("}\n");}
}

void ArmSWP(struct Arm_SingleDataSwap inst){
    char b = '\0';
    if(inst.B)b = 'B';
    printf("SWP%s%c R%d, R%d, [R%d]\n", cond[inst.cond], b, inst.Rd, inst.Rm, inst.Rn);
}

void ArmSWI(struct Arm_SoftwareInterrupt inst){
    //printf(" ignored:%x ", ~inst.ignored);
    if(((inst.ignored >> 16) & 0xff) <= 0x2A)printf("SWI%s %s\n", cond[inst.cond], swi_func[((inst.ignored >> 16) & 0xff)]);
    else{printf("SWI%s Unknown\n", cond[inst.cond]);}
}

void ArmUDF(struct Arm_Undefined inst){
    printf("Undefined\n");
}

/*
void Arm_BX(struct ArmBranchAndExchange inst, struct arm7tdmi CPU){

}*/

