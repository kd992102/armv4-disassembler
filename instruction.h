struct Arm_Branch Branch_inst;
struct Arm_DataProcess DataPro_inst;
struct Arm_PSRTransf PSRTransf_inst;
struct Arm_Multiply Multi_inst;
struct Arm_MultiplyLong MultiLong_inst;
struct Arm_SingleDataTransfer SingleDataTrans_inst;
struct Arm_HalfwordDataTransferRegisterOffset SingleDataTransSign_inst;

extern struct arm7tdmi cpu;

struct Arm_Branch ArmBranchDec(u32 instruction){
    Branch_inst.cond = (instruction >> 28) & 0xf;
    Branch_inst.eigen = (instruction >> 25) & 0x7;
    Branch_inst.L = (instruction >> 24) & 0x1;
    Branch_inst.offset = (instruction & 0xffffff);
    return Branch_inst;
}

struct Arm_DataProcess ArmDataProcDec(u32 instruction){
    DataPro_inst.cond = (instruction >> 28) & 0xf;
    DataPro_inst.eigen = (instruction >> 25) & 0x7;
    DataPro_inst.Opcode = (instruction >> 21) & 0xf;
    DataPro_inst.S = (instruction >> 20) & 0x1;
    DataPro_inst.Rn = (instruction >> 16) & 0xf;
    DataPro_inst.Rd = (instruction >> 12) & 0xf;
    DataPro_inst.Operand2 = instruction & 0xfff;
    return DataPro_inst;
}

struct Arm_PSRTransf ArmPSRTransfDec(u32 instruction){
    PSRTransf_inst.cond = (instruction >> 28) & 0xf;
    PSRTransf_inst.eigen = (instruction >> 23) & 0x1f;
    PSRTransf_inst.P = (instruction >> 22) & 0x1;
    PSRTransf_inst.eigen2 = (instruction >> 16) & 0x3f;
    PSRTransf_inst.Rd = (instruction >> 12) & 0xf;
    PSRTransf_inst.Operand = instruction & 0xfff;
    return PSRTransf_inst;
}

struct Arm_Multiply ArmMultiDec(u32 instruction){
    Multi_inst.cond = (instruction >> 28) & 0xf;
    Multi_inst.eigen = (instruction >> 22) & 0xf6;
    Multi_inst.A = (instruction >> 21) & 0x1;
    Multi_inst.S = (instruction >> 20) & 0x1;
    Multi_inst.Rd = (instruction >> 16) & 0xf;
    Multi_inst.Rn = (instruction >> 12) & 0xf;
    Multi_inst.Rs = (instruction >> 8) & 0xf;
    Multi_inst.eigen2 = (instruction >> 4) & 0xf;
    Multi_inst.Rm = (instruction) & 0xf;
}

struct Arm_MultiplyLong ArmMultLongiDec(u32 instruction){
    MultiLong_inst.cond = (instruction >> 28) & 0xf;
    MultiLong_inst.eigen = (instruction >> 23) & 0xf4;
    MultiLong_inst.U = (instruction >> 22) & 0x1;
    MultiLong_inst.A = (instruction >> 21) & 0x1;
    MultiLong_inst.S = (instruction >> 20) & 0x1;
    MultiLong_inst.RdHi = (instruction >> 16) & 0xf;
    MultiLong_inst.RdLo = (instruction >> 12) & 0xf;
    MultiLong_inst.Rs = (instruction >> 8) & 0xf;
    MultiLong_inst.eigen2 = (instruction >> 4) & 0xf;
    MultiLong_inst.Rm = (instruction) & 0xf;
}

struct Arm_SingleDataTransfer SingleDataTransDec(u32 instrcution){
    SingleDataTrans_inst.cond = (instruction >> 28) & 0xf;
    SingleDataTrans_inst.eigen = (instruction >> 26) & 0x3;
    SingleDataTrans_inst.I = (instruction >> 25) & 0x1;
    SingleDataTrans_inst.P = (instruction >> 24) & 0x1;
    SingleDataTrans_inst.U = (instruction >> 23) & 0x1;
    SingleDataTrans_inst.B = (instruction >> 22) & 0x1;
    SingleDataTrans_inst.W = (instruction >> 21) & 0x1;
    SingleDataTrans_inst.L = (instruction >> 20) & 0x1;
    SingleDataTrans_inst.Rn = (instruction >> 16) & 0xf;
    SingleDataTrans_inst.Rd = (instruction >> 12) & 0xf;
    SingleDataTrans_inst.Offset = (instruction) & 0xfff;
}

