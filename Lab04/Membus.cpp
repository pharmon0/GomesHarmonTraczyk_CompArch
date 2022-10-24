/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Membus Function Code */
#include "Membus.h"

//============================================
// Constructor
//============================================
Membus::Membus(void){
    int x = 0;
}

//============================================
// Membus::process
//============================================
bool Membus::process(void){
    if(!this->cpu.process()){
        return false;
    }
    this->ram.portI.memctrl.all = this->cpu.portI.memctrl.all;
    this->ram.portI.address     = this->cpu.portI.address;
    this->ram.portI.data        = this->cpu.portI.data;
    this->ram.portD.memctrl.all = this->cpu.portD.memctrl.all;
    this->ram.portD.address     = this->cpu.portD.address;
    this->ram.portD.data        = this->cpu.portD.data;
    this->ram.processPortI();
    this->ram.processPortD();
    this->cpu.portI.memctrl.all = this->ram.portI.memctrl.all;
    this->cpu.portI.address     = this->ram.portI.address;
    this->cpu.portI.data        = this->ram.portI.data;
    this->cpu.portD.memctrl.all = this->ram.portD.memctrl.all;
    this->cpu.portD.address     = this->ram.portD.address;
    this->cpu.portD.data        = this->ram.portD.data;

    return true;
}