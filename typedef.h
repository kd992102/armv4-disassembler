//rom start at 0x8000000
#include <stdint.h>

typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;

//ARM
#define CART_BASE_ADDR 0x8000000
#define DataProcess_eigen 0xc000000
#define Multiply_eigen 0xfc000f0
#define MultiplyLong_eigen 0xf8000f0
#define SingleDataSwap_eigen 0xfb00ff0
#define BranchAndExchange_eigen 0xffffff0
#define HalfwordDataTransfer_eigen 0xe000090
#define SingleDataTransfer_eigen 0xc000000
#define Undefined_eigen 0xe000010
#define BlockDataTransfer_eigen 0xe000000
#define Branch_eigen 0xe000000
#define CoprocessorDataT_eigen 0xe000000
#define CoprocessorDataO_eigen 0xf000010
#define CoprocessorRegisterT_eigen 0xf000010
#define SoftwareInterrupt_eigen 0xf000000
//Thumb
#define MoveShiftedReg_eigen 0xe000
#define AddSub_eigen 0xf800
#define Immediate_eigen 0xe000
#define ALU_eigen 0xfc00
#define BranchEX_eigen 0xfc00
#define PCLoad_eigen 0xf800
#define LSReg_eigen 0xf200
#define LSseBH_eigen 0xf200
#define LSImm_eigen 0xe000
#define LSH_eigen 0xf000
#define SPLS_eigen 0xf000
#define LAddr_eigen 0xf000
#define AddSP_eigen 0xff00
#define PushReg_eigen 0xf600
#define MulLS_eigen 0xf000
#define CondB_eigen 0xf000
#define SWI_eigen 0xff00
#define Uncond_eigen 0xf800
#define LongBL_eigen 0x8000

//Exception Vectors
#define __RESET__ 0x00000000
#define __UNDEFINED__ 0x00000004
#define __SOFTWARE_INTERRUPT__ 0x00000008
#define __ABORT_PREFETCH__ 0x0000000C
#define __ABORT_DATA__ 0x00000010
#define __RESERVED__ 0x00000014
#define __IRQ__ 0x00000018
#define __FIQ__ 0x0000001C

//Address
#define Base_addr 0x00000000

u8 cpu_mode = 1;//arm mode
u8 cpu_read_offset = 4;//arm mode, 4byte

static char swi_func[0x2B][0x20] = {
    "SoftReset\0",
    "RegisterRamReset\0",
    "Halt\0",
    "Stop\0",
    "IntrWait\0",
    "VBlankIntrWait\0",
    "Div\0",
    "DivArm\0",
    "Sqrt\0",
    "ArcTan\0",
    "ArcTan2\0",
    "CpuSet\0",
    "CpuFastSet\0",
    "GetBiosChecksum\0",
    "BgAffineSet\0",
    "ObjAffineSet\0",
    "BitUnPack\0",
    "LZ77UnCompWram\0",
    "LZ77UnCompVram\0",
    "HuffUnComp\0",
    "RLUnCompReadNormalWram\0",
    "RLUnCompReadNormalVram\0",
    "Diff8bitUnFilterWram\0",
    "Diff8bitUnFilterVram\0",
    "Diff16bitUnFilter\0",
    "SoundBias\0",
    "SoundDriverInit\0",
    "SoundDriverMode\0",
    "SoundDriverMain\0",
    "SoundDriverVSync\0",
    "SoundChannelClear\0",
    "MidiKey2Freq\0",
    "MusicPlayerOpen\0",
    "MusicPlayerStart\0",
    "MusicPlayerStop\0",
    "MusicPlayerContinue\0",
    "MusicPlayerFadeOut\0",
    "MultiBoot\0",
    "HardReset\0",
    "CustomHalt\0",
    "SoundDriverVSyncOff\0",
    "SoundDriverVSyncOn\0",
    "SoundGetJumpList\0"
};

static char cond[15][3] = {
    "EQ\0","NE\0","CS\0","CC\0","MI\0","PL\0","VS\0","VC\0","HI\0","GE\0","LT\0","GT\0","LE\0", "AL\0"
};

