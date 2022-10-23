/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Decode Stage Functions Code */
#include "Decoder.h"

//============================================
// Constructors
//============================================
Decoder::Decoder(void){
    this->immctrl.all = 0;
    this->rfctrl.all  = 0;
    this->aluctrl.all = 0;
    this->memctrl.all = 0;
    this->ctrl.all    = 0;
}

//============================================
// Decoder::process
// - Updates Decoder DataPorts based on CtrlPorts
//============================================
void Decoder::process(uint32_t input){
    //load input data as instruction
    instruction_t i;
    i.value = input;
    //clear all ctrlpaths
    this->ctrl.all    = 0;
    this->rfctrl.all  = 0;
    this->memctrl.all = 0;
    this->immctrl.all = 0;
    this->aluctrl.all = 0;
    switch(i.base.opcode){
        case OPCODE_R: //instruction is R-type
            this->rfctrl.selrs1 = i.R.rs1; //get RS1 from i
            this->rfctrl.selrs2 = i.R.rs2; //get RS2 from i
            this->rfctrl.selrd  = i.R.rd;  //get RD  from i
            this->rfctrl.rfwen  = 1;       //enable RF Writeback
            cout << "Decoded :: R-Type Instruction\n Instruction: '";
            switch(i.R.funct3){ //Determine ALUOP from funct3, funct7
                case F3_ADD: //ADD or SUB
                    this->aluctrl.aluop = (i.R.funct7 == 0)?(ALU_ADD):(ALU_SUB);
                    if(i.R.funct7==0){cout << "ADD";}
                    else {cout <<"SUB";}
                break;
                case F3_SLL: //SLL
                    this->aluctrl.aluop = ALU_SLL;
                    cout << "SLL";
                break;
                case F3_SLT: //SLT
                    this->aluctrl.aluop = ALU_CMP;
                    cout << "SLT";
                break;
                case F3_SLTU: //SLTU
                    this->aluctrl.aluop = ALU_CMPU;
                    cout << "SLTU";
                break;
                case F3_XOR: //XOR
                    this->aluctrl.aluop = ALU_XOR;
                    cout << "XOR";
                break;
                case F3_SRL: //SRL or SRA
                    this->aluctrl.aluop = (i.R.funct7 == 0)?(ALU_SRL):(ALU_SRA);
                    if(i.R.funct7==0){cout << "SRL";}
                    else {cout <<"SRA";}
                break;
                case F3_OR: //OR
                    this->aluctrl.aluop = ALU_OR;
                    cout << "OR";
                break;
                case F3_AND: //AND
                    this->aluctrl.aluop = ALU_AND;
                    cout << "AND";
                break;
            }
            cout << " X" << this->rfctrl.selrd
                 << ",X" << this->rfctrl.selrs1
                 << ",X" << this->rfctrl.selrs2
                 << "'" << endl;
            this->printCtrl();
        break;
        case OPCODE_FR: //instruction is FP R-type
            this->rfctrl.selrs1 = i.R.rs1; //get RS1 from i
            this->rfctrl.selrs2 = i.R.rs2; //get RS2 from i
            this->rfctrl.selrd  = i.R.rd;  //get RD  from i
            this->rfctrl.rfwen  = 1;       //enable RF Writeback
            this->rfctrl.r1flop = 1;       //enable FP register for r1
            this->rfctrl.r2flop = 1;       //enable FP register for r2
            this->rfctrl.rdflop = 1;       //enable FP register for rd
            this->aluctrl.alufp = 1;       //enable ALU FP operation
            cout << "Decoded :: R-Type Instruction\n Instruction: '";
            switch(i.I.funct3){
                case F3_ADD: //FADD or FSUB
                    this->aluctrl.aluop = (i.R.funct7 == 0)?(ALU_ADD):(ALU_SUB);
                    if(i.R.funct7==0){cout << "ADD";}
                    else {cout <<"SUB";}
                break;
            }
        break;
        case OPCODE_I: //instruction is I-type
            this->rfctrl.selrs1   = i.I.rs1; //get RS1 from i
            this->rfctrl.selrd    = i.I.rd;  //get RD  from i
            this->rfctrl.rfwen    = 1;       //enable RF Writeback
            this->aluctrl.alubsel = 1;       //set ALU B SRC to immediate
            this->immctrl.value   = (i.I.im11_5<<5) | i.I.im4_0; //imm[11:0]
            cout << "Decoded :: I-Type Instruction\n Instruction: '";
            switch(i.I.funct3){//Determine ALUOP from funct3, funct7
                case F3_ADD: //ADDI or SUBI
                    this->aluctrl.aluop = ALU_ADD;
                    cout << "ADDI";
                break;
                case F3_SLL: //SLLI
                    this->immctrl.value &= 0b11111;
                    this->aluctrl.aluop = ALU_SLL;
                    cout << "SLLI";
                break;
                case F3_SLT: //SLTI
                    this->aluctrl.aluop = ALU_CMP;
                    cout << "SLTI";
                break;
                case F3_SLTU: //SLTUI
                    this->aluctrl.aluop = ALU_CMPU;
                    cout << "SLTIU";
                break;
                case F3_XOR: //XORI
                    this->aluctrl.aluop = ALU_XOR;
                    cout << "XORI";
                break;
                case F3_SRL: //SRLI or SRAI
                    this->immctrl.value &= 0b11111;
                    this->aluctrl.aluop = (i.R.funct7 == 0)?(ALU_SRL):(ALU_SRA);
                    if(i.R.funct7==0){cout << "SRLI";}
                    else {cout <<"SRAI";}
                break;
                case F3_OR: //ORI
                    this->aluctrl.aluop = ALU_OR;
                    cout << "ORI";
                break;
                case F3_AND: //ANDI
                    this->aluctrl.aluop = ALU_AND;
                    cout << "ANDI";
                break;
            }
            cout << " X" << this->rfctrl.selrd
                 << ",X" << this->rfctrl.selrs1
                 << "," << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "'" << endl;
            this->printCtrl();
        break;
        case OPCODE_L: //instruction is I-type:Load
            this->rfctrl.selrs1   = i.I.rs1; //get RS1 from i
            this->rfctrl.selrd    = i.I.rd;  //get RD  from i
            this->rfctrl.rfwen    = 1;       //enable RF Writeback
            this->aluctrl.alubsel = 1;       //set ALU B SRC to immediate
            this->immctrl.value   = (i.I.im11_5<<5) | i.I.im4_0; //imm[11:0]
            cout << "Decoded :: I-Type Instruction\n Instruction: '";
            switch(i.I.funct3){ //determine sign-extension and shift amount by funct3
                case F3_LBU:
                    this->memctrl.memrsgn = 0;   //disable sign-extension
                    this->memctrl.memrsz  = 0b01;//Byte size
                    cout << "LBU";
                break;
                case F3_LB:
                    this->memctrl.memrsgn = 1;   //enable sign-extension
                    this->memctrl.memrsz  = 0b01;//Byte size
                    cout << "LB";
                break;
                case F3_LHU:
                    this->memctrl.memrsgn = 0;   //disable sign-extension
                    this->memctrl.memrsz  = 0b10;//Halfword size
                    cout << "LHU";
                break;
                case F3_LH:
                    this->memctrl.memrsgn = 1;   //enable sign-extension
                    this->memctrl.memrsz  = 0b10;//Halfword size
                    cout << "LH";
                break;
                case F3_LW:
                    this->memctrl.memrsgn = 1;   //enable sign-extension
                    this->memctrl.memrsz  = 0b11;//Word size
                    cout << "LW";
                break;
            }
            cout << " X" << this->rfctrl.selrd
                 << ",X" << this->rfctrl.selrs1
                 << "[" << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "]'" << endl;
            this->printCtrl();
        break;
        case OPCODE_FL: //instruction is FP-Load type
            // TODO
            this->rfctrl.selrs1   = i.I.rs1; //get RS1 from i
            this->rfctrl.selrd    = i.I.rd;  //get RD  from i
            this->rfctrl.rfwen    = 1;       //enable RF Writeback
            this->rfctrl.r1flop   = 1;       //enable FP register for r1
            this->rfctrl.rdflop   = 1;       //enable FP register for rd
            this->aluctrl.alubsel = 1;       //set ALU B SRC to immediate
            this->immctrl.value   = (i.I.im11_5<<5) | i.I.im4_0; //imm[11:0]
            cout << "Decoded :: I-Type Instruction\n Instruction: '";
            switch(i.I.funct3){ //determine sign-extension and shift amount by funct3
                case F3_LW:
                    this->memctrl.memrsgn = 1;   //enable sign-extension
                    this->memctrl.memrsz  = 0b11;//Word size
                    cout << "FLW";
                break;
            }
            cout << " X" << this->rfctrl.selrd
                 << ",X" << this->rfctrl.selrs1
                 << "[" << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "]'" << endl;
            this->printCtrl();
        break;
        case OPCODE_S: //instruction is S-type
            this->rfctrl.selrs1   = i.S.rs1; //get RS1 from i
            this->rfctrl.selrs2   = i.S.rs2; //get RD  from i
            this->aluctrl.alubsel = 1;       //set ALU B SRC to immediate
            this->immctrl.value   = (i.S.im11_5<<5) | i.S.im4_0; //imm[11:0]->imm[11:0]
            cout << "Decoded :: S-Type Instruction\n Instruction: '";
            switch(i.S.funct3){ //determine sign-extension and shift amount by funct3
                case F3_SB:
                    this->memctrl.memwsz  = 0b01;//Byte size
                    cout << "SB";
                break;
                case F3_SH:
                    this->memctrl.memwsz  = 0b10;//Halfword size
                    cout << "SH";
                break;
                case F3_SW:
                    this->memctrl.memwsz  = 0b11;//Word size
                    cout << "SW";
                break;
            }
            cout << " X" << this->rfctrl.selrs2
                 << ",X" << this->rfctrl.selrs1
                 << "[" << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "]'" << endl;
            this->printCtrl();
        break;
        case OPCODE_FS: //instruction is FP S-type
            this->rfctrl.selrs1   = i.S.rs1; //get RS1 from i
            this->rfctrl.selrs2   = i.S.rs2; //get RS2  from i
            this->rfctrl.r1flop   = 1;       //enable FP register for r1
            this->rfctrl.r2flop   = 1;       //enable FP register for r2
            this->aluctrl.alubsel = 1;       //set ALU B SRC to immediate
            this->immctrl.value   = (i.S.im11_5<<5) | i.S.im4_0; //imm[11:0]->imm[11:0]
            cout << "Decoded :: S-Type Instruction\n Instruction: '";
            switch(i.S.funct3){ //determine sign-extension and shift amount by funct3
                case F3_SW:
                    this->memctrl.memwsz  = 0b11;//Word size
                    cout << "SW";
                break;
            }
            cout << " X" << this->rfctrl.selrs2
                 << ",X" << this->rfctrl.selrs1
                 << "[" << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "]'" << endl;
            this->printCtrl();
        break;
        case OPCODE_B: //instruction is B-type
            this->rfctrl.selrs1   = i.B.rs1; //get RS1 from i
            this->rfctrl.selrs2   = i.B.rs2; //get RS2 from i
            this->ctrl.btype      = 1;       //Attempt Branch
            this->immctrl.value = (i.B.im12<<11) | (i.B.im11<<10) //imm[12:1]->imm[11:0]
                                | (i.B.im10_5<<4)| (i.B.im4_1);
            cout << "Decoded :: B-Type Instruction\n Instruction: '";
            switch(i.B.funct3){
                case 0b000: //BEQ
                    this->aluctrl.aluop = ALU_CMP;
                    this->aluctrl.alucomp  = 0b01; //EQ
                    cout << "BEQ";
                break;
                case 0b001: //BNE
                    this->aluctrl.aluop = ALU_CMP;
                    this->aluctrl.alucomp  = 0b10; //NE
                    cout << "BNE";
                break;
                case 0b100: //BLT
                    this->aluctrl.aluop = ALU_CMP;
                    this->aluctrl.alucomp  = 0b00; //LT
                    cout << "BLT";
                break;
                case 0b101: //BGE
                    this->aluctrl.aluop = ALU_CMP;
                    this->aluctrl.alucomp  = 0b11; //GE
                    cout << "BGE";
                break;
                case 0b110: //BLTU
                    this->aluctrl.aluop = ALU_CMPU;
                    this->aluctrl.alucomp  = 0b00; //LT
                    cout << "BLTU";
                break;
                case 0b111: //BGEU
                    this->aluctrl.aluop = ALU_CMPU;
                    this->aluctrl.alucomp  = 0b11; //GE
                    cout << "BGEU";
                break;
            }
            cout << " X" << this->rfctrl.selrs1
                 << ",X" << this->rfctrl.selrs2
                 << "," << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "'" << endl;
            this->printCtrl();
        break;
        case OPCODE_AUIPC: //instruction is U-Type (AUIPC)
            this->aluctrl.aluasel = 1;       //ALU A Source is PC
        case OPCODE_LUI: //instruction is U-Type
            this->rfctrl.selrd    = i.U.rd;  //get RD from i
            this->rfctrl.rfwen    = 1;       //enable RF Writeback
            this->aluctrl.alubsel = 1;       //set ALU B SRC to immediate
            this->aluctrl.aluop   = ALU_ADD; //ALU OP: X = A + B
            this->immctrl.immsize = 1;       //immediate size: 20-bit
            this->immctrl.immshft = 1;       //immediate is upper 20-bit
            this->immctrl.value = i.U.im31_12; //Imm[31:12]->Imm[19:0]
            cout << "Decoded :: U-Type Instruction\n Instruction: '";
            cout << (i.U.opcode == OPCODE_AUIPC)?("AUIPC"):("LUI");
            cout << " X" << this->rfctrl.selrd
                 << "," << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "'" << endl;
            this->printCtrl();
        break;
        case OPCODE_JAL: //instruction is JAL
            this->rfctrl.selrd    = i.J.rd;  //get RD from i
            this->rfctrl.rfwen    = 1;       //enable RF Writeback
            this->ctrl.jtype      = 1;       //enable unconditional-jump mode
            this->aluctrl.aluasel = 1;       //ALU A Source is PC
            this->aluctrl.alubsel = 1;       //ALU B Source is immediate
            this->aluctrl.aluop   = ALU_ADD; //ALU OP: X = A + B
            this->immctrl.immsize = 1;       //immediate size: 20-bit
            this->immctrl.value = (i.J.im20<<19) | (i.J.im19_12<<11) //Imm[20:1]->Imm[19:0]
                                | (i.J.im11<<10) | i.J.im10_1;
            cout << "Decoded :: J-Type Instruction\n Instruction: 'JAL"
                 << " X" << this->rfctrl.selrd
                 << "," << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "'" << endl;
            this->printCtrl();
        break;
        case OPCODE_JALR: //instruction is JALR
            this->rfctrl.selrs1   = i.I.rs1; //get RS1 from i
            this->rfctrl.selrd    = i.J.rd;  //get RD  from i
            this->rfctrl.rfwen    = 1;       //enable RF Writeback
            this->ctrl.jtype      = 1;       //enable unconditional-jump mode
            this->aluctrl.alubsel = 1;       //ALU B Source is immediate
            this->aluctrl.aluop   = ALU_ADD; //ALU OP: X = A + B
            this->immctrl.value = (i.I.im11_5<<5) | i.I.im4_0; //imm[11:0]
            cout << "Decoded :: J-Type Instruction\n Instruction: 'JALR"
                 << " X" << this->rfctrl.selrd
                 << ",X" << this->rfctrl.selrs1
                 << "," << immSignExtendShift(this->immctrl.value,this->immctrl.immsize,this->immctrl.immshft)
                 << "'" << endl;
            this->printCtrl();
        break;
        case OPCODE_HALT: //instruction is Custom HALT
            this->ctrl.halt = 1;//Halt Program
            cout << "Decoded :: HALT Instruction\n Instruction: 'HALT'" << endl;
            this->printCtrl();
        break;
        default: //ERROR, UNRECOGNIZED INSTRUCTION
            cout << "Decoded :: UNKOWN INSTRUCTION! PANIC!!!" << endl;
        break;
    }
}

