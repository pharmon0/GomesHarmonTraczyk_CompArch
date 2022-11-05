/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pipeline Registers Function Code */
#include "PipelineRegisters.h"

//============================================
// Constructors
//============================================
FDRegister::FDRegister(void){
    this->input.i   = 0;
    this->input.pc  = 0;
    this->output.i  = 0;
    this->output.pc = 0;
    this->ctrl.all  = 0;
}
DERegister::DERegister(void){
    this->input.noop         = true;
    this->input.imm          = 0;
    this->input.pc           = 0;
    this->input.rfctrl.all   = 0;
    this->input.aluctrl.all  = 0;
    this->input.memctrl.all  = 0;
    this->input.bctrl.all    = 0;
    this->output.noop        = true;
    this->output.imm         = 0; 
    this->output.pc          = 0;
    this->output.rfctrl.all  = 0;
    this->output.aluctrl.all = 0;
    this->output.memctrl.all = 0;
    this->output.bctrl.all   = 0;
    this->ctrl.all = 0;
}
EWRegister::EWRegister(void){
    this->input.noop          = true;
    this->input.aluX.integer  = 0;
    this->input.rs2.integer   = 0;
    this->input.pcplus        = 0;
    this->input.rfctrl.all    = 0;
    this->input.memctrl.all   = 0;
    this->input.bctrl.all     = 0;
    this->output.noop         = true;
    this->output.aluX.integer = 0;
    this->output.rs2.integer  = 0;
    this->output.pcplus       = 0;
    this->output.rfctrl.all   = 0;
    this->output.memctrl.all  = 0;
    this->output.bctrl.all    = 0;
    this->ctrl.all = 0;
}
//============================================
// Clock Processes
//============================================
void FDRegister::process(void){
    //if stalling, do nothing.
    if(this->ctrl.stall) return;
    //if no_op, load noop instead of input.
    if(this->ctrl.no_op){
        this->output.i = 0;
        this->output.pc = 0;
    } else{
        this->output.i  = this->input.i;
        this->output.pc = this->input.pc;
    }
}
void DERegister::process(void){
    //if stalling, do nothing.
    if(this->ctrl.stall) return;
    //if no_op, load noop instead of input.
    if(this->ctrl.no_op){
        this->output.noop         = true;
        this->output.imm          = 0;
        this->output.pc           = 0;
        this->output.rfctrl.all   = 0;
        this->output.aluctrl.all  = 0;
        this->output.memctrl.all  = 0;
        this->output.bctrl.all    = 0;
    } else{
        this->output.noop        = this->input.noop;
        this->output.imm         = this->input.imm; 
        this->output.pc          = this->input.pc;
        this->output.rfctrl.all  = this->input.rfctrl.all;
        this->output.aluctrl.all = this->input.aluctrl.all;
        this->output.memctrl.all = this->input.memctrl.all;
        this->output.bctrl.all   = this->input.bctrl.all;
    }
}
void EWRegister::process(void){
    //if stalling, do nothing.
    if(this->ctrl.stall) return;
    //if no_op, load noop instead of input.
    if(this->ctrl.no_op){
        this->output.noop          = true;
        this->output.aluX.integer  = 0;
        this->output.rs2.integer   = 0;
        this->output.pcplus        = 0;
        this->output.rfctrl.all    = 0;
        this->output.memctrl.all   = 0;
        this->output.bctrl.all     = 0;
    } else{
        this->output.noop         = this->input.noop;
        this->output.aluX.integer = this->input.aluX.integer;
        this->output.rs2.integer  = this->input.rs2.integer; 
        this->output.pcplus       = this->input.pcplus;
        this->output.rfctrl.all   = this->input.rfctrl.all;
        this->output.memctrl.all  = this->input.memctrl.all;
        this->output.bctrl.all    = this->input.bctrl.all;
    }
}