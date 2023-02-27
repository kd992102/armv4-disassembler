extern struct arm7tdmi cpu;

//done
void CsprUpdate(i32 result){
    //u8 N = ((cpu.CPSR >> 31) & 0x1);//N:negative
    //u8 Z = ((cpu.CPSR >> 30) & 0x1);//Z:zero
    //u8 C = ((cpu.CPSR >> 29) & 0x1);//C:carry
    //u8 V = ((cpu.CPSR >> 28) & 0x1);//V:overflow
    if(result){
        cpu.CPSR = cpu.CPSR & 0xbfffffff;//if result != 0, set Z = 0
    }
    else{
        cpu.CPSR = cpu.CPSR | (1 << 30);//if result == 0, set Z = 1
    }

    if(result & (1 << 31) == 0x80000000){
        cpu.CPSR = cpu.CPSR | (1 << 31);//if result[31] = 1, set N = 1
    }
    else{cpu.CPSR = cpu.CPSR & 0x7fffffff;}//if result[31] != 1, set N = 0
}

//return shift amount
u32 ArmImmOp(u16 Immediate){
    u8 is_R = (Immediate >> 4) & 0x1;
    u8 s_type = (Immediate >> 5) & 0x3;
    u8 s_imm = (Immediate >> 7) & 0x1f;
    u8 Rm = Immediate & 0xf;
    u8 Rs = 0;
    u32 s_result;
    switch(is_R){
        case 0:
            switch(s_type){
                case 0:
                    //logical shift left
                    s_result = (cpu.reg[Rm] << s_imm);
                    break;
                case 1:
                    //logical shift right
                    s_result = (cpu.reg[Rm] >> s_imm) & 0x7ffffff;
                    break;
                case 2:
                    //arithmetic right
                    s_result = (cpu.reg[Rm] >> s_imm);
                    break;
                case 3:
                    //rotate right
                    s_result = (cpu.reg[Rm] << s_imm) + ((cpu.reg[Rm] >> s_imm) & ((0xffffffff >> s_imm) ^ (0xffffffff << (32 - s_imm))));
                    break;
            }
            break;
        case 1:
            Rs = (Immediate >> 8) & 0xf;
            s_imm = cpu.reg[Rs] & 0xf;
            switch(s_type){
                case 0:
                    //logical shift left
                    s_result = (cpu.reg[Rm] << s_imm);
                    break;
                case 1:
                    //logical shift right
                    s_result = (cpu.reg[Rm] >> s_imm) & 0x7ffffff;
                    break;
                case 2:
                    //arithmetic right
                    s_result = (cpu.reg[Rm] >> s_imm);
                    break;
                case 3:
                    //rotate right
                    s_result = (cpu.reg[Rm] << s_imm) + ((cpu.reg[Rm] >> s_imm) & ((0xffffffff >> s_imm) ^ (0xffffffff << (32 - s_imm))));
                    break;
            }
            break;
    }
    return s_result;
}

void ArmBX(struct Arm_BranchAndExchange instruction){
    if(chk_cond(instruction.cond)){
        if((cpu.reg[instruction.Rn] & 0x1)){
            cpu.state=0;//change to thumb state
        }
        else{
            cpu.state=1;//change to arm state
        }
        //jmp
        cpu.reg[14] = cpu.reg[15] + 0x4;
        cpu.reg[15] = cpu.Mem[cpu.reg[instruction.Rn]];
    }
}
//Modified
void ArmBranch(struct Arm_Branch instruction){
    //chk_cond(instruction->cond, CPU)
    if(chk_cond(instruction.cond)){
        i32 offset = instruction.offset << 2;
        u8 msb=0;
        while(offset){
            offset = offset >> 1;
            msb += 1;
        }
        msb = msb - 1;
        offset = (0x80000000 >> msb) | (instruction.offset << 2);
        switch(instruction.L & 0x1){
            case 0:
                cpu.reg[15] = cpu.reg[15] + offset;
                break;
            case 1:
                cpu.reg[14] = cpu.reg[15] + 0x4;//Link
                cpu.reg[15] = cpu.reg[15] + offset;
                break;
                /*execute function and return*/
        }
    }
}