struct Arm_HalfwordDataTransferRegisterOffset SingleDataTransSignedDec(u32 instrcution){
    SingleDataTransSign_inst.cond = (instruction >> 28) & 0xf;
    SingleDataTransSign_inst.eigen = (instruction >> 25) & 0x3;
    SingleDataTransSign_inst.P = (instruction >> 24) & 0x1;
    SingleDataTransSign_inst.U = (instruction >> 23) & 0x1;
    SingleDataTransSign_inst.eigen2 = (instruction >> 22) & 0x1;
    SingleDataTransSign_inst.W = (instruction >> 21) & 0x1;
    SingleDataTransSign_inst.L = (instruction >> 20) & 0x1;
    SingleDataTransSign_inst.Rn = (instruction >> 16) & 0xf;
    SingleDataTransSign_inst.Rd = (instruction >> 12) & 0xf;
    SingleDataTransSign_inst.eigen3 = (instruction >> 7) & 0x1f;
    SingleDataTransSign_inst.S = (instruction >> 6) & 0x1;
    SingleDataTransSign_inst.H = (instruction >> 5) & 0x1;
    SingleDataTransSign_inst.eigen4 = (instruction >> 4) & 0x1;
    SingleDataTransSign_inst.Rm = (instruction) & 0xf;
}
/*struct Arm_HalfwordDataTransferRegisterOffset{
    u8 cond:4;
    u8 eigen:3;
    u8 P:1;
    u8 U:1;
    u8 eigen2:1;
    u8 W:1;
    u8 L:1;
    u8 Rn:4;
    u8 Rd:4;
    u16 eigen3:5;
    u8 S:1;
    u8 H:1;
    u8 eigen4:1; //1
    u8 Rm:4;
};*/

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

    //if a 31 bit != b 31 bit and a 31 bit != result 31 bit, then set V = 1
    //if((a >> 30) != (b >> 30) && (a >> 30) != (result >> 30))cpu.CPSR = cpu.CPSR | (1 << 28);
    //else{cpu.CPSR = cpu.CPSR & 0xefffffff;}
}

