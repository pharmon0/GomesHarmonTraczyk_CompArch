/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * CPU Object Function Code */
#include "CPU.h"

//============================================
// Constructors
//============================================
CPU::CPU(void){
    this->regfile = *new RegisterFile();
    this->decoder = *new Decoder();
    this->pc = 0x00000000;
    this->clock = 0x00000000;
}

//============================================
// run
// - Runs CPU Loop until halt is called
//============================================
void CPU::run(void){ do{
    cout << setfill('=') << setw(80) << "" << endl; 
    cout << "Clock Cycle : " << this->clock
         << "   PC : " << hexString(this->pc)
         << " (" << to_string(pc/4) << ")" << endl;
    cout << setfill('=') << setw(80) << "" << endl;
   //----------------------------------------
   // Instruction Fetch
   //----------------------------------------
    uint32_t fetched = InstructionMemory[pc/4];
    cout << "fetched : " << hexString(fetched) << " : 0b" << bitset<32>(fetched) << endl;
    cout << setfill('-') << setw(80) << "" << endl;
   //----------------------------------------
   // Instruction Decode & Process Immediate
   //----------------------------------------
    this->decoder.process(fetched);
    cout << setfill('-') << setw(80) << "" << endl;
    int32_t immediate = immSignExtendShift(this->decoder.immctrl.value,
                                           this->decoder.immctrl.immsize,
                                           this->decoder.immctrl.immshft);
    cout << "Immediate Sign Extension:"
         << "\n After Extension : " << hexString(immediate) << " (" << immediate << ")" << endl; 
    cout << setfill('-') << setw(80) << "" << endl;
   //----------------------------------------
   // Register File Load & Execute ALU
   //----------------------------------------
    this->regfile.ctrl.selrs1 = this->decoder.rfctrl.selrs1;
    this->regfile.ctrl.selrs2 = this->decoder.rfctrl.selrs2;
    this->regfile.ctrl.selrd  = this->decoder.rfctrl.selrd;
    this->regfile.ctrl.rfwen  = this->decoder.rfctrl.rfwen;
    this->regfile.processRead();
    this->alu.ctrl.comp  = this->decoder.aluctrl.alucomp;
    this->alu.ctrl.aluop = this->decoder.aluctrl.aluop;
    this->alu.A = (this->decoder.aluctrl.aluasel)?(this->pc):(this->regfile.rs1);
    this->alu.B = (this->decoder.aluctrl.alubsel)?(immediate):(this->regfile.rs2);
    this->alu.process();
   //----------------------------------------
   // Memory Access !!Not Fully Implemented!!
   //----------------------------------------
    //MemoryController class is crippled for demo purposes
    this->mem.address = this->alu.X;
    this->mem.ctrl.memrsz = this->decoder.memctrl.memrsz;
    this->mem.ctrl.memwsz = this->decoder.memctrl.memwsz;
    this->mem.processLoad();
    int32_t loaded = memSignExtend(this->mem.loaded,
                                      this->decoder.memctrl.memrsgn,
                                      this->decoder.memctrl.memrsz);
    this->mem.processStore(this->regfile.rs2);
   //----------------------------------------
   // Update PC and Register File Store
   //----------------------------------------
    int32_t dataout = (this->decoder.memctrl.memrsz != 0)?(loaded):(this->alu.X);
    uint32_t pcplus = (this->decoder.ctrl.btype && this->alu.X)?(immediate):(4U);
    pcplus += this->pc;
    this->regfile.processWrite((this->decoder.ctrl.jtype)?(pcplus):(dataout));
    //DECODER_DEBUG disables PC branching. PC will always get PC+4.
    #ifndef DECODER_DEBUG
    this->pc = (this->decoder.ctrl.jtype)?(dataout):(pcplus);
    #else
    this->pc += 4U;
    #endif
   //----------------------------------------
   // Increment CPU Clock Count
   //----------------------------------------
    this->clock++;
    cout << "\n" << endl;
}while(!this->decoder.ctrl.halt);
cout << "Program Halt Instruction Called!\nShutting Down..." << endl;
} //End of CPU::run()