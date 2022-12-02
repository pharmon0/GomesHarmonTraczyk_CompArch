/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Main Program Code */

//============================================
// Libraries
//============================================

//============================================
// Constants and Definitions
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
// Main Program
//============================================
int main(void){
    //load program instructions
    //TODO

    //build cores
    //Empty. No cores needed

    //build caches
    vector<Cache> cache = {Cache("cache_0", 65536, 64, ASSOCIATIVITY_2_WAY),
                           Cache("cache_1", 65536, 64, ASSOCIATIVITY_2_WAY),
                           Cache("cache_2", 65536, 64, ASSOCIATIVITY_2_WAY),
                           Cache("cache_3", 65536, 64, ASSOCIATIVITY_2_WAY)};

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