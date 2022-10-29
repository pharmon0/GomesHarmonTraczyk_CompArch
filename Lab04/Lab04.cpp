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
#ifndef TICKS_PER_CLOCK
#define TICKS_PER_CLOCK 10
#endif
#define MEM_TICKS      20 //20 ticks per Memory Access
#define ALU_INT_CYCLES  1 //10 ticks per integer ALU OP
#define ALU_FLOP_CYCLES 5 //50 ticks per floating ALU OP

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
    Core cpuA = Core("cpuA", ALU_INT_CYCLES, ALU_FLOP_CYCLES);

    //create and populate membus for instruction and data
    Membus busD = Membus({&(cpuA.portD)},&(ram.portD));
    Membus busI = Membus({&(cpuA.portI)},&(ram.portI));
    
    //run simulation loop until CPU halts
    uint64_t tick = 0;
    bool noHalt = true;
    while(noHalt){
        cout << "\n*** Simulation Loop : Tick #" << tick << endl;
        //tick the CPU
        noHalt = cpuA.process(tick);

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

    uint32_t clockCycles = tick / TICKS_PER_CLOCK;
    uint32_t instructions = cpuA.instructionCount;
    float cpi = ((float)clockCycles)/((float)instructions);
    cout << "\nProgram halted after " << tick << " ticks (" << clockCycles << " cycles), executing "
         << instructions << " instructions, giving an average CPI of " << cpi
         << "\nMemory Dump saved to ./text/memorydump.txt" << endl;
    ram.printToFile("text/memorydump.txt");

    return 0;
}