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
#define ARRAY_A_START 0x400
#define ARRAY_B_START 0x800
#define CPUA_INIT_PC 0
#define CPUA_INIT_SP 0x2FF
#define CPUB_INIT_PC 0x100
#define CPUB_INIT_SP 0x3FF

//============================================
// Main Program
//============================================
int main(void){
    //create and populate the system memory
    Memory ram = Memory(MEM_TICKS);
    ram.populateFloat("text/A.txt",ARRAY_A_START); //populate A array
    ram.populateFloat("text/B.txt",ARRAY_B_START); //populate B array
    ram.populate("text/vadd.asm",CPUA_INIT_PC); //populate instruction space
    ram.populate("text/vsub.asm",CPUB_INIT_PC); //populate instruction space

    //create a CPU core
    Core cpuA = Core("cpuA", CPUA_INIT_PC, CPUA_INIT_SP, ALU_INT_CYCLES, ALU_FLOP_CYCLES);
    Core cpuB = Core("cpuB", CPUB_INIT_PC, CPUB_INIT_SP, ALU_INT_CYCLES, ALU_FLOP_CYCLES);

    //create and populate membus for instruction and data
    Membus dataBus = Membus({&(cpuA.portD), &(cpuB.portD)},&(ram.portD));
    
    //run simulation loop until CPU halts
    uint64_t tick = 0;
    bool noHalt = true;
    bool runA = true;
    bool runB = true;
    uint64_t ticksA = 0;
    uint64_t ticksB = 0;
    while(noHalt){
        cout << "\n*** Simulation Loop : Tick #" << tick << endl;
        //tick the CPU
        if(runA){
            runA = cpuA.process(tick);
            ticksA++;
        }
        if(runB){
            runB = cpuB.process(tick);
            ticksB++;
        }
        noHalt = runA || runB;

        //update the membus
        dataBus.process(tick);
        ram.portIA = cpuA.portI;
        ram.portIB = cpuB.portI;

        //tick the RAM
        ram.process(tick);

        //update the membus
        dataBus.process(tick);
        cpuA.portI = ram.portIA;
        cpuB.portI = ram.portIB;

        tick++;
    }

    uint32_t ccA = ticksA / TICKS_PER_CLOCK;
    uint32_t instructionsA = cpuA.instructionCount;
    float cpiA = ((float)ccA)/((float)instructionsA);
    uint32_t ccB = ticksB / TICKS_PER_CLOCK;
    uint32_t instructionsB = cpuB.instructionCount;
    float cpiB = ((float)ccB)/((float)instructionsB);
    cout << "\nCPU_A ran for " << ccA << " Clock Cycles (" << ticksA << " Ticks) and ran "
         << instructionsA << " Instructions, giving an average CPI of " << cpiA << endl;
    cout << "\nCPU_B ran for " << ccB << " Clock Cycles (" << ticksB << " Ticks) and ran "
         << instructionsB << " Instructions, giving an average CPI of " << cpiB << endl;
    cout << "\nMemory Byte Dump saved to ./text/memorydump.txt\nMemory Array Values saved to ./text/arraysout.txt" << endl;
    ram.printBytes("text/memorydump.txt");
    ram.printFloats("text/arraysout.txt");

    return 0;
}