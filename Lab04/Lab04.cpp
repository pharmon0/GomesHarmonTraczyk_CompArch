/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Main Program Code */

//============================================
// Libraries
//============================================
#include "Membus.h"
#include <iostream>
#include <cstdint>
#include <bitset>
#include <string>
#include <vector>
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::bitset;

//============================================
// Constants and Definitions
//============================================
#define MEM_TICKS      20 //20 ticks per Memory Access
#define ALU_INT_TICKS  10 //10 ticks per integer ALU OP
#define ALU_FLOP_TICKS 50 //50 ticks per floating ALU OP

//============================================
// Main Program
//============================================
int main(void){
    //create and populate the system memory
    Memory ram = Memory(MEM_TICKS);
    ram.populateFloat("text/A.txt",0x400); //populate A array
    ram.populateFloat("text/B.txt",0x800); //populate B array
    ram.populate("text/vadd.asm",0); //populate instruction space

    //create a CPU core
    Core cpuA = Core(ALU_INT_TICKS, ALU_FLOP_TICKS);

    //create and populate membus for instruction and data
    Membus busD = Membus({&(cpuA.portD)},&(ram.portD));
    Membus busI = Membus({&(cpuA.portI)},&(ram.portI));
    
    //run simulation loop until CPU halts
    uint64_t tick = 0;
    bool halt = false;
    while(!halt){
        //tick the CPU
        halt = cpuA.process(tick);

        //update the membus
        busD.process(tick);
        busI.process(tick);

        //tick the RAM
        ram.process(tick);

        //update the membus
        busD.process(tick);
        busI.process(tick);

        tick++;
    }
    /*
    long int clock = system.cpu.clock;
    long int instructions = system.cpu.instcnt;
    float cpi = clock / instructions;
    int ticks = clock * 10;
    cout << "With a total of " << clock
         << " Clock Cycles and " << instructions
         <<  " Instructions, the average system CPI was "
         << cpi
         << "\n\n Total System Ticks: " << ticks 
         << endl;
    */
    return 0;
}