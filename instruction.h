struct Arm_Branch Branch_inst;
struct Arm_DataProcess DataPro_inst;
struct Arm_PSRTransf PSRTransf_inst;

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

//struct Arm_

//done
i32 CsprUpdate(i32 a, i32 b, i32 result){
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

    if(result & (1 << 31) == (1 << 31)){
        cpu.CPSR = cpu.CPSR | (5 << 29);//if result[31] = 1, set N、C = 1
    }
    else{cpu.CPSR = cpu.CPSR & 0x5fffffff;}//if result[31] != 1, set N、C = 0

    if((a >> 30) != (b >> 30) && (a >> 30) != (result >> 30))cpu.CPSR = cpu.CPSR | (1 << 28);
    else{cpu.CPSR = cpu.CPSR & 0xefffffff;}
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
    i32 ext_Operand2;
    i32 result;
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
                CsprUpdate(cpu.reg[instruction.Rn], ext_Operand2, result);
                break;
            case 9:
                //TEQ
                result = cpu.reg[instruction.Rn] ^ ext_Operand2;
                CsprUpdate(cpu.reg[instruction.Rn], ext_Operand2, result);
                break;
            case 10:
                //CMP
                result = cpu.reg[instruction.Rn] - ext_Operand2;
                CsprUpdate(cpu.reg[instruction.Rn], ext_Operand2, result);
                break;
            case 11:
                //CMN
                result = cpu.reg[instruction.Rn] + ext_Operand2;
                CsprUpdate(cpu.reg[instruction.Rn], ext_Operand2, result);
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
/*
void Arm_BX(struct ArmBranchAndExchange instruction, struct arm7tdmi CPU){

}*/

