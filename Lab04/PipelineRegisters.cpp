/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pipeline Registers Function Code */
#include "PipelineRegisters.h"

//============================================
// Constructors
//============================================
FDRegister::FDRegister(void){
    this->input.i = 0;
    this->input.pc = 0;
    this->output = this->input;
    this->ctrl.all = 0;
}
DERegister::DERegister(void){
    this->input.imm = 0;
    this->input.pc = 0;
    this->input.rfctrl.all = 0;
    this->input.aluctrl.all = 0;
    this->input.memctrl.all = 0;
    this->input.bctrl.all = 0;
    this->output = this->input;
    this->ctrl.all = 0;
}
EWRegister::EWRegister(void){
    this->input.aluX.integer = 0;
    this->input.rs2.integer = 0;
    this->input.imm = 0;
    this->input.pc = 0;
    this->input.rfctrl.all = 0;
    this->input.aluctrl.all = 0;
    this->input.memctrl.all = 0;
    this->input.bctrl.all = 0;
    this->output = this->input;
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
        this->output = this->input;
    }
}
void DERegister::process(void){
    //if stalling, do nothing.
    if(this->ctrl.stall) return;
    //if no_op, load noop instead of input.
    if(this->ctrl.no_op){
        this->input.imm = 0;
        this->input.pc = 0;
        this->input.rfctrl.all = 0;
        this->input.aluctrl.all = 0;
        this->input.memctrl.all = 0;
        this->input.bctrl.all = 0;
    } else{
        this->output = this->input;
    }
}
void EWRegister::process(void){
    //if stalling, do nothing.
    if(this->ctrl.stall) return;
    //if no_op, load noop instead of input.
    if(this->ctrl.no_op){
        this->input.aluX.integer = 0;
        this->input.rs2.integer = 0;
        this->input.imm = 0;
        this->input.pc = 0;
        this->input.rfctrl.all = 0;
        this->input.aluctrl.all = 0;
        this->input.memctrl.all = 0;
        this->input.bctrl.all = 0;
    } else{
        this->output = this->input;
    }
}