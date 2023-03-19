struct Arm_Branch Branch_inst;
struct Arm_DataProcess DataPro_inst;
struct Arm_PSRTransf PSRTransf_inst;
struct Arm_Multiply MUL_inst;
struct Arm_MultiplyLong MULL_inst;
struct Arm_SingleDataTransfer SDT_inst;
struct Arm_HalfwordDataTransferOffset SDTS_inst;
struct Arm_BlockDataTransfer BDT_inst;
struct Arm_SingleDataSwap SWP_inst;
struct Arm_SoftwareInterrupt SWI_inst;
struct Arm_Undefined UDF_inst;
struct Arm_BranchAndExchange BX_inst;

struct Arm_Branch ArmBranchDec(u32 instruction){
    Branch_inst.cond = (instruction >> 28) & 0xf;
    Branch_inst.eigen = (instruction >> 25) & 0x7;
    Branch_inst.L = (instruction >> 24) & 0x1;
    Branch_inst.offset = (instruction & 0xffffff);
    return Branch_inst;
}

struct Arm_DataProcess ArmDataProcDec(u32 instruction){
    DataPro_inst.cond = (instruction >> 28) & 0xf;
    DataPro_inst.eigen = (instruction >> 26) & 0x3;
    DataPro_inst.I = (instruction >> 25) & 0x1;
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

struct Arm_Multiply ArmMULDec(u32 instruction){
    MUL_inst.cond = (instruction >> 28) & 0xf;
    MUL_inst.eigen = (instruction >> 22) & 0xf6;
    MUL_inst.A = (instruction >> 21) & 0x1;
    MUL_inst.S = (instruction >> 20) & 0x1;
    MUL_inst.Rd = (instruction >> 16) & 0xf;
    MUL_inst.Rn = (instruction >> 12) & 0xf;
    MUL_inst.Rs = (instruction >> 8) & 0xf;
    MUL_inst.eigen2 = (instruction >> 4) & 0xf;
    MUL_inst.Rm = (instruction) & 0xf;
    return MUL_inst;
}

struct Arm_MultiplyLong ArmMULLDec(u32 instruction){
    MULL_inst.cond = (instruction >> 28) & 0xf;
    MULL_inst.eigen = (instruction >> 23) & 0xf4;
    MULL_inst.U = (instruction >> 22) & 0x1;
    MULL_inst.A = (instruction >> 21) & 0x1;
    MULL_inst.S = (instruction >> 20) & 0x1;
    MULL_inst.RdHi = (instruction >> 16) & 0xf;
    MULL_inst.RdLo = (instruction >> 12) & 0xf;
    MULL_inst.Rs = (instruction >> 8) & 0xf;
    MULL_inst.eigen2 = (instruction >> 4) & 0xf;
    MULL_inst.Rm = (instruction) & 0xf;
    return MULL_inst;
}

struct Arm_SingleDataTransfer ArmSDTDec(u32 instruction){
    SDT_inst.cond = (instruction >> 28) & 0xf;
    SDT_inst.eigen = (instruction >> 26) & 0x3;
    SDT_inst.I = (instruction >> 25) & 0x1;
    SDT_inst.P = (instruction >> 24) & 0x1;
    SDT_inst.U = (instruction >> 23) & 0x1;
    SDT_inst.B = (instruction >> 22) & 0x1;
    SDT_inst.W = (instruction >> 21) & 0x1;
    SDT_inst.L = (instruction >> 20) & 0x1;
    SDT_inst.Rn = (instruction >> 16) & 0xf;
    SDT_inst.Rd = (instruction >> 12) & 0xf;
    SDT_inst.Offset = (instruction) & 0xfff;
    return SDT_inst;
}

struct Arm_HalfwordDataTransferOffset ArmSDTSDec(u32 instruction){
    SDTS_inst.cond = (instruction >> 28) & 0xf;
    SDTS_inst.eigen = (instruction >> 25) & 0x3;
    SDTS_inst.P = (instruction >> 24) & 0x1;
    SDTS_inst.U = (instruction >> 23) & 0x1;
    SDTS_inst.I = (instruction >> 22) & 0x1;
    SDTS_inst.W = (instruction >> 21) & 0x1;
    SDTS_inst.L = (instruction >> 20) & 0x1;
    SDTS_inst.Rn = (instruction >> 16) & 0xf;
    SDTS_inst.Rd = (instruction >> 12) & 0xf;
    SDTS_inst.Hi = (instruction >> 8) & 0xf;
    SDTS_inst.eigen3 = (instruction >> 7) & 0x1;
    SDTS_inst.S = (instruction >> 6) & 0x1;
    SDTS_inst.H = (instruction >> 5) & 0x1;
    SDTS_inst.eigen4 = (instruction >> 4) & 0x1;
    SDTS_inst.Rm = (instruction) & 0xf;
    return SDTS_inst;
}

struct Arm_BlockDataTransfer ArmBDTDec(u32 instruction){
    BDT_inst.cond = (instruction >> 28) & 0xf;
    BDT_inst.eigen = (instruction >> 25) & 0x7;
    BDT_inst.P = (instruction >> 24) & 0x1;
    BDT_inst.U = (instruction >> 23) & 0x1;
    BDT_inst.S = (instruction >> 22) & 0x1;
    BDT_inst.W = (instruction >> 21) & 0x1;
    BDT_inst.L = (instruction >> 20) & 0x1;
    BDT_inst.Rn = (instruction >> 16) & 0xf;
    BDT_inst.RegisterList = (instruction) & 0xffff;
    return BDT_inst;
}

struct Arm_SingleDataSwap ArmSWPDec(u32 instruction){
    SWP_inst.cond = (instruction >> 28) & 0xf;
    SWP_inst.eigen = (instruction >> 23) & 0x1f;
    SWP_inst.B = (instruction >> 22) & 0x1;
    SWP_inst.eigen2 = (instruction >> 20) & 0x3;
    SWP_inst.Rn = (instruction >> 16) & 0xf;
    SWP_inst.Rd = (instruction >> 12) & 0xf;
    SWP_inst.eigen3 = (instruction >> 4) & 0xff;
    SWP_inst.Rm = (instruction) & 0xf;
    return SWP_inst;
}

struct Arm_SoftwareInterrupt ArmSWIDec(u32 instruction){
    SWI_inst.cond = (instruction >> 28) & 0xf;
    SWI_inst.eigen = (instruction >> 24) & 0xf;
    SWI_inst.ignored = (instruction) & 0xffffff;
    return SWI_inst;
}

struct Arm_Undefined ArmUDFDec(u32 instruction){
    UDF_inst.cond = (instruction >> 28) & 0xf;
    UDF_inst.eigen = (instruction >> 25) & 0xf;
    UDF_inst.space = (instruction >> 5) & 0x1ffffd0;
    UDF_inst.eigen2 = (instruction >> 4) & 0x1;
    UDF_inst.tail = (instruction) & 0xf;
    return UDF_inst;
}

struct Arm_BranchAndExchange ArmBXDec(u32 instruction){
    BX_inst.cond = (instruction >> 28) & 0xf;
    BX_inst.eigen = (instruction >> 4) & 0xffffff;
    BX_inst.Rn = (instruction) & 0xf;
    return BX_inst;
}