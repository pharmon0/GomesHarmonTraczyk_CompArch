/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Membus Function Code */
#include "Membus.h"

//============================================
// Constructor
//  Links the membus to all cores and the ram
//   through pointers to their memports
//============================================
Membus::Membus(vector<memport_t*> cachePorts, memport_t* memPort){
    this->mem = memPort;
    this->ports = cachePorts;
    this->toMem = true;
    this->token = 0;
}

//============================================
// Membus::process
//  TODO make this function work for arbitrary number of Cores
//   AKA: Bus Arbitration
//============================================
void Membus::process(uint64_t tick){
    cout << " Current Token Holder : CPU" << (int)this->token;
    if(!(tick % TICKS_PER_CLOCK)){
        if(!(*this->ports.at(token)).memctrl.request){
            cout << " | Token Handoff to CPU";
            this->token = (this->token < this->ports.size()-1)?(this->token+1):(0);
            cout << (int)this->token;
        }
    }
    cout << endl;
    if(this->toMem){
        //TODO iterate through ports according to bus arbitration
        *(this->mem) = *(this->ports.at(token));
        this->toMem = false;
    }else{
        //TODO iterate through ports according to bus arbitration
        *(this->ports.at(token)) = *(this->mem);
        this->toMem = true;
    }
}