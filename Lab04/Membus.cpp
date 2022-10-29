/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Membus Function Code */
#include "Membus.h"

//============================================
// Constructor
//  Links the membus to all cores and the ram
//   through pointers to their memports
//============================================
Membus::Membus(vector<memport_t*> corePorts, memport_t* memPort){
    this->mem = memPort;
    this->ports = corePorts;
    this->toMem = true;
}

//============================================
// Membus::process
//  TODO make this function work for arbitrary number of Cores
//   AKA: Bus Arbitration
//============================================
void Membus::process(uint64_t tick){
    if(this->toMem){
        //TODO iterate through ports according to bus arbitration
        *(this->mem) = *(this->ports.at(0));
        this->toMem = false;
    }else{
        //TODO iterate through ports according to bus arbitration
        *(this->ports.at(0)) = *(this->mem);
        this->toMem = true;
    }
}