struct Thumb_MVReg th_mvreg_inst;
struct Thumb_AS th_as_inst;
struct Thumb_Imm th_imm_inst;
struct Thumb_ALU th_alu_inst;
struct Thumb_BX th_bx_inst;
struct Thumb_PcLoad th_pcload_inst;
struct Thumb_LSReg th_lsreg_inst;
struct Thumb_LSBH th_lsbh_inst;
struct Thumb_LSImm th_lsimm_inst;
struct Thumb_LSH th_lsh_inst;
struct Thumb_SPLS th_spls_inst;
struct Thumb_LAddr th_laddr_inst;
struct Thumb_AddSP th_addsp_inst;
struct Thumb_PPReg th_ppreg_inst;
struct Thumb_MulLS th_mulls_inst;
struct Thumb_CondB th_condb_inst;
struct Thumb_SWI th_swi_inst;
struct Thumb_UcondB th_ucondb_inst;
struct Thumb_LongBL th_longbl_inst;

struct Thumb_MVReg ThumbMVRegDec(u16 instruction){
    th_mvreg_inst.eigen = (instruction >> 13) & 0x7;
    th_mvreg_inst.Op = (instruction >> 11) & 0x3;
    th_mvreg_inst.Offset = (instruction >> 6) & 0x1f;
    th_mvreg_inst.Rs = (instruction >> 3) & 0x7;
    th_mvreg_inst.Rd = (instruction) & 0x7;
    return th_mvreg_inst;
}

struct Thumb_AS ThumbASDec(u16 instruction){
    th_as_inst.eigen = (instruction >> 11) & 0x1f;
    th_as_inst.I = (instruction >> 10) & 0x1;
    th_as_inst.Op = (instruction >> 9) & 0x1;
    th_as_inst.Rn = (instruction >> 6) & 0x7;
    th_as_inst.Rs = (instruction >> 3) & 0x7;
    th_as_inst.Rd = (instruction) & 0x7;
    return th_as_inst;
}

struct Thumb_Imm ThumbImmDec(u16 instruction){
    th_imm_inst.eigen = (instruction >> 13) & 0x7;
    th_imm_inst.Op = (instruction >> 11) & 0x3;
    th_imm_inst.Rd = (instruction >> 8) & 0x7;
    th_imm_inst.Offset = (instruction) & 0xff;
    return th_imm_inst;
}

struct Thumb_ALU ThumbALUDec(u16 instruction){
    th_alu_inst.eigen = (instruction >> 10) & 0x3f;
    th_alu_inst.Op = (instruction >> 6) & 0xf;
    th_alu_inst.Rs = (instruction >> 3) & 0x7;
    th_alu_inst.Rd = (instruction) & 0x7;
    return th_alu_inst;
}

struct Thumb_BX ThumbBXDec(u16 instruction){
    th_bx_inst.eigen = (instruction >> 10) & 0x3f;
    th_bx_inst.Op = (instruction >> 8) & 0x3;
    th_bx_inst.H1 = (instruction >> 7) & 0x1;
    th_bx_inst.H2 = (instruction >> 6) & 0x1;
    th_bx_inst.Rs = (instruction >> 3) & 0x7;
    th_bx_inst.Rd = (instruction) & 0x7;
    return th_bx_inst;
}

struct Thumb_PcLoad ThumbPCloadDec(u16 instruction){
    th_pcload_inst.eigen = (instruction >> 11) & 0x1f;
    th_pcload_inst.Rd = (instruction >> 8) & 0x7;
    th_pcload_inst.word = (instruction) & 0xff;
    return th_pcload_inst;
}

struct Thumb_LSReg ThumbLSRegDec(u16 instruction){
    th_lsreg_inst.eigen = (instruction >> 12) & 0xf;
    th_lsreg_inst.L = (instruction >> 11) & 0x1;
    th_lsreg_inst.B = (instruction >> 10) & 0x1;
    th_lsreg_inst.eigen2 = (instruction >> 9) & 0x1;
    th_lsreg_inst.Ro = (instruction >> 6) & 0x7;
    th_lsreg_inst.Rb = (instruction >> 3) & 0x7;
    th_lsreg_inst.Rd = (instruction) & 0x7;
    return th_lsreg_inst;
}