void ArmDataProc(struct Arm_DataProcess instruction){
    u32 ext_Operand2;
    u32 result;
    switch(instruction.eigen){
        case 0:
            //operand2 as a register
            ext_Operand2 = ArmImmOp(instruction.Operand2);
            break;
        case 1:
            //operand2 as an immediate value
            ext_Operand2 = ((instruction.Operand2 & 0xff) >> ((instruction.Operand2 >> 8) & 0xf)) | ((instruction.Operand2 & 0xff) << ((instruction.Operand2 >> 8) & 0xf));
            break;
    }
    //check condition
    if(chk_cond(instruction.cond)){
        switch(instruction.Opcode){
            case 0:
                //AND
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] & ext_Operand2;
                break;
            case 1:
                //EOR
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] ^ ext_Operand2;
                break;
            case 2:
                //SUB
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] - ext_Operand2;
                break;
            case 3:
                //RSB
                cpu.reg[instruction.Rd] = ext_Operand2 - cpu.reg[instruction.Rn];
                break;
            case 4:
                //ADD
                //printf("Rn:%d, Op2:%d\n", cpu.reg[instruction.Rn],ext_Operand2);
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] + ext_Operand2;
                break;
            case 5:
                //ADC
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] + ext_Operand2 + ((cpu.CPSR >> 29) & 0x1);
                break;
            case 6:
                //SBC
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] - ext_Operand2 + ((cpu.CPSR >> 29) & 0x1) - 1;
                break;
            case 7:
                //RSC
                cpu.reg[instruction.Rd] = ext_Operand2 - cpu.reg[instruction.Rn] + ((cpu.CPSR >> 29) & 0x1) - 1;
                break;
            case 8:
                //TST
                result = cpu.reg[instruction.Rn] & ext_Operand2;
                CsprUpdate(result);
                break;
            case 9:
                //TEQ
                result = cpu.reg[instruction.Rn] ^ ext_Operand2;
                CsprUpdate(result);
                break;
            case 10:
                //CMP
                result = cpu.reg[instruction.Rn] - ext_Operand2;
                CsprUpdate(result);
                break;
            case 11:
                //CMN
                result = cpu.reg[instruction.Rn] + ext_Operand2;
                CsprUpdate(result);
                break;
            case 12:
                //ORR
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] | ext_Operand2;
                break;
            case 13:
                //MOV
                cpu.reg[instruction.Rd] = ext_Operand2;
                break;
            case 14:
                //BIC
                cpu.reg[instruction.Rd] = cpu.reg[instruction.Rn] & !(ext_Operand2);
                break;
            case 15:
                //MVN
                cpu.reg[instruction.Rd] = !(ext_Operand2);
                break;
        }
    }
}