//============================================
// Decoder::printCtrl
// - Returns string form of controlpaths
//============================================
void Decoder::printCtrl(void){
    cout << "  Control Signals:"
         << "\n\tselrs1 : " << bitset<5>(this->rfctrl.selrs1)
         << "   selrs2 : " << bitset<5>(this->rfctrl.selrs2)
         << "   selrd : " << bitset<5>(this->rfctrl.selrd)
         << "   rfwen : " << bitset<1>(this->rfctrl.rfwen)
         << "\n\taluop : " << bitset<4>(this->aluctrl.aluop)
         << "   alucomp : " << bitset<2>(this->aluctrl.alucomp)
         << "   aluasel : " << bitset<1>(this->aluctrl.aluasel)
         << "   alubsel : " << bitset<1>(this->aluctrl.alubsel)
         << "\n\tmemwsz : " << bitset<2>(this->memctrl.memwsz)
         << "   memrsz : " << bitset<2>(this->memctrl.memrsz)
         << "   memrsgn : " << bitset<1>(this->memctrl.memrsgn)
         << "\n\timmsize : " << bitset<1>(this->immctrl.immsize)
         << "   immshft : " << bitset<1>(this->immctrl.immshft)
         << "\n\t   immediate : " << bitset<20>(this->immctrl.value)
         << "\n\tbtype : " << bitset<1>(this->ctrl.btype)
         << "   jtype : " << bitset<1>(this->ctrl.jtype)
         << "   halt : " << bitset<1>(this->ctrl.halt)
         << endl;
}