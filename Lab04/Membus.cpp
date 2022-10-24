/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Membus Function Code */
#include "Membus.h"

//============================================
// Constructor
//============================================
Membus::Membus(void){
}

//============================================
// Membus::process
//============================================
bool Membus::process(void){
    if(!this->cpu.process()){
        return false;
    }
    //TODO map cpu dataports to ram dataports
    this->ram.processPortI();
    this->ram.processPortD();
}