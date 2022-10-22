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
   //writeback
   //make request to portD
 //Execute Stage (Process Register File and ALU)
   //access register file
   //run alu
   //save EWRegister
    this->ewr.process();
 //Decode Stage (Decode Fetched instructions)
   //run decoder
    //TODO update Decoder to use port->process paradigm instead of process(data)
    this->dec.instruction = this->fdr.output.i;
    this->dec.process();
   //Process Immediate
    int32_t immediate = immSignExtendShift(this->decoder.immctrl.value,
                                           this->decoder.immctrl.immsize,
                                           this->decoder.immctrl.immshft);
    //TODO handle data hazards
    this->ctrl.stallD = 0; //PLACEHOLDER FOR DATAHAZARD CONTROL
   //save DERegister
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
   //if memory is busy, stall and wait.
    this->ctrl.stallF = this->portI.ctrl.membusy;
   //save FDRegister
    this->fdr.ctrl.no_op  = this->ctrl.stallF;
    this->fdr.ctrl.stall  = this->ctrl.stallF
                         || this->ctrl.stallD
                         || this->ctrl.stallE
                         || this->ctrl.stallW; 
    this->fdr.input.i = this->portI.data;
    this->fdr.input.pc = this->pc;
    this->fdr.process();
   //Update PC
    //TODO update PC
   //Make Memory Request for next cycle
    if(this->portI.ctrl.membusy){
        this->portI.address      = this->pc;
        this->portI.ctrl.membusy = 1;
    }
   //return TRUE if halt.
    return this->dec.bctrl.halt;
}