//return shift amount
u32 ArmImmOperand(u16 Immediate){
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

u32 Arm_B_and_BL(struct Arm_Branch instruction){
    //chk_cond(instruction->cond, CPU)
    if(1){
        switch(instruction.L & 0x1){
            case 0:
                printf("B 0x%08x\n", (u32)((instruction.offset + 0x2) << 2));
                break;
            case 1:
                //CPU.r14 = CPU.pc + 0x4; //next
                printf("BL 0x%08x\n", (u32)((instruction.offset + 0x2) << 2));
                break;
                /*execute function and return*/
        }
        return (u32)((instruction.offset + 0x2) << 2);
    }
}

void Arm_DataProc(struct Arm_DataProcess instruction){
    u32 ext_Operand2;
    u32 result;
    printf("Processing...\n");
    switch(instruction.eigen){
        case 0:
            //operand2 as a register
            printf("case1\n");
            ext_Operand2 = ArmImmOperand(instruction.Operand2);
            break;
        case 1:
            //operand2 as an immediate value
            printf("case2\n");
            ext_Operand2 = ((instruction.Operand2 & 0xff) >> ((instruction.Operand2 >> 8) & 0xf)) | ((instruction.Operand2 & 0xff) << ((instruction.Operand2 >> 8) & 0xf));
            break;
    }
    printf("check condition\n");
    //check condition
    if(1){
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
    printf("done.\n");
}

void Arm_PSRTransfer(struct Arm_PSRTransf instruction){
    int S_op;
    if((instruction.eigen == 2) && (instruction.eigen2 == 0xf)){
        //MRS
        printf("MRS\n");
        if(instruction.P == 0x1){
            cpu.reg[instruction.Rd] = cpu.SPSR;
        }
        else{
            printf("cpu.CPSR:%u\n", cpu.CPSR);
            cpu.reg[instruction.Rd] = cpu.CPSR;
        }
    }
    else if((instruction.eigen2 == 40 | instruction.eigen2 == 41)){
        //MSR register/immdiate -> PSR bits
        if(instruction.eigen == 0x2){
           S_op = ArmImmOperand(instruction.Operand);
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

void Arm_Mul(struct Arm_Multiply instruction){
    if(instruction.A == 0){
        //MUL, multiple
        cpu.reg[instruction.Rd] = (cpu.reg[instruction.Rm] * cpu.reg[instruction.Rs]) & 0xffffffff;
        if(instruction.S){CsprUpdate(cpu.reg[instruction.Rm], cpu.reg[instruction.Rs], cpu.reg[instruction.Rd]);}
    }
    else{
        //MLA,multiple and add
        cpu.reg[instruction.Rd] = (cpu.reg[instruction.Rm] * cpu.reg[instruction.Rs] & 0xffffffff + cpu.reg[instruction.Rn]) & 0xffffffff;
        if(instruction.S){CsprUpdate((cpu.reg[instruction.Rm] * cpu.reg[instruction.Rs]) & 0xffffffff, cpu.reg[instruction.Rn], cpu.reg[instruction.Rd]);}
    }
    //lack cspr update
}

void Arm_MulLong(struct Arm_Multiply instruction){
    if(instruction.A == 0){
        //MUL, multiple
        if(instruction.U){
            cpu.reg[instruction.RdLo] = (u32)(((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) & 0xffffffff);
            cpu.reg[instruction.RdHi] = (u32)((((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) & 0xffffffff00000000) >> 32);
        }
        else{
            cpu.reg[instruction.RdLo] = (u32)(((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) & 0xffffffff);
            cpu.reg[instruction.RdHi] = (u32)((((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) & 0xffffffff00000000) >> 32);
        }
    }
    else{
        //MLA,multiple and add
        if(instruction.U){
            cpu.reg[instruction.RdLo] = (u32)((((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) + ((u64)cpu.reg[instruction.RdHi] << 32) + ((u64)cpu.reg[instruction.RdLo])) & 0xffffffff);
            cpu.reg[instruction.RdHi] = (u32)((((u64)cpu.reg[instruction.Rm] * (u64)cpu.reg[instruction.Rs]) + ((u64)cpu.reg[instruction.RdHi] << 32) + ((u64)cpu.reg[instruction.RdLo])) & 0xffffffff00000000 >> 32);
        }
        else{
            cpu.reg[instruction.RdLo] = (u32)((((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) + ((i64)cpu.reg[instruction.RdHi] << 32) + ((i64)cpu.reg[instruction.RdLo])) & 0xffffffff);
            cpu.reg[instruction.RdHi] = (u32)((((i64)cpu.reg[instruction.Rm] * (i64)cpu.reg[instruction.Rs]) + ((i64)cpu.reg[instruction.RdHi] << 32) + ((i64)cpu.reg[instruction.RdLo])) & 0xffffffff00000000 >> 32);
        }
        
    }
    //lack cspr update
}

//Single Data Transfer
void Arm_SDT(struct Arm_SingleDataTransfer instruction){
    u32 Operand = instruction.Offset;
    u32 Mem_addr;
    if(chk_cond(instruction.cond)){
        //check if I=1, if true, shift the register
        if(instruction.I){
            Operand = ArmImmOperand(instruction.Offset);
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
                cpu.reg[instruction.Rd] = Mem[Mem_addr] & 0xff;
            }
            else{
                cpu.reg[instruction.Rd] = Mem[Mem_addr];
            }
        }
        else{//STR
            if(instruction.B){
                Mem[Mem_addr] = cpu.reg[instruction.Rd] & 0xff;
            }
            else{
                Mem[Mem_addr] = cpu.reg[instruction.Rd];
            }
        }
        //if P=0, add/sub index after transfer
        if(instruction.P == 0){
            if(instruction.U){
                Mem_addr = cpu.reg[instruction.Rn] + Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
            else{
                Mem_addr = cpu.reg[instruction.Rn] - Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
        }
    }
}
//signed
void Arm_SDTS(struct Arm_HalfwordDataTransferRegisterOffset instruction){
    u32 Operand = cpu.reg[instruction.Rm];
    u32 Mem_addr;
    if(chk_cond(instruction.cond)){
        //if P=1, add/sub index first
        if(instruction.S == 0 && instruction.H == 1){
            //unsigned halfword
        }
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
                cpu.reg[instruction.Rd] = Mem[Mem_addr] & 0xffff;
            }
            else if(instruction.S == 1 && instruction.H == 0){//Signed Byte
                cpu.reg[instruction.Rd] = Mem[Mem_addr] & 0xff + (Mem[Mem_addr] & 0x);
            }
        }
        //if P=0, add/sub index after transfer
        if(instruction.P == 0){
            if(instruction.U){
                Mem_addr = cpu.reg[instruction.Rn] + Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
            else{
                Mem_addr = cpu.reg[instruction.Rn] - Operand;
                if(instruction.W)cpu.reg[instruction.Rn] = Mem_addr;
            }
        }
    }
}


/*
void Arm_BX(struct ArmBranchAndExchange instruction, struct arm7tdmi CPU){

}*/

