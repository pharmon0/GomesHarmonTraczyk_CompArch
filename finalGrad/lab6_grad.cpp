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
#include "Settings.h"
#include "Cache.h"
#include "Memory.h"
#include "Node.h"
using std::string;

//============================================
// Main Program
//============================================
int main(void){
    //load program instructions
    //TODO

    //build cores
    //Empty. No cores needed

    //build caches
    Cache cache_0 = Cache("cache_0", 65536, 64, ASSOCIATIVITY_2_WAY);
    Cache cache_1 = Cache("cache_1", 65536, 64, ASSOCIATIVITY_2_WAY);
    Cache cache_2 = Cache("cache_2", 65536, 64, ASSOCIATIVITY_2_WAY);
    Cache cache_3 = Cache("cache_3", 65536, 64, ASSOCIATIVITY_2_WAY);

    //attach cores to caches
    //No cores needed. Use fake request feeders
    //TODO

    //build memory
    //TODO
    Memory memory_0 = Memory();
    Memory memory_1 = Memory();
    Memory memory_2 = Memory();
    Memory memory_3 = Memory();

    //build nodes
    //TODO
    Node node_0 = Node();
    Node node_1 = Node();
    Node node_2 = Node();
    Node node_3 = Node();

    //attach caches to nodes
    node_0.attach_cache(&cache_0);
    node_1.attach_cache(&cache_1);
    node_2.attach_cache(&cache_2);
    node_3.attach_cache(&cache_3);

    //attach memory to nodes
    node_0.attach_memory(&memory_0);
    node_1.attach_memory(&memory_1);
    node_2.attach_memory(&memory_2);
    node_3.attach_memory(&memory_3);

    //attach nodes to nodes
    node_0.add_connection(&node_0);
    node_0.add_connection(&node_1);
    node_0.add_connection(&node_2);
    node_0.add_connection(&node_3);
    node_1.add_connection(&node_0);
    node_1.add_connection(&node_1);
    node_1.add_connection(&node_2);
    node_1.add_connection(&node_3);
    node_2.add_connection(&node_0);
    node_2.add_connection(&node_1);
    node_2.add_connection(&node_2);
    node_2.add_connection(&node_3);
    node_3.add_connection(&node_0);
    node_3.add_connection(&node_1);
    node_3.add_connection(&node_2);
    node_3.add_connection(&node_3);

    //run main program loop
    //TODO

    return 0;
}