/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Main Program Code */

//============================================
// Headers and Namespaces
//============================================
#include <cstdint>
#include <string>
#include <vector>
#include "Settings.h"
#include "Cache.h"
#include "Memory.h"
#include "Node.h"
using std::string;
using std::vector;

//============================================
// Definitions and Constants
//============================================
#define CACHE_SIZE 65536
#define CACHE_BLOCK_SIZE 64
#define CACHE_ACCESS_TICKS 10

//============================================
// Main Program
//============================================
int main(void){
    //load program instructions
    //TODO

    //build cores
    //No cores needed. Build fake request feeders
    //TODO

    //build caches
    vector<Cache> cache = {Cache("cache_0", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_1", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_2", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_3", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY)};
        //override automatic access time calculations
        for(int i = 0; i < cache.size(); i++){
            cache.at(i).set_access_time(CACHE_ACCESS_TICKS);
        }

    //attach cores to caches
    //No cores needed. Use fake request feeders
    //TODO

    //build memory
    //TODO
    vector<Memory> memory = {Memory(),
                             Memory(),
                             Memory(),
                             Memory()};

    //build nodes
    //TODO
    vector<Node> node = {Node(),
                         Node(),
                         Node(),
                         Node()};

    //attach caches to nodes
    for(int i = 0; i < node.size(); i++){
        node.at(i).attach_cache(&cache.at(i));
    }

    //attach memory to nodes
    for(int i = 0; i < node.size(); i++){
        node.at(i).attach_memory(&memory.at(i));
    }

    //attach nodes to nodes
    for(int i = 0; i < node.size(); i++){
        for(int j = 0; i < node.size(); i++){
            node.at(i).add_connection(&node.at(j));
        }
    }

    //run main program loop
    //TODO

    return 0;
}