void ArmPSRT(struct Arm_PSRTransf instruction){
    int S_op;
    if((instruction.eigen == 2) && (instruction.eigen2 == 0xf)){
        //MRS
        if(instruction.P == 0x1){
            cpu.reg[instruction.Rd] = cpu.SPSR;
        }
        else{
            cpu.reg[instruction.Rd] = cpu.CPSR;
        }
    }
    else if((instruction.eigen2 == 40 | instruction.eigen2 == 41)){
        //MSR register/immdiate -> PSR bits
        if(instruction.eigen == 0x2){
           S_op = ArmImmOp(instruction.Operand);
        }
        else{S_op = cpu.reg[(instruction.Operand & 0xf)];}

        if(instruction.P == 0x1){
            cpu.SPSR = S_op;
        }
        else{
            cpu.CPSR = S_op;
        }
    }
}
//OK
void ArmMUL(struct Arm_Multiply instruction){
    if(instruction.A == 0){
        //MUL, multiple
        cpu.reg[instruction.Rd] = (cpu.reg[instruction.Rm] * cpu.reg[instruction.Rs]) & 0xffffffff;
        if(instruction.S){CsprUpdate(cpu.reg[instruction.Rd]);}
    }
    else{
        //MLA,multiple and add
        cpu.reg[instruction.Rd] = (((cpu.reg[instruction.Rm] * cpu.reg[instruction.Rs]) & 0xffffffff) + cpu.reg[instruction.Rn]) & 0xffffffff;
        if(instruction.S){CsprUpdate(cpu.reg[instruction.Rd]);}
    }
    //lack cspr update
}
//OK
void ArmMULL(struct Arm_MultiplyLong instruction){
    //printf("Hi:%d, Lo:%d\n", instruction.RdHi, instruction.RdLo);
    //printf("res:%lld\n", (u64)cpu.reg[instruction.Rm] * cpu.reg[instruction.Rs]);
    if(instruction.A == 0){
        //MUL, multiple
        if(instruction.U){
            cpu.reg[instruction.RdLo] = (u32)(((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) & 0xffffffff);
            cpu.reg[instruction.RdHi] = ((((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) & 0xffffffff00000000) >> 32);
        }
        else{
            cpu.reg[instruction.RdLo] = (u32)(((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) & 0xffffffff);
            cpu.reg[instruction.RdHi] = ((((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) & 0xffffffff00000000) >> 32);
        }
    }
    else{
        //MLA,multiple and add
        if(instruction.U==0){
            cpu.reg[instruction.RdLo] = ((((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) + ((u64)cpu.reg[instruction.RdHi] << 32) + ((u64)cpu.reg[instruction.RdLo])) & 0xffffffff);
            cpu.reg[instruction.RdHi] = (((((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) + ((u64)cpu.reg[instruction.RdHi] << 32) + ((u64)cpu.reg[instruction.RdLo])) & 0xffffffff00000000) >> 32);
        }
        else{
            cpu.reg[instruction.RdLo] = ((((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) + ((i64)cpu.reg[instruction.RdHi] << 32) + ((i64)cpu.reg[instruction.RdLo])) & 0xffffffff);
            cpu.reg[instruction.RdHi] = (((((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) + ((i64)cpu.reg[instruction.RdHi] << 32) + ((i64)cpu.reg[instruction.RdLo])) & 0xffffffff00000000) >> 32);
        }
        
    }
    //lack cspr update
}

//Single Data Transfer
void ArmSDT(struct Arm_SingleDataTransfer instruction){
    u32 Operand = instruction.Offset;
    u32 Mem_addr;
    if(chk_cond(instruction.cond)){
        //check if I=1, if true, shift the register
        if(instruction.I){
            Operand = ArmImmOp(instruction.Offset);
        }
        //if P=1, add/sub index first
        if(instruction.P){
            if(instruction.U){
                Mem_addr = cpu.reg[instruction.Rn] + Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
            else{
                Mem_addr = cpu.reg[instruction.Rn] - Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
        }
        else{
            Mem_addr = cpu.reg[instruction.Rn];
        }
        //STR or LDR
        if(instruction.L){
            //LDR
            if(instruction.B){
                cpu.reg[instruction.Rd] = cpu.Mem[Mem_addr] & 0xff;
            }
            else{
                cpu.reg[instruction.Rd] = cpu.Mem[Mem_addr];
            }
        }
        else{//STR
            if(instruction.B){
                cpu.Mem[Mem_addr] = cpu.reg[instruction.Rd] & 0xff;
            }
            else{
                cpu.Mem[Mem_addr] = cpu.reg[instruction.Rd];
            }
        }
        //if P=0, add/sub index after transfer
        if(instruction.P == 0){
            if(instruction.U){
                Mem_addr = cpu.reg[instruction.Rn] + Operand;
                //if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
            else{
                Mem_addr = cpu.reg[instruction.Rn] - Operand;
                //if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
        }
    }
}

//signed
void ArmSDTS(struct Arm_HalfwordDataTransferOffset instruction){
    u32 Operand = cpu.reg[instruction.Rm];
    u32 Mem_addr;
    if(instruction.I)Operand = instruction.Hi << 4 | instruction.Rm;
    if(chk_cond(instruction.cond)){
        //if P=1, add/sub index first
        if(instruction.P){
            if(instruction.U){
                Mem_addr = cpu.reg[instruction.Rn] + Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
            else{
                Mem_addr = cpu.reg[instruction.Rn] - Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
        }
        else{
            Mem_addr = cpu.reg[instruction.Rn];
        }
        //STR or LDR
        if(instruction.L){
            //LDR
            if(instruction.S == 0 && instruction.H == 1){//Unsigned Halfword
                cpu.reg[instruction.Rd] = cpu.Mem[Mem_addr] & 0xffff;
            }
            else if(instruction.S == 1 && instruction.H == 0){//Signed Byte
                if((cpu.Mem[Mem_addr] >> 7) == 0x1){
                    //singed
                    cpu.reg[instruction.Rd] = (cpu.Mem[Mem_addr] & 0xff) | 0xffffff00;
                }
                else{
                    cpu.reg[instruction.Rd] = (cpu.Mem[Mem_addr] & 0xff);
                }
            }
            else if(instruction.S == 1 && instruction.H == 1){//Signed halfword
                if((cpu.Mem[Mem_addr] >> 14) & 0x1){
                    //singed
                    cpu.reg[instruction.Rd] = (cpu.Mem[Mem_addr] & 0xffff) | 0xffff0000;
                }
                else{
                    cpu.reg[instruction.Rd] = (cpu.Mem[Mem_addr] & 0xffff);
                }
            }
        }
        else{
            if(instruction.S == 0 && instruction.H == 1){//Unsigned Halfword
                cpu.Mem[Mem_addr] = cpu.reg[instruction.Rd] & 0xffff;
            }
        }
        //if P=0, add/sub index after transfer
        if(instruction.P == 0){
            if(instruction.U){
                Mem_addr = cpu.reg[instruction.Rn] + Operand;
                //if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
            else{
                Mem_addr = cpu.reg[instruction.Rn] - Operand;
                //if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
        }
    }
}


void ArmBDT(struct Arm_BlockDataTransfer instruction){
    u8 *RegList;
    u32 ptr;
    RegList = malloc(sizeof(u8));
    if(chk_cond(instruction.cond)){
        for(int i=0;i<16;i++){
            if((instruction.RegisterList >> i) & 0x1){
                *RegList = i;
                RegList = realloc(RegList, sizeof(u8)*(i+1));//???safe
            }
        }
        //Load
        for(int i=0;i<(sizeof(RegList)/sizeof(u8));i++){
            if(instruction.L){
                if(instruction.P){
                    if(instruction.U){
                        ptr = cpu.reg[instruction.Rn] + 0x4;
                        cpu.reg[RegList[i]] = cpu.Mem[ptr];
                    }
                    else{
                        ptr = cpu.reg[instruction.Rn] - 0x4;
                        cpu.reg[RegList[i]] = cpu.Mem[ptr];
                    }
                }
                else{
                    if(instruction.U){
                        cpu.reg[RegList[i]] = cpu.Mem[ptr];
                        ptr = cpu.reg[instruction.Rn] + 0x4;
                    }
                    else{
                        cpu.reg[RegList[i]] = cpu.Mem[ptr];
                        ptr = cpu.reg[instruction.Rn] - 0x4;
                    }
                }
            }
            else{//store
                if(instruction.P){
                    if(instruction.U){
                        ptr = cpu.reg[instruction.Rn] + 0x4;
                        cpu.Mem[ptr] = cpu.reg[RegList[i]];
                    }
                    else{
                        ptr = cpu.reg[instruction.Rn] - 0x4;
                        cpu.Mem[ptr] = cpu.reg[RegList[i]];
                    }
                }
                else{
                    if(instruction.U){
                        cpu.Mem[ptr] = cpu.reg[RegList[i]];
                        ptr = cpu.reg[instruction.Rn] + 0x4;
                    }
                    else{
                        cpu.Mem[ptr] = cpu.reg[RegList[i]];
                        ptr = cpu.reg[instruction.Rn] - 0x4;
                    }
                }
            }
        }
        if(instruction.W)cpu.reg[instruction.Rn] = ptr;
    }
    free(RegList);
    RegList = NULL;
}

void ArmSWP(struct Arm_SingleDataSwap instruction){
    if(chk_cond(instruction.cond)){
        if(instruction.B){
            cpu.reg[instruction.Rd] = cpu.Mem[cpu.reg[instruction.Rn]] & 0xff;
            cpu.Mem[cpu.reg[instruction.Rn]] = cpu.reg[instruction.Rm] & 0xff;
        }
        else{
            cpu.reg[instruction.Rd] = cpu.Mem[cpu.reg[instruction.Rn]];
            cpu.Mem[cpu.reg[instruction.Rn]] = cpu.reg[instruction.Rm];
        }
    }
}

void ArmSWI(struct Arm_SoftwareInterrupt instruction){
    if(chk_cond(instruction.cond)){
        cpu.reg[14] = cpu.reg[15] + 0x4;//store next instruction addr
        cpu.SPSR = cpu.CPSR;//store User mode
        cpu.reg[15] = Base_addr + 0x8;//Program Counter point to SWI offset in vector table
        cpu.CPSR = (cpu.CPSR & 0x1f) | 0x13;//Enter Supervisor mode
    }
}

void ArmUDF(struct Arm_Undefined instruction){
    if(chk_cond(instruction.cond)){
        printf("Undefined\n");
    }
}

/*
void Arm_BX(struct ArmBranchAndExchange instruction, struct arm7tdmi CPU){

}*/