struct Thumb_LSBH ThumbLSBHDec(u16 instruction){
    th_lsbh_inst.eigen = (instruction >> 12) & 0xf;
    th_lsbh_inst.H = (instruction >> 11) & 0x1;
    th_lsbh_inst.S = (instruction >> 10) & 0x1;
    th_lsbh_inst.eigen2 = (instruction >> 9) & 0x1;
    th_lsbh_inst.Ro = (instruction >> 6) & 0x7;
    th_lsbh_inst.Rb = (instruction >> 3) & 0x7;
    th_lsbh_inst.Rd = (instruction) & 0x7;
    return th_lsbh_inst;
}

struct Thumb_LSImm ThumbLSImmDec(u16 instruction){
    th_lsimm_inst.eigen = (instruction >> 13) & 0x7;
    th_lsimm_inst.B = (instruction >> 12) & 0x1;
    th_lsimm_inst.L = (instruction >> 11) & 0x1;
    th_lsimm_inst.Offset = (instruction >> 6) & 0x1f;
    th_lsimm_inst.Rb = (instruction >> 3) & 0x7;
    th_lsimm_inst.Rd = (instruction) & 0x7;
    return th_lsimm_inst;
}

struct Thumb_LSH ThumbLSHDec(u16 instruction){
    th_lsh_inst.eigen = (instruction >> 12) & 0xf;
    th_lsh_inst.L = (instruction >> 11) & 0x1;
    th_lsh_inst.Offset = (instruction >> 6) & 0x1f;
    th_lsh_inst.Rb = (instruction >> 3) & 0x7;
    th_lsh_inst.Rd = (instruction) & 0x7;
    return th_lsh_inst;
}

struct Thumb_SPLS ThumbSPLSDec(u16 instruction){
    th_spls_inst.eigen = (instruction >> 12) & 0xf;
    th_spls_inst.L = (instruction >> 11) & 0x1;
    th_spls_inst.Rd = (instruction >> 8) & 0x7;
    th_spls_inst.word = (instruction) & 0xff;
    return th_spls_inst;
}

struct Thumb_LAddr ThumbLAddrDec(u16 instruction){
    th_laddr_inst.eigen = (instruction >> 12) & 0xf;
    th_laddr_inst.SP = (instruction >> 11) & 0x1;
    th_laddr_inst.Rd = (instruction >> 8) & 0x7;
    th_laddr_inst.word = (instruction) & 0xff;
    return th_laddr_inst;
}

struct Thumb_AddSP ThumbAddSPDec(u16 instruction){
    th_addsp_inst.eigen = (instruction >> 8) & 0xff;
    th_addsp_inst.S = (instruction >> 7) & 0x1;
    th_addsp_inst.word = (instruction) & 0x7f;
    return th_addsp_inst;
}

struct Thumb_PPReg ThumbPPRegDec(u16 instruction){
    th_ppreg_inst.eigen = (instruction >> 12) & 0xf;
    th_ppreg_inst.L = (instruction >> 11) & 0x1;
    th_ppreg_inst.eigen2 = (instruction >> 9) & 0x3;
    th_ppreg_inst.R = (instruction >> 8) & 0x1;
    th_ppreg_inst.Rlist = (instruction) & 0xff;
    return th_ppreg_inst;
}

struct Thumb_MulLS ThumbMulLSDec(u16 instruction){
    th_mulls_inst.eigen = (instruction >> 12) & 0xf;
    th_mulls_inst.L = (instruction >> 11) & 0x1;
    th_mulls_inst.Rb = (instruction >> 8) & 0x7;
    th_mulls_inst.Rlist = (instruction) & 0xff;
    return th_mulls_inst;
}

struct Thumb_CondB ThumbCondBDec(u16 instruction){
    th_condb_inst.eigen = (instruction >> 12) & 0xf;
    th_condb_inst.cond = (instruction >> 8) & 0xf;
    th_condb_inst.SOffset = (instruction) & 0xff;
    return th_condb_inst;
}

struct Thumb_SWI ThumbSWIDec(u16 instruction){
    th_swi_inst.eigen = (instruction >> 8) & 0xff;
    th_swi_inst.value = (instruction) & 0xff;
    return th_swi_inst;
}

struct Thumb_UcondB ThumbUcondBDec(u16 instruction){
    th_ucondb_inst.eigen = (instruction >> 11) & 0x1f;
    th_ucondb_inst.Offset = (instruction) & 0x7ff;
    return th_ucondb_inst;
}

struct Thumb_LongBL ThumbLongBLDec(u16 instruction){
    th_longbl_inst.eigen = (instruction >> 12) & 0xf;
    th_longbl_inst.H = (instruction >> 11) & 0x1;
    th_longbl_inst.Offset = (instruction) & 0x7ff;
    return th_longbl_inst;
}