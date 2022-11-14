/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Membus Function Code */
#include "Membus.h"

//============================================
// Constructor
//  Links the membus to all cores and the ram
//   through pointers to their memports
//============================================
Membus::Membus(vector<blockport_t*> cachePorts, blockport_t* memPort){
    this->mem = memPort;
    this->ports = cachePorts;
    this->toMem = true;
    this->token = 0;
}

//============================================
// Membus::process
//  TODO make this function work with cache coherence
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
        *(this->mem) = *(this->ports.at(token));
        this->toMem = false;
    }else{
        *(this->ports.at(token)) = *(this->mem);
        this->toMem = true;
    }
}