static char opcode[16][4] = {
    "AND\0","EOR\0","SUB\0","RSB\0","ADD\0","ADC\0","SBC\0","RSC\0","TST\0","TEQ\0","CMP\0","CMN\0","ORR\0","MOV\0","BIC\0","MVN\0"
};

static char rottype[4][4] = {
    "LSL\0","LSR\0","ASR\0","ROR\0"
};

static char psr[2][5] = {
    "CPSR\0", "SPSR\0"
};

static char sh_label[3][3] = {
    "H\0", "SH\0", "SB\0"
};

static char addr_mode[8][3] = {
    "DA\0", "IA\0", "DB\0", "IB\0", "DA\0", "IA\0", "DB\0", "IB\0"
};

static char addr_mode_stack[8][3] = {
    "ED\0", "EA\0", "FD\0", "FA\0", "FA\0", "FD\0", "EA\0", "ED\0"
};

static char th_mcas[4][4] = {
    "MOV\0", "CMP\0", "ADD\0", "SUB\0"
};

static char th_opcode[16][4] = {
    "AND\0","EOR\0","LSL\0","LSR\0","ASR\0","ADC\0","SBC\0","ROR\0","TST\0","NEG\0","CMP\0","CMN\0","ORR\0","MUL\0","BIC\0","MVN\0"
};

static char th_bxcode[4][4] = {
    "ADD\0", "CMP\0", "MOV\0", "BX\0"
};

struct Arm_DataProcess{
    u8 cond:4;
    u8 eigen:2;
    u8 I:1;
    u8 Opcode:4;
    u8 S:1;
    u8 Rn:4;
    u8 Rd:4;
    u16 Operand2:12;
};

struct Arm_PSRTransf{
    u8 cond:4;
    u8 eigen:5;
    u8 P:1;
    u8 eigen2:6;
    u8 Rd:4;
    u16 Operand:12;
};

struct Arm_Multiply{
    u8 cond:4;
    u8 eigen:6;
    u8 A:1;
    u8 S:1;
    u8 Rd:4;
    u8 Rn:4;
    u8 Rs:4;
    u8 eigen2:4;
    u8 Rm:4;
};

struct Arm_MultiplyLong{
    u8 cond:4;
    u8 eigen:5;
    u8 U:1;
    u8 A:1;
    u8 S:1;
    u8 RdHi:4;
    u8 RdLo:4;
    u8 Rs:4;
    u8 eigen2:4;
    u8 Rm:4;
};

struct Arm_SingleDataSwap{
    u8 cond:4;
    u8 eigen:6;
    u8 B:1;
    u8 eigen2:2;
    u8 Rn:4;
    u8 Rd:4;
    u8 eigen3:6;
    u8 Rm:4;
};

struct Arm_BranchAndExchange{
    u8 cond:4;
    u32 eigen:24;
    u8 Rn;
};

struct Arm_HalfwordDataTransferOffset{
    u8 cond:4;
    u8 eigen:3;
    u8 P:1;
    u8 U:1;
    u8 I:1;
    u8 W:1;
    u8 L:1;
    u8 Rn:4;
    u8 Rd:4;
    u8 Hi:4;
    u8 eigen3:1;
    u8 S:1;
    u8 H:1;
    u8 eigen4:1; //1
    u8 Rm:4;
};

struct Arm_SingleDataTransfer{
    u8 cond:4;
    u8 eigen:2;
    u8 I:1;
    u8 P:1;
    u8 U:1;
    u8 B:1;
    u8 W:1;
    u8 L:1;
    u8 Rn:4;
    u8 Rd:4;
    u32 Offset:12;
};

struct Arm_Undefined{
    u8 cond:4;
    u8 eigen:3;
    u32 space:20;
    u8 eigen2:1;
    u8 tail:4;
};

struct Arm_BlockDataTransfer{
    u8 cond:4;
    u8 eigen:3;
    u8 P:1;
    u8 U:1;
    u8 S:1;
    u8 W:1;
    u8 L:1;
    u8 Rn:4;
    u32 RegisterList:16;
};

