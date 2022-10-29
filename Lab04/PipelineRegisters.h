/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pipeline Registers Object Module */

// Start Header Guard
#ifndef GHT_PIPELINE_REGISTERS
#define GHT_PIPELINE_REGISTERS

//============================================
// Libraries
//============================================
#include <cstdint>    //Standard Int Types
#include "Datatypes.h"//Custom Datatypes Header

//============================================
// Constants and Macros
//============================================

//============================================
//============================================
//============================================
// Pipeline Register Class - FD Register
//============================================
//============================================
//============================================
class FDRegister{
 public:
    struct{  //Register Input Dataports
        uint32_t i;  //fetched instruction
        uint32_t pc; //fetched instruction address 
    } input; //Register Input Dataports

    struct{  //Register Output Dataports
        uint32_t i;  //fetched instruction
        uint32_t pc; //fetched instruction address 
    } output;//Register Output Dataports

    union{   //Register Controlports
        uint8_t all;
        struct{
            uint8_t stall : 1; //blocks register write
            uint8_t no_op : 1; //loads noop data instead of input data
            uint8_t:0;//union alignment
        };
    } ctrl;  //Register Controlports
 
    //Constructors
    FDRegister(void);

    //Clocked Process
    void process(void);
};

//============================================
//============================================
//============================================
// Pipeline Register Class - DE Register
//============================================
//============================================
//============================================
class DERegister{
 public:
    struct{  //Register Input Dataports
        bool     noop;//noop flag for system statistics
        uint32_t pc;  //Program Counter Value;
        int32_t imm;  //immediate value;

        union{    //Register File Control Signals
            uint32_t all;
            struct{
                uint32_t selrs1 : 5; //select RS1        |
                uint32_t selrs2 : 5; //select RS2        |
                uint32_t  selrd : 5; //select RD         |  
                uint32_t  rfwen : 1; //enable write to RD| 0:disable, 1:enable
                uint32_t r1flop : 1; //RS1 is float
                uint32_t r2flop : 1; //RS2 is float
                uint32_t rdflop : 1; //Rd  is float
                uint32_t:0;//union alignment
            };
        } rfctrl; //Register File Control Signals

        union{    //ALU Control Signals
            uint16_t all;
            struct{
                uint16_t   aluop : 4; //select ALU Operation      |
                uint16_t alucomp : 2; //select ALU Comparison Type|
                uint16_t aluasel : 1; //select ALU A Source       | 0:RS1, 1:PC
                uint16_t alubsel : 1; //select ALU B Source       | 0:RS2, 1:Immediate
                uint16_t   alufp : 1; //operation is float
                uint16_t:0;//union alignment
            };
        } aluctrl;//ALU Control Signals

        union{    //Memory Control Signals
            uint8_t all;
            struct{
                uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t memrsgn : 1; //enable sign extend of load| 0:disable, 1:enable based on memrsz
                uint8_t:0;//union alignment
            };
        } memctrl;//Memory Control Signals

        union{    //Branching Control Signals
            uint8_t all;
            struct{
                uint8_t  halt : 1; //halt CPU clock loop    | 0:continue , 1:HALT
                uint8_t btype : 1; //Branch on ALU Condition| 0:no branch, 1:Check ALU
                uint8_t jtype : 1; //Send ALU result to PC and store PC+4 in RD
                uint8_t:0;//union alignment
            };
        } bctrl;  //Branching Control Signals

    } input; //Register Input Dataports

    struct{  //Register Output Dataports
        bool     noop;//noop flag for system statistics
        uint32_t pc;  //Program Counter Value;
        int32_t imm;  //immediate value;

        union{    //Register File Control Signals
            uint32_t all;
            struct{
                uint32_t selrs1 : 5; //select RS1        |
                uint32_t selrs2 : 5; //select RS2        |
                uint32_t  selrd : 5; //select RD         |  
                uint32_t  rfwen : 1; //enable write to RD| 0:disable, 1:enable
                uint32_t r1flop : 1; //RS1 is float
                uint32_t r2flop : 1; //RS2 is float
                uint32_t rdflop : 1; //Rd  is float
                uint32_t:0;//union alignment
            };
        } rfctrl; //Register File Control Signals

        union{    //ALU Control Signals
            uint16_t all;
            struct{
                uint16_t   aluop : 4; //select ALU Operation      |
                uint16_t alucomp : 2; //select ALU Comparison Type|
                uint16_t aluasel : 1; //select ALU A Source       | 0:RS1, 1:PC
                uint16_t alubsel : 1; //select ALU B Source       | 0:RS2, 1:Immediate
                uint16_t   alufp : 1; //operation is float
                uint16_t:0;//union alignment
            };
        } aluctrl;//ALU Control Signals

