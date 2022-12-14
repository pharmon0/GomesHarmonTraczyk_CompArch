/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Main Program Code */

//============================================
// Libraries
//============================================
#include "Membus.h"
#include "Core.h"
#include "Cache.h"
#include "Memory.h"
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
#ifndef MEM_TICKS
#define MEM_TICKS      100 //100 ticks per Memory Access
#endif
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
    Memory ram = Memory(MEM_TICKS, 32);
    ram.populateFloat("text/A.txt",ARRAY_A_START); //populate A array
    ram.populateFloat("text/B.txt",ARRAY_B_START); //populate B array
    ram.populate("text/vadd.asm",CPUA_INIT_PC); //populate instruction space
    ram.populate("text/vsub.asm",CPUB_INIT_PC); //populate instruction space

    //create a CPU core
    Core cpuA = Core("cpuA", CPUA_INIT_PC, CPUA_INIT_SP, ALU_INT_CYCLES, ALU_FLOP_CYCLES);
    Core cpuB = Core("cpuB", CPUB_INIT_PC, CPUB_INIT_SP, ALU_INT_CYCLES, ALU_FLOP_CYCLES);

    //create Cache
    Cache cacheAI = Cache(256, 32, CACHE_ASS_DIRECT, &(cpuA.portI), "AI");
    Cache cacheAD = Cache(512, 32, CACHE_ASS_DIRECT, &(cpuA.portD), "AD");
    Cache cacheBI = Cache(256, 32, CACHE_ASS_DIRECT, &(cpuB.portI), "BI");
    Cache cacheBD = Cache(512, 32, CACHE_ASS_DIRECT, &(cpuB.portD), "BD");

    //create and populate membus for instruction and data
    Membus dataBus = Membus({&(cacheAD.membusPort), &(cacheBD.membusPort)},&(ram.portD));
    
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

        //perform cache operations
        cacheAD.process();
        cacheAI.process();
        cacheBD.process();
        cacheBI.process();

        //update the membus
        dataBus.process(tick);
        ram.portIA = cacheAI.membusPort;
        ram.portIB = cacheBI.membusPort;

        //tick the RAM
        ram.process(tick);

        //update the membus
        dataBus.process(tick);
        cacheAI.membusPort = ram.portIA;
        cacheBI.membusPort = ram.portIB;

        tick++;
        
        //if(tick > 100) break;
    }

    //MASTER TODO LIST
    //TODO finish coherence operation in Membus.cpp/h
    //TODO finish implementing Cache.cpp

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