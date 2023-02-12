//rom start at 0x8000000
#include <stdint.h>

#define CART_BASE_ADDR 0x8000000

typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;

#define DataProcess_eigen 0xc000000
#define Multiply_eigen 0xfc000f0
#define MutiplyLong_eigen 0xf8000f0
#define SingleDataSwap_eigen 0xfb00ff0
#define BranchAndExchange_eigen 0xffffff0
#define HalfwordDataTransfer_eigen 0xe000090
//#define HalfwordDataTransferIO_eigen 0xe400090
#define SingleDataTransfer_eigen 0xc000000
#define Undefined_eigen 0xe000010
#define BlockDataTransfer_eigen 0xe000000
#define Branch_eigen 0xe000000
#define CoprocessorDataT_eigen 0xe000000
#define CoprocessorDataO_eigen 0xf000010
#define CoprocessorRegisterT_eigen 0xf000010
#define SoftwareInterrupt_eigen 0xf000000

struct Arm_DataProcess{
    u8 cond:4;
    u8 eigen:3;
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
    u8 cond;
    u8 one;
    u16 two;
    u32 three;
};

struct arm7tdmi{
    i32 reg[16];
    //u32 nptr;
    u32 CPSR;
    u32 SPSR;
    u32 pipeline[3];
    u32 Mem[32768];
};

struct gba_rom_header{
    u32 Entry_point;
    u8 Nintendo_Logo[156];
    u8 Game_title[12];
    u32 Game_code;
    u8 Maker_code[2];
    u8 Fixed_value;
    u8 Main_unit_code;
    u8 Device_type;
    u8 Reserved_area[7];
    u8 Software_version;
    u8 Complement_check;
    u16 Reserved_area_end;
};

struct gba_rom{
    struct gba_rom_header *header;
    u32 *gba_rom_buf;
    int FileSize;
    char *FileName;
};
