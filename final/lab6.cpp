/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Main Program Code */

//============================================
// Headers and Namespaces
//============================================
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include "Definitions.h"
#include "Cache.h"
#include "Memory.h"
#include "Bus.h"
#include "Core.h"
#include <iomanip>
using std::setw;
using std::setfill;
using std::string;
using std::vector;
using std::cout;
using std::endl;

//============================================
// Definitions and Constants
//============================================
#define BLOCK_SIZE 64
#define CACHE_SIZE 65536
#define MEMORY_SIZE 262144
#define CACHE_ACCESS_TICKS 10
#define MEMORY_ACCESS_TICKS 30
#define BUS_ACCESS_TICKS 10

//============================================
// Main Program
//============================================
int main(void){
    //load program instructions
    //TODO

    //build caches
    cout << "Constructing Caches" << endl;
    vector<Cache> cache = {Cache("cache_0", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_1", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_2", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_3", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY)};
        //override automatic access time calculations
        for(int i = 0; i < cache.size(); i++){
            cache.at(i).set_access_time(CACHE_ACCESS_TICKS);
        }

    //build cores
    //No cores needed. Build fake request feeders
    //FIXME make this actually do stuff
    cout << "Constructing Cores" << endl;
    vector<Core> core = {Core("core_0", 0, &cache.at(0)),
                         Core("core_1", 1, &cache.at(1)),
                         Core("core_2", 2, &cache.at(2)),
                         Core("core_3", 3, &cache.at(3))};

    //attach cores to caches
    for(int i = 0; i < cache.size(); i++){
        cache.at(i).attach_cpu(&core.at(i));
    }

    //build memory
    cout << "Constructing Memory" << endl;
    Memory memory = Memory("main_memory", MEMORY_SIZE, BLOCK_SIZE, MEMORY_ACCESS_TICKS);

    //build bus
    cout << "Constructing Bus" << endl;
    Bus bus = Bus("memory_bus", cache, &memory, BUS_ACCESS_TICKS);
    
    //attach bus to memory
    memory.attach_bus(&bus);

    //attach bus to caches
    for(int i = 0; i < cache.size(); i++){
        cache.at(i).attach_bus(&bus);
    }

    //run main program loop
    uint32_t tick = 1;
    bool halt = false;
    do{
        cout << setfill('=') << setw(80) << "" << endl;
        cout << "Tick " << tick << endl;
        cout << setfill('=') << setw(80) << "" << endl;

        //process clock tick
        halt = true;
        for(int i = 0; i < core.size(); i++){
            bool state = core.at(i).process(tick);
            halt = halt && state;
        }
        //Bus arbiter handoff
        bus.update_token(tick);

        if(halt){
            cout << "\nAll cores complete. Halting..." << endl;
        }

        //Move to next tick
        tick++;
        if(tick > 100){
            //DEBUG PROGRAM HALT
            halt = true;
        }
    }while(!halt);

    return 0;
}