struct Arm_Branch{
    u8 cond:4;
    u8 eigen:3;
    u8 L:1;
    u32 offset:24;
};

struct Arm_CoprocessorDataTransfer{
    u8 cond:4;
    u8 eigen:3;
    u8 P:1;
    u8 U:1;
    u8 N:1;
    u8 W:1;
    u8 L:1;
    u8 Rn:4;
    u8 CRd:4;
    u8 CPsharp:4;
    u16 offset:8;
};

struct Arm_CoprocessorDataOperation{
    u8 cond:4;
    u8 eigen:4;
    u8 CPOpc:4;
    u8 CRn:4;
    u8 CRd:4;
    u8 CPsharp:4;
    u8 CP:3;
    u8 eigen1:1;
    u8 CRm:4;
};

struct Arm_CoprocessorRegisterTransfer{
    u8 cond:4;
    u8 eigen:4;
    u8 CPOpc:3;
    u8 L:1;
    u8 CRn:4;
    u8 Rd:4;
    u8 CPsharp:4;
    u8 CP:3;
    u8 eigen1:1;
    u8 CRm:4;
};

struct Arm_SoftwareInterrupt{
    u8 cond:4;
    u8 eigen:4;
    u32 ignored:24;
};


//Thumb
struct Thumb_MVReg{
    u8 eigen:3;
    u8 Op:2;
    u8 Offset:5;
    u8 Rs:3;
    u8 Rd:3;
};

struct Thumb_AS{
    u8 eigen:5;
    u8 I:1;
    u8 Op:1;
    u8 Rn:3;
    u8 Rs:3;
    u8 Rd:3;
};

struct Thumb_Imm{
    u8 eigen:3;
    u8 Op:2;
    u8 Rd:3;
    u8 Offset:8;
};

struct Thumb_ALU{
    u8 eigen:6;
    u8 Op:4;
    u8 Rs:3;
    u8 Rd:3;
};

struct Thumb_BX{
    u8 eigen:6;
    u8 Op:2;
    u8 H1:1;
    u8 H2:1;
    u8 Rs:3;
    u8 Rd:3;
};

struct Thumb_PcLoad{
    u8 eigen:5;
    u8 Rd:3;
    u8 word:8;
};

struct Thumb_LSReg{
    u8 eigen:4;
    u8 L:1;
    u8 B:1;
    u8 eigen2:1;
    u8 Ro:3;
    u8 Rb:3;
    u8 Rd:3;
};

struct Thumb_LSBH{
    u8 eigen:4;
    u8 H:1;
    u8 S:1;
    u8 eigen2:1;
    u8 Ro:3;
    u8 Rb:3;
    u8 Rd:3;
};

struct Thumb_LSImm{
    u8 eigen:3;
    u8 B:1;
    u8 L:1;
    u8 Offset:5;
    u8 Rb:3;
    u8 Rd:3;
};

struct Thumb_LSH{
    u8 eigen:4;
    u8 L:1;
    u8 Offset:5;
    u8 Rb:3;
    u8 Rd:3;
};

struct Thumb_SPLS{
    u8 eigen:4;
    u8 L:1;
    u8 Rd:3;
    u8 word:8;
};

struct Thumb_LAddr{
    u8 eigen:4;
    u8 SP:1;
    u8 Rd:3;
    u8 word:8;
};

struct Thumb_AddSP{
    u8 eigen:8;
    u8 S:1;
    u8 word:7;
};

struct Thumb_PPReg{
    u8 eigen:4;
    u8 L:1;
    u8 eigen2:2;
    u8 R:1;
    u8 Rlist:8;
};

struct Thumb_MulLS{
    u8 eigen:4;
    u8 L:1;
    u8 Rb:3;
    u8 Rlist:8;
};

struct Thumb_CondB{
    u8 eigen:4;
    u8 cond:4;
    u8 SOffset:8;
};

struct Thumb_SWI{
    u8 eigen:8;
    u8 value:8;
};

struct Thumb_UcondB{
    u8 eigen:5;
    u16 Offset:11;
};

struct Thumb_LongBL{
    u8 eigen:4;
    u8 H:1;
    u16 Offset:11;
};