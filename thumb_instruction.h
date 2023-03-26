void ThumbMVReg(struct Thumb_MVReg inst){
    printf("MOVS R%d, R%d, %s #0x%x\n", inst.Rd, inst.Rs, rottype[inst.Op], inst.Offset);
    //change state
}

void ThumbAS(struct Thumb_AS inst){
    if(inst.I){
        //SUB
        if(inst.I)printf("SUB R%d, R%d, #0x%x\n", inst.Rd, inst.Rs, inst.Rn);
        else{printf("SUB R%d, R%d, R%d\n", inst.Rd, inst.Rs, inst.Rn);}
    }
    else{
        //ADD
        if(inst.I)printf("ADD R%d, R%d, #0x%x\n", inst.Rd, inst.Rs, inst.Rn);
        else{printf("ADD R%d, R%d, R%d\n", inst.Rd, inst.Rs, inst.Rn);}
    }
}

void ThumbImm(struct Thumb_Imm inst){
    printf("%s R%d, #0x%x\n", th_mcas[inst.Op], inst.Rd, inst.Offset);
}

void ThumbALU(struct Thumb_ALU inst){
    printf("%s R%d, R%d\n", th_opcode[inst.Op], inst.Rd, inst.Rs);
}

void ThumbBX(struct Thumb_BX inst){
    if(inst.H1 == 0 && inst.Op == 3){
        if(inst.H2){
            printf("%s R%d\n", th_bxcode[inst.Op], inst.Rs + 8);
        }
        else{printf("%s R%d\n", th_bxcode[inst.Op], inst.Rs);}
    }
    else if(inst.H1 == 1 && inst.H2 == 0){
        printf("%s R%d, R%d\n", th_bxcode[inst.Op], inst.Rd + 8, inst.Rs);
    }
    else if(inst.H1 == 0 && inst.H2 == 1){
        printf("%s R%d, R%d\n", th_bxcode[inst.Op], inst.Rd, inst.Rs + 8);
    }
    else{
        printf("%s R%d, R%d\n", th_bxcode[inst.Op], inst.Rd + 8, inst.Rs + 8);
    }
}

void ThumbPCLoad(struct Thumb_PcLoad inst){
    printf("LDR R%d, [PC, #0x%x]\n", inst.Rd, inst.word);
}

void ThumbLSReg(struct Thumb_LSReg inst){
    char b = '\0';
    if(inst.B)b = 'B';
    if(inst.L){
        printf("LDR%c R%d, [R%d, R%d]\n", b, inst.Rd, inst.Rb, inst.Ro);
    }
    else{
        printf("STR%c R%d, [R%d, R%d]\n", b, inst.Rd, inst.Rb, inst.Ro);
    }
}

void ThumbLSBH(struct Thumb_LSBH inst){
    if(inst.H){
        printf("LDRH R%d, [R%d, R%d]\n", inst.Rd, inst.Rb, inst.Ro);
    }
    else{
        if(inst.S)printf("LDRB R%d, [R%d, R%d]\n", inst.Rd, inst.Rb, inst.Ro);
        else{printf("STRH R%d, [R%d, R%d]\n", inst.Rd, inst.Rb, inst.Ro);}
    }
}

void ThumbLSImm(struct Thumb_LSImm inst){
    char b = '\0';
    if(inst.B)b = 'B';
    if(inst.L){
        printf("LDR%c R%d, [R%d, #0x%x]\n", b, inst.Rd, inst.Rb, inst.Offset);
    }
    else{
        printf("STR%c R%d, [R%d, #0x%x]\n", b, inst.Rd, inst.Rb, inst.Offset);
    }
}

void ThumbLSH(struct Thumb_LSH inst){
    if(inst.L){
        printf("LDRH R%d, [R%d, #0x%x]\n", inst.Rd, inst.Rb, inst.Offset);
    }
    else{
        printf("STRH R%d, [R%d, #0x%x]\n", inst.Rd, inst.Rb, inst.Offset);
    }
}

void ThumbSPLS(struct Thumb_SPLS inst){
    if(inst.L){
        printf("LDRH R%d, [SP, #0x%x]\n", inst.Rd, inst.word);
    }
    else{
        printf("STRH R%d, [SP, #0x%x]\n", inst.Rd, inst.word);
    }
}

void ThumbLAddr(struct Thumb_LAddr inst){
    if(inst.SP)printf("ADD R%d, SP, #0x%x\n", inst.Rd, inst.word);
    else{printf("ADD R%d, PC, #0x%x\n", inst.Rd, inst.word);}
}

void ThumbAddSP(struct Thumb_AddSP inst){
    if(inst.S)printf("ADD SP, #0x%x\n", inst.word);
    else{printf("ADD SP, #-0x%x\n", inst.word);}
}

void ThumbPPReg(struct Thumb_PPReg inst){
    int c = 0;
    if(inst.L){
        printf("POP {");
    }
    else{
        printf("PUSH {");
    }
    for(int i=0;i<8;i++){
        if(inst.Rlist & (1 << i)){
            if(c!=0)printf(",R%d", i);
            else{printf("R%d", i);}
            c+=1;
        }
    }
    if(inst.L && inst.R)printf(", PC}\n");
    else if(inst.L == 0 && inst.R == 1)printf(", LR]\n");
    else{printf("}\n");}
}

void ThumbMulLS(struct Thumb_MulLS inst){
    int c = 0;
    if(inst.L)printf("LDMIA R%d!, {", inst.Rb);
    else{printf("STMIA R%d!, {", inst.Rb);}
    for(int i=0;i<8;i++){
        if(inst.Rlist & (1 << i)){
            if(c!=0)printf(",R%d", i);
            else{printf("R%d", i);}
            c+=1;
        }
    }
    printf("}\n");
}

void ThumbCondB(struct Thumb_CondB inst){
    u32 offset;
    if((inst.SOffset >> 7))offset = (inst.SOffset << 1) | 0xffffff00;
    else{offset = (u32)(inst.SOffset << 1);}
    if((inst.SOffset >> 7) & 0x1){
        printf("B%s 0x%x\n", cond[inst.cond], addr + 0x4 + offset);
    }
    else{
        printf("B%s 0x%x\n", cond[inst.cond], addr + 0x4 - (inst.SOffset << 1));
    }
}

void ThumbSWI(struct Thumb_SWI inst){
    printf("SWI 0x%x\n", inst.value);
}

void ThumbUcond(struct Thumb_UcondB inst){
    u32 offset;
    if((inst.Offset >> 10))offset = (inst.Offset << 1) | 0xfffff000;
    else{offset = (u32)(inst.Offset << 1);}
    if((inst.Offset >> 10) & 0x1){
        printf("B 0x%x\n", addr + 0x4 + offset);
    }
    else{
        printf("B 0x%x\n", addr + 0x4 + offset);
    }
}

void ThumbLongBL(struct Thumb_LongBL inst){
    if(inst.H)printf("BL 0x%x\n", inst.Offset << 12 + addr + 0x4);
    else{printf("BL LR, 0x%x\n", inst.Offset << 1);}
}