        union{    //Memory Control Signals
            uint8_t all;
            struct{
                uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t memrsgn : 1; //enable sign extend of load| 0:disable, 1:enable based on memrsz
                uint8_t:0;//union alignment
            };
        } memctrl;//Memory Control Signals

        union{    //Branching Control Signals
            uint8_t all;
            struct{
                uint8_t  halt : 1; //halt CPU clock loop    | 0:continue , 1:HALT
                uint8_t btype : 1; //Branch on ALU Condition| 0:no branch, 1:Check ALU
                uint8_t jtype : 1; //Send ALU result to PC and store PC+4 in RD
                uint8_t:0;//union alignment
            };
        } bctrl;  //Branching Control Signals

    } output;//Register Output Dataports

    union{   //Register Controlports
        uint8_t all;
        struct{
            uint8_t stall : 1; //blocks register write
            uint8_t no_op : 1; //loads noop data instead of input data
            uint8_t:0;//union alignment
        };
    } ctrl;  //Register Controlports
 
    //Constructors
    DERegister(void);

    //Clocked Process
    void process(void);
};

//============================================
//============================================
//============================================
// Pipeline Register Class - EW Register
//============================================
//============================================
//============================================
//============================================
class EWRegister{
 public:
    struct{  //Register Input Dataports
        bool     noop;//noop flag for system statistics
        data32_t aluX;//ALU Result
        data32_t rs2; //RS2 Data
        uint32_t pcplus;  //Modified Program Counter Value;

        union{    //Register File Control Signals
            uint32_t all;
            struct{
                uint32_t selrs1 : 5; //select RS1        |
                uint32_t selrs2 : 5; //select RS2        |
                uint32_t  selrd : 5; //select RD         |  
                uint32_t  rfwen : 1; //enable write to RD| 0:disable, 1:enable
                uint32_t r1flop : 1; //RS1 is float
                uint32_t r2flop : 1; //RS2 is float
                uint32_t rdflop : 1; //Rd  is float
                uint32_t:0;//union alignment
            };
        } rfctrl; //Register File Control Signals

        union{    //Memory Control Signals
            uint8_t all;
            struct{
                uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t memrsgn : 1; //enable sign extend of load| 0:disable, 1:enable based on memrsz
                uint8_t:0;//union alignment
            };
        } memctrl;//Memory Control Signals

        union{    //Branching Control Signals
            uint8_t all;
            struct{
                uint8_t   halt : 1; //halt CPU clock loop    | 0:continue , 1:HALT
                uint8_t branch : 1; //Should a branch occur?
                uint8_t  jtype : 1; //Send ALU result to PC and store PC+4 in RD
                uint8_t:0;//union alignment
            };
        } bctrl;  //Branching Control Signals

    } input; //Register Input Dataports

    struct{  //Register Output Dataports
        bool     noop;//noop flag for system statistics
        data32_t aluX;//ALU Result
        data32_t rs2; //RS2 Data
        uint32_t pcplus;  //Modified Program Counter Value;

        union{    //Register File Control Signals
            uint32_t all;
            struct{
                uint32_t selrs1 : 5; //select RS1        |
                uint32_t selrs2 : 5; //select RS2        |
                uint32_t  selrd : 5; //select RD         |  
                uint32_t  rfwen : 1; //enable write to RD| 0:disable, 1:enable
                uint32_t r1flop : 1; //RS1 is float
                uint32_t r2flop : 1; //RS2 is float
                uint32_t rdflop : 1; //Rd  is float
                uint32_t:0;//union alignment
            };
        } rfctrl; //Register File Control Signals

        union{    //Memory Control Signals
            uint8_t all;
            struct{
                uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t memrsgn : 1; //enable sign extend of load| 0:disable, 1:enable based on memrsz
                uint8_t:0;//union alignment
            };
        } memctrl;//Memory Control Signals

        union{    //Branching Control Signals
            uint8_t all;
            struct{
                uint8_t   halt : 1; //halt CPU clock loop    | 0:continue , 1:HALT
                uint8_t branch : 1; //Should a branch occur?
                uint8_t  jtype : 1; //Send ALU result to PC and store PC+4 in RD
                uint8_t:0;//union alignment
            };
        } bctrl;  //Branching Control Signals

    } output;//Register Output Dataports

    union{   //Register Controlports
        uint8_t all;
        struct{
            uint8_t stall : 1; //blocks register write
            uint8_t no_op : 1; //loads noop data instead of input data
            uint8_t:0;//union alignment
        };
    } ctrl;  //Register Controlports
 
    //Constructors
    EWRegister(void);

    //Clocked Process
    void process(void);
};

// End Header Guard
#endif