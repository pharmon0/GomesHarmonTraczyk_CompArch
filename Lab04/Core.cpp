/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Core Code */
#include "Core.h"

//============================================
// Constructors
//============================================
Core::Core(void){

}

//============================================
// CPU Core Clock Cycle Process
//  This is the bulk of the system operation
//============================================
bool Core::process(void){
 //Writeback Stage (Memory Access and Register File Writeback)
   //check portD
    //TODO memory D-Port
   //writeback
    this->rf.rfctrl.rfwen = this->ewr.output.rfctrl.rfwen;
    this->rf.rfctrl.selrd = this->ewr.output.rfctrl.selrd;
    //this->rf.rd = //TODO;
    this-rf.processWrite();
   //make request to portD
    //TODO memory D-Port
    this->ctrl.stallW = 0; //TODO stall based on D-Port

 //Execute Stage (Process Register File and ALU)
   //access register file
    this->rf.rfctrl = this->der.output.rfctrl;
    this->rf.processRead();
   //run alu
    if(this->der.output.bctrl.halt) return 1; //HALT ON EXECUTE OF HALT INSTRUCTION
    this->alu.A = (this->der.output.aluctrl.aluasel)?(this->der.output.pc):(this->rf.rs1);
    this->alu.B = (this->der.output.aluctrl.alubsel)?(this->der.output.imm):(this->rf.rs2);
    this->alu.aluctrl.aluop = this->der.output.aluctrl.aluop;
    this->alu.aluctrl.alucomp = this->der.output.aluctrl.alucomp;
    //TODO set ALU float mode
    this->alu.process();
    this->ctrl.stallE = alu.aluctrl.busy;
   //save EWRegister
    this->ewr.input.aluX = this->alu.X;
    this->ewr.input.pc   = this->der.output.pc;
    this->ewr.input.imm  = this->der.output.imm;
    this->ewr.input.rs2  = this->rf.rs2;
    this->ewr.process();

 //Decode Stage (Decode Fetched instructions)
   //run decoder
    //TODO update Decoder to use port->process paradigm instead of process(data)
    this->dec.instruction = this->fdr.output.i;
    this->dec.process();
   //save DERegister
    //TEST data hazards
    //TODO update for floating points
    if(der.output.rfctrl.selrd == dec.rfctrl.rs1 || der.output.rfctrl.selrd == dec.rfctrl.rs2){
      this->ctrl.stallD = 1;
      if(der.output.rfctrl.selrd == 0){
        this->ctrl.stallD = 0;
      }
    }
      
    this->der.input.imm = immSignExtendShift(this->decoder.immctrl.value,
                                           this->decoder.immctrl.immsize,
                                           this->decoder.immctrl.immshft);
    this->der.input.rfctrl  = this->dec.rfctrl;
    this->der.input.aluctrl = this->dec.aluctrl;
    this->der.input.memctrl = this->dec.memctrl;
    //TODO update Decoder to rename ctrl to bctrl
    this->der.input.bctrl = this->dec.bctrl;
    this->der.input.pc    = this->fdr.output.pc;
    this->der.ctrl.no_op  = this->ctrl.stallD;
    this->der.ctrl.stall  = this->ctrl.stallD
                         || this->ctrl.stallE
                         || this->ctrl.stallW; 
    this->der.process();

 //Fetch Stage (Interact with I Port and Update PC)
   //Make Memory Request for next cycle if not busy
    this->ctrl.stallF = this->portI.ctrl.membusy;
    if(!this->portI.ctrl.membusy){
        this->portI.address      = this->pc;
        this->portI.ctrl.membusy = 1;
    }
   //save FDRegister
    this->fdr.ctrl.no_op  = this->ctrl.stallF;
    this->fdr.ctrl.stall  = this->ctrl.stallD
                         || this->ctrl.stallE
                         || this->ctrl.stallW; 
    this->fdr.input.i = this->portI.data;
    this->fdr.input.pc = this->pc;
    this->fdr.process();
   //Update PC
    if(!this->fdr.ctrl.stall && !this->ctrl.stallF)
      this->pc += 4U;
    return false;
}